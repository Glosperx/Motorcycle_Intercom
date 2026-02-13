# Motorcycle_Intercom
**Smart Moto Intercom & Safety System**

## Description
This project focuses on creating a **Smart Intercom System for Motorcyclists** designed for decentralized communication and enhanced rider safety. The system uses an **ESP-NOW Mesh Network** to allow riders to communicate in remote areas without cellular or internet coverage.

Beyond communication, the device acts as a safety hub:
* **Crash Detection:** Uses an IMU to detect high-G impacts or unusual tilt angles.
* **Emergency SOS:** Automatically broadcasts GPS coordinates to the group mesh if a crash is detected.
* **NFC Medical ID:** A passive NFC tag stores vital data (blood type, allergies, ICE contacts) that can be scanned by first responders with any smartphone.


### Circuit Image
![Circuit Image](circuit1.png)

### 3D Printed case
![Final Image 1](1.png)
![Final Image 2](2.png)

### Second intercom
![Intercom 2](blue1.png)
### Final 
![3D printed final](total2.png)


## Video
[See the circuit in action on YouTube](https://youtu.be/ce2_XN7YsI8)  

### Bill of Materials (BOM) - Motorcycle Intercom (2 Units)

| Amount | Part Type | Properties | Datasheet |
| :--- | :--- | :--- | :--- |
| 2 | **ESP32 DevKit V1** | Dual-mode WiFi + BT| [Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |
| 2 | **MAX98357A I2S Amp** | Amplifier | [Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/max98357a-max98357b.pdf) |
| 2 | **INMP441 Microphone** |Digital Microphone I2S| [Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/INMP441.pdf) |
| 2 | **MPU-6050 (IMU)** | 6-axis Gyro + Accelerometer for Crash Detection | [Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) |
| 2 | **1W Speakers** | 8 Ohm; Helmet compatible | - |
| 2 | **NFC Tag NTAG215** | Passive Medical ID storage | - |
| 4 | **AA Batteries** | 1.5V Power Source | - |
| 2 | **3D Printed Case** | Custom design; Material: PLA | - |


## Tutorial source
- I don't plan to follow any tutorials. This is an original architectural design integrating decentralized networking with sensor fusion logic.

## Questions

### Q1 - What is the system boundary?
The system boundary is the physical intercom unit attached to the helmet. 
* **Inputs:** Voice signals, motion data (acceleration/rotation), and GPS coordinates.
* **Outputs:** Real-time audio to the rider, SOS data packets to the mesh network, and medical info via NFC.

### Q2 - Where does intelligence live?
Intelligence lives at the **Edge** on the ESP32. The MCU performs local data processing to detect crashes and manages the mesh routing logic without requiring an external server or cloud connection.

### Q3 - What is the hardest technical problem?
The most significant challenge is the **reliable transmission of data packets between the two devices in a Mesh network**. Maintaining low-latency audio while simultaneously handling high-priority emergency packets (SOS) in a dynamic, high-speed environment is technically demanding.

### Q4 - What is the minimum demo?
The minimum demo (MVP) will demonstrate the **successful connectivity between the two intercom devices**. This involves establishing a stable mesh link and verifying that a data packet (or alert) sent from one device is correctly received and processed by the second device.

### Q5 - Why is this not just a tutorial?
This project is an **original integration** of multiple complex systems: decentralized networking (Mesh), sensor fusion for safety (IMU/GPS), and emergency medical accessibility (NFC).


## Pin Map
The following table outlines the wiring configuration based on the 38-pin ESP32 DevKit:

| Component | Function | ESP32 GPIO Pins | Physical Pins |
| :--- | :--- | :--- | :--- |
| **I2S Audio** | BCLK / SCK | 14 | 12 |
| | WS / LRC | 27 | 11 |
| | Mic Data (SD) | 32 | 7 |
| | Amp Data (DIN) | 33 | 8 |
| **MPU6050** | I2C SDA | 21 | 33 |
| | I2C SCL | 22 | 36 |
| **Power** | 5V Input | VIN | 19 |
| | Common Ground | GND | 38 |


![Pinout](esp_pinout.png)
