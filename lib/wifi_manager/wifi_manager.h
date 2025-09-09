#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class WiFiManager {
private:
    bool isConnected;
    unsigned long lastConnectionAttempt;
    
public:
    WiFiManager();
    bool begin();
    bool connect();
    bool isWiFiConnected();
    void reconnect();
    int scanNetworks();
    String getNetworkSSID(int index);
    int getNetworkRSSI(int index);
    IPAddress getLocalIP();
    void printNetworkInfo();
};

#endif // WIFI_MANAGER_H
