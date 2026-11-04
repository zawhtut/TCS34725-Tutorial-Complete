---
layout: default
title: "Read Raw RGBC"
nav_order: 8
---

# Read Raw RGBC: Complete Beginner Program

**Goal:** Connect to the sensor, start measurements, read four raw channels, and close the device. Everything is in one `main()` function. **No custom functions or custom structures.**

Create a file named **`TCS_main.c`** in a separate practice folder:

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main(void)
{
    int fd;
    unsigned char enable[2];
    unsigned char command;
    unsigned char data[8];
    unsigned int red, green, blue, clear;

    /* Open I2C bus 1 on the Raspberry Pi. */
    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        perror("Cannot open I2C");
        return 1;
    }

    /* Choose address 0x29, used by the TCS34725. */
    if (ioctl(fd, I2C_SLAVE, 0x29) < 0) {
        perror("Cannot select sensor");
        close(fd);
        return 1;
    }

    /* Write 0x01 to register 0x00 to turn on sensor power. */
    enable[0] = 0x80;  /* Command for register 0x00 (ENABLE) */
    enable[1] = 0x01;  /* PON = 1 */
    if (write(fd, enable, 2) != 2) {
        perror("Cannot power on sensor");
        close(fd);
        return 1;
    }

    usleep(3000);      /* Wait 3 milliseconds for oscillator startup. */

    /* Enable the RGBC measurement circuit as well as power. */
    enable[1] = 0x03;  /* PON = 1 and AEN = 1 */
    if (write(fd, enable, 2) != 2) {
        perror("Cannot enable RGBC");
        close(fd);
        return 1;
    }

    printf("Reading TCS34725...\n\n");

    for (int i = 0; i < 5; i++) {
        /* Allow enough time for a measurement. */
        usleep(700000);

        /* Start at register 0x14 and read consecutive registers. */
        command = 0xB4;
        if (write(fd, &command, 1) != 1) {
            perror("Cannot select RGBC registers");
            break;
        }

        /* Get eight bytes: two each for C, R, G, and B. */
        if (read(fd, data, 8) != 8) {
            perror("Cannot read RGBC bytes");
            break;
        }

        /* First byte is low; second byte is high (256 times bigger). */
        clear = data[0] + data[1] * 256;
        red   = data[2] + data[3] * 256;
        green = data[4] + data[5] * 256;
        blue  = data[6] + data[7] * 256;

        printf("Sample %d: R=%u G=%u B=%u C=%u\n",
               i + 1, red, green, blue, clear);
    }

    /* Switch the sensor off, then close our Linux I2C connection. */
    enable[1] = 0x00;
    write(fd, enable, 2);
    close(fd);

    printf("Sensor closed.\n");
    return 0;
}
```

## Compile and run

```bash
gcc TCS_main.c -o TCS_main
./TCS_main
```

Example of **raw** output (your measurements will differ):

```text
Sample 1: R=644 G=1004 B=392 C=2085
```

## Why `&command` in `write()`?

`command` stores one byte. `&command` is its **memory address**. The `write()` function needs an address so it knows where the byte is stored. `write(fd, &command, 1)` sends **one byte** from that address.

## Why `data[8]`?

There are four channels, and each channel uses **two bytes**. Four times two equals eight.

## What this example intentionally leaves out

- It doesn't check the sensor ID or the STATUS `AVALID` bit. The long wait is only a simple first demonstration, and a returned sample may not necessarily be fresh.
- It doesn't configure gain or integration time. Those can be introduced later once basic communication works.
- It does not use the optional interrupt pin.
- It doesn't calculate true sRGB yet.

For a more reliable driver, add sensor ID checks, data-ready checks, and more complete error handling when we [build a library](library-usage.html). Next, learn what [Clear](clear-channel.html) means.
