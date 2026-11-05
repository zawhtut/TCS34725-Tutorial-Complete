---
layout: default
title: Hardware Interrupts
nav_order: 16
---

# TCS34725 Hardware Interrupts

## 1. What Is the INT Pin?

The TCS34725 color sensor has a pin called **INT**, which stands for *interrupt*.

**INT is an output pin on the TCS34725, not an input pin.**

This means the TCS34725 can send an interrupt signal **to the Raspberry Pi**, but other sensors do not send signals into the TCS34725 through this pin.

For example:

```text
TCS34725 Sensor                Raspberry Pi
+----------------+             +----------------+
|                |             |                |
|  INT (Output)  | ----------> | GPIO (Input)   |
|                |             |                |
+----------------+             +----------------+
```

The arrow shows the direction of the signal.

The TCS34725 generates an interrupt when its **Clear-channel measurement** meets a configured threshold condition.

The Raspberry Pi can then detect that signal and respond by reading the sensor over I²C.

### What About Other Sensors?

Suppose we also have a motion sensor.

```text
Motion Sensor -----> Raspberry Pi <----- TCS34725
                         |
                         v
                    C Program
```

Both sensors can send signals or measurements to the Raspberry Pi.

The Raspberry Pi's C program decides what to do with those signals.

For example, when the motion sensor detects movement, the program could read RGB values from the TCS34725.

However, the motion sensor does **not** interrupt the TCS34725 directly through its INT pin.

The TCS34725 receives configuration commands through I²C.

---

## 2. How Does the TCS34725 Generate an Interrupt?

The TCS34725 has four measurement channels:

- Red (R)
- Green (G)
- Blue (B)
- Clear (C)

The Clear channel measures incoming light without an RGB color filter.

The interrupt feature uses the **Clear-channel value**.

We can configure two thresholds:

- Lower threshold
- Upper threshold

For example:

```text
Lower threshold = 500
Upper threshold = 2000
```

The sensor checks whether its Clear-channel measurement is within the configured range.

| Clear Value | Condition |
|---|---|
| 300 | Below lower threshold |
| 1000 | Within range |
| 2200 | Above upper threshold |

When the Clear value is outside the range, the sensor can generate an interrupt according to its persistence setting.

### Active-Low Interrupt

The TCS34725 INT output is **active-low**.

This means:

```text
INT = HIGH    No interrupt asserted

INT = LOW     Interrupt asserted
```

The actual HIGH voltage depends on the breakout board's pull-up connection. The Raspberry Pi GPIO input must use compatible 3.3 V logic.

**Important:** The INT signal does not simply mean that a new RGB measurement is available. It indicates a configured Clear-channel threshold condition.

The sensor's AVALID status bit is used to check whether RGBC measurement data is valid.

---

## 3. Connecting INT to the Raspberry Pi

The sensor still uses SDA and SCL for I²C communication.

The INT pin is an additional connection.

| TCS34725 Pin | Raspberry Pi |
|---|---|
| SDA | GPIO 2, physical pin 3 |
| SCL | GPIO 3, physical pin 5 |
| GND | Ground |
| VIN/VCC | Appropriate power for the breakout board |
| INT | GPIO 17, physical pin 11 (example) |

GPIO 17 is only an example. Another available GPIO input can be used.

Before connecting INT, check the breakout board's electrical specifications and pull-up requirements.

**Do not connect INT to SDA or SCL.** It is a separate signal.

---

## 4. TCS34725 Interrupt Registers

We configure the interrupt through the sensor's I²C registers.

| Register | Address | Purpose |
|---|---|---|
| ENABLE | `0x00` | Enable sensor power, RGBC measurements, and interrupts |
| AILTL | `0x04` | Lower threshold, low byte |
| AILTH | `0x05` | Lower threshold, high byte |
| AIHTL | `0x06` | Upper threshold, low byte |
| AIHTH | `0x07` | Upper threshold, high byte |
| PERS | `0x0C` | Interrupt persistence |
| STATUS | `0x13` | Read interrupt status |

These addresses belong to the TCS34725 sensor.

They are not Raspberry Pi GPIO addresses.

---

## 5. Setting Interrupt Thresholds in C

Suppose we want:

```text
Lower threshold = 500
Upper threshold = 2000
```

Each threshold is a 16-bit number stored in two 8-bit registers.

### Lower Threshold

500 in hexadecimal is `0x01F4`.

We split it into:

```text
Low byte  = 0xF4
High byte = 0x01
```

Assume our sensor driver provides:

```c
int sensor_write(unsigned char reg,
                 unsigned char value);
```

The parameters mean:

- `reg` — The register address we want to write.
- `value` — The byte we want to store in that register.

We can write:

```c
sensor_write(0x04, 0xF4);
sensor_write(0x05, 0x01);
```

### Upper Threshold

2000 in hexadecimal is `0x07D0`.

```text
Low byte  = 0xD0
High byte = 0x07
```

Write the values:

```c
sensor_write(0x06, 0xD0);
sensor_write(0x07, 0x07);
```

These calls assume the sensor has already been opened and initialized. In a complete application, check each function's return value.

---

## 6. Setting Interrupt Persistence

The TCS34725 has a PERS register at address `0x0C`.

This register controls how many consecutive out-of-range Clear measurements are required before an interrupt occurs.

For example:

```c
sensor_write(0x0C, 0x01);
```

`0x01` means one out-of-range measurement cycle.

Another example:

```c
sensor_write(0x0C, 0x03);
```

`0x03` means three consecutive out-of-range measurement cycles.

This can help control how the sensor responds to brief changes in light.

---

## 7. Enabling the Interrupt

The ENABLE register is at address `0x00`.

It contains several control bits:

| Bit | Value | Purpose |
|---|---|---|
| PON | `0x01` | Turn on sensor power |
| AEN | `0x02` | Enable RGBC measurements |
| AIEN | `0x10` | Enable RGBC interrupt |

To enable all three:

```c
sensor_write(0x00, 0x13);
```

Here, `0x13` combines the three settings:

```text
0x01 = Power on
0x02 = RGBC measurement enabled
0x10 = Interrupt enabled

Total = 0x13
```

The sensor should first be powered on and given its required startup time before enabling measurements.

### Complete Interrupt Configuration

After normal sensor initialization:

```c
/* Lower threshold = 500 */
sensor_write(0x04, 0xF4);
sensor_write(0x05, 0x01);

/* Upper threshold = 2000 */
sensor_write(0x06, 0xD0);
sensor_write(0x07, 0x07);

/* Interrupt after one out-of-range cycle */
sensor_write(0x0C, 0x01);

/* Enable sensor, RGBC measurements, and INT */
sensor_write(0x00, 0x13);
```

This example uses an existing `sensor_write()` function. It does not show a complete standalone interrupt-monitoring application.

---

## 8. Detecting INT on the Raspberry Pi

When an interrupt occurs, the TCS34725 pulls its INT output LOW.

The Raspberry Pi can detect this through a GPIO input.

Linux provides GPIO device interfaces such as:

```text
/dev/gpiochip0
```

A commonly used library for GPIO access is **libgpiod**.

Install the GPIO tools:

```bash
sudo apt install gpiod
```

List GPIO devices:

```bash
gpiodetect
```

Inspect their GPIO lines:

```bash
gpioinfo
```

To monitor the INT pin, use `gpiomon`.

For example, with an older libgpiod CLI version:

```bash
gpiomon -f gpiochip0 17
```

This example assumes GPIO 17 is line offset 17 on `gpiochip0`.

**Check the actual GPIO chip and line mapping on your Raspberry Pi.** The command syntax and line numbering can differ between libgpiod versions and Raspberry Pi models.

You can check your installed command's syntax using:

```bash
gpiomon --help
```

A C program can also monitor the GPIO line using libgpiod functions.

If the program uses the libgpiod library, it generally needs the linker option:

```bash
-lgpiod
```

The I²C-only program does not need this library.

---

## 9. Checking Interrupt Status Through I²C

The STATUS register is at address `0x13`.

Bit 4, called AINT, indicates the RGBC interrupt status.

Suppose our driver provides:

```c
int sensor_read(unsigned char reg,
                unsigned char *value);
```

The first parameter selects the sensor register.

The second parameter receives the value read from that register.

We can check the interrupt status using:

```c
unsigned char status;

if (sensor_read(0x13, &status) == 0) {
    if ((status & 0x10) != 0) {
        printf("Clear-channel interrupt detected.\n");
    }
}
```

Here:

- `0x13` is the STATUS register address.
- `0x10` identifies bit 4.
- `&` is the bitwise AND operator, used to check that bit.

This method checks the interrupt status through I²C.

It does not require connecting the physical INT pin.

However, checking STATUS repeatedly is still polling. It is different from receiving a hardware GPIO interrupt event.

---

## 10. Clearing the Interrupt

The TCS34725 provides a special command to clear the interrupt latch.

The command byte is:

```text
0xE6
```

If `fd` is the already-open I²C file descriptor:

```c
unsigned char command = 0xE6;

if (write(fd, &command, 1) != 1) {
    perror("Failed to clear interrupt");
}
```

The `write()` call sends the command to the sensor over I²C.

The command clears the latched RGBC interrupt condition.

It does not remove the configured threshold values.

If the out-of-range condition continues, the sensor may generate another interrupt according to its configuration.

---

## 11. Example Situations

### Example 1 — Detecting an Object Blocking Light

Suppose the sensor normally reads:

```text
Clear = 1500
```

An object passes between the sensor and a light source:

```text
Clear = 300
```

If the lower threshold is 500, the sensor can assert INT when the reading drops below the threshold.

The Raspberry Pi can detect the interrupt and then read the RGBC measurements.

### Example 2 — Detecting a Bright Light

Suppose the sensor normally reads:

```text
Clear = 1000
```

A brighter light increases the measurement:

```text
Clear = 2500
```

If the upper threshold is 2000, the sensor can generate an interrupt when that threshold is exceeded.

### Example 3 — Detecting a Change Before Reading Color

Suppose colored objects move in front of the sensor.

The program could use an interrupt to detect a significant change in Clear-channel intensity.

After detecting the event, the program reads the Red, Green, Blue, and Clear values over I²C.

The interrupt itself does not identify the object's color. It only reports the configured Clear-channel condition.

---

## 12. Polling and Interrupts

Our basic `TCS_main.c` example uses polling.

```c
usleep(700000);

/* Read RGBC measurements */
```

The program waits and then requests another measurement.

With the INT pin, the sensor can notify the Raspberry Pi when a configured Clear-channel threshold condition occurs.

| Polling | Hardware Interrupt |
|---|---|
| Program reads at regular intervals | Sensor signals a configured threshold event |
| Uses I²C | Uses I²C plus a GPIO input for physical notification |
| No INT wire needed | INT wire needed |
| Useful for continuous measurements | Useful for responding to selected light changes |

Both methods still use I²C for reading RGBC measurements.

An application can also combine them, such as displaying continuous RGB readings while monitoring INT for a significant change in light.

The approach to use depends on the behavior the application needs.