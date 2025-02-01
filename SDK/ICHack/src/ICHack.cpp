#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>
#include "hardware/vreg.h"
#include "pico/stdlib.h"
#include "pico.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "pico/sleep.h"
#include "hardware/rosc.h"
#include "hardware/structs/scb.h"
#include "pico/rand.h"

#include "config.h"
#include "letters.h"
#include "snake.h"
#include "input.h"
#include "render.h"
#include "tetris.h"

#if DEBUG
#include "debug.h"
#include "ICHack.h"
#endif

#include "credits.h"
#include "quotes.h"
#include "logo_patterns.h"

enum Mode
{
  DISPLAY = 0,
  SNAKE = 1,
  TETRIS = 2,
  CREDITS = 3,
  QUOTES = 4,
};

// Global variables
uint32_t current_time;
uint32_t next_sleep_time;

uint32_t *flash_target_start = (uint32_t *)(XIP_BASE + CHAR_START);
uint32_t *flash_size = flash_target_start;

Mode mode = Mode::DISPLAY;

const uint8_t *flash_data_start = (uint8_t*)flash_size + sizeof(uint32_t);

uint32_t fb[5]{0, 0, 0, 0, 0};
uint32_t bit_streams[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define GPIO_INIT_OUT(PIN) \
  gpio_init(PIN);          \
  gpio_set_dir(PIN, GPIO_OUT);

const uintptr_t alpha_start = 0x20030000 + 50;
uint8_t *alpha_size_ptr = (uint8_t *)alpha_start;

void handle_gamepad();

void display_ich_render(uint8_t &phase, uint8_t &logo_phase_select);

void display_render_slow_down(uint8_t &ready, uint8_t &phase, uint8_t &logo_phase_select);

void init_ich_phase(uint8_t &phase, uint8_t &logo_phase_select);


uint *vco_freq_out;
uint *post_div1_out;
uint *post_div2_out;

static void sleep_callback(void) {
    return;
}

static void rtc_sleep(int8_t minute_to_sleep_to, int8_t second_to_sleep_to) {

    datetime_t t_alarm = {
            .year  = 2025,
            .month = 02,
            .day   = 01,
            .dotw  = 6, // 0 is Sunday, so 5 is Friday
            .hour  = 12,
            .min   = minute_to_sleep_to,
            .sec   = second_to_sleep_to
    };

    sleep_goto_sleep_until(&t_alarm, &sleep_callback);
}

void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig){

    //Re-enable ring Oscillator control
    rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

    //reset procs back to default
    scb_hw->scr = scb_orig;
    clocks_hw->sleep_en0 = clock0_orig;
    clocks_hw->sleep_en1 = clock1_orig;

    //reset clocks
    clocks_init();
    stdio_init_all();
    vreg_set_voltage(VREG_VOLTAGE_0_90);
    set_sys_clock_khz(18000, 1);

    return;
}

int main()
{
  vreg_set_voltage(vreg_voltage::VREG_VOLTAGE_0_90);
#if DEBUG
  set_sys_clock_48mhz();
#else
  set_sys_clock_khz(18000, 1);
#endif

  stdio_init_all();

  init_input();

#if DEBUG
  print("sys printf");
#endif

  uint8_t alpha_size = *alpha_size_ptr;
  uint8_t alpha_width = string_width(alpha_size);


  GPIO_INIT_OUT(LED_0);
  GPIO_INIT_OUT(LED_1);
  GPIO_INIT_OUT(LED_2);
  GPIO_INIT_OUT(CLK);
  GPIO_INIT_OUT(DIN);
  GPIO_INIT_OUT(OE);
  GPIO_INIT_OUT(RCLK);

  gpio_put(OE, 0);
  uart_default_tx_wait_blocking();

  static unsigned char alpha[] = "\203ICHACK 25\200 HELLO WORLD \203ICHACK 25\200";
  static uint32_t banner_size = sizeof(alpha);

  const static uint8_t num_chars = FB_WIDTH / 5;
  if (*flash_size > 128)
  {
    *flash_size = 128;
  }

    //save values for later
    uint scb_orig = scb_hw->scr;
    uint clock0_orig = clocks_hw->sleep_en0;
    uint clock1_orig = clocks_hw->sleep_en1;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    //crudely reset the clock each time to the value below
    datetime_t t = {
            .year  = 2025,
            .month = 02,
            .day   = 01,
            .dotw  = 6, // 0 is Sunday, so 5 is Friday
            .hour  = 12,
            .min   = 00,
            .sec   = 00
    };

    // Start the Real time clock
    rtc_init();

    current_time = to_ms_since_boot(get_absolute_time());
    next_sleep_time = current_time + AWAKE_TIME_1;

  while (true)
  {
    // print_buf(flash_data_start, *flash_size);

    current_time = to_ms_since_boot(get_absolute_time());

    if (current_time > next_sleep_time) {
        sleep_run_from_xosc();
        rtc_set_datetime(&t);
        clr_ICH();
        clear_fb(fb);
        render_fb(fb, bit_streams);
        rtc_sleep(SLEEP_TIME_MINS, SLEEP_TIME_SECS);

        //reset processor and clocks back to defaults
        recover_from_sleep(scb_orig, clock0_orig, clock1_orig);

        current_time = to_ms_since_boot(get_absolute_time());
        next_sleep_time = current_time + AWAKE_TIME_2;
    }

    if (mode == Mode::DISPLAY)
    {
      static uint8_t q = 0;
      static uint8_t logo_phase_select = 0;
      static uint8_t phase = 0;
      static uint8_t ready = 0;
      init_ich_phase(phase, logo_phase_select);

      while(mode == Mode::DISPLAY)
      {

        for (size_t n = 0; n < string_width(banner_size) + FB_WIDTH; n++)
        {
          clear_fb(fb);
          render_fb(fb, bit_streams, 0);
          string_banner_output(fb, alpha, banner_size, n);
          standard_render_fb(fb, bit_streams);
          display_render_slow_down(ready, phase, logo_phase_select);
          handle_gamepad();
          if(mode != Mode::DISPLAY)
            break;
        }

        clear_fb(fb);
        standard_render_fb(fb, bit_streams);
        clr_ICH();

        if (alpha_width && (mode == Mode::DISPLAY))
        {
          for (size_t n = 0; n < string_width(*flash_size) + FB_WIDTH; n++)
          {
            clear_fb(fb);
            render_fb(fb, bit_streams, 0);
            string_banner_output(fb, flash_data_start, *flash_size, n);
            standard_render_fb(fb, bit_streams);
            display_render_slow_down(ready, phase, logo_phase_select);
            handle_gamepad();
            if (mode != Mode::DISPLAY)
              break;
          }
        }
        q++;
        if (q > 10) {
          mode = Mode::QUOTES;
        }
        break;
      }
    }
    else if (mode == Mode::SNAKE)
    {
      Snake::play_snake(fb, bit_streams);
      mode = Mode::DISPLAY;
    }
    else if (mode == Mode::TETRIS)
    {
      tetris::play_tetris(fb, bit_streams);
      mode = Mode::DISPLAY;
    }
    else if (mode == Mode::CREDITS)
    {
      render_credits(fb, bit_streams);
      mode = Mode::DISPLAY;
    }
    else if (mode == Mode::QUOTES)
    {
      render_quotes(fb, bit_streams);
      mode = Mode::DISPLAY;
    }
  }
}

void display_render_slow_down(uint8_t &ready, uint8_t &phase, uint8_t &logo_phase_select)
{
  ready++;
  if (ready >= 4)
  {
    display_ich_render(phase, logo_phase_select);
    ready = 0;
  }
}

void display_ich_render(uint8_t &phase, uint8_t &logo_phase_select)
{
  init_ich_phase(phase, logo_phase_select);

  if(phase < PATTERN_LENGTH)
    ICH(logo_patterns[logo_phase_select][phase]);
  else
    clr_ICH();

  phase++;
}

void init_ich_phase(uint8_t &phase, uint8_t &logo_phase_select)
{
  if (phase >= PATTERN_LENGTH  * 8)
  {
    phase = 0;
    logo_phase_select = get_rand_32() % (ARRAY_LENGTH(logo_patterns));
  }
}

uint32_t button_press_history = 0; 

#define TETRIS_CODE  0x11332424 // Konami code
#define SNAKE_CODE   0x333      // DOWN DOWN DOWN
#define CREDITS_CODE 0x222      // LEFT LEFT LEFT
#define QUOTES_CODE  0x444      // RIGHT RIGHT RIGHT

inline void handle_gamepad()
{

  if (game_pad.updated())
  {
    current_time = to_ms_since_boot(get_absolute_time());
    next_sleep_time = current_time + AWAKE_TIME_1;
    ButtonDirection v = game_pad.latest_button_press();
    if (v != ButtonDirection::NONE)
    {
      button_press_history <<= 4;
      button_press_history |= v;
    }
  }

    if ((button_press_history & 0xFFFFFFFF) == TETRIS_CODE)
    {
      mode = Mode::TETRIS;
      button_press_history = 0;
    }
    else if ((button_press_history & 0xFFF) == SNAKE_CODE)
    {
      mode = Mode::SNAKE;
      button_press_history = 0;
    }
    else if ((button_press_history & 0xFFF) == CREDITS_CODE)
    {
      mode = CREDITS;
      button_press_history = 0;
    }
    else if ((button_press_history & 0xFFF) == QUOTES_CODE)
    {
      mode = QUOTES;
      button_press_history = 0;
    }
  
  game_pad.reset();
}
