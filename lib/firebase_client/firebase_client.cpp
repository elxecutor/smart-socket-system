#include "firebase_client.h"
#include "config.h"
#include "firebase-config.h"
#include "wifi_manager.h"
#include "gps_manager.h"
#include "optocoupler_manager.h"

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

String FirebaseClient::createJSONPayload(int networkCount, WiFiManager* wifiMgr, GPSManager* gpsMgr, OptocouplerManager* optocouplerMgr) {
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    
    // Add timestamp (both epoch milliseconds and readable format)
    doc["timestamp"] = millis();
    doc["datetime"] = String(__DATE__) + " " + String(__TIME__);
    
    // Add external power data from optocoupler
    JsonObject power = doc.createNestedObject("external_power");
    if (optocouplerMgr) {
        power["status"] = optocouplerMgr->getPowerStatusString();
        power["status_boolean"] = optocouplerMgr->isPowerPresent();
        power["stability"] = optocouplerMgr->getPowerStability();
        power["time_since_change"] = optocouplerMgr->getTimeSinceLastChange();
        power["state_changes"] = optocouplerMgr->getStateChangeCount();
        power["total_on_time"] = optocouplerMgr->getTotalPowerOnTime();
        power["total_off_time"] = optocouplerMgr->getTotalPowerOffTime();
        power["last_power_on"] = optocouplerMgr->getLastPowerOnTime();
        power["last_power_off"] = optocouplerMgr->getLastPowerOffTime();
        
        // Calculate uptime percentage
        unsigned long totalTime = optocouplerMgr->getTotalPowerOnTime() + optocouplerMgr->getTotalPowerOffTime();
        if (totalTime > 0) {
            float uptime = (float)optocouplerMgr->getTotalPowerOnTime() / totalTime * 100.0;
            power["uptime_percentage"] = uptime;
        } else {
            power["uptime_percentage"] = 0.0;
        }
        
        power["source"] = "OPTOCOUPLER";
        power["config"] = optocouplerMgr->getConfigInfo();
    } else {
        power["status"] = "UNKNOWN";
        power["status_boolean"] = false;
        power["source"] = "NOT_INITIALIZED";
    }
    
    // Add system information
    JsonObject system = doc.createNestedObject("system");
    system["uptime_ms"] = millis();
    system["free_heap"] = ESP.getFreeHeap();
    system["wifi_connected"] = (wifiMgr && wifiMgr->isWiFiConnected());
    
    // Add location data - Use GPS if available, otherwise fallback to default
    JsonObject location = doc.createNestedObject("location");
    JsonObject gpsInfo = doc.createNestedObject("gps_info");
    
    if (gpsMgr && gpsMgr->isLocationValid()) {
        // Use real GPS coordinates
        location["lat"] = gpsMgr->getLatitude();
        location["lng"] = gpsMgr->getLongitude();
        location["source"] = "GPS";
        
        // Add detailed GPS information
        gpsInfo["altitude"] = gpsMgr->getAltitude();
        gpsInfo["speed_kmh"] = gpsMgr->getSpeed();
        gpsInfo["satellites"] = gpsMgr->getSatelliteCount();
        gpsInfo["signal_quality"] = gpsMgr->getSignalQuality();
        gpsInfo["gps_time"] = gpsMgr->getFormattedDateTime();
        gpsInfo["time_since_update"] = gpsMgr->getTimeSinceLastUpdate();
        gpsInfo["active"] = gpsMgr->isGPSActive();
        gpsInfo["time_valid"] = gpsMgr->isTimeValid();
    } else {
        // Fallback to default coordinates
        location["lat"] = DEFAULT_LATITUDE;
        location["lng"] = DEFAULT_LONGITUDE;
        location["source"] = "DEFAULT";
        
        // GPS status information
        if (gpsMgr) {
            gpsInfo["active"] = gpsMgr->isGPSActive();
            gpsInfo["satellites"] = gpsMgr->getSatelliteCount();
            gpsInfo["signal_quality"] = gpsMgr->getSignalQuality();
            gpsInfo["time_since_update"] = gpsMgr->getTimeSinceLastUpdate();
        } else {
            gpsInfo["active"] = false;
            gpsInfo["status"] = "GPS_NOT_INITIALIZED";
        }
    }
    
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

bool FirebaseClient::sendData(int networkCount, WiFiManager* wifiMgr, GPSManager* gpsMgr, OptocouplerManager* optocouplerMgr) {
    if (wifiMgr && !wifiMgr->isWiFiConnected()) {
        DEBUG_PRINTLN("❌ Cannot send data - WiFi not connected");
        return false;
    }
    
    String url = constructURL();
    String jsonPayload = createJSONPayload(networkCount, wifiMgr, gpsMgr, optocouplerMgr);
    
    DEBUG_PRINTF("Firebase URL: %s\n", url.c_str());
    DEBUG_PRINTF("JSON Payload: %s\n", jsonPayload.c_str());
    
    // Print GPS status for debugging
    if (gpsMgr) {
        if (gpsMgr->isLocationValid()) {
            DEBUG_PRINTF("📍 Using GPS coordinates: %.6f, %.6f\n", 
                        gpsMgr->getLatitude(), gpsMgr->getLongitude());
        } else {
            DEBUG_PRINTLN("📍 Using default coordinates (GPS not available)");
        }
    } else {
        DEBUG_PRINTLN("📍 Using default coordinates (GPS not initialized)");
    }
    
    // Print optocoupler status for debugging
    if (optocouplerMgr) {
        DEBUG_PRINTF("🔌 External Power: %s (%s)\n", 
                    optocouplerMgr->getPowerStatusString().c_str(),
                    optocouplerMgr->getPowerStability().c_str());
    } else {
        DEBUG_PRINTLN("🔌 External Power: Not monitored");
    }
    
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
