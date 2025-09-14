#include "gps_manager.h"
#include "config.h"

GPSManager::GPSManager() {
    gpsSerial = nullptr;
    gpsInitialized = false;
    lastValidUpdate = 0;
    lastStatusCheck = 0;
    latitude = 0.0;
    longitude = 0.0;
    altitude = 0.0;
    speed = 0.0;
    locationValid = false;
    timeValid = false;
    gpsBaudRate = 9600;
    gpsTimeout = GPS_TIMEOUT_MS;
}

bool GPSManager::begin(HardwareSerial* serial, int baudRate) {
    if (!serial) {
        DEBUG_PRINTLN("❌ GPS Manager: Invalid serial port");
        return false;
    }
    
    gpsSerial = serial;
    gpsBaudRate = baudRate;
    
    // Initialize GPS serial communication
    gpsSerial->begin(gpsBaudRate);
    
    DEBUG_PRINTLN("🛰️  GPS Manager initialized");
    
    gpsInitialized = true;
    lastStatusCheck = millis();
    
    return true;
}

bool GPSManager::update() {
    if (!gpsInitialized || !gpsSerial) {
        return false;
    }
    
    bool newData = false;
    
    // Read available GPS data
    while (gpsSerial->available() > 0) {
        if (gps.encode(gpsSerial->read())) {
            newData = true;
            
            // Update location data if valid
            if (gps.location.isValid()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                locationValid = true;
                lastValidUpdate = millis();
                
                // Remove verbose location update messages
            }
            
            // Update altitude if valid
            if (gps.altitude.isValid()) {
                altitude = gps.altitude.meters();
            }
            
            // Update speed if valid
            if (gps.speed.isValid()) {
                speed = gps.speed.kmph();
            }
            
            // Update time validity
            timeValid = gps.date.isValid() && gps.time.isValid();
        }
    }
    
    // Check for GPS timeout - only warn occasionally, not continuously
    if (millis() - lastStatusCheck > 30000) { // Check every 30 seconds instead of 5
        lastStatusCheck = millis();
        
        if (millis() > 5000 && gps.charsProcessed() < 10) {
            DEBUG_PRINTLN("⚠️  GPS: No data received - check wiring");
            locationValid = false;
        }
    }
    
    // Check if location data is stale
    if (locationValid && (millis() - lastValidUpdate > gpsTimeout)) {
        locationValid = false;
        // Don't spam warnings about stale data
    }
    
    return newData;
}

bool GPSManager::isLocationValid() {
    return locationValid && gps.location.isValid();
}

bool GPSManager::isGPSActive() {
    return gpsInitialized && (gps.charsProcessed() > 10);
}

double GPSManager::getLatitude() {
    return isLocationValid() ? latitude : 0.0;
}

double GPSManager::getLongitude() {
    return isLocationValid() ? longitude : 0.0;
}

double GPSManager::getAltitude() {
    return (gps.altitude.isValid()) ? altitude : 0.0;
}

double GPSManager::getSpeed() {
    return (gps.speed.isValid()) ? speed : 0.0;
}

int GPSManager::getSatelliteCount() {
    return gps.satellites.isValid() ? gps.satellites.value() : 0;
}

String GPSManager::getFormattedDateTime() {
    if (!isTimeValid()) {
        return "INVALID";
    }
    
    char dateTime[32];
    snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02d %02d:%02d:%02d",
             gps.date.year(), gps.date.month(), gps.date.day(),
             gps.time.hour(), gps.time.minute(), gps.time.second());
    
    return String(dateTime);
}

unsigned long GPSManager::getTimeSinceLastUpdate() {
    return millis() - lastValidUpdate;
}

void GPSManager::printGPSStatus() {
    Serial.println("--- GPS Status ---");
    Serial.printf("GPS Active: %s\n", isGPSActive() ? "YES" : "NO");
    Serial.printf("Location Valid: %s\n", isLocationValid() ? "YES" : "NO");
    Serial.printf("Time Valid: %s\n", isTimeValid() ? "YES" : "NO");
    
    if (isLocationValid()) {
        Serial.printf("Latitude: %.6f°\n", getLatitude());
        Serial.printf("Longitude: %.6f°\n", getLongitude());
    } else {
        Serial.println("Location: INVALID");
    }
    
    if (gps.altitude.isValid()) {
        Serial.printf("Altitude: %.2f m\n", getAltitude());
    } else {
        Serial.println("Altitude: INVALID");
    }
    
    if (gps.speed.isValid()) {
        Serial.printf("Speed: %.2f km/h\n", getSpeed());
    } else {
        Serial.println("Speed: INVALID");
    }
    
    Serial.printf("Satellites: %d\n", getSatelliteCount());
    Serial.printf("Signal Quality: %s\n", getSignalQuality().c_str());
    Serial.printf("GPS Date&Time: %s\n", getFormattedDateTime().c_str());
    Serial.printf("Time Since Last Update: %lu ms\n", getTimeSinceLastUpdate());
    Serial.printf("Characters Processed: %lu\n", gps.charsProcessed());
    Serial.println("---");
}

void GPSManager::printDebugInfo() {
    Serial.println("--- GPS Debug Info ---");
    Serial.printf("GPS Initialized: %s\n", gpsInitialized ? "YES" : "NO");
    Serial.printf("Serial Port: %s\n", gpsSerial ? "Connected" : "NULL");
    Serial.printf("Baud Rate: %d\n", gpsBaudRate);
    Serial.printf("Timeout: %lu ms\n", gpsTimeout);
    Serial.printf("Characters Processed: %lu\n", gps.charsProcessed());
    Serial.printf("Sentences with Fix: %lu\n", gps.sentencesWithFix());
    Serial.printf("Failed Checksum: %lu\n", gps.failedChecksum());
    Serial.printf("Passed Checksum: %lu\n", gps.passedChecksum());
    
    printGPSStatus();
}

bool GPSManager::isTimeValid() {
    return timeValid;
}

String GPSManager::getSignalQuality() {
    if (!isGPSActive()) {
        return "NO_SIGNAL";
    }
    
    int satellites = getSatelliteCount();
    
    if (satellites >= 8) {
        return "EXCELLENT";
    } else if (satellites >= 6) {
        return "GOOD";
    } else if (satellites >= 4) {
        return "FAIR";
    } else if (satellites > 0) {
        return "POOR";
    } else {
        return "NO_SIGNAL";
    }
}