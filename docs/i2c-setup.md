---
layout: default
title: Enable I²C
nav_order: 3
---
# Enable I²C on Raspberry Pi

These instructions target Raspberry Pi OS or another Debian-based Linux image with the standard Pi I²C controller. System menus and configuration paths may vary.

## 1. Install compiler, development headers and diagnostic utility

```bash
sudo apt update
sudo apt install -y build-essential i2c-tools libi2c-dev
```

## 2. Enable I²C

On Raspberry Pi OS with `raspi-config` available:

```bash
sudo raspi-config
```

Select **Interface Options → I2C → Enable**, then reboot if prompted. On other Debian-based images, use the image's documented device-tree/I²C method instead.

## 3. Check the Linux device

```bash
ls -l /dev/i2c-*
```

Typically you will use `/dev/i2c-1`, but **verify** the bus on your board/image.

## 4. Detect the sensor

```bash
i2cdetect -y 1
```

On bus 1 a working TCS34725 normally responds at hexadecimal address **`29`**. `UU` means the address is in use by a kernel driver. A scan can have side effects on some connected devices; avoid scanning a shared production bus.

## 5. Permission errors

Inspect the device permissions:

```bash
ls -l /dev/i2c-1
groups
```

On many Pi images, membership in the `i2c` group grants access. If that group and device permission policy exist, you can run:

```bash
sudo usermod -aG i2c "$USER"
```

Log out/in afterward. Do **not** `chmod 666` device nodes as a permanent workaround. For a one-off hardware diagnostic, running a command with `sudo` can help confirm a permission issue.

Next: [GPIO versus I²C](gpio-vs-i2c.html).
