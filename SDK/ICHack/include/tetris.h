#ifndef TETRIS_H
#define TETRIS_H

#include "letters.h"
#include "logo_patterns.h"
#include "pico/rand.h"

// Dear programmer:
// When I wrote this code, only god and
// I knew how it worked.
// Now, only god knows it!
// 
// Therefore, if you are trying to optimize
// this routine and it fails (most surely),
// please increase this counter as a
// warning for the next person:
// 
// total hours wasted here = 0

bool TLCH = false;

namespace tetris
{
    uint32_t high_score = 0;
    uint8_t current_ich_state = 0;

    void tetris_update_ich();

    struct Tetromino_3
    {
        const uint8_t states[4][3];

        int8_t x = 0;
        int8_t y = FB_HEIGHT / 2;

        int8_t rot = 0;

        void reset()
        {
            x = 0;
            y = FB_HEIGHT / 2;
            rot = 0;
        }

        void next_rot()
        {
            rot++;
            rot &= 0b11;
        }

        inline const uint8_t *state()
        {
            return states[rot];
        }

        inline bool y_valid()
        {
            return !((y == FB_HEIGHT - 1) && state()[0]) && !(!y && state()[2]);
        }

        bool update_fb(uint32_t *fb, const ButtonDirection &user_input)
        {
#if DEBUG
            printf("%d %d\n", x, y);
#endif
            // Clear Previous
            tetris_update_ich();
            reset_block(fb);

            if (!check_can_move(fb))
            {
                x--;
                update_fb(fb);
                return false;
            }

            print_frame_buffer(fb);

            switch (user_input)
            {
            case ButtonDirection::RIGHT:
                y = std::min(++y, (int8_t)(FB_HEIGHT - 1));
                if (!y_valid())
                {
                    y--;
                }
                break;
            case ButtonDirection::LEFT:
                y = std::max(--y, (int8_t)0);
                if (!y_valid())
                {
                    y++;
                }
                break;
            case ButtonDirection::UP:
                next_rot();
                if (!check_can_move(fb) || !y_valid())
                {
                    if (y == 0)
                    {
                        y++;
                        if (!check_can_move(fb))
                        {
                            y--;
                            goto fb_updated_bad;
                        }
                        goto fb_update_fin;
                    }
                    else if (y == 4)
                    {
                        y--;
                        if (!check_can_move(fb))
                        {
                            y++;
                            goto fb_updated_bad;
                        }
                        goto fb_update_fin;
                    }
                fb_updated_bad:
                    rot--;
                    if (rot == -1)
                    {
                        rot = 3;
                    }
                }
            fb_update_fin:

                break;

            default:
                break;
            }

            update_fb(fb);
            x++;
            return true;
        }

        void reset_block(uint32_t *fb)
        {
            if (x > 1)
            {
                fb[y + 1] &= (-1) ^ (state()[0] << (x - 2));
                fb[y] &= (-1) ^ (state()[1] << (x - 2));
                fb[y - 1] &= (-1) ^ (state()[2] << (x - 2));
            }
            else if (x == 1)
            {
                fb[y + 1] &= (-1) ^ (state()[0] >> 1);
                fb[y] &= (-1) ^ (state()[1] >> 1);
                fb[y - 1] &= (-1) ^ (state()[2] >> 1);
            }
        }

        void update_fb(uint32_t *fb)
        {
            if (x == 0)
            {
                fb[y + 1] |= state()[0] >> 1;
                fb[y] |= state()[1] >> 1;
                fb[y - 1] |= state()[2] >> 1;
            }
            else
            {
                fb[y + 1] |= state()[0] << (x - 1);
                fb[y] |= state()[1] << (x - 1);
                fb[y - 1] |= state()[2] << (x - 1);
            }
        };

        bool check_can_move(uint32_t *fb)
        {
            if (x >= FB_WIDTH - 2)
            {
                if ((state()[2] << (x - 1)) & FB_TOP_MASK)
                {
                    return false;
                }
                if ((state()[1] << (x - 1)) & FB_TOP_MASK)
                {
                    return false;
                }
                if ((state()[0] << (x - 1)) & FB_TOP_MASK)
                {
                    return false;
                }
            }

            if (x == 0)
            {
                if (((fb[y + 1] & (state()[0] >> 1)) & FB_MASK))
                {
                    return false;
                }
                if (((fb[y + 0] & (state()[1] >> 1)) & FB_MASK))
                {
                    return false;
                }
                if (((fb[y - 1] & (state()[2] >> 1)) & FB_MASK))
                {
                    return false;
                }
            }

            if (x >= 1)
            {
                if (((fb[y + 1] & (state()[0] << (x - 1))) & FB_MASK))
                {
                    return false;
                }
                if (((fb[y + 0] & (state()[1] << (x - 1))) & FB_MASK))
                {
                    return false;
                }
                if (((fb[y - 1] & (state()[2] << (x - 1))) & FB_MASK))
                {
                    return false;
                }
            }

            return true;
        }
    };

    struct TetrominoSquare
    {
        int8_t x = 0;
        int8_t y = FB_HEIGHT / 2;

        void next_rot()
        {
        }

        bool update_fb(uint32_t *fb, const ButtonDirection &user_input)
        {
#if DEBUG
            printf("%d %d\n", x, y);
#endif
            tetris_update_ich();
            if (!check_can_move(fb))
                return false;

            if (x > 0)
            {
                fb[y + 1] &= (-1) ^ (0b11 << (x - 1));
                fb[y] &= (-1) ^ (0b11 << (x - 1));
            }
            else if (x >= 0)
            {
                fb[y + 1] &= (-1) ^ (1 << (x));
                fb[y] &= (-1) ^ (1 << (x));
            }
            switch (user_input)
            {
            case ButtonDirection::RIGHT:
                y = std::min(++y, (int8_t)(FB_HEIGHT - 2));
                break;
            case ButtonDirection::LEFT:
                y = std::max(--y, (int8_t)0);
                break;

            default:
                break;
            }

            fb[y + 1] |= (0b11 << x);
            fb[y] |= (0b11 << x);

            x++;
            return true;
        }

        bool check_can_move(uint32_t *fb)
        {
            if (((fb[y + 1] >> (x + 1)) & 1) || ((fb[y] >> (x + 1)) & 1) || x >= (FB_WIDTH - 1))
            {
                return false;
            }

            return true;
        }
    };

    struct TetrominoLine
    {
        uint8_t states[4][4] = {
            {
                0b0010,
                0b0010,
                0b0010,
                0b0010,
            },
            {
                0b0000,
                0b0000,
                0b1111,
                0b0000,
            },
            {
                0b0100,
                0b0100,
                0b0100,
                0b0100,
            },
            {
                0b0000,
                0b1111,
                0b0000,
                0b0000,
            },
        };

        uint8_t x = 0;
        uint8_t y = FB_HEIGHT / 2;

        uint8_t rot = 0;

        void next_rot()
        {
            rot++;
            rot &= 0b11;
        }

        inline uint8_t *state()
        {
            return states[rot];
        }
    };

    void play_tetris(uint32_t *const fb, uint32_t bit_streams[10]);

    void tetris_render(uint32_t *const fb, uint32_t bit_streams[10]);

    Tetromino_3 tetromino_T{{{0b010,
                              0b111,
                              0b000},
                             {0b010,
                              0b011,
                              0b010},
                             {0b000,
                              0b111,
                              0b010},
                             {0b010,
                              0b110,
                              0b010}}};
    Tetromino_3 tetromino_Z{{{0b110,
                              0b011,
                              0b000},
                             {0b001,
                              0b011,
                              0b010},
                             {0b000,
                              0b110,
                              0b011},
                             {0b010,
                              0b110,
                              0b100}}};
    Tetromino_3 tetromino_RZ{{{0b011,
                               0b110,
                               0b000},
                              {0b010,
                               0b011,
                               0b001},
                              {0b000,
                               0b011,
                               0b110},
                              {0b100,
                               0b110,
                               0b010}}};
    Tetromino_3 tetromino_RL{{{0b001,
                               0b111,
                               0b000},
                              {0b010,
                               0b010,
                               0b011},
                              {0b000,
                               0b111,
                               0b100},
                              {0b110,
                               0b010,
                               0b010}}};
    Tetromino_3 tetromino_L{{{0b100,
                              0b111,
                              0b000},
                             {0b011,
                              0b010,
                              0b010},
                             {0b000,
                              0b111,
                              0b001},
                             {0b010,
                              0b010,
                              0b110}}};

    uint32_t score = 0;
    void clear_rows(uint32_t *fb)
    {
        int8_t x = FB_WIDTH;
        print_frame_buffer(fb);
        while (x > 0)
        {
            for (size_t i = 0; i < FB_HEIGHT; i++)
            {
                if (!((fb[i] >> (x - 1)) & 0b1))
                {
                    goto cc_not_found;
                }
            }

#if DEBUG
            print("clearing ");
            printf("%d \n", x);
            print_frame_buffer(fb);
#endif

            for (size_t i = 0; i < FB_HEIGHT; i++)
            {
                uint32_t t = (fb[i] >> (x));
                fb[i] <<= (33 - x);
                fb[i] >>= (32 - x);
                fb[i] |= (t << (x));
            }
            score++;
            goto cc_found;

        cc_not_found:
            x--;

        cc_found:
            TLCH = true;
            continue;
        }
        for (size_t i = 0; i < FB_HEIGHT; i++)
        {
            fb[i] &= FB_MASK;
        }
        print_frame_buffer(fb);
    }

    bool finished(uint32_t *const fb)
    {
        for (size_t i = 0; i < FB_HEIGHT; i++)
        {
            if (fb[i] & 0b111)
            {
                return false;
            }
        }

        return true;
    }

    void loading_screen(uint32_t *const fb, uint32_t bit_streams[10])
    {
        const static unsigned char tetris_str[] = "TETRIS";

        for (uint8_t i = 0; i < (string_width(ARRAY_LENGTH(tetris_str)) + (FB_WIDTH << 1)); i += 2)
        {
            string_banner_output(fb, tetris_str, ARRAY_LENGTH(tetris_str), i);
            standard_render_fb(fb, bit_streams);
            if (game_pad.latest_button_press() == ButtonDirection::UP)
                goto tetris_loading_screen_end;
        }

    tetris_loading_screen_end:
        clear_fb(fb);
        return;
    }

    void tetris_update_ich()
    {
        // set_mask_ICH(current_ich_state);
        // current_ich_state++;
        // if (current_ich_state > 0b1010)
        // {
        //     current_ich_state = 0;
        //     clr_ICH();
        // }
        if (TLCH){
            set_all_ICH();
            TLCH = false;
        }    
        else {
            clr_ICH();
        }
        
    }

    void play_tetris(uint32_t *const fb, uint32_t bit_streams[10])
    {
        clear_fb(fb);
        clr_ICH();
        print("Setup Done\n");
        loading_screen(fb, bit_streams);
        score = 0;

        while (finished(fb))
        {
            uint8_t rand_ = 0;
            do
            {
                rand_ = (uint8_t)get_rand_32() & 0b0111;
            } while (!rand_);

            bool still_alive = true;
            if (rand_ == 1)
            {
                TetrominoSquare tetromino_square;
                while (still_alive = tetromino_square.update_fb(fb, game_pad.latest_button_press()))
                {
                    game_pad.reset();
                        tetris_render(fb, bit_streams);
                }
            }
            else
            {
                switch (rand_)
                {
                case 7:
                case 2:
                    tetromino_T.reset();
                    while (still_alive = tetromino_T.update_fb(fb, game_pad.latest_button_press()))
                    {
                        game_pad.reset();
                        tetris_render(fb, bit_streams);
                    }
                    break;
                case 3:
                    tetromino_Z.reset();
                    while (still_alive = tetromino_Z.update_fb(fb, game_pad.latest_button_press()))
                    {
                        game_pad.reset();
                        tetris_render(fb, bit_streams);
                    }
                    break;
                case 4:
                    tetromino_L.reset();
                    while (still_alive = tetromino_L.update_fb(fb, game_pad.latest_button_press()))
                    {
                        game_pad.reset();
                        tetris_render(fb, bit_streams);
                    }
                    break;
                case 5:
                    tetromino_RZ.reset();
                    while (still_alive = tetromino_RZ.update_fb(fb, game_pad.latest_button_press()))
                    {
                        game_pad.reset();
                        tetris_render(fb, bit_streams);
                    }
                    break;
                default:
                    tetromino_RL.reset();
                    while (still_alive = tetromino_RL.update_fb(fb, game_pad.latest_button_press()))
                    {
                        game_pad.reset();
                        tetris_render(fb, bit_streams);
                    }
                    break;
                }
            }
            clear_rows(fb);
            print("yy\n");
        }

        clear_fb(fb);
        if (score > high_score)
        {
            high_score = score;
        }

#if DEBUG
        printf("high score: %d, value: %d\n", high_score, score);
#endif
        unsigned char start[] = "SC0R3:00000|H1 SC0R3:00000";
        for (size_t i = 0; i < 3; i++)
        {
            start[8 - i] = '0' + score % 10;
            score /= 10;

            start[ARRAY_LENGTH(start) - 4 - i] = '0' + (high_score % 10);
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

    inline void tetris_render(uint32_t *const fb, uint32_t bit_streams[10])
    {
        for (size_t i = 0; i < (4 - std::min(score >> 2, (uint32_t)3)); i++)
        {
            standard_render_fb(fb, bit_streams);
        }
    };
}; // namespace tetris

#endif