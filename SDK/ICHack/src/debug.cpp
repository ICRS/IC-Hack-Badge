#include "debug.h"

void print_buf(const uint8_t *buf, size_t len)
{
#if DEBUG
    printf("%.*s\n", len, buf); // 5 here refers to # of characters
#endif
}

void print_frame_buffer(const uint32_t *const fb)
{
#if DEBUG
  printf("FB: \n");
  for (size_t i = 0; i < FB_HEIGHT; i++)
  {
    for (int j = sizeof(uint32_t) * 8 - 1; j >= 0; j--)
    {
      printf("%d", (fb[i] >> j) & 1);
    }
    printf("\n");
  }
#endif
}

void print_buf_hex(const uint8_t *buf, size_t len)
{
#if DEBUG
  for (size_t i = 0; i < len; ++i)
  {
    printf("%02x", buf[i]);
    if (i % 16 == 15)
      printf("\n");
    else
      printf(" ");
  }
#endif
}

void print(const char *buf)
{
#if DEBUG
  printf(buf);
#endif
}
