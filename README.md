# 8x8 LED Matrix Clock (PC-Synced)

A custom-designed, real-time clock displayed on a **single 8x8 LED matrix** (MAX7219) powered by Arduino. Instead of relying on an external RTC (Real-Time Clock) hardware module, this project uses a background Python service to automatically sync the time directly from a connected PC via USB.

## Features
* **Ultra-Compact Custom UI:** Innovatively displays Hours, Minutes, and Seconds simultaneously on just 64 pixels (8x8 grid).
* **Smart Typography:** Features custom "squeezed" double-digits (10, 11, 12) and a progress-bar style indicator for the tens place of minutes.
* **PC Time Auto-Sync:** A Python script runs in the background on the host PC, automatically detecting the Arduino and sending the current time via Serial.
* **Software RTC:** Once synced, the Arduino maintains time independently using its internal `millis()` function.
* **Plug-and-Play:** The Python script auto-detects USB connections/disconnections and heals the serial connection automatically.

##  Hardware Used
* Arduino (Uno / Nano)
* MAX7219 8x8 LED Matrix Module
* USB Cable (for power and PC data sync)
* Jumper Wires

### Wiring Configuration
| MAX7219 Pin | Arduino Pin |
| :--- | :--- |
| **VCC** | 5V |
| **GND** | GND |
| **DIN** | D11 |
| **CS (LOAD)** | D10 |
| **CLK** | D13 |

## 💻 Software & Dependencies

**Arduino:**
* IDE: Arduino IDE
* Library: `LedControl` by Eberhard Fahle (Install via Library Manager)

**Python:**
* Version: Python 3.x
* Packages: `pyserial` (Install via `pip install pyserial`)

## How It Works

### 1. The Display Architecture
Displaying a full HH:MM:SS clock on a single 8x8 matrix requires a specialized layout:
* **Top Left (4x5):** Displays the current Hour (1-12).
* **Top Right (5 LEDs):** Acts as a progress bar for the "Tens" place of the minutes (e.g., 3 LEDs lit = 30+ minutes).
* **Middle Right (3x5):** Displays the "Ones" place of the minutes.
* **Bottom Left (3x2):** A 6-LED repeating cycle that tracks seconds. 

### 2. The Auto-Sync Logic
1. The Arduino boots up and starts an internal timer using `millis()`.
2. The Python script (`auto_sync.py`) continuously scans the PC's COM ports for an Arduino or CH340 serial chip.
3. Upon detection, Python reads the PC's system time and sends a formatted string: `TIME HH MM SS\n`.
4. The Arduino parses this serial command, overwrites its internal time variables, and resumes ticking.

## Installation & Setup

### Step 1: Arduino Setup
1. Open `Arduino_Code/Matrix_Clock.ino` in the Arduino IDE.
2. Install the `LedControl` library if you haven't already.
3. Connect your Arduino and upload the code. 

### Step 2: Python Service Setup
1. Ensure Python is installed on your PC.
2. Open your terminal or command prompt and install the required serial library:
   ```bash
   pip install pyserial

