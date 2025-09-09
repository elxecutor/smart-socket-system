#include "led_manager.h"
#include "config.h"

LEDManager::LEDManager() : powerLedState(false), statusLedState(false) {
}

void LEDManager::begin() {
    pinMode(POWER_LED_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);
    
    // Initialize LEDs to OFF state
    digitalWrite(POWER_LED_PIN, LOW);
    digitalWrite(STATUS_LED_PIN, LOW);
    
    Serial.printf("LEDs initialized - Power: Pin %d, Status: Pin %d\n", 
                 POWER_LED_PIN, STATUS_LED_PIN);
}

void LEDManager::setPowerLED(bool state) {
    powerLedState = state;
    digitalWrite(POWER_LED_PIN, state ? HIGH : LOW);
}

void LEDManager::setStatusLED(bool state) {
    statusLedState = state;
    digitalWrite(STATUS_LED_PIN, state ? HIGH : LOW);
}

void LEDManager::blinkStatusLED(int times, int duration) {
    for (int i = 0; i < times; i++) {
        setStatusLED(true);
        delay(duration);
        setStatusLED(false);
        if (i < times - 1) {
            delay(duration);
        }
    }
}

void LEDManager::showConnecting() {
    // Blink status LED while connecting
    setStatusLED(!statusLedState);
}

void LEDManager::showSuccess() {
    // Quick double blink for success
    blinkStatusLED(LED_SUCCESS_BLINKS, 50);
    delay(50);
    setStatusLED(true); // Return to solid state
}

void LEDManager::showError() {
    // Long blink for error
    setStatusLED(false);
    delay(LED_ERROR_DURATION);
    setStatusLED(true);
}

void LEDManager::updatePowerStatus(bool powerAvailable) {
    setPowerLED(powerAvailable);
}
