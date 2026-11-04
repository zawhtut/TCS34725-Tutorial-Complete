---
layout: default
title: "Hardware Interrupts (Optional)"
nav_order: 16
---

# Hardware Interrupts (Optional)

**This is an advanced, optional topic.** We do **not** need the INT pin or `libgpiod` to read raw RGBC values. The basic program uses I²C and waits before requesting another reading.

## Polling versus interrupts

- **Polling:** The program asks the sensor for readings at intervals. Our beginner example does this.
- **Hardware interrupt:** The sensor signals the Raspberry Pi through its optional **INT** pin when a configured condition occurs. The program can wait for that signal instead of checking continuously.

## What does the TCS34725 INT pin mean?

The TCS34725 has an **active-low INT output**. Its RGBC interrupt feature is based on the **Clear channel crossing a configured threshold range**, possibly for several consecutive measurement cycles. It is **not simply a signal for every new RGB sample**.

The sensor supports:

- A **lower Clear threshold** in registers `0x04`–`0x05`.
- An **upper Clear threshold** in registers `0x06`–`0x07`.
- A **persistence setting** in register `0x0C` (how many out-of-range cycles before an interrupt).
- An interrupt enable bit **AIEN**, value `0x10`, in ENABLE register `0x00`.

We configure these registers **over I²C**, just like the basic program.

## How would we connect it?

1. Keep GND, power, SDA and SCL as before.
2. If the breakout exposes `INT`, connect it to an **available Raspberry Pi GPIO input** compatible with 3.3 V logic. Check that breakout's output electrical behavior and any pull-up requirements first.
3. Use a Linux GPIO interface (commonly `libgpiod`) to watch that input for an edge or change.
4. When the interrupt occurs, read the sensor status and RGBC data over **I²C**.
5. Clear the interrupt condition using the sensor's documented special command, then continue.

**Do not wire INT to SDA or SCL.** It is a separate signal.

## Do we need `-lgpiod`?

Only if our C program actually calls functions from the `libgpiod` library. We do **not** link it for the beginner polling example. GPIO APIs and compile instructions differ between libgpiod major versions, so use the instructions for the version installed on your Pi if you implement INT monitoring.

## Why no complete interrupt-listener program yet?

First learn simple I²C reading. Interrupt handling introduces a second Linux interface (GPIO), extra wiring, threshold configuration and signal timing. It is easier to understand once basic readings work.

See [References](references.html) for sensor register details.
