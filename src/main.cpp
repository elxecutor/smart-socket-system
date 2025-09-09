/**
 * IoT Power & WiFi Monitor
 * 
 * A complete IoT system for monitoring power availability and WiFi networks
 * with real-time data transmission to Firebase and web visualization.
 * 
 * Features:
 * - Power monitoring via analog sensor
 * - WiFi network scanning and mapping  
 * - Real-time Firebase data storage
 * - Visual LED status indicators
 * - Web dashboard with interactive map
 * 
 * Hardware:
 * - ESP32 DevKit v1
 * - Power sensor on pin 34
 * - Power status LED on pin 2 (Green)
 * - Connection status LED on pin 4 (Blue)
 * 
 * @version 1.0.0
 * @date 2025-09-09
 * @author IoT Power Monitor System
 */

#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "sensor_manager.h"
#include "led_manager.h"
#include "firebase_client.h"

// Global objects
WiFiManager wifiManager;
SensorManager sensorManager;
LEDManager ledManager;
FirebaseClient firebaseClient;

// Timing variables
unsigned long lastDataSend = 0;
unsigned long lastWiFiScan = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Allow serial to initialize
    
    Serial.println("\n" "================================");
    Serial.println(PROJECT_NAME);
    Serial.println("Version: " PROJECT_VERSION);
    Serial.println("Build: " FIRMWARE_BUILD_DATE);
    Serial.println("================================\n");
    
    // Initialize hardware components
    Serial.println("Initializing hardware...");
    ledManager.begin();
    sensorManager.begin();
    
    // Connect to WiFi
    Serial.println("Starting WiFi connection...");
    ledManager.showConnecting();
    
    if (wifiManager.begin()) {
        ledManager.setStatusLED(true);
        Serial.println("✅ WiFi connection successful");
    } else {
        ledManager.showError();
        Serial.println("❌ WiFi connection failed - entering retry mode");
    }
    
    // Initialize Firebase client
    Serial.println("Initializing Firebase client...");
    if (firebaseClient.begin()) {
        Serial.println("✅ Firebase client ready");
    } else {
        Serial.println("❌ Firebase client initialization failed");
    }
    
    Serial.println("\n🚀 System ready - starting main loop\n");
    
    // Initial sensor reading
    sensorManager.update();
    ledManager.updatePowerStatus(sensorManager.getPowerStatus());
}

void loop() {
    // Check WiFi connection
    if (!wifiManager.isWiFiConnected()) {
        ledManager.setStatusLED(false);
        wifiManager.reconnect();
        if (wifiManager.isWiFiConnected()) {
            ledManager.setStatusLED(true);
        }
    }
    
    // Update sensors
    sensorManager.update();
    
    // Update power LED based on sensor status
    ledManager.updatePowerStatus(sensorManager.getPowerStatus());
    
    // Periodic data transmission
    if (millis() - lastDataSend >= SENSOR_READ_INTERVAL) {
        lastDataSend = millis();
        
        // Print sensor data
        sensorManager.printSensorData();
        
        // Scan WiFi networks
        Serial.println("\n--- WiFi Network Scan ---");
        int networkCount = wifiManager.scanNetworks();
        
        // Send data to Firebase
        if (wifiManager.isWiFiConnected()) {
            Serial.println("\n--- Firebase Transmission ---");
            
            // Flash status LED during transmission
            ledManager.setStatusLED(false);
            delay(LED_BLINK_DURATION);
            ledManager.setStatusLED(true);
            
            bool success = firebaseClient.sendData(
                sensorManager.getPowerValue(),
                sensorManager.getPowerStatus(),
                networkCount,
                &wifiManager
            );
            
            if (success) {
                ledManager.showSuccess();
                Serial.println("✅ Data transmission successful\n");
            } else {
                ledManager.showError();
                Serial.println("❌ Data transmission failed\n");
            }
        } else {
            Serial.println("❌ Skipping Firebase transmission - WiFi not connected\n");
        }
        
        // Print system status
        Serial.println("--- System Status ---");
        Serial.printf("Uptime: %lu ms\n", millis());
        Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
        Serial.printf("WiFi Status: %s\n", wifiManager.isWiFiConnected() ? "Connected" : "Disconnected");
        Serial.printf("Power Status: %s\n", sensorManager.getPowerStatus() ? "ON" : "OFF");
        Serial.println("========================\n");
    }
    
    // Small delay to prevent watchdog issues
    delay(100);
}

/**
 * Additional utility functions for debugging and monitoring
 */
void printSystemInfo() {
    Serial.println("=== System Information ===");
    Serial.printf("Chip Model: %s\n", ESP.getChipModel());
    Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
    Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Uptime: %lu ms\n", millis());
    Serial.println("==========================\n");
}
