# Smart Door — Full Project Report

## 1. Project Overview

| Field | Details |
|-------|---------|
| Project Name | Smart Door |
| Category | IoT / Embedded Systems / Home Security |
| Language | C++ (Arduino Framework) |
| Hardware | ESP32, OV2640 Camera, Doorbell Button, Solenoid Lock |
| Core Feature | Doorbell-triggered face recognition & access control |
| Alert System | Real-time photo alert to owner via Telegram |

### Abstract

Smart Door is an ESP32-based intelligent doorbell and access control system. Unlike traditional systems that run face recognition continuously, Smart Door activates **only when a visitor presses the doorbell button** — making it energy-efficient and eliminating false triggers.

When the button is pressed, the bell rings and the camera captures the visitor's face. If the face matches a saved authorized user, the door is unlocked automatically. If the face is unknown, access is denied and the home owner instantly receives an alert notification along with a photo of the visitor for manual review.

---

## 2. Problem Statement

Traditional doorbells only produce a sound alert, forcing homeowners to physically check the door. This is inconvenient, time-consuming, and can be unsafe—especially when the visitor is unknown. Additionally, if no one is at home, there is no way to monitor visitors, which increases the risk of theft or suspicious activity.

**Problems with existing solutions:**
- Traditional bells give no information about the visitor
- Smart video doorbells still rely on the owner to manually grant/deny access
- Continuous face recognition systems waste power and generate false triggers
- No automated way to let in trusted people when the owner is unavailable

---

## 3. Objectives

- Integrate a physical doorbell button as the sole trigger for the face recognition process
- Ring the bell immediately when the button is pressed — no delay
- Capture a clear photo of the visitor the moment the doorbell is pressed
- Run face recognition to compare the face against the saved authorized user database
- Automatically unlock the door for recognized, saved faces
- Send an instant alert with the visitor's photo to the home owner for unrecognized faces
- Return to standby after each interaction to conserve power

---

## 4. System Architecture

### 4.1 Hardware Layer

| Component | Role |
|-----------|------|
| ESP32 | Main controller — handles button, bell, camera, lock, Wi-Fi |
| Push Button | Doorbell input — visitor presses to initiate the system |
| Buzzer / Bell | Rings immediately on button press |
| OV2640 Camera | Captures real-time photo of visitor |
| Solenoid Lock | Unlocks door on authorized face match |
| Relay Module | Controls high-voltage lock from ESP32 GPIO |
| 5V Power Supply | Powers all components continuously |

### 4.2 Processing Layer

- **GPIO Interrupt Handler** — detects button press and triggers bell + camera pipeline
- **Camera Pipeline** — activates, captures, and preprocesses the visitor image
- **Face Recognition Engine** — compares captured face against flash-stored database
- **Decision Logic** — grants access or triggers alert based on recognition result

### 4.3 Communication Layer

- **Wi-Fi (ESP32 built-in)** — connects to home network for alert delivery
- **Telegram Bot API** — delivers photo + notification to owner's phone
- **Optional remote unlock** — owner can send command back to unlock door

---

## 5. How It Works — Step by Step

### Step 1 — Visitor Presses the Doorbell Button
The visitor presses the physical push button mounted outside the door. This sends a digital interrupt signal to the ESP32 GPIO pin, waking the system from standby.

### Step 2 — Bell Rings Immediately
The ESP32 triggers the buzzer/bell module in real time (< 50ms). The bell rings like a normal doorbell, alerting people inside.

### Step 3 — Camera Activates & Captures Photo
Simultaneously with the bell, the OV2640 camera module is activated. It captures a real-time image of the visitor and preprocesses it (brightness correction, face detection framing).

### Step 4 — Face Recognition Runs
The ESP32 runs the face recognition algorithm on the captured image. It extracts facial features and compares them against the authorized user database stored in flash memory.

### Step 5A — Face Recognized ✅
- Door lock actuator is triggered via relay
- Door **unlocks automatically**
- Visitor enters without any owner involvement
- Door **re-locks** after a set interval (e.g., 5 seconds)

### Step 5B — Face NOT Recognized ❌
- Door **remains locked**
- ESP32 sends a push notification to the owner's Telegram
- **Visitor's captured photo is attached** to the alert
- Owner reviews the photo and can remotely unlock or ignore

### Step 6 — Return to Standby
System returns to deep sleep / low-power standby, waiting for the next button press.

---

## 6. Technology Stack

| Component | Details |
|-----------|---------|
| Microcontroller | ESP32 Dual-core Xtensa LX6, 240 MHz |
| Language | C++ (Arduino Framework / ESP-IDF) |
| Input Trigger | Physical push button on GPIO interrupt |
| Bell | Buzzer module, GPIO-triggered |
| Camera | OV2640, activated on button press |
| Face Recognition | ESP-WHO library (on-device) |
| Face Database | NVS / SPIFFS flash memory on ESP32 |
| Door Lock | Solenoid lock via GPIO relay |
| Alert Delivery | Telegram Bot API over Wi-Fi |
| Connectivity | IEEE 802.11 b/g/n Wi-Fi |
| Power Mode | Deep sleep standby between presses |

---

## 7. Key Features

| Feature | Description |
|---------|-------------|
| Button-Triggered | Face recognition runs ONLY on button press |
| Instant Bell | Bell rings in < 50ms of button press |
| Auto Photo Capture | Camera captures visitor on every press |
| On-Device Recognition | No internet required for face matching |
| Auto Unlock | Door opens automatically for saved faces |
| Photo Alert | Owner receives visitor photo for unknown faces |
| Auto Re-lock | Door locks again after entry interval |
| Deep Sleep Standby | Low power between doorbell presses |
| Multi-User | Multiple authorized faces can be registered |

---

## 8. Challenges & Solutions

| Challenge | Solution |
|-----------|----------|
| Bell must ring with zero delay | Bell trigger is the first GPIO action in the interrupt handler |
| Camera warm-up time | Camera pre-initialized at boot, kept in standby |
| Accuracy in outdoor lighting | Image preprocessing before recognition |
| Fast photo delivery to owner | JPEG compression + Telegram Bot API |
| Multiple triggers from one press | Software debounce + cooldown timer |
| ESP32 memory for face database | Optimized feature vectors in NVS flash |

---

## 9. Results

- Bell rings in < 50ms of button press — no perceptible delay
- Face captured and recognized within 1–2 seconds
- Authorized faces correctly unlock door without owner action
- Unknown visitor photo delivered to owner in 3–5 seconds
- System tested successfully with multiple registered users and unknown faces

---

## 10. Future Scope

- **Remote Unlock** — Owner taps a button in the Telegram alert to unlock remotely
- **Two-Way Audio** — Talk to visitor from phone
- **Night Vision** — IR LED for low-light face capture
- **Anti-Spoofing** — Liveness detection to prevent photo bypass
- **Mobile App** — Manage authorized faces from a phone app
- **Visitor Log** — Timestamped history of all doorbell events with photos
- **Cloud Backup** — Store visitor photos in cloud storage

---

## 11. Conclusion

Smart Door reimagines the traditional doorbell by adding intelligent face recognition and instant owner alerts — all triggered by the single, familiar action of pressing a doorbell button. Known visitors are let in automatically, and the home owner is immediately notified with a photo of anyone not recognized.

Built on the ESP32 using C++, the project proves that affordable embedded hardware can deliver sophisticated security features previously only possible with expensive dedicated systems.
