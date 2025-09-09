#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "config.h"

class SensorManager {
private:
    int powerValue;
    bool powerStatus;
    unsigned long lastReading;
    
public:
    SensorManager();
    void begin();
    void update();
    int getPowerValue();
    bool getPowerStatus();
    bool hasNewReading();
    void printSensorData();
};

#endif // SENSOR_MANAGER_H
