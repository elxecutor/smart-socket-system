#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include "config.h"

class LEDManager {
private:
    bool powerLedState;
    bool statusLedState;
    
public:
    LEDManager();
    void begin();
    void setPowerLED(bool state);
    void setStatusLED(bool state);
    void blinkStatusLED(int times = 1, int duration = LED_BLINK_DURATION);
    void showConnecting();
    void showSuccess();
    void showError();
    void updatePowerStatus(bool powerAvailable);
};

#endif // LED_MANAGER_H
