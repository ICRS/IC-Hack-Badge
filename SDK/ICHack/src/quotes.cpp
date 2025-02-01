#include "quotes.h"

#include "config.h"
#include "letters.h"
#include "render.h"
#include "logo_patterns.h"
#include "letters.h"
#include "pico/rand.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include "debug.h"

uint32_t *quote_ptr = (uint32_t *)(XIP_BASE + QUOTE_START);
uint32_t num_quotes = *quote_ptr;
uint32_t *quote_len_ptr = (uint32_t*)(quote_ptr + 1);
unsigned char *quotes = (unsigned char*)(quote_ptr + 1 + num_quotes);

// Quotes
void render_quotes(uint32_t *fb, uint32_t *bit_streams)
{
    int rand_quote = get_rand_32() % num_quotes;
    uint32_t quote_size = *(quote_len_ptr + rand_quote);
    unsigned char *quote_start = quotes;
    for (size_t i = 0; i < rand_quote; i++)
    {
        quote_start += *(quote_len_ptr + i);
    }

    print_buf(quote_start, quote_size);

    for (size_t n = 0; n < string_width(quote_size) + FB_WIDTH; n++)
    {
        clear_fb(fb);
        string_banner_output(fb, quote_start, quote_size, n);
        standard_render_fb(fb, bit_streams);
        render_ICH(n);
    }

}
