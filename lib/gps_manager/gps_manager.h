#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

/**
 * GPS Manager Class
 * 
 * Manages GPS functionality using TinyGPS++ library
 * Provides location data, time synchronization, and GPS status monitoring
 */
class GPSManager {
private:
    TinyGPSPlus gps;
    HardwareSerial* gpsSerial;
    
    // GPS status tracking
    bool gpsInitialized;
    unsigned long lastValidUpdate;
    unsigned long lastStatusCheck;
    
    // GPS data
    double latitude;
    double longitude;
    double altitude;
    double speed;
    bool locationValid;
    bool timeValid;
    
    // Configuration
    int gpsBaudRate;
    unsigned long gpsTimeout;
    
public:
    /**
     * Constructor
     */
    GPSManager();
    
    /**
     * Initialize GPS manager
     * @param serial Pointer to hardware serial port (e.g., &Serial2)
     * @param baudRate GPS module baud rate (default: 9600)
     * @return true if initialization successful
     */
    bool begin(HardwareSerial* serial, int baudRate = 9600);
    
    /**
     * Update GPS data (call frequently in main loop)
     * @return true if new valid data was received
     */
    bool update();
    
    /**
     * Check if GPS has valid location data
     * @return true if location is valid and recent
     */
    bool isLocationValid();
    
    /**
     * Check if GPS module is responding
     * @return true if GPS module is responding
     */
    bool isGPSActive();
    
    /**
     * Get current latitude
     * @return latitude in decimal degrees (returns 0.0 if invalid)
     */
    double getLatitude();
    
    /**
     * Get current longitude
     * @return longitude in decimal degrees (returns 0.0 if invalid)
     */
    double getLongitude();
    
    /**
     * Get current altitude
     * @return altitude in meters (returns 0.0 if invalid)
     */
    double getAltitude();
    
    /**
     * Get current speed
     * @return speed in km/h (returns 0.0 if invalid)
     */
    double getSpeed();
    
    /**
     * Get number of satellites in view
     * @return satellite count (0 if no GPS data)
     */
    int getSatelliteCount();
    
    /**
     * Get GPS time as formatted string
     * @return formatted time string (YYYY-MM-DD HH:MM:SS)
     */
    String getFormattedDateTime();
    
    /**
     * Get time since last valid GPS update
     * @return milliseconds since last valid update
     */
    unsigned long getTimeSinceLastUpdate();
    
    /**
     * Print GPS status and data to Serial
     */
    void printGPSStatus();
    
    /**
     * Print detailed GPS information for debugging
     */
    void printDebugInfo();
    
    /**
     * Check if GPS has valid time data
     * @return true if GPS time is valid
     */
    bool isTimeValid();
    
    /**
     * Get GPS quality indicator
     * @return "EXCELLENT", "GOOD", "FAIR", "POOR", or "NO_SIGNAL"
     */
    String getSignalQuality();
};

#endif // GPS_MANAGER_H