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

**Use GPIO only if you add an extra feature** such as a separately controlled LED or interrupt line. Such optional functionality requires its own electrical and software design, and is **not necessary** for the tutorial's RGBC polling code.

Next: [Registers](registers.html).
