---
layout: default
title: "Build a Reusable Library"
nav_order: 15
---

# Move the Working Code into a Reusable Library

**Goal:** After the one-file program works, let another C project use the sensor without copying all the low-level I²C steps.

Do **not** begin with a large library. First confirm `TCS_main.c` reads the sensor.

## Step 1: Identify the repeated jobs

Our one-file program already performs these actions:

1. Open I²C and select address `0x29`.
2. Turn on the sensor.
3. Read the raw four channels.
4. Turn off the sensor and close the connection.

Those are good candidates to become functions because **another program will need them too**.

## Step 2: Small file layout

```text
RGB_Sensor_Project/
  main.c
  RGB_Library/
    rgb_sensor.h
    rgb_sensor.c
```

`rgb_sensor.h` tells other files which functions are available. `rgb_sensor.c` contains the actual function code. `main.c` calls them.

For example, the header could eventually declare:

```c
#ifndef RGB_SENSOR_H
#define RGB_SENSOR_H

int Sensor_Init(void);
int Sensor_ReadRGB(unsigned int *red,
                   unsigned int *green,
                   unsigned int *blue,
                   unsigned int *clear);
void Sensor_Close(void);

#endif
```

### Why does `*red` appear here?

A function can return only one value directly with `return`. Using **pointers** lets a function fill several variables supplied by its caller. The `*` means the parameter is a pointer; the caller will pass an address such as `&red`. We introduce this only when splitting the working program into functions.

## Step 3: Add processing *after* reading works

Later, a separate `color_detection.c` can convert raw readings into 8-bit RGB, approximate names and similarity scores. The main program should not need to know sensor register addresses.

## Step 4: Test reusability

Make a second small `main.c` that calls the public functions. If it works without copying register commands, the library is reusable.

The team assignment also requires a **Use section**. Explain headers, function parameters, return values, how to compile, and a short example of calling the library. Keep explanations next to the code when possible.
