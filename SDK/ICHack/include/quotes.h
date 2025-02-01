#ifndef QUOTES_H
#define QUOTES_H

#include "config.h"
#include "letters.h"


// Quotes
extern uint32_t *quote_ptr;
extern uint32_t num_quotes;
extern uint32_t *quote_len_ptr;
extern unsigned char *quotes;

void render_quotes(uint32_t *fb, uint32_t *bit_streams);

#endif