#include "render.h"

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

inline void set_mask_ICH(uint8_t v)
{
  gpio_set_mask(((1 & v) << LED_0) | ((1 & (v >> 1)) << LED_1) | ((1 & (v >> 2)) << LED_2));
}


void fb_to_bit_streams(uint32_t *const fb, uint32_t *bit_streams)
{
  uint32_t tmp = 0;

  for (uint8_t col = 0; col < 10; col++)
  {
    if (col < 5)
    {
      tmp = fb[4 - col] & EVEN_MASK;
    }
    else
    {
      tmp = (fb[col - 5] >> 1) & EVEN_MASK;
    }

    tmp = (tmp & ~SHIFT_0_MASK) | ((tmp & SHIFT_0_MASK) >> 1);
    tmp = (tmp & ~SHIFT_1_MASK) | ((tmp & SHIFT_1_MASK) >> 1);
    tmp = (tmp & ~SHIFT_2_MASK) | ((tmp & SHIFT_2_MASK) >> 1);
    tmp = (tmp & ~SHIFT_3_MASK) | ((tmp & SHIFT_3_MASK) >> 1);

    bit_streams[col] = tmp | col_masks[col];
  }
}



void render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10],
    uint32_t delay)
{
  fb_to_bit_streams(fb, bit_streams);
  for (size_t b = 0; b < 10; b++)
  {
    gpio_put(OE, 0);
    for (size_t i = 0; i < 10; i++)
    {
      gpio_put(RCLK, 0);
      shiftOut(DIN, CLK, 0, bit_streams[i], 24);
      gpio_put(RCLK, 1);
      if (!delay)
        sleep_us(delay);
    }
    gpio_put(OE, 1);
  }
}

void standard_render_fb(
    uint32_t fb[5],
    uint32_t bit_streams[10])
{
  for (size_t i = 0; i < 8; i++)
  {
    render_fb(fb, bit_streams, 10000);
    sleep_ms(10);
  }
}

