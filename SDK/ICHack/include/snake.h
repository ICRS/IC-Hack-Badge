#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "logo_patterns.h"
#include "pico/stdlib.h"
#include "pico/rand.h"

#include "render.h"
#include "letters.h"
#include "config.h"
#include "debug.h"
#include "input.h"

#include <algorithm>

uint8_t high_score = 0;
bool LCH = false;

namespace Snake{
    enum class Direction
    {
        UP = 0,
        LEFT = 1,
        DOWN = 2,
        RIGHT = 3
    };

    struct Coord
    {
        uint8_t x = FB_WIDTH / 2;
        uint8_t y = FB_HEIGHT / 2;

        bool operator==(const Coord &) const = default;
    };

    Direction direction = Direction::UP;
    std::deque<Coord> snake;
    Coord fruitCoord;

    void snake_update_bf(uint32_t *fb);

    inline void updatefruitCoordRand()
    {
        do
        {
            int rand_ = get_rand_32();
            uint8_t x = (uint8_t)std::min(rand_ & 0b11111, FB_WIDTH - 1);
            uint8_t y = (uint8_t)std::min((rand_ >> 5) & 0b111, FB_HEIGHT - 1);
            fruitCoord = Coord{x, y};
        } while (std::find(snake.begin(), snake.end(), fruitCoord) != snake.end());
    }

    inline void snake_setup()
    {
        snake.clear();
        direction = Direction::LEFT;
        snake.emplace_back(Coord(FB_WIDTH / 2 + 1, FB_HEIGHT / 2));
        snake.emplace_back(Coord(FB_WIDTH / 2, FB_HEIGHT / 2));
        snake.emplace_back(Coord(FB_WIDTH / 2 - 1, FB_HEIGHT / 2));
        updatefruitCoordRand();
    }

    void handle_game_pad();
    bool check_end();

    void loading_screen(uint32_t *fb, uint32_t bit_streams[10])
    {
        const static unsigned char tetris_str[] = "SNAKE";
        for (size_t i = 0; i < 6; i++)
        {
            if(i & 1)
            {
                string_banner_output(fb, tetris_str, ARRAY_LENGTH(tetris_str), FB_WIDTH);
                standard_render_fb(fb, bit_streams);
            }
            else
            {
                clear_fb(fb);
                standard_render_fb(fb, bit_streams);
            }
        }
    }

    void play_snake(uint32_t *fb, uint32_t bit_streams[10])
    {
        clear_fb(fb);
        snake_setup();
        loading_screen(fb, bit_streams);
        print("Setup Done\n");

        clr_ICH();
        do
        {
            clear_fb(fb);

            if (LCH)
            {
                set_all_ICH();
                LCH = false;
            }
            else
            {
                clr_ICH();
            }

            snake_update_bf(fb);
            handle_game_pad();
            for (size_t i = 1; i < std::max(60 - (snake.size() / 4), 16u); i++)
            {
                render_fb(fb, bit_streams, 10000);
                sleep_ms(4);
            }

            Coord head = snake.front();
            switch (direction)
            {
            case Direction::UP:
                head.y--;
                break;
            case Direction::LEFT:
                head.x++;
                break;
            case Direction::DOWN:
                head.y++;
                break;
            case Direction::RIGHT:
                head.x--;
                break;

            default:
                break;
            }
            snake.emplace_front(head);
            if (head != fruitCoord)
            {
                snake.pop_back();
            }
            else
            {
                LCH = true;
                updatefruitCoordRand();
            }

        } while (!check_end());

        uint8_t value = snake.size();
        if (value > high_score)
        {
            high_score = value;
        }

#if DEBUG
        printf("high score: %d, value: %d\n", high_score, value);
#endif
        unsigned char start[] = "SC0R3:000|H1 SC0R3:000";
        for (size_t i = 0; i < 3; i++)
        {
            start[8 - i] = '0' + value % 10;
            value /= 10;

            start[ARRAY_LENGTH(start) - 2 - i] = '0' + (high_score % 10);
            high_score /= 10;
        }

        for (size_t i = 0; i < (string_width(ARRAY_LENGTH(start)) + (FB_WIDTH << 1)); i++)
        {
            string_banner_output(fb, start, ARRAY_LENGTH(start), i);
            standard_render_fb(fb, bit_streams);
            if ((i > 30) && game_pad.updated())
                break;
        }
        clear_fb(fb);
        render_fb(fb, bit_streams);
        game_pad.reset();
    }

    void handle_game_pad()
    {
        if (game_pad.updated())
        {
            if (game_pad.up.pressed)
            {
                direction = Direction::RIGHT;
            }
            if (game_pad.down.pressed)
            {
                direction = Direction::LEFT;
            }
            if (game_pad.left.pressed)
            {
                direction = Direction::UP;
            }
            if (game_pad.right.pressed)
            {
                direction = Direction::DOWN;
            }

            game_pad.reset();
        }
    }

    void snake_update_bf(uint32_t *fb)
    {
        for (const auto &coord : snake)
        {
            fb[coord.y] |= (1 << coord.x);
        }
        fb[fruitCoord.y] |= (1 << fruitCoord.x);
    }

    bool check_end()
    {
        Coord &front = snake.front();
        if (std::find(snake.begin() + 1, snake.end(), front) != snake.end())
        {
            return true;
        }
        if (!((front.x < FB_WIDTH) && (front.x >= 0) && (front.y < FB_HEIGHT) && (front.y >= 0)))
        {
            return true;
        }

        return false;
    }
}

#endif