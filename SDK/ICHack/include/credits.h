#ifndef CREDITS_H
#define CREDITS_H

#include <cstring>
#include "config.h"
#include "logo_patterns.h"

#define BADGE_CREDITS_STR "\203ICHACK BADGE V1|HARDWARE BY AGENT B. SM1TH|FIRMWARE WU H00|DESIGN: A.VOTIN+K.WANG\204"

extern uint8_t *ich_target_start;

// Sine Wave for Bg
const uint32_t sine_background[] = {
    0b011100000000011100000000,
    0b100010000000100010000000,
    0b000001000001000001000001,
    0b000000100010000000100010,
    0b000000011100000000011100,
};

// only first 24 bits of uint in bg are considered
void circular_fb_background(uint32_t *fb, const uint32_t *const bg, uint8_t offset);

/*
Render funny credits
*/
void render_credits(uint32_t *fb, uint32_t *bit_streams);

#endif