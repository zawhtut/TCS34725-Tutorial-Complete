---
layout: default
title: Hardware Setup
nav_order: 2
---
# Hardware connections

**Turn off and unplug the Raspberry Pi before wiring.** Check your actual TCS34725 breakout board's pin labels and electrical specifications. The bare IC and different breakouts can have different supply/logic requirements.

On a Raspberry Pi Zero 2 W's standard 40-pin header:

| Raspberry Pi | Physical pin | Connect to breakout |
| --- | ---: | --- |
| GPIO 2 / SDA1 | 3 | SDA |
| GPIO 3 / SCL1 | 5 | SCL |
| Ground | 6 | GND |
| 3.3 V power | 1 | **VIN/VCC only if breakout supports 3.3 V supply** |

Use a common ground. **Never drive the Pi SDA/SCL lines at 5 V**. Boards that accept 5 V power are not automatically safe for 5 V I²C signaling; verify onboard level shifting. If unsure, choose a verified 3.3 V-compatible breakout and supply it per its documentation.

`INT` is optional for our polling examples. Any onboard illumination LED control is breakout-specific; leave extra pins disconnected unless needed.

## Checklist

- [ ] Verify breakout model and voltage range.
- [ ] Verify SDA and SCL are not swapped.
- [ ] Verify GND is shared.
- [ ] Use short, secure connections; turn power off before changing them.
- [ ] Enable I²C in Linux before testing.

Next: [I²C setup](i2c-setup.html).
