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
    void end();
};

#endif // FIREBASE_CLIENT_H
