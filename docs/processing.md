---
layout: default
title: "8-bit RGB and Hex"
nav_order: 10
---

# Convert Raw RGB to 8-bit RGB and Hex

**Goal:** Start with raw counts and print familiar RGB values from 0–255. We are not claiming that these are calibrated display colors.

We will use a simple learning method: find the largest of Red, Green and Blue; scale it to 255; scale the others proportionally.

## Add this underneath the raw reading in `TCS_main.c`

```c
unsigned int maximum = red;

if (green > maximum) {
    maximum = green;
}

if (blue > maximum) {
    maximum = blue;
}

unsigned int red_8 = 0;
unsigned int green_8 = 0;
unsigned int blue_8 = 0;

if (maximum > 0) {
    red_8   = red * 255 / maximum;
    green_8 = green * 255 / maximum;
    blue_8  = blue * 255 / maximum;
}

printf("8-bit RGB: R=%u G=%u B=%u\n", red_8, green_8, blue_8);
printf("RGB Hex: #%02X%02X%02X\n", red_8, green_8, blue_8);
```

These lines belong **inside the `for` loop, after** we calculate `red`, `green`, and `blue`.

## Understand the calculation

If the raw values are `R=644`, `G=1004`, and `B=392`, the largest value is `1004`.

```text
Red   = 644 * 255 / 1004 = 163
Green = 1004 * 255 / 1004 = 255
Blue  = 392 * 255 / 1004 = 99
```

The result is approximately `RGB(163, 255, 99)` or `#A3FF63`.

## What does `%02X` mean?

- `%X` prints a number in hexadecimal using uppercase letters.
- `2` gives at least two character positions.
- `0` fills an unused leading position with zero.

So `10` prints as `0A`, and `255` prints as `FF`. Three two-digit values give `#RRGGBB`.

## Is this accurate sRGB?

**No.** This approach throws away absolute brightness and does not correct the sensor's spectral response. It is useful for seeing color proportions while learning. A dark and a bright object can produce similar hex output. More reliable color output requires testing and [calibration](calibration.html).

Next: [Color Names](color-names.html).
