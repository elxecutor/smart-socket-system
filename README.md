# IoT Power & WiFi Monitor System - 5V DC Edition

A complete IoT system for monitoring 5V DC power availability and WiFi networks with real-time data transmission to Firebase and interactive web visualization.

## Table of Contents
- [Features](#features)
- [5V DC Power Detection](#5v-dc-power-detection)
- [Installation](#installation)
- [Usage](#usage)
- [File Overview](#file-overview)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Features
- **🔌 5V DC Power Monitoring**: Advanced voltage detection with precision measurement
- **📡 WiFi Network Mapping**: Automatic scanning and RSSI measurement  
- **☁️ Real-time Data Storage**: Enhanced Firebase integration with comprehensive power status capture
- **🌐 Interactive Web Dashboard**: Live map with power and WiFi data
- **⚙️ Calibration System**: Built-in calibration for optimal detection
- **📊 Power Quality Monitoring**: Voltage ripple and stability analysis
- **📱 Database Analytics**: Optimized data structure for monitoring and alerts

## 5V DC Power Detection

### Key Features
- **Accurate Voltage Measurement**: Real voltage calculation with 1.2mV precision
- **Smart Thresholds**: Configurable ON/OFF thresholds with hysteresis
- **Power Quality Assessment**: Monitors voltage stability and ripple
- **Calibration Mode**: Interactive calibration for optimal performance
- **Debug Interface**: Serial commands for monitoring and diagnostics
- **Enhanced Database Capture**: Multiple data formats for comprehensive monitoring

### Detection Thresholds
- **Power ON**: >4.52V (ADC: 3700)
- **Power OFF**: <3.66V (ADC: 3000)  
- **Hysteresis**: 0.85V band prevents oscillation
- **Quality Check**: <200mV ripple for "Good" rating
- **Database Capture**: Both boolean and string formats for optimal querying

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
- **5V DC Power Source**: Target voltage to monitor
- **Voltage Divider Components**: 
  - 1.8kΩ resistor (R1)
  - 3.3kΩ resistor (R2)
  - Optional: 10-100μF capacitor for filtering

### System Configuration
- **5V DC Monitoring**: Pin 34 with voltage divider, precision thresholds
- **WiFi Settings**: Configure SSID and password in config.h
- **Database Integration**: Enhanced Firebase capture with multiple data formats
- **Calibration**: Send 'c' command via serial for calibration
- **Debug Mode**: Send 'd' command for detailed sensor data

### Serial Commands
- `c` or `C`: Run calibration sequence  
- `d` or `D`: Display detailed measurement data

## File Overview
```
├── include/          # Header files and configuration
│   ├── config.h              # Main system configuration
│   ├── power-config.h        # 5V DC power detection settings
│   ├── firebase-config.h     # Firebase credentials  
│   ├── wifi_manager.h        # WiFi management interface
│   ├── sensor_manager.h      # Advanced sensor management
│   └── firebase_client.h     # Firebase REST API client
├── lib/              # Modular library implementations
│   ├── wifi_manager/         # WiFi connection & scanning
│   ├── sensor_manager/       # 5V DC power sensor with averaging
│   └── firebase_client/      # Firebase communication
├── src/              # Main application code
│   └── main.cpp              # Enhanced application with 5V DC support
├── web/              # Web dashboard files
│   ├── index.html            # Main dashboard
│   └── library.json          # Web library metadata
├── docs/             # Project documentation
│   ├── README.md                      # This file
│   ├── power-system-design.md         # 5V DC detection system guide
│   ├── DATABASE_CAPTURE_SPEC.md       # Database structure and query guide
│   ├── LED_REMOVAL_DATABASE_ENHANCEMENT_SUMMARY.md # Change summary
│   ├── SIMULATION_TEST_REPORT.md      # Testing and validation guide
│   ├── FIREBASE_SETUP.md              # Firebase setup guide
│   └── FIREBASE_404_FIX.md            # Troubleshooting guide
└── platformio.ini    # PlatformIO build configuration
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
For questions or support, please open an issue or contact the maintainer via [X](https://x.com/elxecutor/).
