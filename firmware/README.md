# Firmware

This directory contains the embedded firmware developed for the IoT Alarm System.

## Target Hardware

- NodeMCU ESP8266
- Arduino Framework
- Arduino IoT Cloud

## Source Files

| File | Description |
|------|-------------|
| `iot-alarm-system.ino` | Main application entry point |
| `microcontroller.*` | Main system controller and state machine |
| `sensor.*` | Sensor abstraction and detection logic |
| `keypad.*` | Analog keypad interface and access code handling |
| `siren.*` | Siren control |
| `thingProperties.h` | Arduino IoT Cloud configuration (credentials removed) |

## Notes

The firmware is organized using an object-oriented architecture and implements:

- Alarm state machine
- Sensor management
- Warm-up and timeout handling
- Analog keypad input
- Dual siren control
- Arduino IoT Cloud integration

For complete documentation, hardware information and project overview, see the repository root `README.md`.