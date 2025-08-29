# Semi-Autonomous Nuclear Inspection Robot

This repository documents a semi‑autonomous robot for remote inspection in nuclear facilities, combining live video from an ESP32‑CAM pan–tilt module with radiation, fire, gas, motion, temperature, and humidity sensing for real-time safety monitoring via a cloud dashboard.  

The design reduces human exposure, provides multi-sensor confirmation, and leverages low-cost modules for scalable deployment in hazardous environments.

---

## Project Images

![Project Poster](images/poster-overview.jpg)  
![Assembled Robot](images/final-design.jpg)  
![Web Control Panel](images/web-control.jpg)  

---

## Key Features

- Remote visual inspection with ESP32‑CAM pan–tilt module.  
- Radiation tracking (CPS/CPM, hourly averages) plus temp, humidity, smoke, gas, and human presence detection.  
- Blynk dashboard for live graphs, gauges, and alerts triggered by hazards.  

---

## Hardware Modules

- **ESP32‑CAM** with two SG90/9g servos for pan–tilt.  
- **Sensors:** DHT11, MQ2, fire sensor, HB100+LM358, Geiger counter.  

---

---

## Prerequisites

- Arduino IDE (v1.8.x or 2.x)  
- ESP32 board support installed via **Additional Boards Manager URLs**  
- USB‑to‑serial adapter or ESP32‑CAM‑MB programmer  
- Stable 5 V supply capable of driving two micro servos  

---

## ESP32 Installation

1. Open Arduino IDE → Preferences → add ESP32 URL in *Additional Boards Manager URLs*.  
2. Boards Manager → install **ESP32 by Espressif Systems**.  
3. Select board: `AI Thinker ESP32-CAM` or `ESP32 Wrover Module`.  
4. Select correct serial port; install drivers if needed.  
5. For upload timeout, hold BOOT until “Connecting…” appears.  

---

## Pan–Tilt Hardware Assembly

1. Mount ESP32‑CAM on two-axis pan–tilt bracket with SG90 servos.  
2. Ensure smooth travel in both axes.  
3. Servo wiring: red = 5V, brown/black = GND, yellow = signal.  
4. Power servos from 5 V, common GND with ESP32‑CAM.  

---

## Wiring (ESP32‑CAM + Servos)

- Servo Tilt → GPIO 14  
- Servo Pan → GPIO 15  
- Servo VCC → 5 V  
- Servo GND → GND (shared with ESP32‑CAM)  

---

## Firmware: Video Stream + Pan–Tilt

- Use ESP32‑CAM web server example streaming MJPEG.  
- Attach servos at 50 Hz using `ESP32Servo` library.  
- Control angles via HTML buttons (up/down/left/right).  
- Set camera model pins, stream port, and server port.  

---

## Flash, Test & Tune

1. Upload sketch; use BOOT or MB adapter if needed.  
2. Open browser → IP from Serial Monitor → verify live video.  
3. Test pan–tilt buttons for full range.  
4. If lag occurs, reduce frame size or use dedicated 5 V supply for servos.  

---

## Blynk Dashboard Mapping

- Map sensor values (radiation, temp, humidity, smoke, gas, human presence) to Blynk datastreams.  
- Use gauges for environment metrics and graphs for time-series trends.  

---

## Safety Notes

- Keep robot outside direct human contact in high-radiation zones.  
- Validate calibration of sensors periodically.  
- Log CPM/CPS over time for anomaly detection.  

---

## Quick Start Checklist

1. Assemble pan–tilt with correct servo orientation.  
2. Wire signals → GPIO 14/15, power → 5 V, GND shared with ESP32‑CAM.  
3. Install ESP32 support, select board & port, upload sketch.  
4. Connect to IP → verify video & pan–tilt → integrate Blynk widgets.  

---

## Badges

- **Status:** Prototype validated  
- **Target Hardware:** ESP32‑CAM, pan–tilt servos, multi-sensor suite, cloud dashboard  

---

## Credits

- ESP32‑CAM pan–tilt streaming workflow adapted from community guides (Random Nerd Tutorials).  
- Arduino IDE ESP32 package installation per official documentation.


