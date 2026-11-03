#define _POSIX_C_SOURCE 200809L
#include "tcs_i2c.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

int tcs_sleep_ms(unsigned int millis)
{
    struct timespec t = { (time_t)(millis / 1000), (long)((millis % 1000) * 1000000UL) };
    while (nanosleep(&t, &t) < 0) {
        if (errno != EINTR) return -1;
    }
    return 0;
}

int tcs_open(const char *path)
{
    if (!path) { errno = EINVAL; return -1; }
    int fd = open(path, O_RDWR);
    if (fd < 0) return -1;
    if (ioctl(fd, I2C_SLAVE, TCS_ADDR) < 0) {
        int saved = errno;
        close(fd);
        errno = saved;
        return -1;
    }
    return fd;
}

/* Two I2C messages in one combined transaction (repeated START). */
int tcs_read(int fd, uint8_t reg, uint8_t *data, size_t len)
{
    if (fd < 0 || !data || len == 0 || len > UINT16_MAX) {
        errno = EINVAL;
        return -1;
    }
    uint8_t cmd = (uint8_t)((len > 1 ? 0xA0 : 0x80) | (reg & 0x1F));
    struct i2c_msg msgs[2] = {
        { .addr=TCS_ADDR, .flags=0, .len=1, .buf=&cmd },
        { .addr=TCS_ADDR, .flags=I2C_M_RD, .len=(uint16_t)len, .buf=data }
    };
    struct i2c_rdwr_ioctl_data transaction = { .msgs=msgs, .nmsgs=2 };
    if (ioctl(fd, I2C_RDWR, &transaction) < 0) return -1;
    return 0;
}

int tcs_write8(int fd, uint8_t reg, uint8_t value)
{
    if (fd < 0) { errno = EINVAL; return -1; }
    uint8_t bytes[2] = { (uint8_t)(0x80 | (reg & 0x1F)), value };
    ssize_t count = write(fd, bytes, sizeof bytes);
    if (count != (ssize_t)sizeof bytes) {
        if (count >= 0) errno = EIO;
        return -1;
    }
    return 0;
}

int tcs_init(int fd, uint8_t atime, uint8_t gain)
{
    if (gain > 3) { errno = EINVAL; return -1; }
    uint8_t id;
    if (tcs_read(fd, REG_ID, &id, 1) < 0) return -1;
    if (id != 0x44 && id != 0x4D) {
        fprintf(stderr,"Unexpected chip ID 0x%02X (check sensor family).\n",id);
        errno = ENODEV;
        return -1;
    }
    if (tcs_write8(fd, REG_ATIME, atime) < 0 ||
        tcs_write8(fd, REG_CONTROL, gain) < 0 ||
        tcs_write8(fd, REG_ENABLE, 0x01) < 0 ||
        tcs_sleep_ms(3) < 0 ||
        tcs_write8(fd, REG_ENABLE, 0x03) < 0) return -1;
    return 0;
}

int tcs_read_raw(int fd, tcs_raw *out)
{
    if (!out) { errno = EINVAL; return -1; }
    uint8_t status;
    if (tcs_read(fd, REG_STATUS, &status, 1) < 0) return -1;
    if ((status & 0x01) == 0) return 1;
    uint8_t b[8];
    if (tcs_read(fd, REG_CDATA, b, sizeof b) < 0) return -1;
    out->clear = (uint16_t)((uint16_t)b[0] | ((uint16_t)b[1] << 8));
    out->red   = (uint16_t)((uint16_t)b[2] | ((uint16_t)b[3] << 8));
    out->green = (uint16_t)((uint16_t)b[4] | ((uint16_t)b[5] << 8));
    out->blue  = (uint16_t)((uint16_t)b[6] | ((uint16_t)b[7] << 8));
    return 0;
}

void tcs_shutdown(int fd)
{
    if (fd < 0) return;
    if (tcs_write8(fd, REG_ENABLE, 0) < 0) perror("Warning: sensor power-down");
    if (close(fd) < 0) perror("Warning: close I2C device");
}
