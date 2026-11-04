---
layout: default
title: "Compile and Run"
nav_order: 14
---

# Compile and Run C Programs

**Goal:** Compile `TCS_main.c` on the Raspberry Pi without introducing a build system yet.

## Simplest GCC command

From the folder containing the file:

```bash
gcc TCS_main.c -o TCS_main
./TCS_main
```

- `gcc` is the C compiler.
- `TCS_main.c` is the source file.
- `-o TCS_main` gives the executable a name.
- `./TCS_main` runs the executable in the current directory.

If you want compiler warnings while learning:

```bash
gcc -Wall -Wextra TCS_main.c -o TCS_main
```

`-Wall` and `-Wextra` show warnings about possible mistakes. They are optional for the first build.

## Do we need `-lgpiod`, `-li2c`, or `-lbcm2835`?

**No** for the basic example. We use Linux system functions (`open()`, `ioctl()`, `write()`, `read()`, `close()`) directly.

The optional INT-pin lesson can use a GPIO library such as `libgpiod`, which is a *separate* feature.

## What about `usleep()`?

```c
#include <unistd.h>

usleep(700000);  /* 700,000 microseconds = 700 milliseconds */
```

`usleep()` is a convenient, older POSIX function available in typical Raspberry Pi GCC environments. If a strict C mode reports an undeclared function, compile with the ordinary `gcc` command above or use the modern `nanosleep()` API after learning how its `timespec` structure works. We don't need to introduce that structure in the first program.

## The older `examples/` folder

The repository may still contain `examples/read_rgbc.c` and `tcs_i2c.c` from an earlier tutorial version. They are separate learning samples. The simplest path now is **the standalone `TCS_main.c` on [Read Raw RGBC](rgbc-acquisition.html)**.

When the group has working basic code, we can introduce the project's shared Makefile.
