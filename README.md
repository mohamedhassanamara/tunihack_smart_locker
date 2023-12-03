# tunihack_smart_locker

## Overview

This project implements a simple smart locker using an ESP32 microcontroller, RFID sensor, LCD monitor, and a WiFi module. The locker grants access to users with valid RFID cards.

## Components

- ESP32 microcontroller
- MFRC522 RFID sensor
- SSD1306 OLED display
- WiFi module

## Dependencies

- [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306) library for the OLED display.
- [MFRC522](https://github.com/miguelbalboa/rfid) library for the RFID sensor.
- [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi) library for WiFi connectivity.

## Setup

1. Install the necessary libraries:
   - Adafruit_SSD1306
   - MFRC522
   - WiFi

2. Replace the placeholders in the code:
   - Set your WiFi credentials: `ssid` and `password`.
   - Set the valid RFID card UID in the `isValidUser` function.

## Usage

1. Connect the hardware components as per your schematic.
2. Upload the code to your ESP32 using the Arduino IDE or PlatformIO.
3. Open the serial monitor to view the ESP32's output.
4. Present a valid RFID card to the sensor.

## Customization

- Adjust the code to match your specific hardware connections.
- Implement additional features or security checks as needed.
- Modify the appearance and behavior of the OLED display.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- This project uses the Adafruit and MFRC522 libraries.
