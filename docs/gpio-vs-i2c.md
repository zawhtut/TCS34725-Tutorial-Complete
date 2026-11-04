---
layout: default
title: GPIO vs I²C
---
# Why we still use I²C but not a GPIO library

The TCS34725 exposes a digital **I²C** register interface, addressed at `0x29`. The Pi's 40-pin header provides SDA and SCL on GPIO 2 and GPIO 3, respectively. When configured for I²C, a **hardware controller and Linux I²C driver** handle signal timing.

Our C program opens `/dev/i2c-1` (or the correct bus), selects address `0x29`, and performs register transactions through the Linux kernel. We do **not** need to bit-bang SDA/SCL with general-purpose GPIO functions or link BCM2835.

```text
C example / team sensor driver
  -> Linux i2c-dev (/dev/i2c-1)
    -> Pi I²C controller
      -> SDA + SCL on GPIO header
        -> TCS34725 address 0x29
```

**Use GPIO only for an optional extra feature**, such as monitoring the sensor's **active-low INT output** or controlling a separate LED. Even with INT, sensor configuration and measurements still use I²C. To monitor the INT line you can use Linux GPIO tools or `libgpiod`; the latter needs `-lgpiod` only if your C program actually calls its API. This functionality is **not necessary** for the tutorial's RGBC polling code.

See [Optional hardware interrupts](interrupts.html) for wiring, thresholds, monitoring, and build requirements.

Next: [Registers](registers.html).
