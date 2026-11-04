---
layout: default
title: "Color Names"
nav_order: 11
---

# Give a Color an Approximate Name

**Goal:** Print a simple color label after converting raw sensor values to `red_8`, `green_8`, and `blue_8`.

## Start with ordinary `if` statements

Add this after the 8-bit conversion:

```c
const char *color_name = "Unknown";

if (maximum == 0 || clear < 20) {
    color_name = "Too dark";
} else if (red_8 > green_8 && red_8 > blue_8) {
    color_name = "Red dominant";
} else if (green_8 > red_8 && green_8 > blue_8) {
    color_name = "Green dominant";
} else if (blue_8 > red_8 && blue_8 > green_8) {
    color_name = "Blue dominant";
} else {
    color_name = "Mixed / similar channels";
}

printf("Color: %s\n", color_name);
```

- `color_name` holds text we want to print.
- `>` compares channel values.
- `&&` means **and**: both comparisons must be true.
- `clear < 20` is only a *temporary learning threshold*, not a universal sensor rule.

This beginner example reports the **strongest color channel**, not a full color classifier. Yellow, cyan and magenta often involve two strong channels, so the result may be “Mixed.”

## How could we improve it later?

After the simple version works, we can keep a list of reference RGB colors and compare each one with our measured 8-bit values. The closest reference could be labeled Red, Green, Yellow, etc.

A distance-based score can be called **similarity** or an approximate **confidence** score, but it is **not a probability** that the color label is correct.

The final assignment needs an approximate color name and confidence. Those belong in the **later reusable color-processing module**, not in the first I²C lesson.
