# Smart Door — Wiring & Pin Connections

## ESP32 Pin Map

| ESP32 Pin | Connected To       | Description                        |
|-----------|--------------------|------------------------------------|
| GPIO 13   | Push Button        | Doorbell input (INPUT_PULLUP)      |
| GPIO 12   | Buzzer (+)         | Bell trigger output                |
| GPIO 4    | Camera (PWDN)      | OV2640 power down control          |
| GPIO 2    | Camera (RESET)     | OV2640 reset                       |
| GPIO 14   | Relay IN           | Controls solenoid door lock        |
| GPIO 15   | Status LED         | Visual indicator (optional)        |
| 3.3V      | Button (one end)   | Pull-up reference                  |
| GND       | Common Ground      | All component grounds              |

## OV2640 Camera Module Connections

| Camera Pin | ESP32 Pin |
|------------|-----------|
| 3V3        | 3.3V      |
| GND        | GND       |
| SIOC (SCL) | GPIO 27   |
| SIOD (SDA) | GPIO 26   |
| VSYNC      | GPIO 25   |
| HREF       | GPIO 23   |
| PCLK       | GPIO 22   |
| XCLK       | GPIO 21   |
| D0–D7      | GPIO 35,34,39,38,37,36,19,18 |
| PWDN       | GPIO 4    |
| RESET      | GPIO 2    |

## Relay / Solenoid Lock

```
ESP32 GPIO 14 ──► Relay IN
Relay COM ──────► 12V Power (+)
Relay NO ───────► Solenoid Lock (+)
Solenoid GND ───► 12V Power (-)
```

> ⚠️ Use a flyback diode across the solenoid to protect the relay from voltage spikes.

## Doorbell Button

```
ESP32 GPIO 13 ──► Button Terminal 1
Button Terminal 2 ──► GND

(GPIO 13 configured as INPUT_PULLUP in code)
```

## Buzzer

```
ESP32 GPIO 12 ──► Buzzer (+)
GND ─────────────► Buzzer (-)
```

## Power Supply

- ESP32: 5V via USB or VIN pin
- Solenoid Lock: Separate 12V supply via relay
- Camera & Buzzer: Powered from ESP32 3.3V / 5V pins
