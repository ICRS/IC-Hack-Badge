#ifndef LOGO_PATTERNS_H
#define LOGO_PATTERNS_H

#include "pico/stdlib.h"
#include "pico.h"

#include "config.h"
#include "render.h"

#define PATTERN_LENGTH (16)
#define LOGO_CHANGE_DELAY (500) // Time between logo changes in ms

const uint8_t logo_patterns[][PATTERN_LENGTH] = {
    {0b100, 0b000, 0b100, 0b000, 0b010, 0b000, 0b010, 0b000, 0b001, 0b000, 0b001, 0b000, 0b111, 0b000, 0b111, 0b000},
    {0b111, 0b110, 0b100, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000},
    {0b001, 0b111, 0b010, 0b111, 0b100, 0b111, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000},
    {0b111, 0b111, 0b000, 0b111, 0b000, 0b111, 0b111, 0b111, 0b111, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000},
    {0b101, 0b011, 0b110, 0b101, 0b011, 0b110, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000},
    {0b001, 0b010, 0b100, 0b001, 0b010, 0b100, 0b001, 0b010, 0b100, 0b001, 0b010, 0b100, 0b001, 0b010, 0b100, 0b000},
    {0b001, 0b010, 0b100, 0b010, 0b001, 0b100, 0b010, 0b001, 0b010, 0b100, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000},
    {0b001, 0b010, 0b100, 0b001, 0b010, 0b100, 0b000, 0b000, 0b111, 0b000, 0b111, 0b000, 0b000, 0b000, 0b000, 0b000},

};

inline void clr_ICH() {
  gpio_clr_mask(ICH_MASK);
}

inline void set_all_ICH()
{
  gpio_set_mask(ICH_MASK);
}

inline void ICH(uint8_t n)
{
  gpio_put(LED_2, n & 1);
  gpio_put(LED_1, (n >> 1) & 1);
  gpio_put(LED_0, (n >> 2) & 1);
}


void render_ICH(uint8_t n);

#endif