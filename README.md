# ESP32 OLED Smart Display (Chronos Integration)

An ESP32-based smart OLED display featuring animated eyes, real-time clock, and navigation data integration using **ChronosESP32**.  
Designed for wearable, vehicle, and experimental UI use-cases.

---

## ✨ Features
- 👀 **Animated Eyes Mode**  
  Realistic eye movement with random motion and blinking animation.
- ⏰ **Clock Mode**  
  Displays real-time time and date synced via Chronos.
- 🧭 **Navigation Mode**  
  Shows navigation icon, distance, and title from connected device.
- 👆 **Touch Control**  
  Single touch input to switch between display modes.
- 🔗 **Bluetooth-based Data Sync**  
  Uses ChronosESP32 for time and navigation data.

---

## 🧠 Display Modes
| Mode | Description |
|----|----|
| Eye Mode | Animated eyes with smooth movement and blinking |
| Clock Mode | Digital clock (HH:MM) and date |
| Navigation Mode | Navigation icon, distance, and title |

Touch input cycles modes in the following order:
Eye Mode - Clock Mode - Navigation Mode.

---

## 🛠 Hardware Requirements
- ESP32 
- OLED SSD1306 (128x64, I2C)
- Capacitive or digital touch button (example: ttp223)
- Smartphone with Chronos-compatible app

---
## 🔌 Pin Configuration
| Component |   GPIO  |
|-----------|---------|
| OLED SDA  | GPIO 21 |
| OLED SCL  | GPIO 22 |
| Touch Pin | GPIO 4  |


---
## Library Used
Ensure the following libraries are installed:
- Adafruit_GFX
- Adafruit_SSD1306
- ChronosESP32
- Wire (built-in)

## setup & installation
1. clone repository:
git clone https://github.com/alvindevIn/ESP-NAV
cd esp32-oled-chronos

2. Open the project using Arduino IDE
3. Board configuration:
    - Board: ESP32 Dev Module
    - Port: Select the correct serial port
4. Upload the code to the ESP32

## Code Setup
setup()
- OLED initialization
- Chronos callbacks
- Touch configuration

loop()
- Chronos loop handler
- Touch handler
- Mode renderer

renderEye()
renderClock()
renderNavigation()

## How to Use
1. Install Chronos from the Play Store
2. Make sure Bluetooth is turned on
3. Open the Chronos app
4. Go to the Watch tab
5. Connect to espNav
6. After connected, go back to the Watch tab
7. Select “ESP32 Manager”
8. Enable Notification Permission
9. Go back, then open the Notification tab
10. Select Apps
11. Make sure Maps is enabled
12. finally, test with Google Maps


👤 Author
Alvin
ESP32 · IoT · Embedded Systems

```c
