/*
Configuration Guide for IoT Power Supply + WiFi Mapping System

===========================================
1. ESP32 Configuration (src/main.cpp)
===========================================

Replace these values in src/main.cpp:

const char* WIFI_SSID = "YOUR_WIFI_NETWORK_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* SCRIPT_URL = "https://script.google.com/macros/s/XXXX/exec";

Power monitoring settings:
- powerPin = 34 (GPIO 34 for analog input)
- Power threshold = 1000 (adjust based on your circuit)
- Update interval = 10000 ms (10 seconds)

Location coordinates (replace with your location):
- net["lat"] = 52.5200;  // Berlin latitude
- net["lng"] = 13.4050;  // Berlin longitude

===========================================
2. Web App Configuration (index.html)
===========================================

Replace YOUR_SHEET_ID in index.html:
const sheetUrl = "https://docs.google.com/spreadsheets/d/YOUR_SHEET_ID/gviz/tq?tqx=out:csv";

Find YOUR_SHEET_ID in your Google Sheets URL:
https://docs.google.com/spreadsheets/d/YOUR_SHEET_ID/edit

Map settings:
- Center: [52.52, 13.40] (Berlin)
- Zoom level: 13
- Auto-refresh: 10 seconds

// ===========================================
// Google Apps Script Setup Instructions
// ===========================================
/*
1. Create a new Google Sheet
2. Go to Extensions > Apps Script  
3. Copy the code from google-apps-script.js
4. Deploy as Web App:
   - Click Deploy > New deployment
   - Choose "Web app" as type
   - Execute as: Me
   - Who has access: Anyone
   - Copy the deployment URL
5. Use this URL as SCRIPT_URL in your ESP32 code
*/

// ===========================================
// Google Sheets Column Structure
// ===========================================
/*
Column A: timestamp (automatic)
Column B: power (ON/OFF)
Column C: ssid (WiFi network name)
Column D: rssi (signal strength)
Column E: lat (latitude)
Column F: lng (longitude)
*/
