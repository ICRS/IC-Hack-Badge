#ifndef LETTERS_H
#define LETTERS_H

#include "config.h"
#include "render.h"

#define CHAR_WIDTH (4)
#define MAX_RENDERED_CHARS (5)
// #define MAX_RENDERED_CHARS (((FB_WIDTH + 1) / CHAR_WIDTH + 1))

constexpr uint32_t string_width_32_t(uint32_t string_width)
{
  return string_width * MAX_RENDERED_CHARS - 1;
}

constexpr uint8_t string_width(uint8_t string_width)
{
  return string_width * MAX_RENDERED_CHARS - 1;
}

/**
 * fb - framebuffer
 * string_buf - string buffer to be outputted to led matrix
 * string_size - string size
 * offset - how much/where to start displaying the string from - 0 means none of the string is visible,
 *  24 would show the first 5 letters
 */
void string_banner_output(uint32_t *const fb, const unsigned char * const string_buf, uint32_t string_size, uint32_t offset);


const static uint8_t SPECIAL_CHARS[][5]
{
    {// \200
     0b1111,
     0b1111,
     0b1110,
     0b1100,
     0b1000},
    {// \201
     0b0011,
     0b0111,
     0b1110,
     0b1100,
     0b1000},
    {// \202
     0b0111,
     0b0111,
     0b0111,
     0b0111,
     0b0111},
    {// \203
     0b0001,
     0b0011,
     0b0111,
     0b1111,
     0b1111},
    {// \204
     0b1000,
     0b1100,
     0b1110,
     0b1111,
     0b1111},
    {// \205
     0b1001,
     0b0000,
     0b0000,
     0b1001,
     0b0110},
};

const static uint8_t INTEGER_MAPPING[10][5]
{
    {// 0
     0b0110,
     0b1011,
     0b1011,
     0b1101,
     0b0110},
    {// 1
     0b0110,
     0b1110,
     0b0010,
     0b0010,
     0b1111},
    {// 2
     0b0110,
     0b1001,
     0b0010,
     0b0100,
     0b1111},
    {// 3
     0b1111,
     0b0001,
     0b0010,
     0b1001,
     0b0110},
    {// 4
     0b0101,
     0b1001,
     0b1111,
     0b0001,
     0b0001},
    {// 5
     0b1111,
     0b1000,
     0b1110,
     0b0001,
     0b1110},
    {// 6
     0b0110,
     0b1000,
     0b1110,
     0b1001,
     0b0110},
    {// 7
     0b1111,
     0b0001,
     0b0010,
     0b0100,
     0b1000},
    {// 8
     0b0110,
     0b1001,
     0b0110,
     0b1001,
     0b0110},
    {// 9
     0b0110,
     0b1001,
     0b0111,
     0b0001,
     0b0110},
};


const static uint8_t PUNCTUATION[][5] = {
    {//
     0b0000,
     0b0000,
     0b0000,
     0b0000,
     0b0000},
    {// //
     0b0011,
     0b0010,
     0b0110,
     0b0100,
     0b1100},
    {// \\.
     0b1100,
     0b0100,
     0b0110,
     0b0010,
     0b0011},
    {// >>
     0b1100,
     0b0110,
     0b0011,
     0b0110,
     0b1100},
    {// <<
     0b0011,
     0b0110,
     0b1100,
     0b0110,
     0b0011},
    {// :
     0b0000,
     0b0100,
     0b0000,
     0b0100,
     0b0000},
    {// ||
     0b0110,
     0b0110,
     0b0110,
     0b0110,
     0b0110},
    {// !
     0b0110,
     0b0110,
     0b0110,
     0b0000,
     0b0110},
    {// (
     0b0110,
     0b1100,
     0b1100,
     0b1100,
     0b0110},
    {// )
     0b0110,
     0b0011,
     0b0011,
     0b0011,
     0b0110},
    {// -
     0b0000,
     0b0000,
     0b0110,
     0b0000,
     0b0000},
    {// +
     0b0000,
     0b0100,
     0b1110,
     0b0100,
     0b0000},
    {// .
     0b0000,
     0b0000,
     0b0000,
     0b0000,
     0b0100},
    {// '
     0b0100,
     0b0100,
     0b0000,
     0b0000,
     0b0000},
    {// ?
     0b1110,
     0b0001,
     0b0110,
     0b0000,
     0b0100},
};

const static uint8_t CHARACTER_MAPPING[26][5] = {
    {// A
     0b0110,
     0b1001,
     0b1111,
     0b1001,
     0b1001},
    {// B
     0b1110,
     0b1001,
     0b1110,
     0b1001,
     0b1110},
    {// C
     0b0111,
     0b1000,
     0b1000,
     0b1000,
     0b0111},
    {// D
     0b1110,
     0b1001,
     0b1001,
     0b1001,
     0b1110},
    {// E
     0b1111,
     0b1000,
     0b1110,
     0b1000,
     0b1111},
    {// F
     0b1111,
     0b1000,
     0b1110,
     0b1000,
     0b1000},
    {// G
     0b1111,
     0b1000,
     0b1011,
     0b1001,
     0b1111},
    {// H
     0b1001,
     0b1001,
     0b1111,
     0b1001,
     0b1001},
    {// I
     0b0111,
     0b0010,
     0b0010,
     0b0010,
     0b0111},
    {// J
     0b1111,
     0b0010,
     0b0010,
     0b1010,
     0b1110},
    {// K
     0b1001,
     0b1010,
     0b1100,
     0b1010,
     0b1001},
    {// L
     0b1000,
     0b1000,
     0b1000,
     0b1000,
     0b1111},
    {// M
     0b1001,
     0b1111,
     0b1101,
     0b1001,
     0b1001},
    {// N
     0b1001,
     0b1101,
     0b1101,
     0b1011,
     0b1001},
    {// O
     0b0110,
     0b1001,
     0b1001,
     0b1001,
     0b0110},
    {// P
     0b1110,
     0b1001,
     0b1110,
     0b1000,
     0b1000},
    {// Q
     0b0110,
     0b1001,
     0b1001,
     0b1010,
     0b0101},
    {// R
     0b1110,
     0b1001,
     0b1110,
     0b1010,
     0b1001},
    {// S
     0b0111,
     0b1000,
     0b0110,
     0b0001,
     0b1110},
    {// T
     0b0111,
     0b0010,
     0b0010,
     0b0010,
     0b0010},
    {// U
     0b1001,
     0b1001,
     0b1001,
     0b1001,
     0b0110},
    {// V
     0b1001,
     0b1001,
     0b1001,
     0b0110,
     0b0110},
    {// W
     0b1001,
     0b1001,
     0b1001,
     0b1101,
     0b1010},
    {// X
     0b1001,
     0b1001,
     0b0110,
     0b1001,
     0b1001},
    {// Y
     0b1001,
     0b1001,
     0b0111,
     0b0010,
     0b1100},
    {// Z
     0b1111,
     0b0010,
     0b0100,
     0b1000,
     0b1111}};

void char_output(uint32_t *const fb, char c, uint32_t offset);

void clear_char_loc(uint8_t offset, uint32_t *const fb);

void string_banner_output_non_clr(uint32_t *const fb, const unsigned char * const string_buf, uint32_t string_size, uint32_t offset);

#endif
