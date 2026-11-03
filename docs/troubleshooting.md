---
layout: default
title: Troubleshooting
---
# Troubleshooting

| Problem | Checks |
| --- | --- |
| `/dev/i2c-1` missing | Enable I²C; verify actual bus number/OS configuration; reboot after config changes. |
| `i2cdetect` shows no `29` | Power off, check GND, SDA/SCL, compatible voltage levels, and pull-ups/breakout. Confirm right bus. |
| Permission denied | Inspect `ls -l /dev/i2c-*`, group memberships, re-login after adding user to `i2c` group. |
| `I2C_RDWR` fails or unsupported | Check correct bus, adapter functionality and permissions; examples specifically require combined transfers. |
| Unexpected ID | Check module family; report `0x44` is common for TCS34721/25, `0x4D` for related 23/27 variants; verify board. |
| STATUS always NOT_READY | Verify ENABLE `PON` then `AEN`, integration time, oscillator delay, and wiring. |
| All channels zero | Check illumination, power, conversion enable, gain, time and bus. |
| Channels saturate | Reduce gain, shorten integration time, reduce illumination. |
| Normalized RGB looks incorrect | Ratios are not calibrated sRGB; tune scaling under controlled light and test multiple references. |
| Readings repeat | AVALID can remain asserted; reading it does not guarantee a new sample. Slow polling to match integration time. |
| Build fails due to missing `<linux/i2c-dev.h>` | Build on Linux; install distro's I²C headers / `libi2c-dev`; check toolchain. |
| Website 404 | Confirm Pages source `main` `/docs`, `docs/index.md`, repository visibility and deployment status. |

## Avoid destructive troubleshooting

Do not permanently change I²C device permissions to world-writable and do not modify your team's main repo to try these examples. If your bus has other devices, avoid blind scans or writes.
