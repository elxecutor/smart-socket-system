// Firebase Configuration Template
// Replace these values with your actual Firebase project configuration

// IMPORTANT: For Firebase ESP32 Client, use only the host part (no https://)
// Using IP address to avoid DNS resolution issues in simulation environments
#define FIREBASE_HOST "esp8266-test-4ed5d-default-rtdb.europe-west1.firebasedatabase.app"
// "esp8266-test-4ed5d-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyAekZM44n4e3T14re3HwZaUAI8OjMDIIOY"

// Alternative: If you need the domain, use the original:
// #define FIREBASE_HOST "esp8266-test-4ed5d-default-rtdb.europe-west1.firebasedatabase.app"

/*
===========================================
Firebase Setup Instructions
===========================================

1. Go to Firebase Console (https://console.firebase.google.com/)
2. Create a new project or select existing project
3. Enable Realtime Database:
   - Go to "Realtime Database" in the left sidebar
   - Click "Create Database"
   - Choose "Start in test mode" for development
   - Select your preferred location

4. Get your configuration:
   - FIREBASE_HOST: Found in Realtime Database URL
     Example: "your-project-default-rtdb.firebaseio.com"

   - FIREBASE_AUTH: Database secret (legacy token)
     - Go to Project Settings > Service Accounts
     - Click "Database secrets" tab
     - Copy the secret key
     OR use Firebase Authentication token

5. Database Rules (for testing):
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   Note: Change to secure rules for production!

6. Data Structure:
   Your data will be stored as:
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

7. Reading Data:
   - Go to Firebase Console > Realtime Database
   - Browse the /iot-data/ node to see your data
   - Use Firebase REST API or SDK to fetch data for web app

===========================================
Security Notes
===========================================

- Never commit real Firebase credentials to public repositories
- Use environment variables or secure configuration management
- Implement proper Firebase Security Rules for production
- Consider using Firebase Authentication for user management
*/
