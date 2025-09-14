#ifndef FIREBASE_CLIENT_H
#define FIREBASE_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "firebase-config.h"
#include "config.h"

// Forward declarations
class WiFiManager;
class GPSManager;
class OptocouplerManager;

class FirebaseClient {
private:
    HTTPClient http;
    String constructURL();
    String createJSONPayload(int networkCount, WiFiManager* wifiMgr, GPSManager* gpsMgr, OptocouplerManager* optocouplerMgr);
    
public:
    FirebaseClient();
    bool begin();
    bool sendData(int networkCount, WiFiManager* wifiMgr, GPSManager* gpsMgr = nullptr, OptocouplerManager* optocouplerMgr = nullptr);
    void end();
};

#endif // FIREBASE_CLIENT_H
