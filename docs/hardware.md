---
layout: default
title: "Hardware Setup"
nav_order: 2
---

# Hardware Setup

**Goal:** Connect the TCS34725 to the Raspberry Pi using the four connections needed for I²C.

| Sensor breakout label | Raspberry Pi | Physical header pin |
|---|---|---|
| GND | GND | 6 |
| SDA | GPIO 2 / SDA1 | 3 |
| SCL | GPIO 3 / SCL1 | 5 |
| VIN / VCC | Power supported by **your specific breakout board** | Commonly 3.3 V (pin 1); some breakout boards support 5 V on VIN |

**Power is board-specific.** The bare TCS34725 chip is not a 5 V part. Confirm whether your breakout has a voltage regulator and level shifting before connecting 5 V. Raspberry Pi GPIO signals are 3.3 V; do not drive SDA or SCL at 5 V.

## What the wires do

- **SDA** carries the data going between the Pi and sensor.
- **SCL** carries the clock pulses that coordinate the transfer.
- **GND** gives both devices the same electrical reference.
- **Power** operates the breakout board.

## Before powering on

1. Shut down or disconnect the Pi's power before moving wires.
2. Check the exact sensor board labels. `LED` and `INT` are not required for basic readings.
3. Connect power and ground correctly, then SDA and SCL.
4. Power the Pi and follow [Enable I²C](i2c-setup.html).

## What about the INT pin?

Leave it disconnected for the basic tutorial. It's an optional output that signals an interrupt condition, explained much later on the [Hardware Interrupts](interrupts.html) page. It is **not** needed to read colors.
