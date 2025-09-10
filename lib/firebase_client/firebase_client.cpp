#include "firebase_client.h"
#include "config.h"
#include "firebase-config.h"
#include "wifi_manager.h"

FirebaseClient::FirebaseClient() {
}

bool FirebaseClient::begin() {
    DEBUG_PRINTLN("Firebase client initialized");
    DEBUG_PRINTF("Firebase Host: %s\n", FIREBASE_HOST);
    
    // Don't print the full auth token for security
    String authPreview = String(FIREBASE_AUTH);
    if (authPreview.length() > 10) {
        authPreview = authPreview.substring(0, 10) + "...";
    }
    DEBUG_PRINTF("Firebase Auth: %s\n", authPreview.c_str());
    
    return true;
}

String FirebaseClient::constructURL() {
    return String("https://") + FIREBASE_HOST + "/iot-data.json?auth=" + FIREBASE_AUTH;
}

String FirebaseClient::createJSONPayload(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr) {
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    
    // Add timestamp (both epoch milliseconds and readable format)
    doc["timestamp"] = millis();
    doc["datetime"] = String(__DATE__) + " " + String(__TIME__);
    
    // Enhanced power data with detailed information
    JsonObject power = doc.createNestedObject("power");
    power["status"] = powerStatus ? "ON" : "OFF";
    power["status_boolean"] = powerStatus;  // Explicit boolean for database queries
    power["raw_adc_value"] = powerValue;
    
    // Calculate and include voltage if we have access to sensor manager functions
    // This assumes the voltage calculation is consistent with power-config.h
    float voltage = (float(powerValue) * 3.3 / 4095.0) / 0.66;
    power["input_voltage"] = voltage;
    power["detection_confidence"] = voltage > 4.0 ? "HIGH" : (voltage > 2.0 ? "MEDIUM" : "LOW");
    
    // Add system information
    JsonObject system = doc.createNestedObject("system");
    system["uptime_ms"] = millis();
    system["free_heap"] = ESP.getFreeHeap();
    system["wifi_connected"] = (wifiMgr && wifiMgr->isWiFiConnected());
    
    // Add location data (static for now)
    JsonObject location = doc.createNestedObject("location");
    location["lat"] = DEFAULT_LATITUDE;
    location["lng"] = DEFAULT_LONGITUDE;
    
    // Add WiFi networks (only if wifiMgr is provided and networkCount > 0)
    if (wifiMgr && networkCount > 0) {
        JsonArray networks = doc.createNestedArray("wifi_networks");
        for (int i = 0; i < networkCount && i < MAX_WIFI_NETWORKS; i++) {
            JsonObject net = networks.createNestedObject();
            net["ssid"] = wifiMgr->getNetworkSSID(i);
            net["rssi"] = wifiMgr->getNetworkRSSI(i);
            net["signal_strength"] = wifiMgr->getNetworkRSSI(i) > -50 ? "STRONG" : 
                                   (wifiMgr->getNetworkRSSI(i) > -70 ? "MEDIUM" : "WEAK");
        }
        system["wifi_networks_detected"] = networkCount;
    } else {
        system["wifi_networks_detected"] = 0;
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool FirebaseClient::sendData(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr) {
    if (wifiMgr && !wifiMgr->isWiFiConnected()) {
        DEBUG_PRINTLN("❌ Cannot send data - WiFi not connected");
        return false;
    }
    
    String url = constructURL();
    String jsonPayload = createJSONPayload(powerValue, powerStatus, networkCount, wifiMgr);
    
    DEBUG_PRINTF("Firebase URL: %s\n", url.c_str());
    DEBUG_PRINTF("JSON Payload: %s\n", jsonPayload.c_str());
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(HTTP_TIMEOUT);
    
    // Add Host header for proper Firebase routing
    http.addHeader("Host", FIREBASE_HOST);
    
    DEBUG_PRINTLN("Sending data to Firebase...");
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        DEBUG_PRINTF("✅ Firebase response: %d\n", httpResponseCode);
        
        if (httpResponseCode == 200) {
            DEBUG_PRINTF("Response: %s\n", response.c_str());
            http.end();
            return true;
        } else {
            DEBUG_PRINTF("⚠️  Unexpected response code: %d\n", httpResponseCode);
            DEBUG_PRINTF("Response: %s\n", response.c_str());
        }
    } else {
        DEBUG_PRINTF("❌ Firebase send failed: %d\n", httpResponseCode);
    }
    
    http.end();
    return false;
}

void FirebaseClient::end() {
    http.end();
}
