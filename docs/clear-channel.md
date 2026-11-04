---
layout: default
title: "The Clear Channel"
nav_order: 9
---

# What Does the Clear Channel Do?

The TCS34725 reports four measurements:

- **R** — light measured through a red filter.
- **G** — light measured through a green filter.
- **B** — light measured through a blue filter.
- **C (Clear)** — light measured without those red, green, or blue filters.

The Clear channel is **not another color**. It gives information about the overall amount of light reaching the sensor.

## Example

```text
R = 644
G = 1004
B = 392
C = 2085
```

Here the Green measurement is strongest, and Clear is a measure of the overall unfiltered light response.

## Why is Clear useful?

1. **Low light:** If Clear is near zero, the RGB readings may be too small for a useful result.
2. **Bright light:** Very high values may indicate saturation. The sensor cannot describe light accurately once its measurement reaches the limit.
3. **Comparing samples:** Clear can help estimate whether lighting changed between readings.
4. **Normalization:** Some algorithms use channel ratios such as `red / clear`. This is a *relative measurement*, not automatically correct 8-bit sRGB.

## Why not simply divide every channel by Clear?

For example:

```c
float red_ratio = (float)red / clear;
```

The `(float)` makes the division produce a fractional result. If `clear == 0`, do **not** divide.

A ratio does not automatically tell us the exact screen color of an object. The sensor has its own filters, the illumination matters, and we still need calibration.

## Important difference: brightness and color

Two objects may have similar R:G:B proportions but very different light levels. If we always scale the largest RGB channel to 255, we throw away much of that brightness information. Keep the original `clear` and raw values when testing.

Continue to [8-bit RGB and Hex](processing.html).
