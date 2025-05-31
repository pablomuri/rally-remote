# Rally Remote

**Rally Remote** emulates a Bluetooth keyboard (HID device) to control Android and iOS devices — ideal for use with navigation and rally apps such as **OsmAnd**, **DMD2**, and digital roadbooks. It is a lightweight Bluetooth controller firmware built for the **ESP32-C3 Super Mini**.

## 🚀 Features

- ✅ Bluetooth HID support (emulates a keyboard)
- ✅ Compatible with Android and iOS devices
- ✅ Designed for rally navigation apps (OsmAnd, DMD2, etc.)
- ✅ 4-button input + 1 joystick (up/down)
- ✅ Supports over-the-air updates via ElegantOTA
- ✅ Easy to build and flash using PlatformIO

## 🔧 Hardware Requirements

- **ESP32-C3 Super Mini** with **USB-C** connector  
- 4 momentary push buttons  
- 1 joystick with up/down direction (digital or analog input)

## 🛠️ Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/install) installed (as CLI or VSCode extension)
- USB-C cable to connect your ESP32-C3 Super Mini

### Cloning the Repository

```
git clone https://github.com/pablomuri/rally-remote.git
cd rally-remote
```

### Build & Upload via USB-C

Connect your ESP32-C3 Super Mini via USB-C and run:

```
pio run --target upload
```

This will compile and upload the firmware using the configuration in `platformio.ini`.

### Upload via OTA (ElegantOTA)

Push the buttons 1 and 2 while before connecting the esp32 to start the OTA mode.

Once the device is connected to Wi-Fi and running, you can upload new firmware via ElegantOTA:

1. Access the device's IP address in your web browser.
2. Use the ElegantOTA interface to upload a new `.bin` file.

> ⚠️ Ensure the device is on the same network as your computer.

## 📦 Project Structure

```
rally-remote/
├── include/          # Header files
├── src/              # Source code (main.cpp, bluetooth HID logic)
├── test/             # (Empty) placeholder for future unit tests
└── platformio.ini    # PlatformIO project config
```

## 📲 Usage

After flashing the device:

1. Pair the ESP32-C3 via Bluetooth with your phone or tablet.
2. The device will appear as a **keyboard**.
3. Press the physical buttons or move the joystick to trigger navigation commands in rally apps.

> Button mapping behavior can be modified in `src/main.cpp`.

## 🤝 Contributing

Contributions are welcome! Feel free to fork the repo and submit a pull request.  
No formal guidelines yet — just keep things clean and documented.

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

🏍️ Ride on and navigate smarter!
