#include "optocoupler_manager.h"
#include "config.h"

OptocouplerManager::OptocouplerManager() {
    optocouplerPin = -1;
    activeLow = true;
    currentPowerState = false;
    lastRawState = false;
    previousPowerState = false;
    lastStateChangeTime = 0;
    debounceDelay = 50;
    powerOnTime = 0;
    powerOffTime = 0;
    lastPowerOnTimestamp = 0;
    lastPowerOffTimestamp = 0;
    stateChangeCount = 0;
}

bool OptocouplerManager::begin(int pin, bool activeLow, unsigned long debounceMs) {
    if (pin < 0) {
        DEBUG_PRINTLN("❌ OptocouplerManager: Invalid pin number");
        return false;
    }
    
    optocouplerPin = pin;
    this->activeLow = activeLow;
    debounceDelay = debounceMs;
    
    // Configure pin as input with pullup
    pinMode(optocouplerPin, INPUT_PULLUP);
    
    // Initialize state
    lastRawState = readRawState();
    currentPowerState = lastRawState;
    previousPowerState = currentPowerState;
    lastStateChangeTime = millis();
    
    DEBUG_PRINTLN("🔌 OptocouplerManager initialized");
    DEBUG_PRINTF("Pin: %d\n", optocouplerPin);
    DEBUG_PRINTF("Active Low: %s\n", activeLow ? "YES" : "NO");
    DEBUG_PRINTF("Debounce: %lu ms\n", debounceDelay);
    DEBUG_PRINTF("Initial State: %s\n", currentPowerState ? "POWER ON" : "POWER OFF");
    
    return true;
}

bool OptocouplerManager::update() {
    if (optocouplerPin < 0) {
        return false;
    }
    
    bool rawState = readRawState();
    bool stateChanged = false;
    
    // Check if raw state has changed
    if (rawState != lastRawState) {
        lastStateChangeTime = millis();
        lastRawState = rawState;
    }
    
    // Apply debouncing
    if ((millis() - lastStateChangeTime) >= debounceDelay) {
        if (rawState != currentPowerState) {
            previousPowerState = currentPowerState;
            currentPowerState = rawState;
            stateChanged = true;
            
            // Update statistics
            updateStatistics(currentPowerState);
            
            DEBUG_PRINTF("🔌 Power State Changed: %s\n", 
                        currentPowerState ? "POWER ON" : "POWER OFF");
        }
    }
    
    return stateChanged;
}

bool OptocouplerManager::readRawState() {
    if (optocouplerPin < 0) {
        return false;
    }
    
    bool pinState = digitalRead(optocouplerPin);
    
    // Apply active low logic if configured
    return activeLow ? !pinState : pinState;
}

void OptocouplerManager::updateStatistics(bool newState) {
    unsigned long currentTime = millis();
    stateChangeCount++;
    
    if (newState) {
        // Power turned ON
        lastPowerOnTimestamp = currentTime;
        if (!previousPowerState) {
            // Calculate time power was off
            if (lastPowerOffTimestamp > 0) {
                powerOffTime += (currentTime - lastPowerOffTimestamp);
            }
        }
    } else {
        // Power turned OFF
        lastPowerOffTimestamp = currentTime;
        if (previousPowerState) {
            // Calculate time power was on
            if (lastPowerOnTimestamp > 0) {
                powerOnTime += (currentTime - lastPowerOnTimestamp);
            }
        }
    }
}

bool OptocouplerManager::isPowerPresent() {
    return currentPowerState;
}

String OptocouplerManager::getPowerStatusString() {
    return currentPowerState ? "ON" : "OFF";
}

bool OptocouplerManager::hasStateChanged() {
    return currentPowerState != previousPowerState;
}

unsigned long OptocouplerManager::getTimeSinceLastChange() {
    return millis() - lastStateChangeTime;
}

unsigned long OptocouplerManager::getTotalPowerOnTime() {
    unsigned long totalOn = powerOnTime;
    
    // Add current session if power is currently on
    if (currentPowerState && lastPowerOnTimestamp > 0) {
        totalOn += (millis() - lastPowerOnTimestamp);
    }
    
    return totalOn;
}

unsigned long OptocouplerManager::getTotalPowerOffTime() {
    unsigned long totalOff = powerOffTime;
    
    // Add current session if power is currently off
    if (!currentPowerState && lastPowerOffTimestamp > 0) {
        totalOff += (millis() - lastPowerOffTimestamp);
    }
    
    return totalOff;
}

unsigned long OptocouplerManager::getStateChangeCount() {
    return stateChangeCount;
}

unsigned long OptocouplerManager::getLastPowerOnTime() {
    return lastPowerOnTimestamp;
}

unsigned long OptocouplerManager::getLastPowerOffTime() {
    return lastPowerOffTimestamp;
}

String OptocouplerManager::getPowerStability() {
    unsigned long timeSinceChange = getTimeSinceLastChange();
    
    if (timeSinceChange > OPTOCOUPLER_STABLE_TIME) {
        return "STABLE";
    } else if (timeSinceChange > debounceDelay * 2) {
        return "SETTLING";
    } else {
        return "UNSTABLE";
    }
}

void OptocouplerManager::printStatus() {
    Serial.println("--- Optocoupler Status ---");
    Serial.printf("External Power: %s\n", getPowerStatusString().c_str());
    Serial.printf("Power Stability: %s\n", getPowerStability().c_str());
    Serial.printf("Time Since Last Change: %lu ms\n", getTimeSinceLastChange());
    Serial.printf("State Changes: %lu\n", getStateChangeCount());
    
    if (currentPowerState) {
        Serial.printf("Current Power Session: %lu ms\n", 
                     lastPowerOnTimestamp > 0 ? millis() - lastPowerOnTimestamp : 0);
    } else {
        Serial.printf("Current Outage Duration: %lu ms\n", 
                     lastPowerOffTimestamp > 0 ? millis() - lastPowerOffTimestamp : 0);
    }
    
    Serial.printf("Total Power On Time: %lu ms\n", getTotalPowerOnTime());
    Serial.printf("Total Power Off Time: %lu ms\n", getTotalPowerOffTime());
    
    unsigned long totalTime = getTotalPowerOnTime() + getTotalPowerOffTime();
    if (totalTime > 0) {
        float uptime = (float)getTotalPowerOnTime() / totalTime * 100.0;
        Serial.printf("Power Uptime: %.1f%%\n", uptime);
    }
    
    Serial.println("---");
}

void OptocouplerManager::printDebugInfo() {
    Serial.println("--- Optocoupler Debug Info ---");
    Serial.printf("Configuration: %s\n", getConfigInfo().c_str());
    Serial.printf("Pin State (Raw): %s\n", getRawState() ? "HIGH" : "LOW");
    Serial.printf("Pin State (Digital): %s\n", 
                 optocouplerPin >= 0 ? (digitalRead(optocouplerPin) ? "HIGH" : "LOW") : "INVALID");
    Serial.printf("Power State (Processed): %s\n", currentPowerState ? "ON" : "OFF");
    Serial.printf("Last Raw State: %s\n", lastRawState ? "ON" : "OFF");
    Serial.printf("Previous Power State: %s\n", previousPowerState ? "ON" : "OFF");
    Serial.printf("Last State Change: %lu ms ago\n", getTimeSinceLastChange());
    Serial.printf("Debounce Delay: %lu ms\n", debounceDelay);
    Serial.printf("Last Power ON: %lu ms\n", lastPowerOnTimestamp);
    Serial.printf("Last Power OFF: %lu ms\n", lastPowerOffTimestamp);
    
    printStatus();
}

void OptocouplerManager::resetStatistics() {
    powerOnTime = 0;
    powerOffTime = 0;
    stateChangeCount = 0;
    lastPowerOnTimestamp = currentPowerState ? millis() : 0;
    lastPowerOffTimestamp = !currentPowerState ? millis() : 0;
    
    DEBUG_PRINTLN("🔌 Optocoupler statistics reset");
}

bool OptocouplerManager::getRawState() {
    return readRawState();
}

String OptocouplerManager::getConfigInfo() {
    char config[100];
    snprintf(config, sizeof(config), "Pin=%d, ActiveLow=%s, Debounce=%lums", 
             optocouplerPin, activeLow ? "YES" : "NO", debounceDelay);
    return String(config);
}