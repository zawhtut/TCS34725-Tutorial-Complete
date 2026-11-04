---
layout: default
title: Registers and Transactions
nav_order: 5
---
# Registers and Linux I²C transactions

## Selected registers

| Address | Symbol | Purpose |
| --- | --- | --- |
| `0x00` | ENABLE | `PON` bit 0, `AEN` bit 1 |
| `0x01` | ATIME | Integration length: `(256-ATIME) * 2.4 ms` |
| `0x0F` | CONTROL | Gain `0=1x`, `1=4x`, `2=16x`, `3=60x` |
| `0x12` | ID | Family identification (`0x44` typical TCS34725) |
| `0x13` | STATUS | `AVALID` bit 0 |
| `0x14`–`0x15` | CDATA | Clear low, high |
| `0x16`–`0x17` | RDATA | Red low, high |
| `0x18`–`0x19` | GDATA | Green low, high |
| `0x1A`–`0x1B` | BDATA | Blue low, high |

The related family ID `0x4D` is used by TCS34723/27 variants. Our example tool **reports** any ID and warns when it is outside `0x44` / `0x4D`; verify which hardware variant is actually connected.

## Command bytes

A normal register command uses `0x80 | register`. A sequential read uses `0xA0 | start_register` (auto-increment). `0xA0 | 0x14 = 0xB4` requests the 8 RGBC bytes starting at `0x14`.

## Robust read on Linux

A register read is a **write of the command byte** followed by **read of the response**. Our shared example driver uses `ioctl(fd, I2C_RDWR, ...)`, which submits the two messages as a single combined I²C transaction with repeated START. This avoids assuming a separate `write()` then `read()` will preserve the register address across a STOP condition.

Linux headers used in the sample are `<linux/i2c.h>`, `<linux/i2c-dev.h>`, `<sys/ioctl.h>`, `<fcntl.h>`, and `<unistd.h>`.

For the exact implementation see `examples/tcs_i2c.c` in the GitHub repository.

**Driver interface vs. device driver:** the kernel is still responsible for the physical I²C bus, and the user-space C code only implements TCS34725 commands/register handling.

Next: [RGBC acquisition](rgbc-acquisition.html).
