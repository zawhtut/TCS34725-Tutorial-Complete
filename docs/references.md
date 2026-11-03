---
layout: default
title: Sources and Scope
---
# References and scope

- [TCS3472 / TCS34725 family datasheet (PDF)](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf) — integration timing, command/register map, STATUS, saturation.
- [Adafruit reference driver register definitions](https://github.com/adafruit/Adafruit_TCS34725/blob/master/Adafruit_TCS34725.h) — reference for addresses, family ID and gain values. This tutorial does not copy its driver code.
- [Linux kernel I²C dev-interface documentation](https://docs.kernel.org/i2c/dev-interface.html) — `/dev/i2c-*`, `I2C_SLAVE`, and `I2C_RDWR`.
- [GitHub Pages publishing source](https://docs.github.com/en/pages/getting-started-with-github-pages/creating-a-github-pages-site) — how to serve `/docs/index.md`.

## Limitations

- C programs require a Linux machine with an accessible compatible I²C bus and correctly wired hardware.
- I²C addresses and IDs should be verified with the specific board in hand.
- The demo's normalization/classification examples are basic educational algorithms, not calibrated scientific color measurement.
- This repo intentionally remains independent from the team source tree; no production signatures or implementation are asserted as final.
