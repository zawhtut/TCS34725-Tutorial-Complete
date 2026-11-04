---
layout: default
title: "Calibration"
nav_order: 13
---

# Calibration: Why Different Lighting Changes the Result

**Goal:** Understand why a sensor that reads raw RGBC successfully may still name colors incorrectly.

A color sensor does not see colors exactly as a human eye or a computer display does. Readings change with the light source, sensor distance, object surface, gain, and integration time.

## Simple test procedure

1. Keep the sensor and object at a fixed distance.
2. Use the same light source for the first set of tests.
3. Record raw `R`, `G`, `B`, and `C` for a red object, green object, blue object, and white reference.
4. Record gain and integration time used for each test.
5. Repeat the readings and compare how much they change.
6. Try another lighting condition and observe the difference.

Example recording table:

| Object | R | G | B | C | Notes |
|---|---:|---:|---:|---:|---|
| Red | — | — | — | — | Same lamp and distance |
| Green | — | — | — | — | Same lamp and distance |
| Blue | — | — | — | — | Same lamp and distance |
| White | — | — | — | — | Same lamp and distance |

## What we can adjust later

- **Gain** if counts are too low or too high.
- **Integration time** if we need more or less light collection.
- **Reference RGB values** for classifying colors observed by our actual sensor.
- **Channel scaling factors** if raw red, green, and blue responses are consistently unbalanced.

Do not assume a ratio such as `red / clear` automatically equals the correct red value on a display.

## Reflected versus emitted light

A colored object reflects light from the lamp. A colored LED emits its own light. They can need different calibration tests. We can use the same basic I²C reading code for both.

The first goal is repeatable readings. Accurate color labels and meaningful confidence come later.
