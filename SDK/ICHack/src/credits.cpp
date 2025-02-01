#include <stdio.h>

#include "credits.h"
#include "input.h"
#include "letters.h"
#include "render.h"
#include "debug.h"

uint8_t *ich_target_start = (uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

// only first 24 bits of uint in bg are considered
void circular_fb_background(uint32_t *fb, const uint32_t *const bg, uint8_t offset)
{
    offset %= FB_WIDTH;
    for (size_t i = 0; i < FB_HEIGHT; i++)
    {
        fb[i] = bg[i] << offset;
        fb[i] |= bg[i] >> (FB_WIDTH - offset);
    }
}

void render_credits(uint32_t *fb, uint32_t *bit_streams)
{
    const static unsigned char credits[] = BADGE_CREDITS_STR;
    const static uint32_t credits_len = ARRAY_LENGTH(credits) - 1;
    const static auto credits_size = (string_width_32_t(credits_len) + (FB_WIDTH << 1) + 1);
    print_buf_hex((uint8_t*)ich_target_start, 32);
    for (size_t i = 0; i < credits_size; i++)
    {
        if (i < FB_WIDTH || i > string_width_32_t(credits_len) - FB_WIDTH)
            circular_fb_background(fb, sine_background, i);

        string_banner_output_non_clr(fb, credits, credits_len, i);
        standard_render_fb(fb, bit_streams);
        if (i & 1)
        {
            auto n = i >> 1;
            auto k = n / 8;
            n %= 8;
            if (ich_target_start[k] & (1 << n))
            {
                set_all_ICH();                
            }
            else
            {
                clr_ICH();
            }
        }
    }

    const static unsigned char smiley[] = " \205 ";
    const static uint32_t smiley_len = ARRAY_LENGTH(smiley) - 1;

    for (size_t i = credits_size; i > credits_size - 100; i--)
    {
        circular_fb_background(fb, sine_background, i);
        string_banner_output_non_clr(fb, smiley, smiley_len, FB_WIDTH - 5);
        standard_render_fb(fb, bit_streams);
    }
    game_pad.reset();
}