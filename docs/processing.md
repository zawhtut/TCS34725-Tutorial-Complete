---
layout: default
title: Processing and Calibration
---
# Processing, calibration, and classification

This page maps to Members 3 and 4; it is **separate from direct I²C access**.

## Measurement quality

- Reject null input and Clear = 0.
- Flag Clear below a configurable threshold as low-light.
- Estimate digital saturation using integration time: `min(65535, cycles * 1024)` with `cycles = 256 - ATIME`.
- High gain/bright illumination may cause earlier analog saturation; evaluate hardware data.

## Normalize

For a simple instructional baseline, compute channel ratios with floating point:

```c
float rn = (float)raw.red   / raw.clear;
float gn = (float)raw.green / raw.clear;
float bn = (float)raw.blue  / raw.clear;
```

Then apply separately tuned scaling constants and clamp into `[0,255]`. **Do not assume those values are calibrated sRGB**. The processing tests include a sample baseline to show numeric behavior, not to claim colorimetric accuracy.

## Reference classification

Use squared Euclidean RGB distance to choose the closest reference:

```text
distance = (r-R)^2 + (g-G)^2 + (b-B)^2
confidence = 100 * (1 - distance / (3*255*255))
```

Use signed differences; confidence is a **similarity score**, not a statistical probability. Reference arrays should be tuned against measured data; bright/dark shade classification may require preserving additional brightness information rather than only ratios.

## Test without the Pi

`make -C examples check` runs `examples/test_processing.c`, which tests Clear-zero rejection, normalized range, a simple exact-reference match, and similarity score bounds. It is **not** the team's finished color library.
