#include "input.h"
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

GamePad game_pad{};

static uint64_t get_time(void)
{
    // Reading low latches the high value
    uint32_t lo = timer_hw->timelr;
    uint32_t hi = timer_hw->timehr;
    return ((uint64_t)hi << 32u) | lo;
};

bool GamePad::updated()
{
    return left.pressed || right.pressed || down.pressed || up.pressed;
}

void GamePad::reset()
{
    left.pressed = false;
    right.pressed = false;
    down.pressed = false;
    up.pressed = false;
}

ButtonDirection GamePad::latest_button_press()
{
    if (left.pressed)
    {
        if (right.pressed && right.updated_time > left.updated_time)
            goto left_end;
        if (up.pressed && up.updated_time > left.updated_time)
            goto left_end;
        if (down.pressed && down.updated_time > left.updated_time)
            goto left_end;
        return ButtonDirection::LEFT;
    }
left_end:
    if (right.pressed)
    {
        if (left.pressed && left.updated_time > right.updated_time)
            goto right_end;
        if (up.pressed && up.updated_time > right.updated_time)
            goto right_end;
        if (down.pressed && down.updated_time > right.updated_time)
            goto right_end;
        return ButtonDirection::RIGHT;
    }
right_end:
    if (up.pressed)
    {
        if (left.pressed && left.updated_time > up.updated_time)
            goto up_end;
        if (right.pressed && right.updated_time > up.updated_time)
            goto up_end;
        if (down.pressed && down.updated_time > up.updated_time)
            goto up_end;
        return ButtonDirection::UP;
    }
up_end:
    if (down.pressed)
    {
        if (left.pressed && left.updated_time > down.updated_time)
            goto down_end;
        if (right.pressed && right.updated_time > down.updated_time)
            goto down_end;
        if (up.pressed && up.updated_time > down.updated_time)
            goto down_end;
        return ButtonDirection::DOWN;
    }
down_end:

    return ButtonDirection::NONE;
}


static void isr_handler(uint buf, uint32_t events)
{
    // uint32_t current_time = millis();
    // if (current_time - up_pressed_time > PRESS_TIMEOUT)
    // {
    uint64_t current_time = get_time();
    if ((buf == BUTTON_UP) && ((game_pad.up.updated_time + PRESS_TIMEOUT) < current_time))
    {
#if DEBUG
        printf("LAST: %llu, current: %llu ", game_pad.up.updated_time, current_time);
#endif
        game_pad.up.pressed = true;
        game_pad.up.updated_time = current_time;
        print("UP\n");
    }
    else if ((buf == BUTTON_DOWN) && ((game_pad.down.updated_time + PRESS_TIMEOUT) < current_time))
    {
#if DEBUG
        printf("LAST: %llu, current: %llu ", game_pad.down.updated_time, current_time);
#endif
        game_pad.down.pressed = true;
        game_pad.down.updated_time = current_time;
        print("DOWN\n");
    }
    else if ((buf == BUTTON_LEFT) && ((game_pad.left.updated_time + PRESS_TIMEOUT) < current_time))
    {
#if DEBUG
        printf("LAST: %llu, current: %llu ", game_pad.left.updated_time, current_time);
#endif
        game_pad.left.pressed = true;
        game_pad.left.updated_time = current_time;
        print("LEFT\n");
    }
    else if ((buf == BUTTON_RIGHT) && ((game_pad.right.updated_time + PRESS_TIMEOUT) < current_time))
    {
#if DEBUG
        printf("LAST: %llu, current: %llu ", game_pad.right.updated_time, current_time);
#endif
        game_pad.right.pressed = true;
        game_pad.right.updated_time = current_time;
        print("RIGHT\n");
    }

}

void init_input()
{
    gpio_pull_up(BUTTON_DOWN);
    gpio_pull_up(BUTTON_UP);
    gpio_pull_up(BUTTON_RIGHT);
    gpio_pull_up(BUTTON_LEFT);

    // gpio_set_irq_enabled_with_callback(BUTTON_UP, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &up_pressed_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL, true, &isr_handler);
    gpio_set_irq_enabled(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_RIGHT, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_LEFT, GPIO_IRQ_EDGE_FALL, true);
}

