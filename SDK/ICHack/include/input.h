#ifndef INPUT_H
#define INPUT_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "config.h"
#include "debug.h"

#ifndef PRESS_TIMEOUT
#define PRESS_TIMEOUT (100 * 1000) // ms
#endif

#ifndef INPUT_TICKS
#define INPUT_TICKS 4 // out of 8
#endif

static uint64_t get_time(void);

struct Button
{
    bool pressed = false;
    uint64_t updated_time = 0;
};

enum ButtonDirection
{
    NONE = 0,
    UP = 1,
    LEFT = 2,
    DOWN = 3,
    RIGHT = 4,
};

struct GamePad
{
    Button left{};
    Button right{};
    Button down{};
    Button up{};

    bool updated();

    void reset();
    
    ButtonDirection latest_button_press();

};

extern GamePad game_pad;

static void isr_handler(uint buf, uint32_t events);

void init_input();


#endif