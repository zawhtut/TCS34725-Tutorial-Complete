---
layout: default
title: "Troubleshooting"
nav_order: 17
---

# Troubleshooting

## `ls /dev/i2c-*` shows nothing

I²C may not be enabled, or your OS uses a different configuration. See [Enable I²C](i2c-setup.html).

## `i2cdetect -y 1` does not show `29`

Check power, ground, SDA and SCL, the bus number, and whether your breakout supports the supplied voltage. Turn off power before rewiring.

## `Permission denied` when opening `/dev/i2c-1`

Check your user's groups with `groups`. On many Pi systems the user must belong to the `i2c` group; sign out and back in after changing membership.

## `read()` or `write()` fails

Check that your program opened `/dev/i2c-1` and selected `0x29`. Confirm the sensor is connected. If separate command writes and data reads are unreliable, a more complete driver may use Linux's `I2C_RDWR` combined transaction interface.

## Five readings are identical

The sensor might be seeing steady light, or the program may be reading before a new measurement is ready. The simple example waits between reads but does not check STATUS AVALID or prove sample freshness. Check those features when upgrading the driver.

## Numbers change, but color names are wrong

A working I²C link is not the same as calibrated color detection. Check lighting, gain, integration time, sensor distance and reference colors. Learn [Clear](clear-channel.html) and [Calibration](calibration.html).

## Output is `RGB Hex: #...`, but it looks too bright

Max-channel normalization forces the largest channel to 255. It removes brightness information; it is not calibrated sRGB.

## Compiler says `usleep` is undeclared

Try the ordinary command `gcc TCS_main.c -o TCS_main` on the Pi. Strict ISO C compilation modes can hide this older POSIX function's declaration. A later version can use `nanosleep()` once we explain `struct timespec`.

## The `Hardware Interrupts` page is missing an executable example

That's intentional. Hardware interrupts are not part of the basic polling implementation. No GPIO library should be necessary just to run `TCS_main.c`.
