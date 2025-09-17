/**
 * IoT GPS & WiFi Monitor with External Power Detection
 * 
 * A complete IoT system for monitoring GPS location, WiFi networks, and external power
 * with real-time data transmission to Firebase and web visualization.
 * 
 * Features:
 * - Real-time GPS tracking with NEO-6M module
 * - GPS location validation and quality monitoring
 * - External power detection using optocoupler
 * - Power state monitoring with debouncing and statistics
 * - WiFi network scanning and mapping  
 * - Real-time Firebase data storage
 * - Web dashboard with interactive map
 * - Comprehensive status monitoring
 * 
 * Hardware:
 * - ESP32 DevKit v1
 * - NEO-6M GPS module connected to Serial2
 * - Optocoupler for external power detection on GPIO 4
 * 
 * @version 2.1.0
 * @date 2025-09-14
 * @author IoT Monitor System
 */

#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "firebase_client.h"
#include "gps_manager.h"
#include "optocoupler_manager.h"

// Global objects
WiFiManager wifiManager;
FirebaseClient firebaseClient;
GPSManager gpsManager;
OptocouplerManager optocouplerManager;

// Timing variables
unsigned long lastDataSend = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Allow serial to initialize
    
    Serial.println("\n" "================================");
    Serial.println(PROJECT_NAME);
    Serial.println("Version: " PROJECT_VERSION);
    Serial.println("Build: " FIRMWARE_BUILD_DATE);
    Serial.println("================================\n");
    
    // Initialize optocoupler manager
    Serial.println("Initializing external power monitoring...");
    if (optocouplerManager.begin(OPTOCOUPLER_PIN, false, OPTOCOUPLER_DEBOUNCE_MS)) {
        Serial.println("✅ Optocoupler initialized");
        Serial.printf("External Power: %s\n", optocouplerManager.getPowerStatusString().c_str());
    } else {
        Serial.println("❌ Optocoupler initialization failed");
    }
    
    // Initialize GPS manager
    Serial.println("Initializing GPS module...");
    if (gpsManager.begin(&Serial2, GPS_BAUDRATE)) {
        Serial.println("✅ GPS module initialized");
    } else {
        Serial.println("❌ GPS module initialization failed");
    }
    
    // Connect to WiFi
    Serial.println("Starting WiFi connection...");
    
    if (wifiManager.begin()) {
        Serial.println("✅ WiFi connection successful");
    } else {
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
}

void loop() {
    // Handle serial commands
    if (Serial.available()) {
        char command = Serial.read();
        if (command == 'g' || command == 'G') {
            Serial.println("Printing GPS status...");
            gpsManager.printGPSStatus();
        } else if (command == 'i' || command == 'I') {
            Serial.println("Printing GPS debug info...");
            gpsManager.printDebugInfo();
        } else if (command == 'p' || command == 'P') {
            Serial.println("Printing power status...");
            optocouplerManager.printStatus();
        } else if (command == 'o' || command == 'O') {
            Serial.println("Printing power debug info...");
            optocouplerManager.printDebugInfo();
        } else if (command == 'r' || command == 'R') {
            Serial.println("Resetting power statistics...");
            optocouplerManager.resetStatistics();
        }
    }
    
    // Check WiFi connection
    if (!wifiManager.isWiFiConnected()) {
        wifiManager.reconnect();
    }
    
    // Update GPS data
    gpsManager.update();
    
    // Update optocoupler data
    if (optocouplerManager.update()) {
        // Power state changed - show brief message
        Serial.printf("🔌 Power: %s\n", optocouplerManager.getPowerStatusString().c_str());
    }
    
    // Periodic data transmission
    if (millis() - lastDataSend >= SENSOR_READ_INTERVAL) {
        lastDataSend = millis();
        
        // Scan WiFi networks
        Serial.println("\n--- Data Transmission ---");
        int networkCount = wifiManager.scanNetworks();
        
        // Send data to Firebase
        if (wifiManager.isWiFiConnected()) {
            bool success = firebaseClient.sendData(
                networkCount,
                &wifiManager,
                &gpsManager,         // Pass GPS manager to Firebase client
                &optocouplerManager  // Pass optocoupler manager to Firebase client
            );
            
            if (success) {
                Serial.println("✅ Data sent to database");
            } else {
                Serial.println("❌ Data transmission failed");
            }
        } else {
            Serial.println("❌ No WiFi - skipping transmission");
        }
        
        // Print compact system status
        Serial.println("--- Status ---");
        Serial.printf("Uptime: %lu min | Heap: %u KB | WiFi: %s\n", 
                     millis()/60000, ESP.getFreeHeap()/1024, 
                     wifiManager.isWiFiConnected() ? "✓" : "✗");
        
        // Compact power status
        Serial.printf("Power: %s", optocouplerManager.getPowerStatusString().c_str());
        if (optocouplerManager.getStateChangeCount() > 0) {
            unsigned long totalTime = optocouplerManager.getTotalPowerOnTime() + optocouplerManager.getTotalPowerOffTime();
            if (totalTime > 0) {
                float uptime = (float)optocouplerManager.getTotalPowerOnTime() / totalTime * 100.0;
                Serial.printf(" (%.1f%% uptime)", uptime);
            }
        }
        Serial.println();
        
        // Compact GPS status
        if (gpsManager.isLocationValid()) {
            Serial.printf("GPS: %.4f,%.4f (%d sats)\n", 
                         gpsManager.getLatitude(), gpsManager.getLongitude(),
                         gpsManager.getSatelliteCount());
        } else {
            Serial.printf("GPS: %s\n", gpsManager.isGPSActive() ? "Searching..." : "Inactive");
        }
        
        Serial.println("Commands: g=GPS p=Power o=Debug r=Reset | ----\n");
    }
    
    // Small delay to prevent watchdog issues
    delay(100);
}
