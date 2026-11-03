---
layout: default
title: RGBC Acquisition
---
# Reading raw RGBC measurements

1. Open the I²C device and set slave address `0x29`.
2. Read the device ID and check against your hardware variant.
3. Write `ATIME` for the desired integration period (for example `0xEB` = 50.4 ms).
4. Write `CONTROL` to choose gain (for example `0x01` = 4x).
5. Write ENABLE `0x01` to power on; wait at least about 2.4 ms for oscillator startup.
6. Write ENABLE `0x03` to enable RGBC conversion.
7. Wait for an integration cycle, then poll STATUS `AVALID` bit 0.
8. Burst read eight bytes from register `0x14`, using command `0xB4`.
9. Combine 16-bit little-endian channel values:

```c
clear = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
red   = (uint16_t)data[2] | ((uint16_t)data[3] << 8);
green = (uint16_t)data[4] | ((uint16_t)data[5] << 8);
blue  = (uint16_t)data[6] | ((uint16_t)data[7] << 8);
```

10. On exit, write ENABLE `0x00` where possible, then close the file descriptor.

## Compile and try

```bash
make -C examples
./examples/test_sensor_id /dev/i2c-1
./examples/read_rgbc /dev/i2c-1 5
```

The second argument is the number of measurements (default 5). The program delays between samples and times out if STATUS never becomes valid.

## Saturation and freshness

A useful **digital** upper-count estimate is `min(65535, (256 - ATIME) * 1024)`. Analog saturation can occur earlier at high gain or bright light, so observe and tune actual readings. Clear = 0 prevents dividing by Clear. `AVALID` means a measurement finished, **not** that a repeated read is necessarily a new integration result. Coordinate sample timing with the chosen integration period.

Next: [Processing](processing.html) or [Compilation](compilation.html).
