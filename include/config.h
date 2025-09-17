#ifndef CONFIG_H
#define CONFIG_H

// Project Configuration
#define PROJECT_NAME "IoT GPS & WiFi Monitor with Power Detection"
#define PROJECT_VERSION "2.1.0"
#define FIRMWARE_BUILD_DATE __DATE__ " " __TIME__

// Network Configuration
#define WIFI_SSID "GL"
#define WIFI_PASSWORD "98754321"
#define WIFI_CONNECTION_TIMEOUT 30000 // 30 seconds
#define WIFI_RETRY_DELAY 500

// Timing Configuration
#define SENSOR_READ_INTERVAL 10000    // 10 seconds between readings
#define WIFI_SCAN_INTERVAL 10000      // 10 seconds between WiFi scans
#define SERIAL_BAUD_RATE 115200

// GPS Configuration
#define GPS_BAUDRATE 9600             // Default NEO-6M GPS module baud rate
#define GPS_TIMEOUT_MS 30000          // 30 seconds timeout for GPS data
#define GPS_UPDATE_INTERVAL 1000      // 1 second between GPS updates

// Optocoupler Configuration
#define OPTOCOUPLER_PIN 34         // GPIO pin connected to optocoupler output
#define OPTOCOUPLER_ACTIVE_LOW true   // Optocoupler output is active low
#define OPTOCOUPLER_DEBOUNCE_MS 50    // Debounce time for power state changes
#define OPTOCOUPLER_STABLE_TIME 5000  // Time to consider power state stable (5 seconds)

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
