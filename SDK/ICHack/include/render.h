#ifndef RENDER_H
#define RENDER_H

#include "pico/stdlib.h"
#include "pico.h"
#include "hardware/pwm.h"

#include "config.h"

#define EVEN_MASK 0b01010101010101010101010101010101
#define SHIFT_0_MASK 4281684
#define SHIFT_1_MASK 43688
#define SHIFT_2_MASK 1360
#define SHIFT_3_MASK 160

#define ICH_MASK ((1 << LED_0) | (1 << LED_1) | (1 << LED_2))

static uint32_t col_masks[10] = {
    732576,
    4402592,
    4795808,
    4861344,
    4918688,
    4924832,
    4925856,
    4926624,
    4926752,
    4926848};

void set_mask_ICH(uint8_t v);

inline void shiftOut(uint32_t dataPin, uint8_t clockPin, bool bitOrder, uint32_t val, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++)
  {
    if (bitOrder == 0)
      gpio_put(dataPin, !!(val & (1 << i)));
    else
      gpio_put(dataPin, !!(val & (1 << (size - 1 - i))));

    gpio_put(clockPin, 1);
    gpio_put(clockPin, 0);
  }
}

inline void clear_fb(uint32_t * const fb)
{
    for (uint8_t j = 0; j < FB_HEIGHT; j++)
    {
        fb[j] = 0;
    }
}

void fb_to_bit_streams(uint32_t *const fb, uint32_t *bit_streams);

void render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10],
    uint32_t delay);

inline void render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10]
    )
{
  render_fb(fb, bit_streams, 10000);
}

void render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10],
    uint32_t delay);

void standard_render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10]);

#endif
