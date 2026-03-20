# 🚪 Smart Door — IoT Face Recognition Access Control

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-IoT-blue?style=for-the-badge&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C++-Language-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Embedded-orange?style=for-the-badge)
![Face Recognition](https://img.shields.io/badge/Face-Recognition-green?style=for-the-badge)

**An ESP32-based smart doorbell system that uses face recognition to control door access — triggered only when a visitor presses the bell button.**

</div>

---

## 📌 Overview

Smart Door is an intelligent doorbell and access control system built on the **ESP32 microcontroller**. Unlike systems that run recognition continuously, Smart Door activates **only when a visitor presses the doorbell button** — making it efficient and precise.

When the button is pressed:
- 🔔 The **bell rings** instantly
- 📸 The **camera captures** the visitor's face
- ✅ If the face is **saved** → door **unlocks automatically**
- ❌ If the face is **unknown** → door stays **locked** and the **owner gets an alert with the visitor's photo**

---

## ⚙️ How It Works

```
Visitor presses button
        │
        ▼
 🔔 Bell rings immediately
        │
        ▼
 📸 Camera captures face
        │
        ▼
 🧠 Face Recognition runs
        │
   ┌────┴────┐
   ▼         ▼
MATCH     NO MATCH
   │         │
   ▼         ▼
🔓 Door   🔒 Door stays locked
unlocks   📲 Owner gets photo alert
```

---

## 🛠️ Tech Stack

| Component         | Details                                      |
|-------------------|----------------------------------------------|
| Microcontroller   | ESP32 (Dual-core, 240MHz, Wi-Fi + BT)        |
| Language          | C++ (Arduino Framework)                      |
| Camera            | OV2640 Camera Module                         |
| Face Recognition  | ESP-WHO (on-device, no cloud needed)         |
| Face Database     | Stored in ESP32 flash memory (NVS/SPIFFS)   |
| Door Lock         | Solenoid lock / Servo via GPIO relay         |
| Bell / Buzzer     | Buzzer module on GPIO                        |
| Alert System      | Telegram Bot API (photo + notification)      |
| Connectivity      | Wi-Fi 802.11 b/g/n via ESP32                 |
| Power             | 5V regulated; deep sleep between presses     |

---

## 🔁 Detailed Flow

### Step 1 — Button Press
The visitor presses the physical push button outside the door. This sends a GPIO interrupt to the ESP32.

### Step 2 — Bell Rings
The bell/buzzer triggers **immediately** (< 50ms) — no delay, normal doorbell experience.

### Step 3 — Camera Captures
The OV2640 camera activates and captures a real-time image of the visitor.

### Step 4 — Face Recognition
The ESP32 runs face recognition locally, comparing the captured face to the saved authorized user database.

### Step 5A — Face Recognized ✅
- Door lock actuator activates
- Door **unlocks automatically**
- Door **re-locks** after a set time (e.g. 5 seconds)

### Step 5B — Face Not Recognized ❌
- Door **remains locked**
- Visitor's photo is sent to the **owner's phone via Telegram**
- Owner can review and decide to remotely unlock or ignore

### Step 6 — Standby
System returns to **deep sleep / low-power standby**, waiting for the next button press.

---

## 📁 Project Structure

```
smart-door/
├── src/
│   ├── main.cpp              # Main program entry point
│   ├── doorbell.cpp          # Button interrupt & bell trigger
│   ├── camera.cpp            # Camera init & image capture
│   ├── face_recognition.cpp  # Face detection & matching logic
│   ├── door_lock.cpp         # Lock/unlock control
│   └── alert.cpp             # Wi-Fi & Telegram alert with photo
├── hardware/
│   └── wiring_diagram.md     # Pin connections & wiring guide
├── docs/
│   └── project_report.md     # Full detailed project report
└── README.md
```

---

## 🔌 Hardware Required

- ESP32 Development Board
- OV2640 Camera Module
- Push Button (Doorbell)
- Buzzer / Bell Module
- Solenoid Lock or Servo Motor (door lock)
- Relay Module (for lock control)
- 5V Power Supply
- Jumper Wires & Breadboard / PCB

---

## 🚀 Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/YOUR_USERNAME/smart-door.git
cd smart-door
```

### 2. Install dependencies
- Install [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- Install **ESP32 board support** in Arduino IDE
- Install **ESP-WHO** library for face recognition

### 3. Configure your credentials
Open `src/alert.cpp` and set:
```cpp
const char* WIFI_SSID     = "your_wifi_name";
const char* WIFI_PASSWORD = "your_wifi_password";
const char* BOT_TOKEN     = "your_telegram_bot_token";
const char* CHAT_ID       = "your_telegram_chat_id";
```

### 4. Enroll authorized faces
Run the enrollment mode to register faces into the database:
```cpp
// Set ENROLL_MODE = true in main.cpp, upload, face the camera
// Set back to false after enrollment is done
#define ENROLL_MODE true
```

### 5. Upload to ESP32
Select your ESP32 board and port in Arduino IDE, then upload.

---

## 📲 Alert Example

When an unknown visitor presses the bell, the owner receives a Telegram message like:

```
🚨 Unknown visitor at the door!
📍 Time: 10:32 AM
📸 [Photo of visitor attached]
```

---

## ✨ Features

- 🔘 **Button-triggered only** — no continuous scanning, no false triggers
- 🔔 **Instant bell ring** on button press
- 📸 **Auto photo capture** of every visitor
- 🧠 **On-device face recognition** — works without internet
- 🔓 **Auto unlock** for saved faces
- 📲 **Photo alert** to owner for unknown visitors
- 😴 **Deep sleep standby** between presses for low power use
- 👥 **Multi-user support** — register multiple authorized faces

---

## 🔮 Future Improvements

- [ ] Remote unlock from phone after receiving alert
- [ ] Two-way audio (talk to visitor via phone)
- [ ] Night vision with IR LEDs
- [ ] Anti-spoofing / liveness detection
- [ ] Mobile app for managing authorized faces
- [ ] Cloud storage for visitor photo logs

---

## 📄 Documentation

See [`docs/project_report.md`](docs/project_report.md) for the full detailed project report.

---

## 👤 Author

**Your Name**
- GitHub: [@YOUR_USERNAME](https://github.com/YOUR_USERNAME)
- Website: [your-website.com](https://your-website.com)

---

<div align="center">
Made with ❤️ using ESP32 & C++
</div>
