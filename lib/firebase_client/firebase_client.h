#ifndef FIREBASE_CLIENT_H
#define FIREBASE_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "firebase-config.h"
#include "config.h"

// Forward declaration
class WiFiManager;

class FirebaseClient {
private:
    HTTPClient http;
    String constructURL();
    String createJSONPayload(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr);
    
public:
    FirebaseClient();
    bool begin();
    bool sendData(int powerValue, bool powerStatus, int networkCount, WiFiManager* wifiMgr);
    bool sendSensorData(int powerValue, bool powerStatus);
    bool sendWiFiData(int networkCount);
    void addWiFiNetwork(StaticJsonDocument<JSON_BUFFER_SIZE>& doc, int index, 
                       const String& ssid, int rssi);
    void end();
};

#endif // FIREBASE_CLIENT_H
