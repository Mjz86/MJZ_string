#pragma once

/* a=target variable, b=bit number to act upon 0-n */
#define MJZ_frm_stack_ovf_BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define MJZ_frm_stack_ovf_BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define MJZ_frm_stack_ovf_BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define MJZ_frm_stack_ovf_BIT_CHECK(a, b) \
  (!!((a) & (1ULL << (b))))  // '!!' to make sure this returns 0 or 1

#define MJZ_frm_stack_ovf_BIT(b) (1ULL << (b))
#define MJZ_frm_stack_ovf_BIT_NOT(b) (~(1ULL << (b)))

#define MJZ_frm_stack_ovf_BITMASK_SET(x, mask) ((x) |= (mask))
#define MJZ_frm_stack_ovf_BITMASK_CLEAR(x, mask) ((x) &= (~(mask)))
#define MJZ_frm_stack_ovf_BITMASK_FLIP(x, mask) ((x) ^= (mask))
#define MJZ_frm_stack_ovf_BITMASK_CHECK_ALL(x, mask) (!(~(x) & (mask)))
#define MJZ_frm_stack_ovf_BITMASK_CHECK_ANY(x, mask) ((x) & (mask))

#define min_macro_(_a, _b) (((_a) < (_b)) ? (_a) : (_b))

// #define Arduino

// #define Arduino 1
#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
template <typename... T>
int sprintf_alt_( char * const buffer_for_print, size_t size_of_msg,
                 const char* const forming__, T... argumentes) {
  #ifndef Arduino
  return sprintf_s(buffer_for_print, size_of_msg, forming__, argumentes...);
  #else
  return sprintf(buffer_for_print, forming__, argumentes...);
  #endif
}
#ifdef Arduino
#include <sys/time.h>
#else

union my_data_randomizer_union {
  uint64_t my_data_randomizer_u;
  uint32_t my_data_randomizer;
  double my_data_randomizer_d; };
extern my_data_randomizer_union my_data_randomizer_uni;
template <typename... T>
uint32_t esp_random( T... arggs ) {
  ++my_data_randomizer_uni.my_data_randomizer_d;
  my_data_randomizer_uni.my_data_randomizer_d *=
    my_data_randomizer_uni.my_data_randomizer_u;
  --my_data_randomizer_uni.my_data_randomizer_d;
  my_data_randomizer_uni.my_data_randomizer_d /=
    my_data_randomizer_uni.my_data_randomizer_u;
  return my_data_randomizer_uni.my_data_randomizer; }

template <class T, class L>
auto min( const T & a, const L & b ) -> decltype( ( b < a ) ? b : a ) {
  return ( b < a ) ? b : a; }

template <class T, class L>
auto max( const T & a, const L & b ) -> decltype( ( b < a ) ? b : a ) {
  return ( a < b ) ? b : a; }

#define __attribute__(x)
#endif
#include <inttypes.h>
#include <math.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

/*
  binary.h - Definitions for binary constants
  Deprecated -- use 0b binary literals instead
  Copyright (c) 2006 David A. Mellis. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License,or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not,write to the Free Software
  Foundation,Inc.,51 Franklin St,Fifth Floor,Boston,MA 02110-1301 USA
*/

#ifndef Binary_h
#define Binary_h

#ifdef Arduino
  #include "mbedtls/md.h"
#endif

namespace mjz_ard {
  /* If supported,0b binary literals are preferable to these constants.
      In that case,warn the user about these being deprecated (if possible). */
#if __cplusplus >= 201402L
  /* C++14 introduces binary literals; C++11 introduces [[deprecated()]] */
  #define DEPRECATED(x) [[deprecated("use " #x " instead")]]
#elif __GNUC__ >= 6
  /* GCC 4.3 supports binary literals; GCC 6 supports __deprecated__ on enums*/
  #define DEPRECATED(x) __attribute__((__deprecated__("use " #x " instead")))
#else
  /* binary literals not supported,or "deprecated" warning not displayable */
  #define DEPRECATED(x)
#endif

  enum {
    B0 DEPRECATED( 0b0 ) = 0,
    B00 DEPRECATED( 0b00 ) = 0,
    B000 DEPRECATED( 0b000 ) = 0,
    B0000 DEPRECATED( 0b0000 ) = 0,
    B00000 DEPRECATED( 0b00000 ) = 0,
    B000000 DEPRECATED( 0b000000 ) = 0,
    B0000000 DEPRECATED( 0b0000000 ) = 0,
    B00000000 DEPRECATED( 0b00000000 ) = 0,
    B1 DEPRECATED( 0b1 ) = 1,
    B01 DEPRECATED( 0b01 ) = 1,
    B001 DEPRECATED( 0b001 ) = 1,
    B0001 DEPRECATED( 0b0001 ) = 1,
    B00001 DEPRECATED( 0b00001 ) = 1,
    B000001 DEPRECATED( 0b000001 ) = 1,
    B0000001 DEPRECATED( 0b0000001 ) = 1,
    B00000001 DEPRECATED( 0b00000001 ) = 1,
    B10 DEPRECATED( 0b10 ) = 2,
    B010 DEPRECATED( 0b010 ) = 2,
    B0010 DEPRECATED( 0b0010 ) = 2,
    B00010 DEPRECATED( 0b00010 ) = 2,
    B000010 DEPRECATED( 0b000010 ) = 2,
    B0000010 DEPRECATED( 0b0000010 ) = 2,
    B00000010 DEPRECATED( 0b00000010 ) = 2,
    B11 DEPRECATED( 0b11 ) = 3,
    B011 DEPRECATED( 0b011 ) = 3,
    B0011 DEPRECATED( 0b0011 ) = 3,
    B00011 DEPRECATED( 0b00011 ) = 3,
    B000011 DEPRECATED( 0b000011 ) = 3,
    B0000011 DEPRECATED( 0b0000011 ) = 3,
    B00000011 DEPRECATED( 0b00000011 ) = 3,
    B100 DEPRECATED( 0b100 ) = 4,
    B0100 DEPRECATED( 0b0100 ) = 4,
    B00100 DEPRECATED( 0b00100 ) = 4,
    B000100 DEPRECATED( 0b000100 ) = 4,
    B0000100 DEPRECATED( 0b0000100 ) = 4,
    B00000100 DEPRECATED( 0b00000100 ) = 4,
    B101 DEPRECATED( 0b101 ) = 5,
    B0101 DEPRECATED( 0b0101 ) = 5,
    B00101 DEPRECATED( 0b00101 ) = 5,
    B000101 DEPRECATED( 0b000101 ) = 5,
    B0000101 DEPRECATED( 0b0000101 ) = 5,
    B00000101 DEPRECATED( 0b00000101 ) = 5,
    B110 DEPRECATED( 0b110 ) = 6,
    B0110 DEPRECATED( 0b0110 ) = 6,
    B00110 DEPRECATED( 0b00110 ) = 6,
    B000110 DEPRECATED( 0b000110 ) = 6,
    B0000110 DEPRECATED( 0b0000110 ) = 6,
    B00000110 DEPRECATED( 0b00000110 ) = 6,
    B111 DEPRECATED( 0b111 ) = 7,
    B0111 DEPRECATED( 0b0111 ) = 7,
    B00111 DEPRECATED( 0b00111 ) = 7,
    B000111 DEPRECATED( 0b000111 ) = 7,
    B0000111 DEPRECATED( 0b0000111 ) = 7,
    B00000111 DEPRECATED( 0b00000111 ) = 7,
    B1000 DEPRECATED( 0b1000 ) = 8,
    B01000 DEPRECATED( 0b01000 ) = 8,
    B001000 DEPRECATED( 0b001000 ) = 8,
    B0001000 DEPRECATED( 0b0001000 ) = 8,
    B00001000 DEPRECATED( 0b00001000 ) = 8,
    B1001 DEPRECATED( 0b1001 ) = 9,
    B01001 DEPRECATED( 0b01001 ) = 9,
    B001001 DEPRECATED( 0b001001 ) = 9,
    B0001001 DEPRECATED( 0b0001001 ) = 9,
    B00001001 DEPRECATED( 0b00001001 ) = 9,
    B1010 DEPRECATED( 0b1010 ) = 10,
    B01010 DEPRECATED( 0b01010 ) = 10,
    B001010 DEPRECATED( 0b001010 ) = 10,
    B0001010 DEPRECATED( 0b0001010 ) = 10,
    B00001010 DEPRECATED( 0b00001010 ) = 10,
    B1011 DEPRECATED( 0b1011 ) = 11,
    B01011 DEPRECATED( 0b01011 ) = 11,
    B001011 DEPRECATED( 0b001011 ) = 11,
    B0001011 DEPRECATED( 0b0001011 ) = 11,
    B00001011 DEPRECATED( 0b00001011 ) = 11,
    B1100 DEPRECATED( 0b1100 ) = 12,
    B01100 DEPRECATED( 0b01100 ) = 12,
    B001100 DEPRECATED( 0b001100 ) = 12,
    B0001100 DEPRECATED( 0b0001100 ) = 12,
    B00001100 DEPRECATED( 0b00001100 ) = 12,
    B1101 DEPRECATED( 0b1101 ) = 13,
    B01101 DEPRECATED( 0b01101 ) = 13,
    B001101 DEPRECATED( 0b001101 ) = 13,
    B0001101 DEPRECATED( 0b0001101 ) = 13,
    B00001101 DEPRECATED( 0b00001101 ) = 13,
    B1110 DEPRECATED( 0b1110 ) = 14,
    B01110 DEPRECATED( 0b01110 ) = 14,
    B001110 DEPRECATED( 0b001110 ) = 14,
    B0001110 DEPRECATED( 0b0001110 ) = 14,
    B00001110 DEPRECATED( 0b00001110 ) = 14,
    B1111 DEPRECATED( 0b1111 ) = 15,
    B01111 DEPRECATED( 0b01111 ) = 15,
    B001111 DEPRECATED( 0b001111 ) = 15,
    B0001111 DEPRECATED( 0b0001111 ) = 15,
    B00001111 DEPRECATED( 0b00001111 ) = 15,
    B10000 DEPRECATED( 0b10000 ) = 16,
    B010000 DEPRECATED( 0b010000 ) = 16,
    B0010000 DEPRECATED( 0b0010000 ) = 16,
    B00010000 DEPRECATED( 0b00010000 ) = 16,
    B10001 DEPRECATED( 0b10001 ) = 17,
    B010001 DEPRECATED( 0b010001 ) = 17,
    B0010001 DEPRECATED( 0b0010001 ) = 17,
    B00010001 DEPRECATED( 0b00010001 ) = 17,
    B10010 DEPRECATED( 0b10010 ) = 18,
    B010010 DEPRECATED( 0b010010 ) = 18,
    B0010010 DEPRECATED( 0b0010010 ) = 18,
    B00010010 DEPRECATED( 0b00010010 ) = 18,
    B10011 DEPRECATED( 0b10011 ) = 19,
    B010011 DEPRECATED( 0b010011 ) = 19,
    B0010011 DEPRECATED( 0b0010011 ) = 19,
    B00010011 DEPRECATED( 0b00010011 ) = 19,
    B10100 DEPRECATED( 0b10100 ) = 20,
    B010100 DEPRECATED( 0b010100 ) = 20,
    B0010100 DEPRECATED( 0b0010100 ) = 20,
    B00010100 DEPRECATED( 0b00010100 ) = 20,
    B10101 DEPRECATED( 0b10101 ) = 21,
    B010101 DEPRECATED( 0b010101 ) = 21,
    B0010101 DEPRECATED( 0b0010101 ) = 21,
    B00010101 DEPRECATED( 0b00010101 ) = 21,
    B10110 DEPRECATED( 0b10110 ) = 22,
    B010110 DEPRECATED( 0b010110 ) = 22,
    B0010110 DEPRECATED( 0b0010110 ) = 22,
    B00010110 DEPRECATED( 0b00010110 ) = 22,
    B10111 DEPRECATED( 0b10111 ) = 23,
    B010111 DEPRECATED( 0b010111 ) = 23,
    B0010111 DEPRECATED( 0b0010111 ) = 23,
    B00010111 DEPRECATED( 0b00010111 ) = 23,
    B11000 DEPRECATED( 0b11000 ) = 24,
    B011000 DEPRECATED( 0b011000 ) = 24,
    B0011000 DEPRECATED( 0b0011000 ) = 24,
    B00011000 DEPRECATED( 0b00011000 ) = 24,
    B11001 DEPRECATED( 0b11001 ) = 25,
    B011001 DEPRECATED( 0b011001 ) = 25,
    B0011001 DEPRECATED( 0b0011001 ) = 25,
    B00011001 DEPRECATED( 0b00011001 ) = 25,
    B11010 DEPRECATED( 0b11010 ) = 26,
    B011010 DEPRECATED( 0b011010 ) = 26,
    B0011010 DEPRECATED( 0b0011010 ) = 26,
    B00011010 DEPRECATED( 0b00011010 ) = 26,
    B11011 DEPRECATED( 0b11011 ) = 27,
    B011011 DEPRECATED( 0b011011 ) = 27,
    B0011011 DEPRECATED( 0b0011011 ) = 27,
    B00011011 DEPRECATED( 0b00011011 ) = 27,
    B11100 DEPRECATED( 0b11100 ) = 28,
    B011100 DEPRECATED( 0b011100 ) = 28,
    B0011100 DEPRECATED( 0b0011100 ) = 28,
    B00011100 DEPRECATED( 0b00011100 ) = 28,
    B11101 DEPRECATED( 0b11101 ) = 29,
    B011101 DEPRECATED( 0b011101 ) = 29,
    B0011101 DEPRECATED( 0b0011101 ) = 29,
    B00011101 DEPRECATED( 0b00011101 ) = 29,
    B11110 DEPRECATED( 0b11110 ) = 30,
    B011110 DEPRECATED( 0b011110 ) = 30,
    B0011110 DEPRECATED( 0b0011110 ) = 30,
    B00011110 DEPRECATED( 0b00011110 ) = 30,
    B11111 DEPRECATED( 0b11111 ) = 31,
    B011111 DEPRECATED( 0b011111 ) = 31,
    B0011111 DEPRECATED( 0b0011111 ) = 31,
    B00011111 DEPRECATED( 0b00011111 ) = 31,
    B100000 DEPRECATED( 0b100000 ) = 32,
    B0100000 DEPRECATED( 0b0100000 ) = 32,
    B00100000 DEPRECATED( 0b00100000 ) = 32,
    B100001 DEPRECATED( 0b100001 ) = 33,
    B0100001 DEPRECATED( 0b0100001 ) = 33,
    B00100001 DEPRECATED( 0b00100001 ) = 33,
    B100010 DEPRECATED( 0b100010 ) = 34,
    B0100010 DEPRECATED( 0b0100010 ) = 34,
    B00100010 DEPRECATED( 0b00100010 ) = 34,
    B100011 DEPRECATED( 0b100011 ) = 35,
    B0100011 DEPRECATED( 0b0100011 ) = 35,
    B00100011 DEPRECATED( 0b00100011 ) = 35,
    B100100 DEPRECATED( 0b100100 ) = 36,
    B0100100 DEPRECATED( 0b0100100 ) = 36,
    B00100100 DEPRECATED( 0b00100100 ) = 36,
    B100101 DEPRECATED( 0b100101 ) = 37,
    B0100101 DEPRECATED( 0b0100101 ) = 37,
    B00100101 DEPRECATED( 0b00100101 ) = 37,
    B100110 DEPRECATED( 0b100110 ) = 38,
    B0100110 DEPRECATED( 0b0100110 ) = 38,
    B00100110 DEPRECATED( 0b00100110 ) = 38,
    B100111 DEPRECATED( 0b100111 ) = 39,
    B0100111 DEPRECATED( 0b0100111 ) = 39,
    B00100111 DEPRECATED( 0b00100111 ) = 39,
    B101000 DEPRECATED( 0b101000 ) = 40,
    B0101000 DEPRECATED( 0b0101000 ) = 40,
    B00101000 DEPRECATED( 0b00101000 ) = 40,
    B101001 DEPRECATED( 0b101001 ) = 41,
    B0101001 DEPRECATED( 0b0101001 ) = 41,
    B00101001 DEPRECATED( 0b00101001 ) = 41,
    B101010 DEPRECATED( 0b101010 ) = 42,
    B0101010 DEPRECATED( 0b0101010 ) = 42,
    B00101010 DEPRECATED( 0b00101010 ) = 42,
    B101011 DEPRECATED( 0b101011 ) = 43,
    B0101011 DEPRECATED( 0b0101011 ) = 43,
    B00101011 DEPRECATED( 0b00101011 ) = 43,
    B101100 DEPRECATED( 0b101100 ) = 44,
    B0101100 DEPRECATED( 0b0101100 ) = 44,
    B00101100 DEPRECATED( 0b00101100 ) = 44,
    B101101 DEPRECATED( 0b101101 ) = 45,
    B0101101 DEPRECATED( 0b0101101 ) = 45,
    B00101101 DEPRECATED( 0b00101101 ) = 45,
    B101110 DEPRECATED( 0b101110 ) = 46,
    B0101110 DEPRECATED( 0b0101110 ) = 46,
    B00101110 DEPRECATED( 0b00101110 ) = 46,
    B101111 DEPRECATED( 0b101111 ) = 47,
    B0101111 DEPRECATED( 0b0101111 ) = 47,
    B00101111 DEPRECATED( 0b00101111 ) = 47,
    B110000 DEPRECATED( 0b110000 ) = 48,
    B0110000 DEPRECATED( 0b0110000 ) = 48,
    B00110000 DEPRECATED( 0b00110000 ) = 48,
    B110001 DEPRECATED( 0b110001 ) = 49,
    B0110001 DEPRECATED( 0b0110001 ) = 49,
    B00110001 DEPRECATED( 0b00110001 ) = 49,
    B110010 DEPRECATED( 0b110010 ) = 50,
    B0110010 DEPRECATED( 0b0110010 ) = 50,
    B00110010 DEPRECATED( 0b00110010 ) = 50,
    B110011 DEPRECATED( 0b110011 ) = 51,
    B0110011 DEPRECATED( 0b0110011 ) = 51,
    B00110011 DEPRECATED( 0b00110011 ) = 51,
    B110100 DEPRECATED( 0b110100 ) = 52,
    B0110100 DEPRECATED( 0b0110100 ) = 52,
    B00110100 DEPRECATED( 0b00110100 ) = 52,
    B110101 DEPRECATED( 0b110101 ) = 53,
    B0110101 DEPRECATED( 0b0110101 ) = 53,
    B00110101 DEPRECATED( 0b00110101 ) = 53,
    B110110 DEPRECATED( 0b110110 ) = 54,
    B0110110 DEPRECATED( 0b0110110 ) = 54,
    B00110110 DEPRECATED( 0b00110110 ) = 54,
    B110111 DEPRECATED( 0b110111 ) = 55,
    B0110111 DEPRECATED( 0b0110111 ) = 55,
    B00110111 DEPRECATED( 0b00110111 ) = 55,
    B111000 DEPRECATED( 0b111000 ) = 56,
    B0111000 DEPRECATED( 0b0111000 ) = 56,
    B00111000 DEPRECATED( 0b00111000 ) = 56,
    B111001 DEPRECATED( 0b111001 ) = 57,
    B0111001 DEPRECATED( 0b0111001 ) = 57,
    B00111001 DEPRECATED( 0b00111001 ) = 57,
    B111010 DEPRECATED( 0b111010 ) = 58,
    B0111010 DEPRECATED( 0b0111010 ) = 58,
    B00111010 DEPRECATED( 0b00111010 ) = 58,
    B111011 DEPRECATED( 0b111011 ) = 59,
    B0111011 DEPRECATED( 0b0111011 ) = 59,
    B00111011 DEPRECATED( 0b00111011 ) = 59,
    B111100 DEPRECATED( 0b111100 ) = 60,
    B0111100 DEPRECATED( 0b0111100 ) = 60,
    B00111100 DEPRECATED( 0b00111100 ) = 60,
    B111101 DEPRECATED( 0b111101 ) = 61,
    B0111101 DEPRECATED( 0b0111101 ) = 61,
    B00111101 DEPRECATED( 0b00111101 ) = 61,
    B111110 DEPRECATED( 0b111110 ) = 62,
    B0111110 DEPRECATED( 0b0111110 ) = 62,
    B00111110 DEPRECATED( 0b00111110 ) = 62,
    B111111 DEPRECATED( 0b111111 ) = 63,
    B0111111 DEPRECATED( 0b0111111 ) = 63,
    B00111111 DEPRECATED( 0b00111111 ) = 63,
    B1000000 DEPRECATED( 0b1000000 ) = 64,
    B01000000 DEPRECATED( 0b01000000 ) = 64,
    B1000001 DEPRECATED( 0b1000001 ) = 65,
    B01000001 DEPRECATED( 0b01000001 ) = 65,
    B1000010 DEPRECATED( 0b1000010 ) = 66,
    B01000010 DEPRECATED( 0b01000010 ) = 66,
    B1000011 DEPRECATED( 0b1000011 ) = 67,
    B01000011 DEPRECATED( 0b01000011 ) = 67,
    B1000100 DEPRECATED( 0b1000100 ) = 68,
    B01000100 DEPRECATED( 0b01000100 ) = 68,
    B1000101 DEPRECATED( 0b1000101 ) = 69,
    B01000101 DEPRECATED( 0b01000101 ) = 69,
    B1000110 DEPRECATED( 0b1000110 ) = 70,
    B01000110 DEPRECATED( 0b01000110 ) = 70,
    B1000111 DEPRECATED( 0b1000111 ) = 71,
    B01000111 DEPRECATED( 0b01000111 ) = 71,
    B1001000 DEPRECATED( 0b1001000 ) = 72,
    B01001000 DEPRECATED( 0b01001000 ) = 72,
    B1001001 DEPRECATED( 0b1001001 ) = 73,
    B01001001 DEPRECATED( 0b01001001 ) = 73,
    B1001010 DEPRECATED( 0b1001010 ) = 74,
    B01001010 DEPRECATED( 0b01001010 ) = 74,
    B1001011 DEPRECATED( 0b1001011 ) = 75,
    B01001011 DEPRECATED( 0b01001011 ) = 75,
    B1001100 DEPRECATED( 0b1001100 ) = 76,
    B01001100 DEPRECATED( 0b01001100 ) = 76,
    B1001101 DEPRECATED( 0b1001101 ) = 77,
    B01001101 DEPRECATED( 0b01001101 ) = 77,
    B1001110 DEPRECATED( 0b1001110 ) = 78,
    B01001110 DEPRECATED( 0b01001110 ) = 78,
    B1001111 DEPRECATED( 0b1001111 ) = 79,
    B01001111 DEPRECATED( 0b01001111 ) = 79,
    B1010000 DEPRECATED( 0b1010000 ) = 80,
    B01010000 DEPRECATED( 0b01010000 ) = 80,
    B1010001 DEPRECATED( 0b1010001 ) = 81,
    B01010001 DEPRECATED( 0b01010001 ) = 81,
    B1010010 DEPRECATED( 0b1010010 ) = 82,
    B01010010 DEPRECATED( 0b01010010 ) = 82,
    B1010011 DEPRECATED( 0b1010011 ) = 83,
    B01010011 DEPRECATED( 0b01010011 ) = 83,
    B1010100 DEPRECATED( 0b1010100 ) = 84,
    B01010100 DEPRECATED( 0b01010100 ) = 84,
    B1010101 DEPRECATED( 0b1010101 ) = 85,
    B01010101 DEPRECATED( 0b01010101 ) = 85,
    B1010110 DEPRECATED( 0b1010110 ) = 86,
    B01010110 DEPRECATED( 0b01010110 ) = 86,
    B1010111 DEPRECATED( 0b1010111 ) = 87,
    B01010111 DEPRECATED( 0b01010111 ) = 87,
    B1011000 DEPRECATED( 0b1011000 ) = 88,
    B01011000 DEPRECATED( 0b01011000 ) = 88,
    B1011001 DEPRECATED( 0b1011001 ) = 89,
    B01011001 DEPRECATED( 0b01011001 ) = 89,
    B1011010 DEPRECATED( 0b1011010 ) = 90,
    B01011010 DEPRECATED( 0b01011010 ) = 90,
    B1011011 DEPRECATED( 0b1011011 ) = 91,
    B01011011 DEPRECATED( 0b01011011 ) = 91,
    B1011100 DEPRECATED( 0b1011100 ) = 92,
    B01011100 DEPRECATED( 0b01011100 ) = 92,
    B1011101 DEPRECATED( 0b1011101 ) = 93,
    B01011101 DEPRECATED( 0b01011101 ) = 93,
    B1011110 DEPRECATED( 0b1011110 ) = 94,
    B01011110 DEPRECATED( 0b01011110 ) = 94,
    B1011111 DEPRECATED( 0b1011111 ) = 95,
    B01011111 DEPRECATED( 0b01011111 ) = 95,
    B1100000 DEPRECATED( 0b1100000 ) = 96,
    B01100000 DEPRECATED( 0b01100000 ) = 96,
    B1100001 DEPRECATED( 0b1100001 ) = 97,
    B01100001 DEPRECATED( 0b01100001 ) = 97,
    B1100010 DEPRECATED( 0b1100010 ) = 98,
    B01100010 DEPRECATED( 0b01100010 ) = 98,
    B1100011 DEPRECATED( 0b1100011 ) = 99,
    B01100011 DEPRECATED( 0b01100011 ) = 99,
    B1100100 DEPRECATED( 0b1100100 ) = 100,
    B01100100 DEPRECATED( 0b01100100 ) = 100,
    B1100101 DEPRECATED( 0b1100101 ) = 101,
    B01100101 DEPRECATED( 0b01100101 ) = 101,
    B1100110 DEPRECATED( 0b1100110 ) = 102,
    B01100110 DEPRECATED( 0b01100110 ) = 102,
    B1100111 DEPRECATED( 0b1100111 ) = 103,
    B01100111 DEPRECATED( 0b01100111 ) = 103,
    B1101000 DEPRECATED( 0b1101000 ) = 104,
    B01101000 DEPRECATED( 0b01101000 ) = 104,
    B1101001 DEPRECATED( 0b1101001 ) = 105,
    B01101001 DEPRECATED( 0b01101001 ) = 105,
    B1101010 DEPRECATED( 0b1101010 ) = 106,
    B01101010 DEPRECATED( 0b01101010 ) = 106,
    B1101011 DEPRECATED( 0b1101011 ) = 107,
    B01101011 DEPRECATED( 0b01101011 ) = 107,
    B1101100 DEPRECATED( 0b1101100 ) = 108,
    B01101100 DEPRECATED( 0b01101100 ) = 108,
    B1101101 DEPRECATED( 0b1101101 ) = 109,
    B01101101 DEPRECATED( 0b01101101 ) = 109,
    B1101110 DEPRECATED( 0b1101110 ) = 110,
    B01101110 DEPRECATED( 0b01101110 ) = 110,
    B1101111 DEPRECATED( 0b1101111 ) = 111,
    B01101111 DEPRECATED( 0b01101111 ) = 111,
    B1110000 DEPRECATED( 0b1110000 ) = 112,
    B01110000 DEPRECATED( 0b01110000 ) = 112,
    B1110001 DEPRECATED( 0b1110001 ) = 113,
    B01110001 DEPRECATED( 0b01110001 ) = 113,
    B1110010 DEPRECATED( 0b1110010 ) = 114,
    B01110010 DEPRECATED( 0b01110010 ) = 114,
    B1110011 DEPRECATED( 0b1110011 ) = 115,
    B01110011 DEPRECATED( 0b01110011 ) = 115,
    B1110100 DEPRECATED( 0b1110100 ) = 116,
    B01110100 DEPRECATED( 0b01110100 ) = 116,
    B1110101 DEPRECATED( 0b1110101 ) = 117,
    B01110101 DEPRECATED( 0b01110101 ) = 117,
    B1110110 DEPRECATED( 0b1110110 ) = 118,
    B01110110 DEPRECATED( 0b01110110 ) = 118,
    B1110111 DEPRECATED( 0b1110111 ) = 119,
    B01110111 DEPRECATED( 0b01110111 ) = 119,
    B1111000 DEPRECATED( 0b1111000 ) = 120,
    B01111000 DEPRECATED( 0b01111000 ) = 120,
    B1111001 DEPRECATED( 0b1111001 ) = 121,
    B01111001 DEPRECATED( 0b01111001 ) = 121,
    B1111010 DEPRECATED( 0b1111010 ) = 122,
    B01111010 DEPRECATED( 0b01111010 ) = 122,
    B1111011 DEPRECATED( 0b1111011 ) = 123,
    B01111011 DEPRECATED( 0b01111011 ) = 123,
    B1111100 DEPRECATED( 0b1111100 ) = 124,
    B01111100 DEPRECATED( 0b01111100 ) = 124,
    B1111101 DEPRECATED( 0b1111101 ) = 125,
    B01111101 DEPRECATED( 0b01111101 ) = 125,
    B1111110 DEPRECATED( 0b1111110 ) = 126,
    B01111110 DEPRECATED( 0b01111110 ) = 126,
    B1111111 DEPRECATED( 0b1111111 ) = 127,
    B01111111 DEPRECATED( 0b01111111 ) = 127,
    B10000000 DEPRECATED( 0b10000000 ) = 128,
    B10000001 DEPRECATED( 0b10000001 ) = 129,
    B10000010 DEPRECATED( 0b10000010 ) = 130,
    B10000011 DEPRECATED( 0b10000011 ) = 131,
    B10000100 DEPRECATED( 0b10000100 ) = 132,
    B10000101 DEPRECATED( 0b10000101 ) = 133,
    B10000110 DEPRECATED( 0b10000110 ) = 134,
    B10000111 DEPRECATED( 0b10000111 ) = 135,
    B10001000 DEPRECATED( 0b10001000 ) = 136,
    B10001001 DEPRECATED( 0b10001001 ) = 137,
    B10001010 DEPRECATED( 0b10001010 ) = 138,
    B10001011 DEPRECATED( 0b10001011 ) = 139,
    B10001100 DEPRECATED( 0b10001100 ) = 140,
    B10001101 DEPRECATED( 0b10001101 ) = 141,
    B10001110 DEPRECATED( 0b10001110 ) = 142,
    B10001111 DEPRECATED( 0b10001111 ) = 143,
    B10010000 DEPRECATED( 0b10010000 ) = 144,
    B10010001 DEPRECATED( 0b10010001 ) = 145,
    B10010010 DEPRECATED( 0b10010010 ) = 146,
    B10010011 DEPRECATED( 0b10010011 ) = 147,
    B10010100 DEPRECATED( 0b10010100 ) = 148,
    B10010101 DEPRECATED( 0b10010101 ) = 149,
    B10010110 DEPRECATED( 0b10010110 ) = 150,
    B10010111 DEPRECATED( 0b10010111 ) = 151,
    B10011000 DEPRECATED( 0b10011000 ) = 152,
    B10011001 DEPRECATED( 0b10011001 ) = 153,
    B10011010 DEPRECATED( 0b10011010 ) = 154,
    B10011011 DEPRECATED( 0b10011011 ) = 155,
    B10011100 DEPRECATED( 0b10011100 ) = 156,
    B10011101 DEPRECATED( 0b10011101 ) = 157,
    B10011110 DEPRECATED( 0b10011110 ) = 158,
    B10011111 DEPRECATED( 0b10011111 ) = 159,
    B10100000 DEPRECATED( 0b10100000 ) = 160,
    B10100001 DEPRECATED( 0b10100001 ) = 161,
    B10100010 DEPRECATED( 0b10100010 ) = 162,
    B10100011 DEPRECATED( 0b10100011 ) = 163,
    B10100100 DEPRECATED( 0b10100100 ) = 164,
    B10100101 DEPRECATED( 0b10100101 ) = 165,
    B10100110 DEPRECATED( 0b10100110 ) = 166,
    B10100111 DEPRECATED( 0b10100111 ) = 167,
    B10101000 DEPRECATED( 0b10101000 ) = 168,
    B10101001 DEPRECATED( 0b10101001 ) = 169,
    B10101010 DEPRECATED( 0b10101010 ) = 170,
    B10101011 DEPRECATED( 0b10101011 ) = 171,
    B10101100 DEPRECATED( 0b10101100 ) = 172,
    B10101101 DEPRECATED( 0b10101101 ) = 173,
    B10101110 DEPRECATED( 0b10101110 ) = 174,
    B10101111 DEPRECATED( 0b10101111 ) = 175,
    B10110000 DEPRECATED( 0b10110000 ) = 176,
    B10110001 DEPRECATED( 0b10110001 ) = 177,
    B10110010 DEPRECATED( 0b10110010 ) = 178,
    B10110011 DEPRECATED( 0b10110011 ) = 179,
    B10110100 DEPRECATED( 0b10110100 ) = 180,
    B10110101 DEPRECATED( 0b10110101 ) = 181,
    B10110110 DEPRECATED( 0b10110110 ) = 182,
    B10110111 DEPRECATED( 0b10110111 ) = 183,
    B10111000 DEPRECATED( 0b10111000 ) = 184,
    B10111001 DEPRECATED( 0b10111001 ) = 185,
    B10111010 DEPRECATED( 0b10111010 ) = 186,
    B10111011 DEPRECATED( 0b10111011 ) = 187,
    B10111100 DEPRECATED( 0b10111100 ) = 188,
    B10111101 DEPRECATED( 0b10111101 ) = 189,
    B10111110 DEPRECATED( 0b10111110 ) = 190,
    B10111111 DEPRECATED( 0b10111111 ) = 191,
    B11000000 DEPRECATED( 0b11000000 ) = 192,
    B11000001 DEPRECATED( 0b11000001 ) = 193,
    B11000010 DEPRECATED( 0b11000010 ) = 194,
    B11000011 DEPRECATED( 0b11000011 ) = 195,
    B11000100 DEPRECATED( 0b11000100 ) = 196,
    B11000101 DEPRECATED( 0b11000101 ) = 197,
    B11000110 DEPRECATED( 0b11000110 ) = 198,
    B11000111 DEPRECATED( 0b11000111 ) = 199,
    B11001000 DEPRECATED( 0b11001000 ) = 200,
    B11001001 DEPRECATED( 0b11001001 ) = 201,
    B11001010 DEPRECATED( 0b11001010 ) = 202,
    B11001011 DEPRECATED( 0b11001011 ) = 203,
    B11001100 DEPRECATED( 0b11001100 ) = 204,
    B11001101 DEPRECATED( 0b11001101 ) = 205,
    B11001110 DEPRECATED( 0b11001110 ) = 206,
    B11001111 DEPRECATED( 0b11001111 ) = 207,
    B11010000 DEPRECATED( 0b11010000 ) = 208,
    B11010001 DEPRECATED( 0b11010001 ) = 209,
    B11010010 DEPRECATED( 0b11010010 ) = 210,
    B11010011 DEPRECATED( 0b11010011 ) = 211,
    B11010100 DEPRECATED( 0b11010100 ) = 212,
    B11010101 DEPRECATED( 0b11010101 ) = 213,
    B11010110 DEPRECATED( 0b11010110 ) = 214,
    B11010111 DEPRECATED( 0b11010111 ) = 215,
    B11011000 DEPRECATED( 0b11011000 ) = 216,
    B11011001 DEPRECATED( 0b11011001 ) = 217,
    B11011010 DEPRECATED( 0b11011010 ) = 218,
    B11011011 DEPRECATED( 0b11011011 ) = 219,
    B11011100 DEPRECATED( 0b11011100 ) = 220,
    B11011101 DEPRECATED( 0b11011101 ) = 221,
    B11011110 DEPRECATED( 0b11011110 ) = 222,
    B11011111 DEPRECATED( 0b11011111 ) = 223,
    B11100000 DEPRECATED( 0b11100000 ) = 224,
    B11100001 DEPRECATED( 0b11100001 ) = 225,
    B11100010 DEPRECATED( 0b11100010 ) = 226,
    B11100011 DEPRECATED( 0b11100011 ) = 227,
    B11100100 DEPRECATED( 0b11100100 ) = 228,
    B11100101 DEPRECATED( 0b11100101 ) = 229,
    B11100110 DEPRECATED( 0b11100110 ) = 230,
    B11100111 DEPRECATED( 0b11100111 ) = 231,
    B11101000 DEPRECATED( 0b11101000 ) = 232,
    B11101001 DEPRECATED( 0b11101001 ) = 233,
    B11101010 DEPRECATED( 0b11101010 ) = 234,
    B11101011 DEPRECATED( 0b11101011 ) = 235,
    B11101100 DEPRECATED( 0b11101100 ) = 236,
    B11101101 DEPRECATED( 0b11101101 ) = 237,
    B11101110 DEPRECATED( 0b11101110 ) = 238,
    B11101111 DEPRECATED( 0b11101111 ) = 239,
    B11110000 DEPRECATED( 0b11110000 ) = 240,
    B11110001 DEPRECATED( 0b11110001 ) = 241,
    B11110010 DEPRECATED( 0b11110010 ) = 242,
    B11110011 DEPRECATED( 0b11110011 ) = 243,
    B11110100 DEPRECATED( 0b11110100 ) = 244,
    B11110101 DEPRECATED( 0b11110101 ) = 245,
    B11110110 DEPRECATED( 0b11110110 ) = 246,
    B11110111 DEPRECATED( 0b11110111 ) = 247,
    B11111000 DEPRECATED( 0b11111000 ) = 248,
    B11111001 DEPRECATED( 0b11111001 ) = 249,
    B11111010 DEPRECATED( 0b11111010 ) = 250,
    B11111011 DEPRECATED( 0b11111011 ) = 251,
    B11111100 DEPRECATED( 0b11111100 ) = 252,
    B11111101 DEPRECATED( 0b11111101 ) = 253,
    B11111110 DEPRECATED( 0b11111110 ) = 254,
    B11111111 DEPRECATED( 0b11111111 ) = 255 }; } // namespace mjz_ard
#undef DEPRECATED

#endif

#ifndef Arduino

#include <stdbool.h>
#include <stdint.h>

namespace mjz_com {
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL 0x0
#define DISPLAY 0x1

  #ifndef constrain
#define constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
  #endif

#ifndef radians
  #define radians(deg) ((deg)*DEG_TO_RAD)
#endif

#ifndef degrees
  #define degrees(rad) ((rad)*RAD_TO_DEG)
#endif

#ifndef sq
  #define sq(x) ((x) * (x))
#endif

  typedef void ( *voidFuncPtr )( void );
  typedef void ( *voidFuncPtrParam )( void * );

  // interrupts() / noInterrupts() must be defined by the core

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) \
  ((bitvalue) ? bitSet((value), (bit)) : bitClear((value), (bit)))

#ifndef bit
  #define bit(b) (1UL << (b))
#endif

  /* TODO: request for removal */
  typedef bool boolean;
  // typedef uint8_t uint8_t;
  typedef uint16_t word;

  void init( void );
  void initVariant( void );

#ifndef HOST
  int atexit( void ( *func )() ) __attribute__( ( weak ) );
#endif
  int main() __attribute__( ( weak ) );

#ifdef EXTENDED_PIN_MODE
  // Platforms who want to declare more than 256 pins need to define
  // EXTENDED_PIN_MODE globally
  typedef uint32_t pin_size_t;
#else
  typedef uint8_t pin_size_t;
#endif

  #ifndef Arduino

#define word(...) makeWord(__VA_ARGS__)

  unsigned long pulseIn( uint8_t pin, uint8_t state,
                         unsigned long timeout = 1000000L );
  unsigned long pulseInLong( uint8_t pin, uint8_t state,
                             unsigned long timeout = 1000000L );

  void tone( uint8_t _pin, unsigned int frequency, unsigned long duration = 0 );
  void noTone( uint8_t _pin );

  // WMath prototypes
  long random( long );
  long random( long, long );
  void randomSeed( unsigned long );
  long map( long, long, long, long, long ); }
  #endif  // __cplusplus

#else
/*
  #include <ArduinoJson.h>
  #include <HTTPClient.h>
  #include <HTTPUpdate.h>
  #include <LittleFS.h>
  #include <Update.h>
  #include <WiFi.h>
  #include <WiFiMulti.h>

  #include "Arduino.h"
  #include "FS.h"
  #include "HardwareSerial.h"
  #include "WString.h"
*/
#endif
/*
  WCharacter.h - Character utility functions for Wiring & mjz_ard
  Copyright (c) 2010 Hernando Barragan. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License,or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not,write to the Free Software
  Foundation,Inc.,51 Franklin St,Fifth Floor,Boston,MA 02110-1301 USA
*/

#ifndef Character_h
#define Character_h

#include <ctype.h>

namespace mjz_ard {

  // WCharacter.h prototypes
  inline bool isAlphaNumeric( int c ) __attribute__( ( always_inline ) );
  inline bool isAlpha( int c ) __attribute__( ( always_inline ) );
  inline bool isAscii( int c ) __attribute__( ( always_inline ) );
  inline bool isWhitespace( int c ) __attribute__( ( always_inline ) );
  inline bool isControl( int c ) __attribute__( ( always_inline ) );
  inline bool isDigit( int c ) __attribute__( ( always_inline ) );
  inline bool isGraph( int c ) __attribute__( ( always_inline ) );
  inline bool isLowerCase( int c ) __attribute__( ( always_inline ) );
  inline bool isPrintable( int c ) __attribute__( ( always_inline ) );
  inline bool isPunct( int c ) __attribute__( ( always_inline ) );
  inline bool isSpace( int c ) __attribute__( ( always_inline ) );
  inline bool isUpperCase( int c ) __attribute__( ( always_inline ) );
  inline bool isHexadecimalDigit( int c ) __attribute__( ( always_inline ) );
  inline int toAscii( int c ) __attribute__( ( always_inline ) );
  inline int toLowerCase( int c ) __attribute__( ( always_inline ) );
  inline int toUpperCase( int c ) __attribute__( ( always_inline ) );

  // Checks for an alphanumeric character.
  // It is equivalent to (isalpha(c) || isdigit(c)).
  inline bool isAlphaNumeric( int c ) { return ( isalnum( c ) == 0 ? false : true ); }

  // Checks for an alphabetic character.
  // It is equivalent to (isupper(c) || islower(c)).
  inline bool isAlpha( int c ) { return ( isalpha( c ) == 0 ? false : true ); }

  // Checks whether c is a 7-bit unsigned char value
  // that fits into the ASCII character set.
  inline bool isAscii( int c ) { return ( ( c & ~0x7f ) != 0 ? false : true ); }

  // Checks for a blank character,that is,a space or a tab.
  inline bool isWhitespace( int c ) { return ( isblank( c ) == 0 ? false : true ); }

  // Checks for a control character.
  inline bool isControl( int c ) { return ( iscntrl( c ) == 0 ? false : true ); }

  // Checks for a digit (0 through 9).
  inline bool isDigit( int c ) { return ( isdigit( c ) == 0 ? false : true ); }

  // Checks for any printable character except space.
  inline bool isGraph( int c ) { return ( isgraph( c ) == 0 ? false : true ); }

  // Checks for a lower-case character.
  inline bool isLowerCase( int c ) { return ( islower( c ) == 0 ? false : true ); }

  // Checks for any printable character including space.
  inline bool isPrintable( int c ) { return ( isprint( c ) == 0 ? false : true ); }

  // Checks for any printable character which is not a space
  // or an alphanumeric character.
  inline bool isPunct( int c ) { return ( ispunct( c ) == 0 ? false : true ); }

  // Checks for white-space characters. For the avr-libc library,
  // these are: space,formfeed ('\f'),newline ('\n'),carriage
  // return ('\r'),horizontal tab ('\t'),and vertical tab ('\v').
  inline bool isSpace( int c ) { return ( isspace( c ) == 0 ? false : true ); }

  // Checks for an uppercase letter.
  inline bool isUpperCase( int c ) { return ( isupper( c ) == 0 ? false : true ); }

  // Checks for a hexadecimal digits,i.e. one of 0 1 2 3 4 5 6 7
  // 8 9 a b c d e f A B C D E F.
  inline bool isHexadecimalDigit( int c ) {
    return ( isxdigit( c ) == 0 ? false : true ); }

  // Converts c to a 7-bit unsigned char value that fits into the
  // ASCII character set,by clearing the high-order bits.
  inline int toAscii( int c ) { return ( c & 0x7f ); }

  // Warning:
  // Many people will be unhappy if you use this function.
  // This function will convert accented letters into random
  // characters.

  // Converts the letter c to lower case,if possible.
  inline int toLowerCase( int c ) { return tolower( c ); }

  // Converts the letter c to upper case,if possible.
  inline int toUpperCase( int c ) { return toupper( c ); }

#define ard_lowByte(w) ((uint8_t)((w)&0xff))
#define ard_highByte(w) ((uint8_t)((w) >> 8))

#define ard_bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define ard_bitSet(value, bit) ((value) |= (1UL << (bit)))
#define ard_bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define ard_bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define ard_bitWrite(value, bit, bitvalue) \
  ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define ard_min(a, b) ((a) < (b) ? (a) : (b))
#define ard_max(a, b) ((a) > (b) ? (a) : (b))
#define ard_abs(x) ((x) > 0 ? (x) : -(x))
#define ard_constrain(amt, low, high) \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define ard_round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define ard_radians(deg) ((deg)*DEG_TO_RAD)
#define ard_degrees(rad) ((rad)*RAD_TO_DEG)
#define ard_sq(x) ((x) * (x))
}  // namespace mjz_ard
#endif

#ifdef Arduino
  #include "optional_otr.hpp"
#endif
