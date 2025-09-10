#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "power-config.h"

class SensorManager {
private:
    // Power detection variables
    int powerValue;
    float inputVoltage;
    bool powerStatus;
    PowerState currentPowerState;
    unsigned long lastReading;
    unsigned long lastStateChange;
    
    // Power averaging and stability
    int powerSamples[POWER_AVERAGE_SAMPLES];
    int sampleIndex;
    bool samplesValid;
    
    // Private methods
    int getAveragePowerReading();
    float calculateInputVoltage(int adcValue);
    PowerState determinePowerState(int adcValue);
    bool isStateStable(PowerState newState);
    
public:
    SensorManager();
    void begin();
    void update();
    void calibrate(); // For 5V DC calibration
    
    // Power reading methods
    int getPowerValue();
    float getInputVoltage();
    bool getPowerStatus();
    PowerState getPowerState();
    
    // Diagnostic methods
    void printSensorData();
    void printCalibrationData();
    bool isPowerQualityGood();
    float getPowerRipple();
};

#endif // SENSOR_MANAGER_H
