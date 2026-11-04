---
layout: default
title: "/dev/mem vs. /dev/i2c"
nav_order: 5
---

# What is `/dev/mem`, and how is it different from `/dev/i2c-1`?

**Goal:** Understand two Linux device files without needing to write a complicated driver.

We already use **SDA** and **SCL** to talk to the TCS34725. But you may also see C examples opening `/dev/mem`. Why are these different?

## 1. First: what is `/dev`?

On Linux, `/dev` is a directory containing **device files**. A device file gives a program a way to ask the operating system to work with hardware.

For example:

- `/dev/i2c-1` is an interface to **I²C bus 1**.
- `/dev/mem` is an interface to **physical address space** (where access is permitted).

**Neither path is an ordinary text file.** Calling `open()` gives our program a *file descriptor*, which is a number Linux uses to identify the open device.

## 2. `/dev/i2c-1`: let Linux handle I²C

Our sensor is an **I²C device**. We need to send commands to its address `0x29` using SDA and SCL.

```text
Our C code
   |
   | open(), ioctl(), write(), read()
   v
/dev/i2c-1  (Linux I2C device interface)
   |
   v
Raspberry Pi I2C controller
   |
   | SDA and SCL wires
   v
TCS34725 sensor (address 0x29)
```

A small piece of C code looks like this:

```c
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int fd = open("/dev/i2c-1", O_RDWR);

/* Tell Linux which device address to use on this I2C bus. */
ioctl(fd, I2C_SLAVE, 0x29);

/* More code would use write() and read() to exchange bytes. */
close(fd);
```

What the important parts mean:

- `fd` is the file descriptor returned by `open()`.
- `"/dev/i2c-1"` is the bus we want to use.
- `O_RDWR` means we want both reading and writing access.
- `I2C_SLAVE` tells Linux that `0x29` is the address of our sensor.
- `write()` and `read()` transfer bytes; Linux takes care of the I²C clock and signals.

This short example **shows the calls**, but real code must also check whether `open()` and `ioctl()` succeeded. See [First C Program](first-program.html) for the full example.

## 3. `/dev/mem`: access physical addresses directly

Some Raspberry Pi examples use `/dev/mem` to access **memory-mapped hardware registers**.

A memory-mapped register is a hardware control location that appears at a physical address. A program can sometimes access it by mapping that address into its own memory with `mmap()`.

```text
Our C code
   |
   | open("/dev/mem") and mmap()
   v
Physical address mapping (if Linux permits it)
   |
   v
Hardware registers, such as peripheral controller registers
```

This is a **lower-level approach**. To use it correctly, the program has to know the right hardware addresses, register layouts, and which bits to change. Those details can vary by Raspberry Pi model. Linux may restrict access, and a wrong write can interfere with hardware or the operating system.

For that reason, **we are not including code that writes through `/dev/mem` in this beginner sensor tutorial**.

## 4. The key difference

| Question | `/dev/i2c-1` | `/dev/mem` |
|---|---|---|
| What does it give us? | A Linux-managed I²C bus | Access to physical address space, where allowed |
| How do we talk to a TCS34725? | Select `0x29`, then send/receive bytes | Would first require controlling hardware at a much lower level |
| Do we control SDA/SCL timing ourselves? | No | We would need to manage hardware controller registers ourselves (or implement even more low-level behavior) |
| Do we need to know Pi peripheral register addresses? | No | Yes, for direct peripheral access |
| Appropriate for this project? | **Yes** | **No** |

**Important:** `/dev/mem` does **not** let you read a TCS34725 register by simply using the sensor's `0x14` register address. That register is inside a **separate I²C chip**, not at a Raspberry Pi physical-memory address. To reach it, the Pi must still perform I²C communication.

## 5. Why do both use `open()`?

Because Linux presents both interfaces as device files. **Using the same C function does not mean they access the same hardware.**

```c
/* Open an I2C bus: the method we use. */
int i2c_fd = open("/dev/i2c-1", O_RDWR);

/* Open physical memory: shown for comparison only. */
int mem_fd = open("/dev/mem", O_RDWR);
```

Both lines ask Linux to open a device file, but the **path changes what device interface you get**. The `/dev/mem` example is only a comparison, not code you need to run. Access to `/dev/mem` commonly requires elevated privileges and may be restricted by the kernel.

## 6. What about `/dev/gpiomem` and GPIO?

You may also see `/dev/gpiomem` in older Raspberry Pi GPIO examples. That is another interface for mapping certain GPIO-related hardware registers; it is **not** the same as `/dev/i2c-1` and is not needed for our regular TCS34725 readings.

Our sensor's **SDA and SCL pins are physically on the GPIO header**, but we use them in their I²C function. Linux handles the I²C transfers. A separate GPIO interface is only relevant if we add something like the optional **INT interrupt pin**. See [GPIO vs. I²C](gpio-vs-i2c.html) and [Hardware Interrupts](interrupts.html).

## 7. Which should our team use?

For CSC615, use **`/dev/i2c-1`** because our objective is to **communicate with a color sensor**, not to write a Raspberry Pi peripheral controller driver.

1. `open()` the I²C bus.
2. `ioctl()` to select the sensor address `0x29`.
3. `write()` the sensor register command.
4. `read()` the sensor measurement bytes.
5. `close()` the bus when finished.

The next page, [First C Program](first-program.html), starts with those calls.

## References

- [Linux kernel: I²C device interface](https://www.kernel.org/doc/html/latest/i2c/dev-interface.html)
- [Linux manual: `/dev/mem`](https://man7.org/linux/man-pages/man4/mem.4.html)
