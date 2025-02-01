#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"

void print_buf(const uint8_t *buf, size_t len);

void print_frame_buffer(const uint32_t *const fb);

void print_buf_hex(const uint8_t *buf, size_t len);

void print(const char *buf);

#endif