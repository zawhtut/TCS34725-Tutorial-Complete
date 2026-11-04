---
layout: default
title: "First C Program"
nav_order: 6
---

# First C Program: Open I²C

**Goal:** Learn just two operations: open the Linux I²C bus and tell Linux which sensor address to use.

Create `TCS_main.c`:

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main(void)
{
    int fd;

    fd = open("/dev/i2c-1", O_RDWR);

    if (fd < 0) {
        perror("Cannot open I2C");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, 0x29) < 0) {
        perror("Cannot select sensor");
        close(fd);
        return 1;
    }

    printf("I2C opened and sensor address selected.\n");

    close(fd);
    return 0;
}
```

Compile and run:

```bash
gcc TCS_main.c -o TCS_main
./TCS_main
```

## What each new part means

- `int fd` stores a **file descriptor**, a number Linux returns for an open file or device.
- `open("/dev/i2c-1", O_RDWR)` opens bus 1 for reading **and** writing.
- `if (fd < 0)` checks whether `open()` failed.
- `ioctl(fd, I2C_SLAVE, 0x29)` asks Linux to communicate with address `0x29` on that bus.
- `close(fd)` closes our connection.

**Important:** Selecting address `0x29` does *not* prove that a sensor is attached. The next step sends actual register commands. See [Sensor Registers](registers.html).

If you are wondering why we open `/dev/i2c-1` instead of `/dev/mem`, read [the device-file comparison](dev-mem-vs-i2c.html).
