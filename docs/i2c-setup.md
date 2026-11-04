---
layout: default
title: "Enable I²C"
nav_order: 3
---

# Enable I²C

**Goal:** Confirm that Linux has an I²C bus and that the sensor answers at its address.

## 1. Install the tools

```bash
sudo apt update
sudo apt install -y i2c-tools build-essential
```

- `i2c-tools` provides `i2cdetect` for checking the bus.
- `build-essential` provides GCC and related build tools.
- `libi2c-dev` is **not required** for our direct `open()`, `ioctl()`, `write()`, and `read()` example.

## 2. Enable I²C if necessary

On Raspberry Pi OS, you can usually run:

```bash
sudo raspi-config
```

Choose **Interface Options → I2C → Enable**. On other Debian-based installations, I²C configuration may be different.

## 3. Find the bus

```bash
ls /dev/i2c-*
```

On our typical Pi setup, `/dev/i2c-1` is used. Seeing `/dev/i2c-2` as well is not a problem.

## 4. Check address `0x29`

```bash
sudo i2cdetect -y 1
```

Look for `29` in the grid. That means a device acknowledged address `0x29` on **bus 1**. It does not prove the device is a TCS34725 yet.

If `29` is missing, check wiring, power and which bus your pins are using. Don't scan random buses without knowing what is attached; scanning can interfere with some devices.

## 5. Permissions

If your C program later shows `Permission denied`, test whether your user belongs to the `i2c` group:

```bash
groups
```

On Raspberry Pi OS, the usual fix is adding your user to `i2c` and signing out/in:

```bash
sudo usermod -aG i2c "$USER"
```

We will use `/dev/i2c-1` in all beginner code. Go next to [GPIO vs. I²C](gpio-vs-i2c.html).
