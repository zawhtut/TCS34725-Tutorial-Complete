---
layout: default
title: "GPIO vs. I²C"
nav_order: 4
---

# GPIO vs. I²C

**Goal:** Understand why SDA and SCL are pins on the GPIO header, but our C code does not manually toggle them.

## GPIO

GPIO stands for *general-purpose input/output*. A program can set a GPIO pin high or low, or read whether it is high or low.

## I²C

I²C is a protocol for devices to exchange data over **SDA** and **SCL**. The Raspberry Pi has an I²C controller and Linux makes it available as `/dev/i2c-1`.

Our C program asks Linux to transfer bytes. Linux and the I²C controller handle the electrical timing. We do **not** write code to toggle SCL ourselves.

```text
C program
   |
open(), ioctl(), write(), read()
   |
Linux I2C driver (/dev/i2c-1)
   |
Pi SDA (GPIO 2), SCL (GPIO 3)
   |
TCS34725 at address 0x29
```

## Do we need a GPIO library?

**No**, not for ordinary color readings. We also don't need BCM2835 or `-lgpiod` during compilation.

A GPIO interface becomes useful only if we later attach the sensor's optional **INT** pin to another GPIO input to receive hardware interrupts. See [Hardware Interrupts (Optional)](interrupts.html).

## A related question: what is `/dev/mem`?

`/dev/mem` is different from an I²C bus device. It is used for access to physical addresses, not for sending I²C bytes to a sensor. Read [**`/dev/mem` vs. `/dev/i2c`**](dev-mem-vs-i2c.html) before the first C program.
