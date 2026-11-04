# TCS34725 Tutorial — Raspberry Pi + Linux I²C + C

A **standalone learning repository**, separate from the team's main `RGB_Sensor_Project`. It walks through wiring, enabling I²C, checking device ID `0x29`, obtaining raw RGBC measurements, and running an offline normalization/classification demonstration.

> **No BCM2835 required.** The sensor uses the **I²C protocol** on Raspberry Pi SDA/SCL header pins. Linux's `/dev/i2c-*` interface provides bus access. General-purpose GPIO bit-banging is not necessary.

## Quick start (on the Raspberry Pi)

1. Wire the sensor as shown in [Hardware setup](docs/hardware.md). **Power off before wiring.** Confirm breakout-board voltage compatibility.
2. Enable I²C (details: [I²C setup](docs/i2c-setup.md)). Reboot if changed.
3. Install tools:

   ```bash
   sudo apt update
   sudo apt install -y build-essential i2c-tools libi2c-dev
   ```
4. Check the bus and sensor:

   ```bash
   ls /dev/i2c-*
   i2cdetect -y 1
   ```

   Look for `29` on the correct bus (often bus 1). Avoid concurrent access by other applications.
5. Compile and run example programs:

   ```bash
   make -C examples
   ./examples/test_sensor_id /dev/i2c-1
   ./examples/read_rgbc /dev/i2c-1 5
   make -C examples check
   ```

If your bus is not `/dev/i2c-1`, substitute the appropriate device. The examples use the `I2C_RDWR` ioctl and require a Linux I²C adapter that supports combined transfers.

## Documentation

- [Tutorial home](docs/index.md)
- [Hardware & connections](docs/hardware.md)
- [Enable I²C on Raspberry Pi](docs/i2c-setup.md)
- [I²C versus GPIO](docs/gpio-vs-i2c.md)
- [Sensor registers and Linux I²C operations](docs/registers.md)
- [Read RGBC values](docs/rgbc-acquisition.md)
- [Optional hardware interrupts / INT pin](docs/interrupts.md)
- [Compile and run C examples](docs/compilation.md)
- [Processing, calibration and classification](docs/processing.md)
- [Using the team's library later](docs/library-usage.md)
- [Troubleshooting](docs/troubleshooting.md)
- [References and limitations](docs/references.md)

## Example programs

| File | Purpose | Sensor required? |
| --- | --- | --- |
| `examples/test_sensor_id.c` | Open bus, read and check ID register | Yes |
| `examples/read_rgbc.c` | Set gain/time, enable ADC, read 8 bytes, print RGBC | Yes |
| `examples/test_processing.c` | Offline validation, normalization, reference matching tests | No |
| `examples/tcs_i2c.c/.h` | Shared learning driver for I²C register access | Used by sensor examples |
| `examples/Makefile` | Build all, test offline, clean | No hardware needed to compile |

The examples are **educational utilities**, **not replacements** for the team's production `rgb_sensor.c` and `color_detection.c`. Calibration is illustrative; RGB ratios are not guaranteed to be accurate sRGB.

## Separate GitHub Pages website (optional)

1. Create a **new repository** named `TCS34725-Tutorial`, not a branch or folder in your main project.
2. Upload the files in this directory, preserving the directory structure.
3. In **Settings → Pages**, choose **Deploy from a branch**, branch `main`, folder `/docs`, then Save.
4. The published site will usually be `https://YOUR-USERNAME.github.io/TCS34725-Tutorial/`.

Pages displays documentation; it **does not execute C**. GitHub Pages is generally public. See [docs/site-setup.md](docs/site-setup.md).

## Team responsibilities

| Member | Main project responsibility |
| --- | --- |
| 1 | Sensor initialization, register helpers, gain/time, cleanup |
| 2 | Read raw RGBC using Member 1's helpers |
| 3 | Validate Clear/saturation and calibrate/normalize |
| 4 | Classify color and calculate similarity score |
| 5 | App loop, terminal output, Makefile, signals |

This repository is a reference only: no changes to the main repo are required.

## Safety & scope

- The **Raspberry Pi GPIO header is 3.3 V logic**. Only connect breakout boards whose SDA/SCL voltage levels and supply requirements are compatible; some boards include regulators/level shifting and some do not.
- Pin mapping here is for a standard 40-pin Raspberry Pi header; check your board revision.
- Reading `AVALID` confirms a completed measurement exists, **not necessarily a fresh sample** since the last read.
- I²C scans may interfere with other bus devices. On a dedicated lab setup with this sensor, `i2cdetect -y 1` is a convenient check.

## License

See [LICENSE](LICENSE) (MIT) for original tutorial content. Third-party specifications and linked documentation remain under their respective licenses.
