#include "sensor_manager.h"
#include "config.h"

SensorManager::SensorManager() 
    : powerValue(0), inputVoltage(0.0), powerStatus(false), 
      currentPowerState(POWER_STATE_UNKNOWN), lastReading(0), lastStateChange(0),
      sampleIndex(0), samplesValid(false) {
    // Initialize sample array
    for (int i = 0; i < POWER_AVERAGE_SAMPLES; i++) {
        powerSamples[i] = 0;
    }
}

void SensorManager::begin() {
    pinMode(POWER_SENSOR_PIN, INPUT);
    
    DEBUG_PRINTF("5V DC Power sensor initialized on pin %d\n", POWER_SENSOR_PIN);
    DEBUG_PRINTF("Power ON threshold: %d (%.2fV)\n", POWER_ON_THRESHOLD, 
                 ADC_TO_INPUT_VOLTAGE(POWER_ON_THRESHOLD));
    DEBUG_PRINTF("Power OFF threshold: %d (%.2fV)\n", POWER_OFF_THRESHOLD,
                 ADC_TO_INPUT_VOLTAGE(POWER_OFF_THRESHOLD));
    DEBUG_PRINTF("Voltage divider ratio: %.2f\n", VOLTAGE_DIVIDER_RATIO);
    
    // Take initial samples to fill the averaging buffer
    for (int i = 0; i < POWER_AVERAGE_SAMPLES; i++) {
        powerSamples[i] = analogRead(POWER_SENSOR_PIN);
        delay(10);
    }
    samplesValid = true;
    
    // Initial reading
    update();
}

void SensorManager::update() {
    if (millis() - lastReading >= POWER_SAMPLE_INTERVAL) {
        // Read new sample and add to circular buffer
        powerSamples[sampleIndex] = analogRead(POWER_SENSOR_PIN);
        sampleIndex = (sampleIndex + 1) % POWER_AVERAGE_SAMPLES;
        
        // Update averaged values
        powerValue = getAveragePowerReading();
        inputVoltage = calculateInputVoltage(powerValue);
        
        // Determine power state with hysteresis
        PowerState newState = determinePowerState(powerValue);
        
        // Only change state if stable for debounce time
        if (isStateStable(newState)) {
            if (newState != currentPowerState) {
                currentPowerState = newState;
                powerStatus = (newState == POWER_STATE_ON);
                lastStateChange = millis();
                
                if (POWER_DEBUG_ENABLED) {
                    DEBUG_PRINTF("Power state changed to: %s (%.2fV, ADC: %d)\n",
                                powerStatus ? "ON" : "OFF", inputVoltage, powerValue);
                }
            }
        }
        
        lastReading = millis();
        
        if (POWER_DEBUG_ENABLED && (millis() % 5000 < POWER_SAMPLE_INTERVAL)) {
            DEBUG_PRINTF("Power: %.2fV (ADC: %d, %s)\n", 
                        inputVoltage, powerValue, powerStatus ? "ON" : "OFF");
        }
    }
}

int SensorManager::getAveragePowerReading() {
    if (!samplesValid) return 0;
    
    long sum = 0;
    for (int i = 0; i < POWER_AVERAGE_SAMPLES; i++) {
        sum += powerSamples[i];
    }
    return sum / POWER_AVERAGE_SAMPLES;
}

float SensorManager::calculateInputVoltage(int adcValue) {
    return ADC_TO_INPUT_VOLTAGE(adcValue);
}

PowerState SensorManager::determinePowerState(int adcValue) {
    if (adcValue >= POWER_ON_THRESHOLD) {
        return POWER_STATE_ON;
    } else if (adcValue <= POWER_OFF_THRESHOLD) {
        return POWER_STATE_OFF;
    } else {
        // In hysteresis zone - maintain current state
        return currentPowerState;
    }
}

bool SensorManager::isStateStable(PowerState newState) {
    return (millis() - lastStateChange) >= POWER_DEBOUNCE_TIME;
}

void SensorManager::calibrate() {
    Serial.println("\n=== 5V DC Power Sensor Calibration ===");
    Serial.println("Taking 100 samples for calibration...");
    
    long sum = 0;
    int minVal = 4095, maxVal = 0;
    
    for (int i = 0; i < 100; i++) {
        int reading = analogRead(POWER_SENSOR_PIN);
        sum += reading;
        minVal = min(minVal, reading);
        maxVal = max(maxVal, reading);
        
        if (i % 10 == 0) {
            Serial.printf("Sample %d: ADC=%d, Voltage=%.2fV\n", 
                         i, reading, calculateInputVoltage(reading));
        }
        delay(50);
    }
    
    int avgReading = sum / 100;
    float avgVoltage = calculateInputVoltage(avgReading);
    float ripple = calculateInputVoltage(maxVal) - calculateInputVoltage(minVal);
    
    Serial.printf("\nCalibration Results:\n");
    Serial.printf("Average ADC: %d\n", avgReading);
    Serial.printf("Average Voltage: %.2fV\n", avgVoltage);
    Serial.printf("Min/Max ADC: %d/%d\n", minVal, maxVal);
    Serial.printf("Voltage Ripple: %.2fV\n", ripple);
    Serial.printf("Recommended thresholds for 5V detection:\n");
    Serial.printf("  ON threshold: %d (%.2fV)\n", (int)(avgReading * 0.9), avgVoltage * 0.9);
    Serial.printf("  OFF threshold: %d (%.2fV)\n", (int)(avgReading * 0.7), avgVoltage * 0.7);
    Serial.println("=================================\n");
}

int SensorManager::getPowerValue() {
    return powerValue;
}

float SensorManager::getInputVoltage() {
    return inputVoltage;
}

bool SensorManager::getPowerStatus() {
    return powerStatus;
}

PowerState SensorManager::getPowerState() {
    return currentPowerState;
}

bool SensorManager::isPowerQualityGood() {
    return getPowerRipple() <= ACCEPTABLE_VOLTAGE_RIPPLE;
}

float SensorManager::getPowerRipple() {
    if (!samplesValid) return 0.0;
    
    int minSample = 4095, maxSample = 0;
    for (int i = 0; i < POWER_AVERAGE_SAMPLES; i++) {
        minSample = min(minSample, powerSamples[i]);
        maxSample = max(maxSample, powerSamples[i]);
    }
    
    return calculateInputVoltage(maxSample) - calculateInputVoltage(minSample);
}

void SensorManager::printSensorData() {
    Serial.println("\n--- 5V DC Power Sensor Reading ---");
    Serial.printf("ADC Value: %d (averaged)\n", powerValue);
    Serial.printf("Input Voltage: %.2fV\n", inputVoltage);
    Serial.printf("Power Status: %s\n", powerStatus ? "ON" : "OFF");
    Serial.printf("Power State: %d\n", currentPowerState);
    Serial.printf("Voltage Ripple: %.2fV\n", getPowerRipple());
    Serial.printf("Power Quality: %s\n", isPowerQualityGood() ? "Good" : "Poor");
    Serial.printf("Thresholds: ON=%.2fV, OFF=%.2fV\n", 
                 ADC_TO_INPUT_VOLTAGE(POWER_ON_THRESHOLD),
                 ADC_TO_INPUT_VOLTAGE(POWER_OFF_THRESHOLD));
    Serial.printf("Last State Change: %lu ms ago\n", millis() - lastStateChange);
    Serial.printf("Samples Valid: %s\n", samplesValid ? "Yes" : "No");
}

void SensorManager::printCalibrationData() {
    Serial.println("\n--- Power Sensor Calibration Data ---");
    Serial.printf("Current samples (last %d readings):\n", POWER_AVERAGE_SAMPLES);
    for (int i = 0; i < POWER_AVERAGE_SAMPLES; i++) {
        int idx = (sampleIndex + i) % POWER_AVERAGE_SAMPLES;
        Serial.printf("  [%d]: ADC=%d, V=%.2f\n", i, powerSamples[idx], 
                     calculateInputVoltage(powerSamples[idx]));
    }
}
