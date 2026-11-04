---
layout: default
title: "Overview"
nav_order: 1
---

# TCS34725: Learn I²C in C, one step at a time

This guide is for our CSC615 team. We will **get raw color readings working first**, then convert them to 8-bit RGB and hexadecimal, and only afterward consider color names, gain, calibration, and interrupts.

We use a Raspberry Pi running Linux, a TCS34725 color sensor, and C. **We do not need BCM2835 or a GPIO library for the basic I²C example.** We communicate through `/dev/i2c-1`.

## Follow the pages in this order

1. [Hardware Setup](hardware.html) — connect four wires safely.
2. [Enable I²C](i2c-setup.html) — check that Linux sees the sensor.
3. [GPIO vs. I²C](gpio-vs-i2c.html) — understand SDA and SCL.
4. [`/dev/mem` vs. `/dev/i2c`](dev-mem-vs-i2c.html) — understand what these device files really do.
5. [First C Program](first-program.html) — open the bus and select address `0x29`.
6. [Sensor Registers](registers.html) — understand the bytes we send.
7. [Read Raw RGBC](rgbc-acquisition.html) — **complete `TCS_main.c`** that reads the sensor.
8. [The Clear Channel](clear-channel.html) — what `C` measures.
9. [8-bit RGB and Hex](processing.html) — add `red_8`, `green_8`, and `blue_8`.
10. [Color Names](color-names.html) — approximate labels, then confidence.
11. [Gain and Integration Time](gain-integration.html) — optional sensor settings.
12. [Calibration](calibration.html) — why readings change with light.
13. [Compile and Run](compilation.html) — use the smallest `gcc` command.
14. [Build a Reusable Library](library-usage.html) — move proven steps into `.c` and `.h`.
15. [Hardware Interrupts (Optional)](interrupts.html) — INT pin and thresholds.
16. [Troubleshooting](troubleshooting.html) — common errors.
17. [References](references.html) — datasheet and Linux documentation.

## Start here on the Raspberry Pi

```bash
ls /dev/i2c-*
sudo i2cdetect -y 1
```

If the scan shows `29`, a device responds at the TCS34725 address. This alone does not prove its identity; a sensor ID read can confirm it later.

**About the files:** This is a *separate tutorial repository*. Do not copy these Markdown files into the team's main project. The existing `examples/` programs may still be present, but the new teaching path uses a simple `TCS_main.c` file you create from the code on the [Read Raw RGBC](rgbc-acquisition.html) page.
