---
layout: default
title: Tutorial Home
---
# TCS34725 + Raspberry Pi: Linux I²C in C

Welcome. This site is a standalone companion to the RGB Sensor team assignment. It does **not** modify or mirror the main application repository.

## Follow these pages in order

1. [Hardware connections](hardware.html) — power, SDA, SCL and safe wiring.
2. [Enable I²C](i2c-setup.html) — kernel device, packages, permissions, scan.
3. [GPIO versus I²C](gpio-vs-i2c.html) — why no GPIO library is needed.
4. [Registers & I²C transactions](registers.html) — protocol and `I2C_RDWR`.
5. [Acquiring RGBC data](rgbc-acquisition.html) — gain, time, bytes and freshness.
6. [Compilation](compilation.html) — build and run the examples.
7. [Processing and calibration](processing.html) — validating and classifying measurements.
8. [Library reuse](library-usage.html) — integrate the final team library without copying its internals.
9. [Troubleshooting](troubleshooting.html) and [References](references.html).

## On the Pi (after wiring and enabling I²C)

```bash
sudo apt update
sudo apt install -y build-essential i2c-tools libi2c-dev
ls /dev/i2c-*
i2cdetect -y 1
```

Compile and run the downloadable C examples from the repository:

```bash
make -C examples
./examples/test_sensor_id /dev/i2c-1
./examples/read_rgbc /dev/i2c-1 5
make -C examples check
```

The website contains explanations; the `.c` files are in the repository's `examples/` directory, not inside the published Pages site.
