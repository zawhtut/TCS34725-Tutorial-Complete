#define _POSIX_C_SOURCE 200809L
#include "tcs_i2c.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static volatile sig_atomic_t running = 1;
static void stop(int signum) { (void)signum; running = 0; }

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "/dev/i2c-1";
    long limit = 5;
    if (argc > 2) {
        char *end;
        errno=0;
        limit = strtol(argv[2], &end, 10);
        if (errno || *end != '\0' || limit < 1 || limit > 100000) {
            fprintf(stderr, "Usage: %s [i2c_device] [samples:1..100000]\n",argv[0]);
            return 2;
        }
    }
    struct sigaction action = {0};
    action.sa_handler=stop;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT,&action,NULL) < 0 || sigaction(SIGTERM,&action,NULL) < 0) {
        perror("sigaction"); return 1;
    }
    int fd=tcs_open(path);
    if (fd < 0) { perror("Open/select I2C device"); return 1; }
    /* ATIME=0xEB is 21 cycles * 2.4ms = 50.4ms; gain=0x01 is 4x. */
    if (tcs_init(fd,0xEB,0x01) < 0) {
        perror("Initialize TCS34725");
        tcs_shutdown(fd);
        return 1;
    }
    int failed=0;
    for (long n=0; running && n < limit; ++n) {
        /* Wait longer than an integration period. AVALID alone does not
           guarantee a fresh measurement if the caller polls too quickly. */
        if (tcs_sleep_ms(70) < 0) { perror("Sleep"); failed=1; break; }
        if (!running) break;
        tcs_raw raw;
        int status=1;
        for (int retry=0; retry<10 && running; ++retry) {
            status=tcs_read_raw(fd,&raw);
            if (status != 1) break;
            if (tcs_sleep_ms(10) < 0) { status=-1; break; }
        }
        if (!running) break;
        if (status < 0) { perror("Read RGBC"); failed=1; break; }
        if (status == 1) { fprintf(stderr,"RGBC data not ready (timeout)\n"); failed=1; break; }
        printf("Sample %ld: C=%u R=%u G=%u B=%u\n",n+1,
               (unsigned)raw.clear,(unsigned)raw.red,(unsigned)raw.green,(unsigned)raw.blue);
    }
    tcs_shutdown(fd);
    return failed ? 1 : 0;
}
