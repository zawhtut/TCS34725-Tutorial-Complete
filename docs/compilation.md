---
layout: default
title: Compilation
nav_order: 8
---
# Compile and run the examples

From the repository root on Raspberry Pi Linux:

```bash
sudo apt install -y build-essential libi2c-dev i2c-tools
make -C examples
```

The examples compile with GCC, warnings enabled, and ISO C11. **No `-lbcm2835` or extra I²C linker library is needed.** The examples use Linux kernel interfaces, not a third-party hardware library.

## Optional INT-pin compilation requirements

The normal polling examples do **not** access general-purpose GPIO and do not link `-lgpiod`.

| Feature | Extra library at link time? |
| --- | --- |
| Existing sensor-ID and RGBC polling programs | No `-lgpiod`, `-lbcm2835`, or `-li2c` |
| Configure TCS34725 interrupt registers over `i2c-dev` | No `-lgpiod` |
| Observe interrupt edges with the `gpiomon` command | No C linking; install the `gpiod` utilities |
| Write your own C program using `libgpiod` | Install `libgpiod-dev` and link against `libgpiod` |

For the optional C GPIO interface, check your installed version and flags:

```bash
sudo apt install -y gpiod libgpiod-dev pkg-config
pkg-config --modversion libgpiod
pkg-config --cflags --libs libgpiod
```

`libgpiod` 1.x and 2.x have different C APIs. The original `examples/Makefile` is still correct for polling and needs no changes. See [Optional hardware interrupts](interrupts.html).

## Hardware checks

```bash
./examples/test_sensor_id /dev/i2c-1
./examples/read_rgbc /dev/i2c-1 5
```

## Offline test (sensor not needed)

```bash
make -C examples check
```

This validates sample processing logic and deterministic reference matching; it does **not** establish real sensor accuracy.

## Clean build outputs

```bash
make -C examples clean
```

## Expected output (illustrative only)

```text
I2C device: /dev/i2c-1
I2C address: 0x29
Device ID: 0x44 (TCS34721/25 family)
```

```text
Sample 1: C=1234 R=440 G=650 B=210
```

Actual readings vary with illumination, distance, gain, and integration time. The command-line applications return a nonzero exit code on I²C failures.

Next: [Troubleshooting](troubleshooting.html).
