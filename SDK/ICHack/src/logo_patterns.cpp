#include "logo_patterns.h"

#include "pico/stdlib.h"
#include "pico.h"

#include "config.h"
#include "render.h"


void render_ICH(uint8_t n)
{
  clr_ICH();
  uint8_t n_ = ((n & 0x3F) >> 2);
  if (!((n_ >> 2) & 0b10))
  {
    n_ &= 0b11;
    if (n_ == 0)
    {
      gpio_put(LED_2, 1);
    }
    else if (n_ == 1)
    {
      gpio_put(LED_1, 1);
    }
    else if (n_ == 2)
    {
      gpio_put(LED_0, 1);
    }
  }
  else
  {
    if (n_ & 0b10)
    {
      set_all_ICH();
    }
  }
}
