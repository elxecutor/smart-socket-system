# IoT GPS & WiFi Monitor with Power Detection System

A complete IoT system for monitoring GPS location, WiFi networks, and external power with real-time data transmission to Firebase and interactive web visualization.

## Table of Contents
- [Features](#features)
- [External Power Detection](#external-power-detection)
- [GPS Tracking](#gps-tracking)
- [Installation](#installation)
- [Usage](#usage)
- [File Overview](#file-overview)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Features
- **Real-time GPS Tracking**: Precision location monitoring with NEO-6M module
- **External Power Monitoring**: Optocoupler-based power detection with statistics
- **WiFi Network Mapping**: Automatic scanning and RSSI measurement  
- **Real-time Data Storage**: Enhanced Firebase integration with comprehensive monitoring
- **Interactive Web Dashboard**: Live map with GPS, power, and WiFi data
- **Power Statistics**: Uptime tracking, state changes, and stability monitoring
- **Database Analytics**: Optimized data structure for monitoring and alerts
- **Debug Interface**: Serial commands for comprehensive system diagnostics

## External Power Detection

### Key Features
- **Optocoupler-based Detection**: Reliable external power monitoring
- **Debounced State Detection**: 50ms debounce prevents false triggers
- **Power Statistics**: Comprehensive uptime tracking and state change monitoring
- **Stability Analysis**: Real-time power stability assessment
- **Debug Interface**: Serial commands for monitoring and diagnostics
- **Enhanced Database Capture**: Detailed power metrics for analysis

### Detection System
- **Pin**: GPIO 4 (configurable)
- **Active Logic**: Active low (configurable)
- **Debounce Time**: 50ms (configurable)
- **Stability Threshold**: 5 seconds for stable state
- **Statistics Tracking**: Total on/off time, uptime percentage, state changes
- **Database Capture**: Boolean and string formats with comprehensive metrics

## GPS Tracking

### Key Features
- **NEO-6M GPS Module**: Professional-grade GPS tracking
- **Automatic Pin Assignment**: Uses ESP32 Serial2 (GPIO 16/17)
- **Location Validation**: Real-time GPS signal quality monitoring
- **Fallback System**: Default coordinates when GPS unavailable
- **Signal Quality**: Satellite count and signal strength monitoring
- **Time Synchronization**: GPS time integration for accurate timestamps

## Installation
Clone the repository and install dependencies:

```bash
git clone https://github.com/elxecutor/smart-socket-system
cd smart-socket-system
pip install platformio
```

## Usage
1. **Configure Firebase**:
   Edit `include/firebase-config.h` with your Firebase project details:
   - FIREBASE_HOST: your-project-default-rtdb.region.firebasedatabase.app
   - FIREBASE_AUTH: your-api-key

2. **Build and Upload**:
   ```bash
   # Build project
   pio run

   # Upload to ESP32
   pio run --target upload

   # Monitor serial output
   pio device monitor
   ```

3. **Web Dashboard Setup**:
   ```bash
   # Open web dashboard
   cd web
   python3 -m http.server 8000

   # Access dashboard at http://localhost:8000
   ```

### Hardware Requirements
- **ESP32 Development Board**: ESP32 DevKit v1 or compatible
- **NEO-6M GPS Module**: For location tracking
- **Optocoupler Module**: For external power detection (e.g., PC817, 4N35)
- **External Power Source**: Target power to monitor

### Hardware Connections
**GPS Module (NEO-6M):**
```
GPS Module    ESP32
VCC      →    3.3V
GND      →    GND
TX       →    GPIO 16 (RX2) - automatic
RX       →    GPIO 17 (TX2) - automatic
```

**Optocoupler:**
```
Optocoupler   ESP32
VCC      →    3.3V
GND      →    GND
Output   →    GPIO 4
```

### System Configuration
- **GPS Tracking**: Serial2 (GPIO 16/17), 9600 baud, automatic pin assignment
- **Power Monitoring**: GPIO 4, active low, 50ms debounce
- **WiFi Settings**: Configure SSID and password in config.h
- **Database Integration**: Enhanced Firebase capture with GPS and power data

### Serial Commands
- `g` or `G`: Display GPS status and location  
- `i` or `I`: Display detailed GPS debug information
- `p` or `P`: Display power status and statistics
- `o` or `O`: Display detailed power debug information
- `r` or `R`: Reset power statistics

## Project File Overview
```
├── src/main.cpp                 # Main application logic
├── lib/
│   ├── wifi_manager/           # WiFi connection management
│   │   ├── wifi_manager.h      # Interface definitions
│   │   └── wifi_manager.cpp    # Implementation
│   ├── gps_manager/            # GPS tracking functionality
│   │   ├── gps_manager.h       # GPS interface and data structures
│   │   └── gps_manager.cpp     # NEO-6M GPS module implementation
│   ├── optocoupler_manager/    # External power detection
│   │   ├── optocoupler_manager.h # Power monitoring interface
│   │   └── optocoupler_manager.cpp # Optocoupler implementation
│   └── firebase_client/        # Database communication
│       ├── firebase_client.h   # Firebase interface
│       └── firebase_client.cpp # HTTP POST implementation
├── include/
│   ├── config.h               # System configuration
│   └── firebase-config.h      # Firebase database settings
├── web/
│   └── index.html            # Web dashboard (future enhancement)
└── docs/
    └── README.md             # Additional documentation
```


## Contributing
We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) and [Code of Conduct](CODE_OF_CONDUCT.md) for details.

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## License
This project is licensed under the [MIT License](LICENSE).

## Contact
For questions or support, please open an issue or contact the maintainer via [X](https://x.com/elxecutor/) or [Glory Akanbi](https://www.linkedin.com/in/glory-akanbi-bba40b30a/).
