#define _POSIX_C_SOURCE 200809L

#include "tcs_i2c.h"

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static volatile sig_atomic_t running = 1;

typedef struct {
    const char *name;
    int red;
    int green;
    int blue;
} ColorReference;

typedef struct {
    int red;
    int green;
    int blue;
    const char *name;
    float confidence;
} DetectedColor;

/* Reference colors for approximate matching. */
static const ColorReference references[] = {
    {"Red",     255,   0,   0},
    {"Green",     0, 255,   0},
    {"Blue",      0,   0, 255},
    {"Yellow",  255, 255,   0},
    {"Cyan",      0, 255, 255},
    {"Magenta", 255,   0, 255},
    {"Orange",  255, 128,   0},
    {"Purple",  128,   0, 255},
    {"White",   255, 255, 255},
    {"Black",     0,   0,   0}
};

/* Signal handler: request shutdown only. */
static void handle_signal(int signum)
{
    (void)signum;
    running = 0;
}

static int setup_signals(void)
{
    struct sigaction action = {0};

    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGINT, &action, NULL) < 0)
        return -1;

    if (sigaction(SIGTERM, &action, NULL) < 0)
        return -1;

    return 0;
}

/* Read one valid RGBC measurement. */
static int read_sample(int fd, tcs_raw *raw)
{
    for (int attempt = 0; attempt < 10 && running; attempt++) {
        int status = tcs_read_raw(fd, raw);

        if (status == 0)
            return 0;

        if (status < 0)
            return -1;

        if (tcs_sleep_ms(10) < 0)
            return running ? -1 : 1;
    }

    return 1;
}

/*
 * Scale raw channels relative to the strongest RGB channel.
 * This is an approximation, not calibrated sRGB.
 */
static void normalize_rgb(const tcs_raw *raw, DetectedColor *color)
{
    unsigned maximum = raw->red;

    if (raw->green > maximum)
        maximum = raw->green;

    if (raw->blue > maximum)
        maximum = raw->blue;

    if (maximum == 0) {
        color->red = 0;
        color->green = 0;
        color->blue = 0;
        return;
    }

    color->red = (int)(255UL * raw->red / maximum);
    color->green = (int)(255UL * raw->green / maximum);
    color->blue = (int)(255UL * raw->blue / maximum);
}

/* Match normalized RGB against reference colors. */
static void detect_color(const tcs_raw *raw, DetectedColor *color)
{
    normalize_rgb(raw, color);

    /* Minimum Clear threshold is a provisional test value. */
    if (raw->clear < 20) {
        color->red = 0;
        color->green = 0;
        color->blue = 0;
        color->name = "Black";
        color->confidence = 100.0f;
        return;
    }

    unsigned best_distance = 195076;
    size_t best_index = 0;

    size_t count = sizeof(references) / sizeof(references[0]);

    for (size_t i = 0; i < count; i++) {
        int dr = color->red - references[i].red;
        int dg = color->green - references[i].green;
        int db = color->blue - references[i].blue;

        unsigned distance = (unsigned)(dr * dr + dg * dg + db * db);

        if (distance < best_distance) {
            best_distance = distance;
            best_index = i;
        }
    }

    color->name = references[best_index].name;
    color->confidence =
        100.0f * (1.0f - (float)best_distance / 195075.0f);
}

/* Print raw measurements and approximate detected color. */
static void display_result(long sample,
                           const tcs_raw *raw,
                           const DetectedColor *color)
{
    printf("\nSample %ld\n", sample);
    printf("----------------------------------\n");

    printf("Raw RGBC   : R=%u G=%u B=%u C=%u\n",
           (unsigned)raw->red,
           (unsigned)raw->green,
           (unsigned)raw->blue,
           (unsigned)raw->clear);

    printf("RGB Hex    : #%02X%02X%02X\n",
           color->red, color->green, color->blue);

    printf("Color      : %s\n", color->name);
    printf("Confidence : %.1f%%\n", color->confidence);

    printf("----------------------------------\n");
    fflush(stdout);
}

int main(int argc, char **argv)
{
    const char *device = "/dev/i2c-1";
    long sample_limit = 5;

    if (argc > 3) {
        fprintf(stderr,
                "Usage: %s [i2c_device] [samples:1..100000]\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    if (argc > 1)
        device = argv[1];

    if (argc > 2) {
        char *end = NULL;
        errno = 0;

        sample_limit = strtol(argv[2], &end, 10);

        if (errno || end == argv[2] || *end != '\0' ||
            sample_limit < 1 || sample_limit > 100000) {
            fprintf(stderr,
                    "Usage: %s [i2c_device] [samples:1..100000]\n",
                    argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (setup_signals() < 0) {
        perror("Signal setup");
        return EXIT_FAILURE;
    }

    int fd = tcs_open(device);

    if (fd < 0) {
        perror("Open/select I2C device");
        return EXIT_FAILURE;
    }

    /* ATIME 0xEB = 50.4 ms; gain 0x01 = 4x. */
    if (tcs_init(fd, 0xEB, 0x01) < 0) {
        perror("Initialize TCS34725");
        tcs_shutdown(fd);
        return EXIT_FAILURE;
    }

    printf("TCS34725 initialized successfully.\n");
    printf("I2C device: %s\n", device);
    printf("Press Ctrl+C to stop.\n");

    int failed = 0;
    long samples_read = 0;

    while (running && samples_read < sample_limit) {
        /* Wait longer than one sensor integration period. */
        if (tcs_sleep_ms(70) < 0) {
            if (!running)
                break;

            perror("Sleep");
            failed = 1;
            break;
        }

        if (!running)
            break;

        tcs_raw raw = {0};
        int status = read_sample(fd, &raw);

        if (!running)
            break;

        if (status != 0) {
            if (status < 0)
                perror("Read RGBC");
            else
                fprintf(stderr, "RGBC data not ready (timeout).\n");

            failed = 1;
            break;
        }

        DetectedColor color = {0};

        detect_color(&raw, &color);

        samples_read++;
        display_result(samples_read, &raw, &color);
    }

    if (!running)
        printf("\nShutdown requested (Ctrl+C / SIGTERM).\n");

    printf("Stopping sensor...\n");
    tcs_shutdown(fd);

    printf("Sensor shutdown complete.\n");
    printf("Total samples: %ld\n", samples_read);
    printf("Program terminated.\n");

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
