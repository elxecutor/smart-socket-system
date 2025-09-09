# IoT Power Supply + WiFi Mapping System

This project uses an **ESP32** to:
- Monitor **power supply availability**.  
- Scan nearby WiFi networks (SSID + RSSI).  
- Store data permanently in **Google Sheets** via Apps Script.  
- Display results on a **Leaflet map web app** in JSON format.

## Project Structure

```
├── platformio.ini              # PlatformIO configuration
├── src/
│   └── main.cpp               # ESP32 firmware
├── google-apps-script.js      # Google Apps Script code
├── index.html                 # Web frontend with Leaflet map
└── README.md                  # This file
```

## 🔹 1. ESP32 Firmware Setup

### Hardware Requirements
- ESP32 development board
- Power monitoring circuit connected to GPIO 34

### Software Requirements
- PlatformIO installed
- Arduino framework for ESP32

### Configuration
1. Edit `src/main.cpp` and update:
   ```cpp
   const char* WIFI_SSID = "Your_WiFi_Network";
   const char* WIFI_PASSWORD = "Your_WiFi_Password";
   const char* SCRIPT_URL = "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec";
   ```

### Build and Upload
```bash
pio run --target upload
```

## 🔹 1.5. Wokwi Simulation

### Quick Start with Wokwi
You can test this project online using Wokwi simulator without any physical hardware:

1. **Open Wokwi**: Go to [wokwi.com](https://wokwi.com)
2. **Create New Project**: Choose "ESP32" template
3. **Upload Files**: 
   - Replace the code with `src/main.cpp`
   - Upload `diagram.json` for the circuit layout
   - Upload `wokwi.toml` for configuration
4. **Start Simulation**: Click the play button

### Circuit Components
The Wokwi simulation includes:
- **ESP32 DevKit v1** - Main microcontroller
- **Green LED (Pin 2)** - Power status indicator
- **Blue LED (Pin 4)** - WiFi/transmission status indicator  
- **Potentiometer (Pin 34)** - Simulates power sensor (0-4095)
- **Push Button** - Reset button for ESP32
- **Resistors** - Current limiting for LEDs and pull-up for reset

### Visual Indicators
- **Green LED**: ON when power value > 1000, OFF otherwise
- **Blue LED**: 
  - Blinking during WiFi connection
  - Solid when connected to WiFi
  - Quick double flash on successful data transmission
  - Long flash on transmission error

### Testing the Power Sensor
- Rotate the potentiometer to simulate different power levels
- Values above ~50% (≈1000) will turn the green LED ON
- Watch the serial monitor for detailed power readings

### WiFi Simulation
Wokwi automatically provides a simulated WiFi environment with the SSID "Wokwi-GUEST" (no password required).

## 🔹 2. Firebase Setup (Recommended)

### Firebase Realtime Database Configuration

1. **Create Firebase Project**:
   - Go to [Firebase Console](https://console.firebase.google.com/)
   - Click "Create a project" or select existing project
   - Enable Google Analytics (optional)

2. **Setup Realtime Database**:
   - In Firebase Console, go to "Realtime Database"
   - Click "Create Database"
   - Choose "Start in test mode" for development
   - Select your preferred location (e.g., us-central1)

3. **Get Configuration**:
   - **Firebase Host**: Copy from Database URL (e.g., `your-project-default-rtdb.firebaseio.com`)
   - **Authentication**: 
     - Go to Project Settings > Service Accounts
     - Database secrets tab → Copy the secret key
     - OR use Firebase Authentication token

4. **Update ESP32 Code**:
   ```cpp
   #define FIREBASE_HOST "your-project-default-rtdb.firebaseio.com"
   #define FIREBASE_AUTH "your-database-secret-key"
   ```

5. **Database Security Rules** (for testing):
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
   ⚠️ **Important**: Change to secure rules for production!

6. **Data Structure**:
   Your data will be stored as:
   ```
   /iot-data/
     ├── 1725123456789/          # timestamp
     │   ├── timestamp: 1725123456789
     │   ├── power/
     │   │   ├── status: "ON"
     │   │   └── value: 1250
     │   ├── location/
     │   │   ├── lat: 52.5200
     │   │   └── lng: 13.4050
     │   └── wifi/
     │       ├── 0/
     │       │   ├── ssid: "Wokwi-GUEST"
     │       │   └── rssi: -45
     │       └── 1/
     │           ├── ssid: "MyNetwork"
     │           └── rssi: -67
   ```

### Firebase Web App Setup

1. **Configure Firebase Web App**:
   - In Firebase Console, go to Project Settings
   - Scroll to "Your apps" → Click "Web app" icon
   - Register app and copy the config object

2. **Update `index-firebase.html`**:
   ```javascript
   const firebaseConfig = {
     apiKey: "your-api-key",
     authDomain: "your-project.firebaseapp.com", 
     databaseURL: "https://your-project-default-rtdb.firebaseio.com",
     projectId: "your-project-id",
     // ... other config
   };
   ```

3. **Features**:
   - Real-time data updates
   - Interactive map with WiFi network markers
   - Power status dashboard
   - WiFi network list with signal strength indicators
   - Auto-refresh every 10 seconds

## 🔹 3. Google Apps Script Setup (Alternative)

1. Create a new Google Sheet
2. Go to `Extensions > Apps Script`
3. Replace the default code with the content from `google-apps-script.js`
4. Deploy as Web App:
   - Click `Deploy > New deployment`
   - Choose `Web app` as type
   - Set execute as: `Me`
   - Set access: `Anyone`
   - Copy the deployment URL
5. Update the `SCRIPT_URL` in your ESP32 code with this URL

### Sheet Structure
The Google Sheet will have the following columns:
| timestamp | power | ssid | rssi | lat | lng |
|-----------|-------|------|-----|-----|

## 🔹 4. Web App Setup

## 🔹 3. Web App Setup

1. Edit `index.html` and replace `YOUR_SHEET_ID` with your Google Sheet ID
2. The Sheet ID can be found in the Google Sheets URL:
   ```
   https://docs.google.com/spreadsheets/d/SHEET_ID_HERE/edit
   ```
3. Open `index.html` in a web browser or serve it using a web server

### Local Development Server
```bash
# Using Python
python -m http.server 8000

# Using Node.js (if you have serve installed)
npx serve .
```

## 🔹 4. Workflow

1. **ESP32** scans WiFi + reads power status → formats JSON
2. **ESP32** sends JSON via HTTP POST → Google Apps Script  
3. **Script** logs data into Google Sheets
4. **Web app** fetches data from Google Sheets →
   - Shows **power status**
   - Displays **WiFi networks + RSSI** on a **Leaflet map**
   - Auto-refreshes every 10 seconds

## 🔹 5. Features

- **Real-time monitoring**: ESP32 sends data every 10 seconds
- **Power status tracking**: Monitors analog pin for power availability
- **WiFi network mapping**: Scans and maps nearby WiFi networks with RSSI values
- **Persistent storage**: All data stored permanently in Google Sheets
- **Interactive map**: Leaflet-based web interface with markers for WiFi networks
- **Auto-refresh**: Web app updates automatically every 10 seconds

## 🔹 6. Customization

### Changing Location Coordinates
Edit the coordinates in `src/main.cpp`:
```cpp
net["lat"] = YOUR_LATITUDE;
net["lng"] = YOUR_LONGITUDE;
```

### Adjusting Power Threshold
Modify the power detection threshold:
```cpp
bool powerAvailable = powerValue > YOUR_THRESHOLD;
```

### Changing Update Frequency
Adjust the delay in the ESP32 loop:
```cpp
delay(YOUR_DELAY_MS); // Default: 10000 (10 seconds)
```

## 🔹 7. Troubleshooting

### ESP32 Not Connecting to WiFi
- Check WiFi credentials
- Verify network is available
- Check ESP32 power supply

### Data Not Appearing in Google Sheets
- Verify Google Apps Script deployment URL
- Check that the script is deployed as "Anyone" can access
- Check ESP32 serial output for HTTP response codes

### Web Map Not Loading
- Verify Google Sheet ID in `index.html`
- Check that the sheet is publicly readable
- Open browser developer tools to check for errors

## 🔹 8. Security Considerations

- The Google Apps Script is set to "Anyone" access for simplicity
- Consider implementing authentication for production use
- WiFi credentials are stored in plain text - use secure methods for production
- The Google Sheet data is publicly readable via CSV export

## ✅ Deliverables

✅ ESP32 firmware (`src/main.cpp`)  
✅ Google Apps Script (`google-apps-script.js`)  
✅ Web frontend (`index.html`)  
✅ PlatformIO configuration (`platformio.ini`)  
✅ Complete documentation (`README.md`)

This provides a **complete IoT pipeline** for real-time power and WiFi monitoring with web-based visualization.
