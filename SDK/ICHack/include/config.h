#ifndef CONFIG_H
#define CONFIG_H

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// LED Matrix Config
#define FB_HEIGHT 5
#define FB_WIDTH 24
#define FB_MASK ((0xffffffff << (32 - FB_WIDTH)) >> (32 - FB_WIDTH))
#define FB_TOP_MASK ((0xffffffff >> FB_WIDTH) << FB_WIDTH)

#define BIT_STREAM_SIZE 10

#define FLASH_TARGET_OFFSET (256 << 10)
// CHAR_START = 0x10040100
#define CHAR_START (FLASH_TARGET_OFFSET + 256)

// QUOTE_START = 0x10040400
#define QUOTE_START (FLASH_TARGET_OFFSET + 1024)

// PINOUT CONFIG
#define LED_PIN 9
#define LED_0 4
#define LED_1 5
#define LED_2 6

#define CLK 17
#define RCLK 18
#define OE 16
#define DIN 15

// INPUT BUTTONS
#define BUTTON_UP (0)
#define BUTTON_DOWN (3)
#define BUTTON_LEFT (2)
#define BUTTON_RIGHT (1)

#define AWAKE_TIME_1 (20 * 60 * 1000) // First awake time in ms
#define AWAKE_TIME_2 (10 * 1000) // Awake time for all other times in ms
#define SLEEP_TIME_MINS 2 // How long to sleep for in mins
#define SLEEP_TIME_SECS 0 // How long to sleep for in secs

#endif
