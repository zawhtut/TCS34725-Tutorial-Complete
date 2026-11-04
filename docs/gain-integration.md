---
layout: default
title: "Gain and Integration Time"
nav_order: 12
---

# Gain and Integration Time (Optional Settings)

We can read raw RGBC without making these adjustable in the first program. **The sensor still uses a gain and integration time**; it just starts with register defaults or settings left by earlier software. For repeatable results, a real driver should configure them deliberately.

## Gain: how sensitive is the sensor?

Gain multiplies the sensor signal before conversion. More gain helps in dim light but makes bright readings saturate sooner.

| Gain | CONTROL register (`0x0F`) value |
|---|---|
| 1× | `0x00` |
| 4× | `0x01` |
| 16× | `0x02` |
| 60× | `0x03` |

For example, **after opening the I²C device** in `TCS_main.c`, and before starting measurements:

```c
unsigned char gain_setting[2];

gain_setting[0] = 0x8F;  /* Register 0x0F plus command bit 0x80 */
gain_setting[1] = 0x01;  /* Select 4x gain */

write(fd, gain_setting, 2);
```

## Integration time: how long does a reading collect light?

Longer integration time usually increases the measurement counts and reduces the rate of new readings. Shorter time responds faster and can help avoid saturation.

| Desired time | ATIME register (`0x01`) value |
|---|---|
| 2.4 ms | `0xFF` |
| 24.0 ms | `0xF6` |
| 50.4 ms | `0xEB` |
| 100.8 ms | `0xD6` |
| 614.4 ms | `0x00` |

For example, to choose about **50.4 milliseconds**:

```c
unsigned char integration_setting[2];

integration_setting[0] = 0x81;  /* Command for ATIME register 0x01 */
integration_setting[1] = 0xEB;  /* Approximately 50.4 ms */

write(fd, integration_setting, 2);
```

Now a wait longer than 50.4 ms can allow a new reading to complete (for example, 70 ms), but STATUS should be checked in a more reliable driver.

## What is saturation?

The ADC has a count limit. A commonly used estimate for the digital saturation count is:

```text
cycles = 256 - ATIME
limit  = minimum of 65535 and cycles * 1024
```

Analog saturation can happen earlier. Actual values also depend on gain, lighting, sensor distance and the object being tested. Don't interpret a high number as accurate once the sensor saturates.

**Do not add these settings until your basic reading program works.** Learn their effect by changing one value at a time. Next: [Calibration](calibration.html).
