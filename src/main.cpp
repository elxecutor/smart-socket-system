/**
 * IoT Power & WiFi Monitor - 5V DC Edition
 * 
 * A complete IoT system for monitoring 5V DC power availability and WiFi networks
 * with real-time data transmission to Firebase and web visualization.
 * 
 * Features:
 * - 5V DC power monitoring with voltage divider and averaging
 * - Hysteresis-based power state detection for stability
 * - Power quality monitoring (voltage ripple detection)
 * - WiFi network scanning and mapping  
 * - Real-time Firebase data storage
 * - Web dashboard with interactive map
 * - Calibration mode for optimal threshold setting
 * 
 * Hardware:
 * - ESP32 DevKit v1
 * - 5V DC power sensor with voltage divider on pin 34
 * - Voltage divider: 5V -> 3.3V (ratio 0.66)
 * 
 * @version 1.1.0
 * @date 2025-09-10
 * @author IoT Power Monitor System
 */

#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "sensor_manager.h"
#include "firebase_client.h"

// Global objects
WiFiManager wifiManager;
SensorManager sensorManager;
FirebaseClient firebaseClient;

// Timing variables
unsigned long lastDataSend = 0;
bool calibrationMode = POWER_CALIBRATION_MODE;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Allow serial to initialize
    
    Serial.println("\n" "================================");
    Serial.println(PROJECT_NAME);
    Serial.println("Version: " PROJECT_VERSION);
    Serial.println("Build: " FIRMWARE_BUILD_DATE);
    Serial.println("Power Type: " POWER_SENSOR_TYPE);
    Serial.println("================================\n");
    
    // Check for calibration mode
    if (calibrationMode) {
        Serial.println("⚙️  CALIBRATION MODE ENABLED ⚙️");
        Serial.println("The system will run calibration after initialization.");
        Serial.println("Send 'c' via Serial to run calibration anytime.\n");
    }
    
    // Initialize hardware components
    Serial.println("Initializing 5V DC power sensor...");
    sensorManager.begin();
    
    // Run calibration if enabled
    if (calibrationMode) {
        delay(2000); // Give time to see the message
        sensorManager.calibrate();
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
    
    // Initial sensor reading
    sensorManager.update();
}

void loop() {
    // Handle serial commands for calibration
    if (Serial.available()) {
        char command = Serial.read();
        if (command == 'c' || command == 'C') {
            Serial.println("Running calibration...");
            sensorManager.calibrate();
        } else if (command == 'd' || command == 'D') {
            Serial.println("Printing detailed sensor data...");
            sensorManager.printCalibrationData();
        }
    }
    
    // Check WiFi connection
    if (!wifiManager.isWiFiConnected()) {
        wifiManager.reconnect();
    }
    
    // Update sensors (this now includes fast sampling and averaging)
    sensorManager.update();
    
    // Periodic data transmission
    if (millis() - lastDataSend >= SENSOR_READ_INTERVAL) {
        lastDataSend = millis();
        
        // Print sensor data with 5V DC specific information
        sensorManager.printSensorData();
        
        // Scan WiFi networks
        Serial.println("\n--- WiFi Network Scan ---");
        int networkCount = wifiManager.scanNetworks();
        
        // Send data to Firebase with enhanced power information
        if (wifiManager.isWiFiConnected()) {
            Serial.println("\n--- Firebase Transmission ---");
            Serial.printf("Power Status: %s (%.2fV, ADC: %d)\n", 
                         sensorManager.getPowerStatus() ? "ON" : "OFF",
                         sensorManager.getInputVoltage(),
                         sensorManager.getPowerValue());
            Serial.printf("Power Quality: %s\n", 
                         sensorManager.isPowerQualityGood() ? "Good" : "Poor");
            
            bool success = firebaseClient.sendData(
                sensorManager.getPowerValue(),
                sensorManager.getPowerStatus(),
                networkCount,
                &wifiManager
            );
            
            if (success) {
                Serial.println("✅ Data transmission successful");
                Serial.println("   → Power status captured in database");
                Serial.println("   → System metrics recorded");
            } else {
                Serial.println("❌ Data transmission failed");
                Serial.println("   → Power status not recorded");
            }
        } else {
            Serial.println("❌ Skipping Firebase transmission - WiFi not connected");
            Serial.println("   → Power status not being recorded to database");
        }
        
        // Print enhanced system status with 5V DC info
        Serial.println("--- System Status ---");
        Serial.printf("Uptime: %lu ms\n", millis());
        Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
        Serial.printf("WiFi Status: %s\n", wifiManager.isWiFiConnected() ? "Connected" : "Disconnected");
        Serial.printf("5V DC Power: %s (%.2fV)\n", 
                     sensorManager.getPowerStatus() ? "ON" : "OFF", 
                     sensorManager.getInputVoltage());
        Serial.printf("Power Quality: %s\n", 
                     sensorManager.isPowerQualityGood() ? "Good" : "Poor");
        Serial.println("Commands: 'c' for calibration, 'd' for detailed data");
        Serial.println("========================\n");
    }
    
    // Small delay to prevent watchdog issues
    delay(100);
}
