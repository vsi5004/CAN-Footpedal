# CAN Footpedal

## Overview

CAN Footpedal is an Arduino-based project designed to capture the position of a foot pedal using a linear potentiometer and transmit this data over a CAN bus. This project is hardware agnostic and can be integrated into any system that accepts CAN input, although I have no guarantee that it will work. It should not be used in timing or safety critical applications.

## Hardware Requirements

- Arduino Nano
- MCP2515 CAN Bus Module with TJA1050 Transceiver
- 10kOhm Linear potentiometer
- Jumper wires
- Breadboard (optional, for prototyping)

## Software Requirements

- Arduino IDE or Visual Studio Code with PlatformIO
- Arduino libraries:
  - `mcp_can.h` for CAN bus functionality
  - `SPI.h` for SPI communication
  - `RunningAverage.h` for smoothing the pedal readings
  - `arduino-timer.h` for non-blocking timing

## Installation

1. **Hardware Setup:**
   - Connect the linear potentiometer to the Arduino's A1 analog pin.
   - Setup the MCP2515 module according to its pin configuration, connecting the CS pin to Arduino pin 10.
   - Ensure all connections are secure and double-check the wiring.

2. **Software Setup:**
   - Install Arduino IDE or Visual Studio Code with the PlatformIO extension.
   - Download the required libraries from the Arduino Library Manager or through PlatformIO.
   - Open the project in your IDE and ensure all library dependencies are correctly configured.

## Configuration

- The MCP2515 is configured to run at 16MHz with a baud rate of 1000kbps. Adjust these settings in the code if your hardware specifications differ.
- The default mode is set to `MCP_NORMAL`. Change `CAN0.setMode(MCP_NORMAL);` to `CAN0.setMode(MCP_LOOPBACK);` to test your transceiver in isolation.
- Change `CAN_ID` to increase or decrease the priority of messages on the CAN bus (if there are other devices transmitting).
- Change `CAN_SEND_DELAY_MS` to increase or decrease the delay between each can message being sent out.
- Change `PEDAL_READ_DELAY_MS` to increase or decrease how quickly the pedal position is updated.

## Usage

- Upload the code to your Arduino Nano.
- The system will start reading the pedal position and transmit the averaged value over CAN at a set interval.
- Open the Serial Monitor at a baud rate of 115200 to view debug messages and pedal position data.

## Contributing

Contributions to the CAN Footpedal project are welcome. Please fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
