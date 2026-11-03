---
layout: default
title: Library Reuse
---
# Using the team's library later (required Use section)

The team's main repository will eventually provide public headers and these functions. **Their exact C signatures have not yet been agreed**, so the sequence below is **pseudocode**, not a drop-in compilable application.

```text
Sensor_Init() -> Sensor_SetGain() / Sensor_SetIntegrationTime()
  -> Sensor_ReadRaw(&raw)
  -> RGB_Validate(&raw)
  -> RGB_Normalize(&raw, &color)
  -> RGB_DetectColor(&color)
  -> display results in the caller
  -> Sensor_Close()
```

## Interface review before code integration

1. Agree on `RGB_Raw` fields: `uint16_t red, green, blue, clear`.
2. Agree on `RGB_Color`: `uint8_t red, green, blue; const char *name; float confidence`.
3. Agree on status codes for success, failure, low-light, saturation, and not-ready.
4. Agree on how current ATIME/integration time reaches Member 3 without coupling `color_detection.c` to hardware.
5. Agree on whether calibration defaults are compile-time constants or a public setter.
6. Agree on the I²C bus device path (typically `/dev/i2c-1`) and whether it is configurable.
7. Agree on ownership: M1 registers/configuration, M2 acquisition, M3 processing, M4 classification, M5 application.
8. Test with a second small C application that includes the library headers and calls **public** functions only. No copying of private register helpers.

## Build notes

A simple project can compile `main.c`, `RGB_Library/rgb_sensor.c`, and `RGB_Library/color_detection.c` with GCC. No `-lbcm2835` is needed when using `i2c-dev`. Your app requires permission to access `/dev/i2c-*`. Exact Makefile options depend on the team's final source code.

This tutorial's `examples/tcs_i2c.*` is instructional and should **not** be pasted into the team's main repo or treated as its official library API.
