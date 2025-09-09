#include "firebase_client.h"
#include "config.h"
#include "firebase-config.h"
#include "wifi_manager.h"

FirebaseClient::FirebaseClient() {
}

bool FirebaseClient::begin() {
    Serial.println("Firebase client initialized");
    Serial.printf("Firebase Host: %s\n", FIREBASE_HOST);
    
    // Don't print the full auth token for security
    String authPreview = String(FIREBASE_AUTH);
    if (authPreview.length() > 10) {
        authPreview = authPreview.substring(0, 10) + "...";
    }
    Serial.printf("Firebase Auth: %s\n", authPreview.c_str());
    
    return true;
}

String FirebaseClient::constructURL() {
    return String("https://") + FIREBASE_HOST + "/iot-data.json?auth=" + FIREBASE_AUTH;
}

String FirebaseClient::createJSONPayload(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr) {
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    
    // Add timestamp
    doc["timestamp"] = millis();
    
    // Add power data
    JsonObject power = doc.createNestedObject("power");
    power["status"] = powerStatus ? "ON" : "OFF";
    power["value"] = powerValue;
    
    // Add location data (static for now)
    JsonObject location = doc.createNestedObject("location");
    location["lat"] = DEFAULT_LATITUDE;
    location["lng"] = DEFAULT_LONGITUDE;
    
    // Add WiFi networks (only if wifiMgr is provided and networkCount > 0)
    if (wifiMgr && networkCount > 0) {
        JsonArray networks = doc.createNestedArray("wifi");
        for (int i = 0; i < networkCount && i < MAX_WIFI_NETWORKS; i++) {
            JsonObject net = networks.createNestedObject();
            net["ssid"] = wifiMgr->getNetworkSSID(i);
            net["rssi"] = wifiMgr->getNetworkRSSI(i);
        }
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool FirebaseClient::sendData(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr) {
    if (wifiMgr && !wifiMgr->isWiFiConnected()) {
        Serial.println("❌ Cannot send data - WiFi not connected");
        return false;
    }
    
    String url = constructURL();
    String jsonPayload = createJSONPayload(powerValue, powerStatus, networkCount, wifiMgr);
    
    DEBUG_PRINTF("Firebase URL: %s\n", url.c_str());
    DEBUG_PRINTF("JSON Payload: %s\n", jsonPayload.c_str());
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(HTTP_TIMEOUT);  // Set HTTP timeout
    
    // Add Host header when using IP address to ensure proper Firebase routing
    http.addHeader("Host", "esp8266-test-4ed5d-default-rtdb.europe-west1.firebasedatabase.app");
    
    Serial.println("Sending data to Firebase...");
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("✅ Firebase response: %d\n", httpResponseCode);
        
        if (httpResponseCode == 200) {
            DEBUG_PRINTF("Response: %s\n", response.c_str());
            http.end();
            return true;
        } else {
            Serial.printf("⚠️  Unexpected response code: %d\n", httpResponseCode);
            Serial.printf("Response: %s\n", response.c_str());
        }
    } else {
        Serial.printf("❌ Firebase send failed: %d\n", httpResponseCode);
    }
    
    http.end();
    return false;
}

bool FirebaseClient::sendSensorData(int powerValue, bool powerStatus) {
    return sendData(powerValue, powerStatus, 0, nullptr);
}

bool FirebaseClient::sendWiFiData(int networkCount) {
    return sendData(0, false, networkCount, nullptr);
}

void FirebaseClient::addWiFiNetwork(StaticJsonDocument<JSON_BUFFER_SIZE>& doc, int index, 
                                   const String& ssid, int rssi) {
    JsonArray networks;
    if (doc.containsKey("wifi")) {
        networks = doc["wifi"];
    } else {
        networks = doc.createNestedArray("wifi");
    }
    
    JsonObject net = networks.createNestedObject();
    net["ssid"] = ssid;
    net["rssi"] = rssi;
}

void FirebaseClient::end() {
    http.end();
}
