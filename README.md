# IoT Power & WiFi Monitor System

A complete IoT system for monitoring power availability and WiFi networks with real-time data transmission to Firebase and interactive web visualization.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [File Overview](#file-overview)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Features
- **Power Monitoring**: Analog sensor reading with configurable threshold
- **WiFi Network Mapping**: Automatic scanning and RSSI measurement  
- **Real-time Data Storage**: Firebase Realtime Database integration
- **Visual Status Indicators**: LED feedback for system status
- **Interactive Web Dashboard**: Live map with power and WiFi data
- **Wokwi Simulation Support**: Complete circuit simulation

## Installation
Clone the repository and install dependencies:

```bash
git clone <repository>
cd iot-projects/web
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
- **Components**: Power sensor on pin 34, LEDs on pins 2 and 4

### System Configuration
- **Power Monitoring**: Pin 34, threshold 1000, interval 10s
- **WiFi Settings**: Configure SSID and password in config.h
- **LED Indicators**: Green on pin 2, Blue on pin 4

## File Overview
```
├── include/          # Header files and configuration
│   ├── config.h              # System configuration constants
│   ├── firebase-config.h     # Firebase credentials  
│   ├── wifi_manager.h        # WiFi management interface
│   ├── sensor_manager.h      # Sensor reading interface
│   ├── led_manager.h         # LED control interface
│   └── firebase_client.h     # Firebase REST API client
├── lib/              # Modular library implementations
│   ├── wifi_manager/         # WiFi connection & scanning
│   ├── sensor_manager/       # Power sensor management
│   ├── led_manager/          # LED status indicators
│   └── firebase_client/      # Firebase communication
├── src/              # Main application code
│   └── main.cpp              # Application entry point
├── web/              # Web dashboard files
│   ├── index-firebase.html   # Main dashboard
│   ├── config-template.js    # Configuration template
│   └── library.json          # Web library metadata
├── docs/             # Project documentation
│   ├── README.md             # This file
│   ├── FIREBASE_SETUP.md     # Firebase setup guide
│   └── FIREBASE_404_FIX.md   # Troubleshooting guide
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
