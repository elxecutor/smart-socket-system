#ifndef CONFIG_H
#define CONFIG_H

// Project Configuration
#define PROJECT_NAME "IoT Power & WiFi Monitor"
#define PROJECT_VERSION "1.0.0"
#define FIRMWARE_BUILD_DATE __DATE__ " " __TIME__

// Hardware Configuration
#define POWER_SENSOR_PIN 34
#define POWER_LED_PIN 2      // Green LED - Power status indicator
#define STATUS_LED_PIN 4     // Blue LED - WiFi/connection status
#define POWER_THRESHOLD 1000 // Analog value threshold for power detection

// Network Configuration
#define WIFI_SSID "el"
#define WIFI_PASSWORD "38a09a59aa48"
#define WIFI_CONNECTION_TIMEOUT 30000 // 30 seconds
#define WIFI_RETRY_DELAY 500

// Timing Configuration
#define SENSOR_READ_INTERVAL 10000    // 10 seconds between readings
#define WIFI_SCAN_INTERVAL 10000      // 10 seconds between WiFi scans
#define SERIAL_BAUD_RATE 115200
#define LED_BLINK_DURATION 100
#define LED_SUCCESS_BLINKS 2
#define LED_ERROR_DURATION 1000

// Data Configuration
#define JSON_BUFFER_SIZE 1024
#define MAX_WIFI_NETWORKS 20
#define DEFAULT_LATITUDE 52.5200
#define DEFAULT_LONGITUDE 13.4050

// HTTP Configuration
#define HTTP_TIMEOUT 15000        // 15 seconds timeout for HTTP requests
#define HTTP_MAX_RETRIES 3        // Maximum number of HTTP retry attempts

// Debug Configuration
#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(x, y...) Serial.printf(x, y)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, y...)
#endif

#endif // CONFIG_H
