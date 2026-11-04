---
layout: default
title: Hardware Interrupts (Optional)
nav_order: 7
---

# Optional TCS34725 hardware interrupts (INT pin)

The existing [RGBC acquisition tutorial](rgbc-acquisition.html) uses **I²C polling** and does not need a GPIO library. This page describes an **optional** alternative: the TCS34725 **INT** output can signal that the **Clear-channel measurement** is outside programmable thresholds. It is **not** automatically a pulse for every new RGB measurement.

The sensor is **still configured and read over I²C** (`/dev/i2c-1`). Only monitoring the optional INT wire uses GPIO. You can use the `gpiomon` command for a first test without writing any `libgpiod` C code.

## 1. Hardware wiring

Keep the usual I²C connections from [Hardware Setup](hardware.html):

| Raspberry Pi Zero 2 W | TCS34725 breakout | Purpose |
| --- | --- | --- |
| Physical pin 3 / GPIO2 | SDA | I²C data |
| Physical pin 5 / GPIO3 | SCL | I²C clock |
| Ground | GND | Common ground |
| Compatible 3.3 V supply | VIN/VCC | Power (confirm breakout requirements) |
| **Physical pin 11 / GPIO17** | **INT**, if exposed | **Optional interrupt input** |

**Power off before wiring.** INT is an **active-low interrupt output**. Check your breakout's electrical design and pull-up provision. If a pull-up is necessary, use **3.3 V**, never 5 V on the Pi GPIO. Some breakouts do not expose INT; if yours doesn't, use the existing polling tutorial instead. GPIO17 is an example, not a project requirement.

## 2. What triggers an interrupt?

The TCS34725 checks the **Clear (C) channel**, not the classification label. A threshold interrupt can occur when the Clear reading is **below the low threshold** or **above the high threshold**, subject to the persistence configuration. Once asserted, the interrupt is latched until you send the special **clear interrupt** command.

| Register | Address | Meaning |
| --- | --- | --- |
| ENABLE | `0x00` | `PON=0x01`, `AEN=0x02`, `AIEN=0x10` |
| AILTL / AILTH | `0x04` / `0x05` | Low Clear threshold, little-endian |
| AIHTL / AIHTH | `0x06` / `0x07` | High Clear threshold, little-endian |
| PERS | `0x0C` | Interrupt persistence (low 4 bits) |
| STATUS | `0x13` | `AINT=0x10` (interrupt); `AVALID=0x01` (measurement valid) |

For example, thresholds **1000** and **10000** request a threshold condition when Clear is outside that range. Pick useful thresholds from **measured Clear values** under your actual conditions.

## 3. Configure the sensor over I²C (C code)

The following functions are **snippets**, designed to be added to a separate experimental C program using the repository's existing `examples/tcs_i2c.h` and `examples/tcs_i2c.c` learning driver. They are **not** already integrated into the team's production library.

```c
#include "tcs_i2c.h"
#include <stdint.h>
#include <unistd.h>

/* Configure thresholds before enabling the hardware INT output. */
static int configure_clear_interrupt(int fd, uint16_t low,
                                     uint16_t high)
{
    if (low > high) return -1;
    if (tcs_write8(fd, 0x04, (uint8_t)low) < 0) return -1;
    if (tcs_write8(fd, 0x05, (uint8_t)(low >> 8)) < 0) return -1;
    if (tcs_write8(fd, 0x06, (uint8_t)high) < 0) return -1;
    if (tcs_write8(fd, 0x07, (uint8_t)(high >> 8)) < 0) return -1;

    /* APERS=1: one out-of-range Clear measurement. */
    if (tcs_write8(fd, 0x0C, 0x01) < 0) return -1;

    /* Existing tcs_init() already enables PON|AEN (0x03). */
    if (tcs_write8(fd, 0x00, 0x13) < 0) return -1;
    return 0;
}

/* The special-function command is a single command byte, 0xE6.
   It is NOT a write to a normal register. */
static int clear_sensor_interrupt(int fd)
{
    const uint8_t command = 0xE6;
    return write(fd, &command, 1) == 1 ? 0 : -1;
}
```

**Use this sequence:** call `tcs_open()`, call `tcs_init(fd, atime, gain)`, call `configure_clear_interrupt(fd, low, high)`, then monitor the GPIO line. When an interrupt occurs, check STATUS `AINT` via `tcs_read()`, read RGBC as needed, and send `clear_sensor_interrupt(fd)` so the INT line can deassert. When finished, disable interrupt output (for example, set ENABLE back to `0x03`) and call `tcs_shutdown(fd)`.

> Do **not** run a separate experimental process that writes the sensor's registers while another program is using the same sensor. Coordinate ownership of the I²C device.

## 4. Monitor INT with Linux GPIO tools

Install the GPIO tools on Raspberry Pi Linux:

```bash
sudo apt update
sudo apt install -y gpiod
```

Find the GPIO controller and line mapping for **GPIO17** on *your specific Pi/OS* (GPIO chip numbers and offsets can differ):

```bash
gpioinfo
```

For **libgpiod 2.x** tools, the following is an example only when the identified chip and offset really correspond to the connected GPIO17 line:

```bash
gpiomon --chip gpiochip0 --edges falling 17
```

Different `gpiomon` versions have different option syntax. Run `gpiomon --help` and adjust accordingly. For **libgpiod 1.x**, the older form is usually `gpiomon -f gpiochip0 17`. On some devices, the line offset is not 17 and the controller is not `gpiochip0`.

The GPIO tool only **listens**. A separate I²C program must configure the threshold, enable `AIEN`, and clear the latched interrupt. If the threshold condition remains true, the line can reassert after clearing; you may not get another falling edge until it has returned high.

## 5. Do we need `-lgpiod` during compilation?

| Code being compiled | Link `-lgpiod`? |
| --- | --- |
| Existing I²C polling examples | **No** |
| Threshold configuration using `tcs_write8()` and `write()` | **No** |
| Testing INT with the `gpiomon` command | **No** (no GPIO C program to compile) |
| A **C** program that calls the `libgpiod` library API | **Yes**, using flags appropriate to the installed library version |

For example, with `libgpiod` development files installed, you can discover the correct compiler/linker flags using:

```bash
sudo apt install -y libgpiod-dev pkg-config
pkg-config --cflags --libs libgpiod
```

The `libgpiod` **1.x** and **2.x** C APIs differ substantially. Do not copy a 1.x C example into a 2.x build without updating it. The tutorial's original `examples/Makefile` needs **no changes** unless you deliberately add a C interrupt-listener example.

## 6. Troubleshooting

- **No INT pin on the breakout:** use I²C polling; nothing is missing for the main project.
- **No falling edge:** confirm INT is wired to the expected GPIO and a 3.3 V-compatible pull-up is present; verify thresholds and `AIEN`.
- **INT remains low:** clear the latched interrupt with **`0xE6`**. If the Clear measurement is still out of range, it may reassert.
- **Permission denied on `/dev/gpiochip*`:** verify permissions/group configuration; avoid permanently weakening device permissions.
- **Wrong GPIO line:** identify the actual chip and line with `gpioinfo`, instead of assuming a universal offset.
- **No hardware test yet:** none of these instructions establishes that INT has been tested on your particular breakout.

## References

- [TCS3472/TCS34725 family datasheet](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf) — interrupt thresholds, persistence, AIEN, and clear command.
- [Adafruit TCS34725 reference driver](https://github.com/adafruit/Adafruit_TCS34725/blob/master/Adafruit_TCS34725.cpp) — illustrative register definitions and clear command.
- [libgpiod documentation](https://libgpiod.readthedocs.io/en/stable/) — GPIO line/event tool usage (verify installed version).

Next: [Compilation](compilation.html) or [RGBC Acquisition](rgbc-acquisition.html).
