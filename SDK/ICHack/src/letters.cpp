#include "letters.h"
#include <stdint.h>
#include "config.h"
#include "render.h"


void string_banner_output(uint32_t *const fb, const unsigned char * const string_buf, uint32_t string_size, uint32_t offset)
{
    clear_fb(fb);
    if (!string_size || !offset)
    {
        return;
    }

    int32_t offset_ = offset;
    uint8_t i = 0;
    while ((offset_ > 0) && (i < string_size))
    {
        if (offset_ < (FB_WIDTH + CHAR_WIDTH))
            char_output(fb, string_buf[i], offset_);

        offset_ -= 5;
        i++;
    }
}

void string_banner_output_non_clr(uint32_t *const fb, const unsigned char * const string_buf, uint32_t string_size, uint32_t offset)
{
    if (!string_size || !offset)
    {
        return;
    }

    int32_t offset_ = offset;
    uint32_t i = 0;
    while ((offset_ > 0) && (i < string_size))
    {
        if (offset_ < (FB_WIDTH + CHAR_WIDTH))
        {
            clear_char_loc(offset_, fb);
            char_output(fb, string_buf[i], offset_);
        }

        offset_ -= 5;
        i++;
    }
}

void char_output(uint32_t *const fb, char c, uint32_t offset)
{
    if (!offset)
    {
        return;
    }
    else if (offset > FB_WIDTH + CHAR_WIDTH + 1)
    {
        return;
    }


    if (c >= 'A' && c <= 'Z')
    {
        for (uint8_t j = 0; j < FB_HEIGHT; j++)
        {
            // fb[j] = 0;
            uint8_t v = CHARACTER_MAPPING[c - 'A'][j];
            if (offset < CHAR_WIDTH)
                fb[j] |= v >> (CHAR_WIDTH - offset);
            else
                fb[j] |= v << (offset - CHAR_WIDTH);
        }
    }
    else if (c >= 'a' && c <= 'z')
    {
        for (uint8_t j = 0; j < FB_HEIGHT; j++)
        {
            // fb[j] = 0;
            uint8_t v = CHARACTER_MAPPING[c - 'a'][j];
            if (offset < CHAR_WIDTH)
                fb[j] |= v >> (CHAR_WIDTH - offset);
            else
                fb[j] |= v << (offset - CHAR_WIDTH);
        }
    }
    else if (c >= '0' && c <= '9')
    {
        for (uint8_t j = 0; j < FB_HEIGHT; j++)
        {
            // fb[j] = 0;
            uint8_t v = INTEGER_MAPPING[c - '0'][j];
            if (offset < CHAR_WIDTH)
                fb[j] |= v >> (CHAR_WIDTH - offset);
            else
                fb[j] |= v << (offset - CHAR_WIDTH);
        }
    }
    else if (c >= '\200')
    {
        for (uint8_t j = 0; j < FB_HEIGHT; j++)
        {
            // fb[j] = 0;
            uint8_t v = SPECIAL_CHARS[c - '\200'][j];
            if (offset < CHAR_WIDTH)
                fb[j] |= v >> (CHAR_WIDTH - offset);
            else
                fb[j] |= v << (offset - CHAR_WIDTH);
        }
    }
    else
    {
        uint8_t index = 0;
        switch (c)
        {
        case '/':
            index = 1;
            break;
        case '\\':
            index = 2;
            break;
        case '>':
            index = 3;
            break;
        case '<':
            index = 4;
            break;
        case ':':
            index = 5;
            break;
        case '|':
            index = 6;
            break;
        case '!':
            index = 7;
            break;
        case '(':
            index = 8;
            break;
        case ')':
            index = 9;
            break;
        case '-':
            index = 10;
            break;
        case '+':
            index = 10;
            break;
        case '.':
            index = 12;
            break;
        case '\'':
            index = 13;
            break;
        case '?':
            index = 14;
            break;
        default:
            break;
        }

        for (uint8_t j = 0; j < FB_HEIGHT; j++)
        {
            // fb[j] = 0;
            uint8_t v = PUNCTUATION[index][j];
            if (offset < CHAR_WIDTH)
                fb[j] |= v >> (CHAR_WIDTH - offset);
            else
                fb[j] |= v << (offset - CHAR_WIDTH);
        }
    }
}

void clear_char_loc(uint8_t offset, uint32_t *const fb)
{
    const uint32_t v = 0b11111;
    for (uint8_t j = 0; j < FB_HEIGHT; j++)
    {

        if (offset < CHAR_WIDTH)
            fb[j] &= ~(v >> (CHAR_WIDTH - offset));
        else
            fb[j] &= ~(v << (offset - CHAR_WIDTH));
    }
}

