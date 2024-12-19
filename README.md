# Smart Pet Feeder System

## Overview
The **Smart Pet Feeder System** is an automated solution to feed pets efficiently and conveniently. This system integrates sensors, actuators, and Wi-Fi connectivity to ensure consistent feeding and allow for remote control via a mobile application. Designed with real-time responsiveness, it also includes alerts for low food levels, ensuring pets are never left unfed.

---

## Key Features

1. **Automated Food Dispensing**
   - Dispenses food at regular intervals.
   - Ensures pets are fed consistently, even when the owner is unavailable.

2. **Remote Control via Wi-Fi**
   - Manually control the food dispensing mechanism through a mobile app.
   - Monitor food levels and pet activity remotely.

3. **Low Food Alerts**
   - Detects low food levels using an IR sensor.
   - Triggers an audible alert through a buzzer.

4. **Pet Presence Detection**
   - Uses an ultrasonic sensor to detect a pet near the feeder.
   - Automatically activates the dispensing mechanism when the pet is detected.

---

## System Architecture
The system employs the following subsystems:

### 1. **Food Level Monitoring**
- **Hardware:** IR Sensor, Buzzer
- **Functionality:** Monitors food level and triggers an audible alert when food is insufficient.

### 2. **Animal Detection and Feeding**
- **Hardware:** Ultrasonic Sensor, Servo Motor
- **Functionality:** Detects the presence of the pet and opens the food compartment.

### 3. **Wi-Fi Communication**
- **Hardware:** RP2040 Pico W Microcontroller
- **Functionality:** Connects to a mobile app via HTTP server, enabling remote control and monitoring.

---

## Software Details
The software is written in C and leverages the FreeRTOS operating system for task concurrency.

### Tasks
- **Control Task:** Monitors sensors and handles feeding logic.
- **Dispense Task:** Operates the servo motor to dispense food.
- **Wi-Fi Task:** Manages HTTP server and Wi-Fi communication.

### Key Functionalities
- **Motor Activation:** Controlled based on pet detection and food availability.
- **Buzzer Alerts:** Triggered when food levels are low and a pet is detected.
- **Wi-Fi Connectivity:** Provides real-time status updates and allows manual feeding via the mobile app.

---

## Usage

### Setting Wi-Fi Credentials
Update the `WIFI_SSID` and `WIFI_PASSWORD` constants in the code with your Wi-Fi credentials:

```c
const char WIFI_SSID[] = "your-wifi-ssid";
const char WIFI_PASSWORD[] = "your-wifi-password";
```

### Running the System
1. Flash the firmware to the RP2040 Pico W microcontroller.
2. Power up the hardware components.
3. Connect the device to the configured Wi-Fi network.
4. Access the HTTP server to monitor and control the feeder.

### Debugging
- Use a serial monitor to view sensor readings and system logs.
- Ensure proper connections of all hardware components if errors are observed.

---

## Important Notes
- Ensure the food compartment is filled before use.
- Avoid placing the feeder in areas with high interference or unstable Wi-Fi signals.
- Periodically clean the feeder to maintain hygiene.

---

## Future Enhancements
- Integration with a cloud platform for advanced analytics.
- Adding a camera for live pet monitoring.
- Developing a mobile app for improved user interaction.

---

For further assistance, feel free to reach out!

