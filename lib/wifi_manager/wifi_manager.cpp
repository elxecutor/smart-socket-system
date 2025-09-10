#include "wifi_manager.h"
#include "config.h"

WiFiManager::WiFiManager() : isConnected(false), lastConnectionAttempt(0) {
}

bool WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    return connect();
}

bool WiFiManager::connect() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // Set DNS servers to help with DNS resolution
    IPAddress dns1(8, 8, 8, 8);       // Google DNS
    IPAddress dns2(1, 1, 1, 1);       // Cloudflare DNS
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, dns1, dns2);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && 
           millis() - startTime < WIFI_CONNECTION_TIMEOUT) {
        delay(WIFI_RETRY_DELAY);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        isConnected = true;
        Serial.println("\nWiFi Connected!");
        printNetworkInfo();
        return true;
    } else {
        isConnected = false;
        Serial.println("\nWiFi Connection Failed!");
        return false;
    }
}

bool WiFiManager::isWiFiConnected() {
    bool connected = WiFi.status() == WL_CONNECTED;
    if (isConnected != connected) {
        isConnected = connected;
        if (!connected) {
            Serial.println("WiFi connection lost!");
        }
    }
    return connected;
}

void WiFiManager::reconnect() {
    if (!isWiFiConnected()) {
        Serial.println("Attempting to reconnect WiFi...");
        connect();
    }
}

int WiFiManager::scanNetworks() {
    Serial.println("Scanning WiFi networks...");
    int networkCount = WiFi.scanNetworks();
    Serial.printf("Networks found: %d\n", networkCount);
    
    for (int i = 0; i < networkCount && i < MAX_WIFI_NETWORKS; i++) {
        Serial.printf("  %d: %s (%d dBm)\n", 
                     i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
    
    return networkCount;
}

String WiFiManager::getNetworkSSID(int index) {
    return WiFi.SSID(index);
}

int WiFiManager::getNetworkRSSI(int index) {
    return WiFi.RSSI(index);
}

IPAddress WiFiManager::getLocalIP() {
    return WiFi.localIP();
}

void WiFiManager::printNetworkInfo() {
    DEBUG_PRINTF("IP Address: %s\n", WiFi.localIP().toString().c_str());
    DEBUG_PRINTF("Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    DEBUG_PRINTF("Subnet: %s\n", WiFi.subnetMask().toString().c_str());
    DEBUG_PRINTF("DNS: %s\n", WiFi.dnsIP().toString().c_str());
    DEBUG_PRINTF("RSSI: %d dBm\n", WiFi.RSSI());
}
