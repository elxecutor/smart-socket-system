#ifndef OPTOCOUPLER_MANAGER_H
#define OPTOCOUPLER_MANAGER_H

#include <Arduino.h>
#include "config.h"

/**
 * OptocouplerManager Class
 * 
 * Manages external power detection using an optocoupler
 * Provides debounced power state detection and status monitoring
 */
class OptocouplerManager {
private:
    // Hardware configuration
    int optocouplerPin;
    bool activeLow;
    
    // State management
    bool currentPowerState;
    bool lastRawState;
    bool previousPowerState;
    
    // Debouncing
    unsigned long lastStateChangeTime;
    unsigned long debounceDelay;
    
    // Statistics
    unsigned long powerOnTime;
    unsigned long powerOffTime;
    unsigned long lastPowerOnTimestamp;
    unsigned long lastPowerOffTimestamp;
    unsigned long stateChangeCount;
    
    // Internal methods
    bool readRawState();
    void updateStatistics(bool newState);
    
public:
    /**
     * Constructor
     */
    OptocouplerManager();
    
    /**
     * Initialize optocoupler manager
     * @param pin GPIO pin connected to optocoupler output
     * @param activeLow true if optocoupler output is active low (default: true)
     * @param debounceMs debounce time in milliseconds (default: 50ms)
     * @return true if initialization successful
     */
    bool begin(int pin, bool activeLow = true, unsigned long debounceMs = 50);
    
    /**
     * Update optocoupler state (call frequently in main loop)
     * @return true if power state changed
     */
    bool update();
    
    /**
     * Get current external power status
     * @return true if external power is detected
     */
    bool isPowerPresent();
    
    /**
     * Get current power state as string
     * @return "ON" or "OFF"
     */
    String getPowerStatusString();
    
    /**
     * Check if power state has changed since last check
     * @return true if state changed
     */
    bool hasStateChanged();
    
    /**
     * Get time since last power state change
     * @return milliseconds since last state change
     */
    unsigned long getTimeSinceLastChange();
    
    /**
     * Get total time power has been on since startup
     * @return milliseconds power has been on
     */
    unsigned long getTotalPowerOnTime();
    
    /**
     * Get total time power has been off since startup
     * @return milliseconds power has been off
     */
    unsigned long getTotalPowerOffTime();
    
    /**
     * Get number of power state changes since startup
     * @return number of state changes
     */
    unsigned long getStateChangeCount();
    
    /**
     * Get timestamp of last power ON event
     * @return millis() timestamp of last power on
     */
    unsigned long getLastPowerOnTime();
    
    /**
     * Get timestamp of last power OFF event
     * @return millis() timestamp of last power off
     */
    unsigned long getLastPowerOffTime();
    
    /**
     * Get power stability indicator
     * @return "STABLE", "UNSTABLE", or "UNKNOWN"
     */
    String getPowerStability();
    
    /**
     * Print optocoupler status to Serial
     */
    void printStatus();
    
    /**
     * Print detailed optocoupler information for debugging
     */
    void printDebugInfo();
    
    /**
     * Reset statistics counters
     */
    void resetStatistics();
    
    /**
     * Get raw optocoupler pin state (without debouncing)
     * @return raw digital pin state
     */
    bool getRawState();
    
    /**
     * Get optocoupler configuration info
     * @return configuration string for debugging
     */
    String getConfigInfo();
};

#endif // OPTOCOUPLER_MANAGER_H