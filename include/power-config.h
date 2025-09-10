#ifndef POWER_CONFIG_H
#define POWER_CONFIG_H

// Power Detection Configuration for 5V DC System
// ================================================

// Hardware Configuration
#define POWER_SENSOR_PIN 34             // ADC pin for power detection
#define POWER_SENSOR_TYPE "5V_DC"       // Power type identifier

// 5V DC Detection Parameters
// ESP32 ADC: 0-4095 (12-bit) corresponds to 0-3.3V input
// For 5V detection, we need voltage divider: 5V -> 3.3V max
// Voltage divider ratio: 3.3V/5V = 0.66
// So 5V input = ~2700 ADC reading (5V * 0.66 * 4095/3.3V)

#define VOLTAGE_DIVIDER_RATIO 0.66f     // Voltage divider for 5V -> 3.3V
#define ADC_MAX_VALUE 4095              // 12-bit ADC maximum
#define ADC_REFERENCE_VOLTAGE 3.3f      // ESP32 ADC reference voltage

// Power Detection Thresholds
#define POWER_ON_THRESHOLD 3700         // ADC value for 5V present (~4.5V actual)
#define POWER_OFF_THRESHOLD 3000        // ADC value for power lost (~3.6V actual)
#define POWER_HYSTERESIS 700            // Hysteresis to prevent oscillation

// Voltage Calculation Macros
#define ADC_TO_VOLTAGE(adc) ((float)(adc) * ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE)
#define ADC_TO_INPUT_VOLTAGE(adc) (ADC_TO_VOLTAGE(adc) / VOLTAGE_DIVIDER_RATIO)

// Power Detection Timing
#define POWER_DEBOUNCE_TIME 100         // ms to debounce power state changes
#define POWER_SAMPLE_INTERVAL 50        // ms between power samples
#define POWER_AVERAGE_SAMPLES 10        // Number of samples to average

// Safety and Validation
#define MIN_VALID_5V 4.5f              // Minimum voltage to consider as valid 5V
#define MAX_VALID_5V 5.5f              // Maximum voltage to consider as valid 5V
#define POWER_STABILITY_TIME 1000       // ms to wait for stable power reading

// Debug Configuration
#define POWER_DEBUG_ENABLED true        // Enable power detection debugging
#define POWER_CALIBRATION_MODE false    // Enable calibration mode

// Power State Definitions
enum PowerState {
    POWER_STATE_UNKNOWN = -1,
    POWER_STATE_OFF = 0,
    POWER_STATE_ON = 1,
    POWER_STATE_UNSTABLE = 2
};

// Power Quality Metrics
#define POWER_QUALITY_SAMPLES 50        // Samples for power quality analysis
#define ACCEPTABLE_VOLTAGE_RIPPLE 0.2f  // Maximum acceptable voltage ripple (V)

#endif // POWER_CONFIG_H
