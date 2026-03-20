/*
 * Smart Door — Main Entry Point
 * ESP32 IoT Face Recognition Doorbell & Access Control
 *
 * Flow:
 *   Doorbell button pressed
 *     → Bell rings immediately
 *     → Camera captures visitor face
 *     → Face recognition runs
 *     → MATCH   : Door unlocks automatically
 *     → NO MATCH: Owner receives photo alert via Telegram
 */

#include <Arduino.h>
#include "doorbell.h"
#include "camera.h"
#include "face_recognition.h"
#include "door_lock.h"
#include "alert.h"

// ── Pin Definitions ──────────────────────────────────────────
#define BUTTON_PIN    13    // Doorbell push button (INPUT_PULLUP)
#define BUZZER_PIN    12    // Bell / buzzer output
#define LOCK_PIN      14    // Relay for solenoid door lock
#define STATUS_LED    15    // Optional status LED

// ── Config ───────────────────────────────────────────────────
#define ENROLL_MODE   false        // Set true to register new faces
#define LOCK_OPEN_MS  5000         // How long door stays unlocked (ms)
#define DEBOUNCE_MS   3000         // Cooldown between button presses (ms)

// ── State ────────────────────────────────────────────────────
volatile bool buttonPressed = false;
unsigned long lastPressTime = 0;

// ── Button ISR ───────────────────────────────────────────────
void IRAM_ATTR onButtonPress() {
  unsigned long now = millis();
  if (now - lastPressTime > DEBOUNCE_MS) {
    buttonPressed = true;
    lastPressTime = now;
  }
}

// ── Setup ────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LOCK_PIN,   OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Ensure door is locked on boot
  doorLock(false);

  // Initialize camera
  initCamera();

  // Initialize Wi-Fi & Telegram
  initAlert();

  // Initialize face recognition database
  initFaceRecognition();

  // Attach doorbell interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);

  Serial.println("[Smart Door] Ready. Waiting for doorbell press...");
}

// ── Main Loop ────────────────────────────────────────────────
void loop() {

  if (!buttonPressed) return;
  buttonPressed = false;

  Serial.println("[Smart Door] Doorbell pressed!");

  // STEP 1: Ring the bell immediately
  ringBell(BUZZER_PIN);

  // STEP 2: Capture visitor face
  Serial.println("[Camera] Capturing visitor image...");
  camera_fb_t* frame = captureImage();

  if (!frame) {
    Serial.println("[Camera] Failed to capture image.");
    return;
  }

  // STEP 3: Run face recognition
  Serial.println("[Face Rec] Running recognition...");
  int result = recognizeFace(frame);

  if (ENROLL_MODE) {
    // Enrollment mode: save this face to database
    enrollFace(frame);
    Serial.println("[Enroll] Face saved to database.");

  } else if (result >= 0) {
    // STEP 4A: Face recognized — unlock door
    Serial.printf("[Access] Authorized user #%d — Unlocking door.\n", result);
    digitalWrite(STATUS_LED, HIGH);
    doorLock(true);                  // Unlock
    delay(LOCK_OPEN_MS);
    doorLock(false);                 // Re-lock
    digitalWrite(STATUS_LED, LOW);
    Serial.println("[Access] Door re-locked.");

  } else {
    // STEP 4B: Unknown face — alert owner with photo
    Serial.println("[Alert] Unknown visitor! Sending photo to owner...");
    sendPhotoAlert(frame);
    Serial.println("[Alert] Owner notified.");
  }

  // Free camera frame buffer
  esp_camera_fb_return(frame);

  Serial.println("[Smart Door] Done. Returning to standby...\n");
}
