# 8x8 LED Matrix Clock (PC-Synced)

A real-time clock displayed on a single MAX7219 8x8 LED matrix. It syncs time directly from a PC via a background Python script over USB, eliminating the need for an external RTC module.

## Components
* Arduino (Uno/Nano)
* MAX7219 8x8 LED Matrix
* USB Cable

## Wiring
| MAX7219 | Arduino |
| :--- | :--- |
| **VCC** | 5V |
| **GND** | GND |
| **DIN** | D11 |
| **CS** | D10 |
| **CLK** | D13 |

## 8x8 Matrix Layout
A standard HH:MM:SS format does not fit on a single 8x8 grid. This project uses a custom UI layout to squeeze all the data into 64 pixels.

**Visual Map:**
🟥 = Hours (4x5)
🟦 = Minutes Tens (Progress bar, 5 LEDs)
🟩 = Minutes Ones (3x5)
🟨 = Seconds (3x2 repeating loop)
⬛ = Unused/Blank

🟥 🟥 🟥 🟥 🟦 🟦 🟦 🟦

🟥 🟥 🟥 🟥 ⬛ ⬛ ⬛ 🟦

🟥 🟥 🟥 🟥 ⬛ 🟩 🟩 🟩

🟥 🟥 🟥 🟥 ⬛ 🟩 🟩 🟩

🟥 🟥 🟥 🟥 ⬛ 🟩 🟩 🟩

⬛ ⬛ ⬛ ⬛ ⬛ 🟩 🟩 🟩

🟨 🟨 🟨 ⬛ ⬛ 🟩 🟩 🟩

🟨 🟨 🟨 ⬛ ⬛ ⬛ ⬛ ⬛

* **Hours (Top-Left):** Displays 1-12. Double digits (10, 11, 12).
* **Minutes Tens (Top-Right Nook):** Lights up sequentially. If the time is 45 minutes, 4 of these LEDs will be on. 
* **Minutes Ones (Middle-Right):** Standard 0-9 digits.
* **Seconds (Bottom-Left):** Lights up from 1 to 6, repeating the cycle every 6 seconds.

## Code & Usage

### 1. `Matrix_Clock.ino`
The Arduino sketch that handles the display layout and internal `millis()` timing.
* **Setup:** Open in the Arduino IDE and install the `LedControl` library.
* **Usage:** Upload to your Arduino board.

### 2. `auto_sync.py`
The background Python service that fetches your PC's time and sends it to the Arduino over Serial.
* **Setup:** Open your terminal and run `pip install pyserial`.
* **Usage:** With the Arduino plugged in, run `python auto_sync.py`. It auto-detects the COM port, syncs the time, and runs silently in the background.
