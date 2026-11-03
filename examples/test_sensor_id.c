#include "tcs_i2c.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "/dev/i2c-1";
    int fd = tcs_open(path);
    if (fd < 0) { perror("Open/select I2C device"); return 1; }
    uint8_t id = 0;
    if (tcs_read(fd, REG_ID, &id, 1) < 0) {
        perror("Read sensor ID");
        close(fd);
        return 1;
    }
    printf("I2C device: %s\nI2C address: 0x29\nDevice ID: 0x%02X\n",path,id);
    close(fd); /* ID-only probe: no conversion was started. */
    if (id == 0x44) puts("Recognized TCS34721/25 family ID.");
    else if (id == 0x4D) puts("Recognized related TCS34723/27 family ID; verify breakout.");
    else { fprintf(stderr,"Unexpected device ID: check connected hardware.\n"); return 1; }
    return 0;
}
