---
layout: default
title: Compilation
---
# Compile and run the examples

From the repository root on Raspberry Pi Linux:

```bash
sudo apt install -y build-essential libi2c-dev i2c-tools
make -C examples
```

The examples compile with GCC, warnings enabled, and ISO C11. **No `-lbcm2835` or extra I²C linker library is needed.** The examples use Linux kernel interfaces, not a third-party hardware library.

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
