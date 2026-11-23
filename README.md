# 🖥️ CYD PC Hardware Monitor (ESP32-2432S028R)

A lightweight, real-time PC hardware resource monitor built for the **Cheap Yellow Display (CYD / ESP32-2432S028R)** using Python and C++ (Arduino / `TFT_eSPI`).

No third-party background software (like AIDA64 or LibreHardwareMonitor) is required — metrics are collected natively via Python and streamed over USB Serial.

## ✨ Features
- **Plug & Play Auto-Detection:** Automatically scans and connects to the CYD serial port across Windows, Linux, and macOS (with manual fallback).
- **Real-Time Metrics:** Tracks **CPU**, **RAM**, **GPU (3D Engine)**, **Disk Usage**, and **Network Speed (Download / Upload in MB/s)**.
- **Dual Display Modes:** Switch seamlessly between a **2×2 Live Graph Grid** (default) and a **Minimalist Large-Text View** using the onboard physical `BOOT` button (`GPIO 0`) — no touchscreen required.
- **Universal GPU Support (Windows):** Uses native Windows performance counters (`typeperf`) to support NVIDIA, AMD, and Intel GPUs out of the box without vendor-specific libraries.
- **Flicker-Free Rendering:** Uses circular history buffers and targeted line redraws on a 320×240 SPI TFT display.

## 🛠️ Hardware & Libraries
- **Board:** ESP32-2432S028R (Cheap Yellow Display, 2.8" 320×240 ILI9341)
- **Arduino Library:** [TFT_eSPI](https://github.com) (configured for CYD pins: `MOSI=13`, `MISO=12`, `SCLK=14`, `CS=15`, `DC=2`, `BL=21`)
- **Python 3.x:** `psutil`, `pyserial`

## 🚀 Quick Start

### 1. Flash the ESP32 (CYD)
1. Open `CYD_PC_Monitor.ino` in **Arduino IDE**.
2. Make sure `TFT_eSPI` is configured for `ESP32-2432S028R`.
3. Select your board (`ESP32 Dev Module`) and COM port, then click **Upload** *(hold the `BOOT` button if your board revision lacks auto-reset)*.

### 2. Run the Python Sender
1. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
2. Run the script (it will find your CYD port automatically):
   ```bash
   python pc_monitor.py
   ```
   *(Tip: Rename the file to `pc_monitor.pyw` on Windows to run it silently in the background without a console window).*

## 📡 Serial Protocol
Data is streamed via USB Serial at `115200 baud` as a semicolon-separated string every second:
```text
CPU;RAM;DISK;GPU;NET_DOWN;NET_UP\n
Example: 14.5;42.1;83.0;28.4;12.5;3.2
```

## 🗺️ Roadmap
- [ ] Add CPU/GPU temperature sensors
- [ ] Night mode dimming using the onboard LDR light sensor (`GPIO 34`)
