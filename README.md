# TCS34725 Beginner Tutorial (C + Raspberry Pi)

This repository is a **standalone beginner guide**. It does not modify the CSC615 team's main RGB Sensor project.

**Start with the [tutorial homepage](docs/index.md)** and follow the numbered lessons. The [complete one-file `TCS_main.c` example](docs/rgbc-acquisition.md) is meant to be copied into a practice folder on the Raspberry Pi.

## Learning order

Hardware → I²C setup → GPIO vs. I²C → `/dev/mem` vs. `/dev/i2c` → first C program → registers → raw RGBC → Clear → 8-bit RGB and hexadecimal → color names → optional gain/integration → calibration → compilation → reusable library → optional interrupts.

## GitHub Pages

The `docs/` directory has complete Just the Docs page headers with unique `nav_order` values (1–18). Keep your **existing** `docs/_config.yml` (and the theme configuration) when copying these replacements. `docs/site-setup.md` is intentionally hidden from navigation and search.

## Important

- No BCM2835, libgpiod, or `-li2c` needed for the basic polling example.
- The optional INT pin requires additional GPIO setup if you decide to use it later.
- Processing examples are approximate and require calibration for accuracy.
- The original repository's `examples/` C files are **not changed** by this Markdown-only update; they may show a different interface than the new one-file learning path.
