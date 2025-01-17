## CAN Communication: Raspberry Pi and Arduino
This project demonstrates how to establish a **CAN (Controller Area Network)** communication between an **Arduino** and a **Raspberry Pi**. The Arduino reads speed data from a sensor, calculates the speed, and sends it to the Raspberry Pi over a CAN bus using an **MCP2515 CAN module**. The Raspberry Pi processes and displays the received data using Python.


<h1>Overview</h1>
Controller Area Network (CAN) is a robust communication protocol commonly used in automotive and industrial applications. This project involves:
  Using an Arduino to send speed data via a CAN bus.
  Configuring a Raspberry Pi to receive and process the data using Python.

## Hardware Requirements
- **Arduino Uno/Nano/Mega** or similar.
- **Raspberry Pi** (any version with SPI support, e.g., Raspberry Pi 4).
- **MCP2515 CAN Module** (two units: one for the Arduino and one for the Raspberry Pi).
- Speed sensor (connected to Arduino).
---

## Connections

### Raspberry Pi to MCP2515 CAN Module
| MCP2515 Pin   | Raspberry Pi GPIO Pin |
|---------------|------------------------|
| **VCC**       | 3.3V or 5V            |
| **GND**       | GND                   |
| **CS**        | GPIO 8 (SPI0_CS0)     |
| **SCK**       | GPIO 11 (SPI0_SCLK)   |
| **MOSI**      | GPIO 10 (SPI0_MOSI)   |
| **MISO**      | GPIO 9 (SPI0_MISO)    |
| **INT**       | GPIO 25 (optional)    |

---

## Setup and Configuration

### Arduino Setup
1. Install the **MCP_CAN** library in the Arduino IDE:
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Search for `MCP_CAN`, CAN-BUS Shield and install it.

2. Upload the Arduino code (provided in the [Arduino Code](#arduino-code) section).

3. Connect the speed sensor to the Arduino and verify it calculates speed correctly.

---

### Raspberry Pi Setup
(https://forums.raspberrypi.com/viewtopic.php?t=353451)
