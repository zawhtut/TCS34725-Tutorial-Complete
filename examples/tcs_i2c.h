#ifndef TUTORIAL_TCS_I2C_H
#define TUTORIAL_TCS_I2C_H
#include <stdint.h>
#include <stddef.h>
#define TCS_ADDR 0x29
#define REG_ENABLE 0x00
#define REG_ATIME 0x01
#define REG_CONTROL 0x0F
#define REG_ID 0x12
#define REG_STATUS 0x13
#define REG_CDATA 0x14

typedef struct {
    uint16_t clear, red, green, blue;
} tcs_raw;

int tcs_open(const char *path);
int tcs_read(int fd, uint8_t reg, uint8_t *data, size_t len);
int tcs_write8(int fd, uint8_t reg, uint8_t value);
int tcs_init(int fd, uint8_t atime, uint8_t gain);
int tcs_read_raw(int fd, tcs_raw *out); /* 0=ok, 1=not ready, -1=error */
void tcs_shutdown(int fd);
int tcs_sleep_ms(unsigned int millis);
#endif
