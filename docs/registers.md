---
layout: default
title: "Sensor Registers"
nav_order: 7
---

# Sensor Registers

**Goal:** Understand what our program sends to the TCS34725 before reading bytes.

A **register** is a small location inside the sensor with a specific job. Some registers control the sensor; others hold measurements.

| Register address | Name | What it is for |
|---|---|---|
| `0x00` | ENABLE | Power on and enable RGBC measurements |
| `0x01` | ATIME | Integration time (optional setting) |
| `0x0F` | CONTROL | Gain (optional setting) |
| `0x12` | ID | Device identification |
| `0x13` | STATUS | Data ready and interrupt status |
| `0x14`–`0x15` | Clear | Low byte, then high byte |
| `0x16`–`0x17` | Red | Low byte, then high byte |
| `0x18`–`0x19` | Green | Low byte, then high byte |
| `0x1A`–`0x1B` | Blue | Low byte, then high byte |

## Why do we send `0x80`?

The TCS34725 expects a **command byte**, not just a register number. The command bit `0x80` says that the following bits identify a register.

For example, the register `0x00` (ENABLE) becomes command `0x80`. To turn on power, we send these **two bytes**:

```c
unsigned char enable[2];

enable[0] = 0x80;  /* Command for ENABLE register 0x00 */
enable[1] = 0x01;  /* Set PON: turn on sensor power */

write(fd, enable, 2);
```

- `fd` is the opened I²C bus from the previous lesson.
- `enable` is an array holding the two bytes we send.
- `2` means send two bytes.

After the oscillator starts, the sensor needs ENABLE value `0x03`: `0x01` (power on) + `0x02` (RGBC measurement enable).

## Why `0xB4` when reading RGBC?

`0xB4` is the command byte for starting at register `0x14` and reading consecutive registers:

```text
0x80  command bit
0x20  automatically move to the next register
0x14  first RGBC register (Clear low byte)
-----
0xB4  complete command
```

Writing `command = 0xB4;` is fine in the first example, **as long as we understand those parts**. The eight bytes will contain Clear, Red, Green, and Blue.

## Hexadecimal numbers

Numbers beginning with `0x` are hexadecimal (base 16). `0x29` is the device address, not the number 29 in decimal.

Learn the complete read sequence next: [Read Raw RGBC](rgbc-acquisition.html).
