---
layout: default
title: "References"
nav_order: 18
---

# References and Further Reading

These links provide the technical details behind the simple examples.

- [TCS3472/TCS34725 family datasheet (ams OSRAM)](https://look.ams-osram.com/m/56f0ca23f11d867d/original/TCS3472-DS000390.pdf) — sensor registers, enable bits, gain, integration time, Clear channel, interrupt behavior. If this PDF moves, search the manufacturer for **TCS3472 datasheet**.
- [Adafruit TCS34725 register definitions](https://github.com/adafruit/Adafruit_TCS34725/blob/master/Adafruit_TCS34725.h) — readable register values such as `ENABLE`, `ATIME`, `CONTROL`, `STATUS`, and interrupt thresholds.
- [Linux I²C dev interface](https://docs.kernel.org/i2c/dev-interface.html) — using `/dev/i2c-*`, `ioctl()` and `I2C_RDWR` from C.
- [Raspberry Pi documentation](https://www.raspberrypi.com/documentation/computers/configuration.html) — system configuration and interfaces.
- [Just the Docs page ordering](https://just-the-docs.com/docs/navigation/main/order/) — how `nav_order` controls the GitHub Pages sidebar.

## Notes on the tutorials

- The code is a **beginner demonstration**, not a hardware-validated production driver.
- Clear-based ratios and max-channel scaling are approximations, not accurate sRGB conversion.
- Linux I²C device files belong to the Linux driver system. The C code does **not** manually drive SDA/SCL.
- The optional INT pin is a separate GPIO signal. It is not needed for normal polling reads.
