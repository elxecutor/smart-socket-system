#include "sensor_manager.h"
#include "config.h"

SensorManager::SensorManager() : powerValue(0), powerStatus(false), lastReading(0) {
}

void SensorManager::begin() {
    pinMode(POWER_SENSOR_PIN, INPUT);
    DEBUG_PRINTF("Power sensor initialized on pin %d\n", POWER_SENSOR_PIN);
    DEBUG_PRINTF("Power threshold set to %d\n", POWER_THRESHOLD);
}

void SensorManager::update() {
    if (millis() - lastReading >= SENSOR_READ_INTERVAL) {
        powerValue = analogRead(POWER_SENSOR_PIN);
        powerStatus = powerValue > POWER_THRESHOLD;
        lastReading = millis();
        
        DEBUG_PRINTF("Power sensor updated: %d (%s)\n", 
                    powerValue, powerStatus ? "ON" : "OFF");
    }
}

int SensorManager::getPowerValue() {
    return powerValue;
}

bool SensorManager::getPowerStatus() {
    return powerStatus;
}

void SensorManager::printSensorData() {
    Serial.println("\n--- Sensor Reading ---");
    Serial.printf("Power Value: %d\n", powerValue);
    Serial.printf("Power Status: %s\n", powerStatus ? "ON" : "OFF");
    Serial.printf("Threshold: %d\n", POWER_THRESHOLD);
    Serial.printf("Last Reading: %lu ms ago\n", millis() - lastReading);
}
