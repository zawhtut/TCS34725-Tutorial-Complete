CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all check clean
all: test_sensor_id read_rgbc test_processing

test_sensor_id: test_sensor_id.c tcs_i2c.c tcs_i2c.h
	$(CC) $(CFLAGS) test_sensor_id.c tcs_i2c.c -o $@

read_rgbc: read_rgbc.c tcs_i2c.c tcs_i2c.h
	$(CC) $(CFLAGS) read_rgbc.c tcs_i2c.c -o $@

test_processing: test_processing.c
	$(CC) $(CFLAGS) test_processing.c -o $@

check: test_processing
	./test_processing

clean:
	rm -f test_sensor_id read_rgbc test_processing
