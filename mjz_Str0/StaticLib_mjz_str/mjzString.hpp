/*
String library for Wiring & Arduino
 ...mostly rewritten by Paul Stoffregen...
Copyright (c) 2009-10 Hernando Barragan. All right reserved.
Copyright 2011, Paul Stoffregen, paul@pjrc.com

This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
/*
this is a simple fast implementation/fork of arduino string and much more...
written by mjz https://github.com/Mjz86
*/

#define NumberOf(arg) ((size_t)(sizeof(arg) / sizeof(arg[0])))
#ifdef __cplusplus
#ifndef __mjz_ard_STRINGS__
#ifndef STRING_SUM_HELPER
#define STRING_SUM_HELPER

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
#include <cstdarg>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

template <typename... Type>
constexpr inline int sprintf_alt_(char *const buffer_for_print,
                                  size_t size_of_msg,
                                  const char *const forming__,
                                  Type... argumentes) {
#ifndef Arduino
  return sprintf_s(buffer_for_print, size_of_msg, forming__, argumentes...);
#else
  return sprintf(buffer_for_print, forming__, argumentes...);
#endif
}

/* a=target variable, b=bit number to act upon 0-n */
#define MJZ_logic_BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define MJZ_logic_BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define MJZ_logic_BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define MJZ_logic_BIT_CHECK(a, b) \
  (!!((a) & (1ULL << (b))))  // '!!' to make sure this returns 0 or 1

#define MJZ_logic_BIT(b) (1ULL << (b))
#define MJZ_logic_BIT_NOT(b) (~(1ULL << (b)))

#define MJZ_logic_BITMASK_SET(x, mask) ((x) |= (mask))
#define MJZ_logic_BITMASK_CLEAR(x, mask) ((x) &= (~(mask)))
#define MJZ_logic_BITMASK_FLIP(x, mask) ((x) ^= (mask))
#define MJZ_logic_BITMASK_CHECK_ALL(x, mask) (!(~(x) & (mask)))
#define MJZ_logic_BITMASK_CHECK_ANY(x, mask) ((x) & (mask))
#define min_macro_(_a, _b) (((_a) < (_b)) ? (_a) : (_b))
#define max_macro_(_a, _b) (((_a) < (_b)) ? (_b) : (_a))
#define MJZ_logic_BL_bit_to_64_bits_not_bool(B) \
  ((((uint64_t) !(B)) + (uint64_t)(-1)))
#define MJZ_logic_BL_bit_to_64_bits(B) (-((int64_t)(B)))
#define MJZ_logic_bit_to_64_bits(B) ((B) ? (uint64_t)(-1) : 0ULL)
#define get_the_absoulot_typed_name(X) ((const char *const)#X)

#ifdef Arduino
#include <sys/time.h>
#else

union my_data_randomizer_union {
  uint64_t my_data_randomizer_u;
  uint32_t my_data_randomizer;
  double my_data_randomizer_d;
};

template <typename... Type>
inline uint32_t esp_random(Type... arggs) {
  static my_data_randomizer_union my_data_randomizer_uni;
  ++my_data_randomizer_uni.my_data_randomizer_d;
  my_data_randomizer_uni.my_data_randomizer_d *=
      my_data_randomizer_uni.my_data_randomizer_u;
  --my_data_randomizer_uni.my_data_randomizer_d;
  my_data_randomizer_uni.my_data_randomizer_d /=
      my_data_randomizer_uni.my_data_randomizer_u;
  return my_data_randomizer_uni.my_data_randomizer;
}

template <class Type, class L>
auto min(const Type &a, const L &b) -> decltype((b < a) ? b : a) {
  return (b < a) ? b : a;
}

template <class Type, class L>
auto max(const Type &a, const L &b) -> decltype((b < a) ? b : a) {
  return (a < b) ? b : a;
}

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
#ifndef Binary_h
#define Binary_h

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
  B0 DEPRECATED(0b0) = 0,
  B00 DEPRECATED(0b00) = 0,
  B000 DEPRECATED(0b000) = 0,
  B0000 DEPRECATED(0b0000) = 0,
  B00000 DEPRECATED(0b00000) = 0,
  B000000 DEPRECATED(0b000000) = 0,
  B0000000 DEPRECATED(0b0000000) = 0,
  B00000000 DEPRECATED(0b00000000) = 0,
  B1 DEPRECATED(0b1) = 1,
  B01 DEPRECATED(0b01) = 1,
  B001 DEPRECATED(0b001) = 1,
  B0001 DEPRECATED(0b0001) = 1,
  B00001 DEPRECATED(0b00001) = 1,
  B000001 DEPRECATED(0b000001) = 1,
  B0000001 DEPRECATED(0b0000001) = 1,
  B00000001 DEPRECATED(0b00000001) = 1,
  B10 DEPRECATED(0b10) = 2,
  B010 DEPRECATED(0b010) = 2,
  B0010 DEPRECATED(0b0010) = 2,
  B00010 DEPRECATED(0b00010) = 2,
  B000010 DEPRECATED(0b000010) = 2,
  B0000010 DEPRECATED(0b0000010) = 2,
  B00000010 DEPRECATED(0b00000010) = 2,
  B11 DEPRECATED(0b11) = 3,
  B011 DEPRECATED(0b011) = 3,
  B0011 DEPRECATED(0b0011) = 3,
  B00011 DEPRECATED(0b00011) = 3,
  B000011 DEPRECATED(0b000011) = 3,
  B0000011 DEPRECATED(0b0000011) = 3,
  B00000011 DEPRECATED(0b00000011) = 3,
  B100 DEPRECATED(0b100) = 4,
  B0100 DEPRECATED(0b0100) = 4,
  B00100 DEPRECATED(0b00100) = 4,
  B000100 DEPRECATED(0b000100) = 4,
  B0000100 DEPRECATED(0b0000100) = 4,
  B00000100 DEPRECATED(0b00000100) = 4,
  B101 DEPRECATED(0b101) = 5,
  B0101 DEPRECATED(0b0101) = 5,
  B00101 DEPRECATED(0b00101) = 5,
  B000101 DEPRECATED(0b000101) = 5,
  B0000101 DEPRECATED(0b0000101) = 5,
  B00000101 DEPRECATED(0b00000101) = 5,
  B110 DEPRECATED(0b110) = 6,
  B0110 DEPRECATED(0b0110) = 6,
  B00110 DEPRECATED(0b00110) = 6,
  B000110 DEPRECATED(0b000110) = 6,
  B0000110 DEPRECATED(0b0000110) = 6,
  B00000110 DEPRECATED(0b00000110) = 6,
  B111 DEPRECATED(0b111) = 7,
  B0111 DEPRECATED(0b0111) = 7,
  B00111 DEPRECATED(0b00111) = 7,
  B000111 DEPRECATED(0b000111) = 7,
  B0000111 DEPRECATED(0b0000111) = 7,
  B00000111 DEPRECATED(0b00000111) = 7,
  B1000 DEPRECATED(0b1000) = 8,
  B01000 DEPRECATED(0b01000) = 8,
  B001000 DEPRECATED(0b001000) = 8,
  B0001000 DEPRECATED(0b0001000) = 8,
  B00001000 DEPRECATED(0b00001000) = 8,
  B1001 DEPRECATED(0b1001) = 9,
  B01001 DEPRECATED(0b01001) = 9,
  B001001 DEPRECATED(0b001001) = 9,
  B0001001 DEPRECATED(0b0001001) = 9,
  B00001001 DEPRECATED(0b00001001) = 9,
  B1010 DEPRECATED(0b1010) = 10,
  B01010 DEPRECATED(0b01010) = 10,
  B001010 DEPRECATED(0b001010) = 10,
  B0001010 DEPRECATED(0b0001010) = 10,
  B00001010 DEPRECATED(0b00001010) = 10,
  B1011 DEPRECATED(0b1011) = 11,
  B01011 DEPRECATED(0b01011) = 11,
  B001011 DEPRECATED(0b001011) = 11,
  B0001011 DEPRECATED(0b0001011) = 11,
  B00001011 DEPRECATED(0b00001011) = 11,
  B1100 DEPRECATED(0b1100) = 12,
  B01100 DEPRECATED(0b01100) = 12,
  B001100 DEPRECATED(0b001100) = 12,
  B0001100 DEPRECATED(0b0001100) = 12,
  B00001100 DEPRECATED(0b00001100) = 12,
  B1101 DEPRECATED(0b1101) = 13,
  B01101 DEPRECATED(0b01101) = 13,
  B001101 DEPRECATED(0b001101) = 13,
  B0001101 DEPRECATED(0b0001101) = 13,
  B00001101 DEPRECATED(0b00001101) = 13,
  B1110 DEPRECATED(0b1110) = 14,
  B01110 DEPRECATED(0b01110) = 14,
  B001110 DEPRECATED(0b001110) = 14,
  B0001110 DEPRECATED(0b0001110) = 14,
  B00001110 DEPRECATED(0b00001110) = 14,
  B1111 DEPRECATED(0b1111) = 15,
  B01111 DEPRECATED(0b01111) = 15,
  B001111 DEPRECATED(0b001111) = 15,
  B0001111 DEPRECATED(0b0001111) = 15,
  B00001111 DEPRECATED(0b00001111) = 15,
  B10000 DEPRECATED(0b10000) = 16,
  B010000 DEPRECATED(0b010000) = 16,
  B0010000 DEPRECATED(0b0010000) = 16,
  B00010000 DEPRECATED(0b00010000) = 16,
  B10001 DEPRECATED(0b10001) = 17,
  B010001 DEPRECATED(0b010001) = 17,
  B0010001 DEPRECATED(0b0010001) = 17,
  B00010001 DEPRECATED(0b00010001) = 17,
  B10010 DEPRECATED(0b10010) = 18,
  B010010 DEPRECATED(0b010010) = 18,
  B0010010 DEPRECATED(0b0010010) = 18,
  B00010010 DEPRECATED(0b00010010) = 18,
  B10011 DEPRECATED(0b10011) = 19,
  B010011 DEPRECATED(0b010011) = 19,
  B0010011 DEPRECATED(0b0010011) = 19,
  B00010011 DEPRECATED(0b00010011) = 19,
  B10100 DEPRECATED(0b10100) = 20,
  B010100 DEPRECATED(0b010100) = 20,
  B0010100 DEPRECATED(0b0010100) = 20,
  B00010100 DEPRECATED(0b00010100) = 20,
  B10101 DEPRECATED(0b10101) = 21,
  B010101 DEPRECATED(0b010101) = 21,
  B0010101 DEPRECATED(0b0010101) = 21,
  B00010101 DEPRECATED(0b00010101) = 21,
  B10110 DEPRECATED(0b10110) = 22,
  B010110 DEPRECATED(0b010110) = 22,
  B0010110 DEPRECATED(0b0010110) = 22,
  B00010110 DEPRECATED(0b00010110) = 22,
  B10111 DEPRECATED(0b10111) = 23,
  B010111 DEPRECATED(0b010111) = 23,
  B0010111 DEPRECATED(0b0010111) = 23,
  B00010111 DEPRECATED(0b00010111) = 23,
  B11000 DEPRECATED(0b11000) = 24,
  B011000 DEPRECATED(0b011000) = 24,
  B0011000 DEPRECATED(0b0011000) = 24,
  B00011000 DEPRECATED(0b00011000) = 24,
  B11001 DEPRECATED(0b11001) = 25,
  B011001 DEPRECATED(0b011001) = 25,
  B0011001 DEPRECATED(0b0011001) = 25,
  B00011001 DEPRECATED(0b00011001) = 25,
  B11010 DEPRECATED(0b11010) = 26,
  B011010 DEPRECATED(0b011010) = 26,
  B0011010 DEPRECATED(0b0011010) = 26,
  B00011010 DEPRECATED(0b00011010) = 26,
  B11011 DEPRECATED(0b11011) = 27,
  B011011 DEPRECATED(0b011011) = 27,
  B0011011 DEPRECATED(0b0011011) = 27,
  B00011011 DEPRECATED(0b00011011) = 27,
  B11100 DEPRECATED(0b11100) = 28,
  B011100 DEPRECATED(0b011100) = 28,
  B0011100 DEPRECATED(0b0011100) = 28,
  B00011100 DEPRECATED(0b00011100) = 28,
  B11101 DEPRECATED(0b11101) = 29,
  B011101 DEPRECATED(0b011101) = 29,
  B0011101 DEPRECATED(0b0011101) = 29,
  B00011101 DEPRECATED(0b00011101) = 29,
  B11110 DEPRECATED(0b11110) = 30,
  B011110 DEPRECATED(0b011110) = 30,
  B0011110 DEPRECATED(0b0011110) = 30,
  B00011110 DEPRECATED(0b00011110) = 30,
  B11111 DEPRECATED(0b11111) = 31,
  B011111 DEPRECATED(0b011111) = 31,
  B0011111 DEPRECATED(0b0011111) = 31,
  B00011111 DEPRECATED(0b00011111) = 31,
  B100000 DEPRECATED(0b100000) = 32,
  B0100000 DEPRECATED(0b0100000) = 32,
  B00100000 DEPRECATED(0b00100000) = 32,
  B100001 DEPRECATED(0b100001) = 33,
  B0100001 DEPRECATED(0b0100001) = 33,
  B00100001 DEPRECATED(0b00100001) = 33,
  B100010 DEPRECATED(0b100010) = 34,
  B0100010 DEPRECATED(0b0100010) = 34,
  B00100010 DEPRECATED(0b00100010) = 34,
  B100011 DEPRECATED(0b100011) = 35,
  B0100011 DEPRECATED(0b0100011) = 35,
  B00100011 DEPRECATED(0b00100011) = 35,
  B100100 DEPRECATED(0b100100) = 36,
  B0100100 DEPRECATED(0b0100100) = 36,
  B00100100 DEPRECATED(0b00100100) = 36,
  B100101 DEPRECATED(0b100101) = 37,
  B0100101 DEPRECATED(0b0100101) = 37,
  B00100101 DEPRECATED(0b00100101) = 37,
  B100110 DEPRECATED(0b100110) = 38,
  B0100110 DEPRECATED(0b0100110) = 38,
  B00100110 DEPRECATED(0b00100110) = 38,
  B100111 DEPRECATED(0b100111) = 39,
  B0100111 DEPRECATED(0b0100111) = 39,
  B00100111 DEPRECATED(0b00100111) = 39,
  B101000 DEPRECATED(0b101000) = 40,
  B0101000 DEPRECATED(0b0101000) = 40,
  B00101000 DEPRECATED(0b00101000) = 40,
  B101001 DEPRECATED(0b101001) = 41,
  B0101001 DEPRECATED(0b0101001) = 41,
  B00101001 DEPRECATED(0b00101001) = 41,
  B101010 DEPRECATED(0b101010) = 42,
  B0101010 DEPRECATED(0b0101010) = 42,
  B00101010 DEPRECATED(0b00101010) = 42,
  B101011 DEPRECATED(0b101011) = 43,
  B0101011 DEPRECATED(0b0101011) = 43,
  B00101011 DEPRECATED(0b00101011) = 43,
  B101100 DEPRECATED(0b101100) = 44,
  B0101100 DEPRECATED(0b0101100) = 44,
  B00101100 DEPRECATED(0b00101100) = 44,
  B101101 DEPRECATED(0b101101) = 45,
  B0101101 DEPRECATED(0b0101101) = 45,
  B00101101 DEPRECATED(0b00101101) = 45,
  B101110 DEPRECATED(0b101110) = 46,
  B0101110 DEPRECATED(0b0101110) = 46,
  B00101110 DEPRECATED(0b00101110) = 46,
  B101111 DEPRECATED(0b101111) = 47,
  B0101111 DEPRECATED(0b0101111) = 47,
  B00101111 DEPRECATED(0b00101111) = 47,
  B110000 DEPRECATED(0b110000) = 48,
  B0110000 DEPRECATED(0b0110000) = 48,
  B00110000 DEPRECATED(0b00110000) = 48,
  B110001 DEPRECATED(0b110001) = 49,
  B0110001 DEPRECATED(0b0110001) = 49,
  B00110001 DEPRECATED(0b00110001) = 49,
  B110010 DEPRECATED(0b110010) = 50,
  B0110010 DEPRECATED(0b0110010) = 50,
  B00110010 DEPRECATED(0b00110010) = 50,
  B110011 DEPRECATED(0b110011) = 51,
  B0110011 DEPRECATED(0b0110011) = 51,
  B00110011 DEPRECATED(0b00110011) = 51,
  B110100 DEPRECATED(0b110100) = 52,
  B0110100 DEPRECATED(0b0110100) = 52,
  B00110100 DEPRECATED(0b00110100) = 52,
  B110101 DEPRECATED(0b110101) = 53,
  B0110101 DEPRECATED(0b0110101) = 53,
  B00110101 DEPRECATED(0b00110101) = 53,
  B110110 DEPRECATED(0b110110) = 54,
  B0110110 DEPRECATED(0b0110110) = 54,
  B00110110 DEPRECATED(0b00110110) = 54,
  B110111 DEPRECATED(0b110111) = 55,
  B0110111 DEPRECATED(0b0110111) = 55,
  B00110111 DEPRECATED(0b00110111) = 55,
  B111000 DEPRECATED(0b111000) = 56,
  B0111000 DEPRECATED(0b0111000) = 56,
  B00111000 DEPRECATED(0b00111000) = 56,
  B111001 DEPRECATED(0b111001) = 57,
  B0111001 DEPRECATED(0b0111001) = 57,
  B00111001 DEPRECATED(0b00111001) = 57,
  B111010 DEPRECATED(0b111010) = 58,
  B0111010 DEPRECATED(0b0111010) = 58,
  B00111010 DEPRECATED(0b00111010) = 58,
  B111011 DEPRECATED(0b111011) = 59,
  B0111011 DEPRECATED(0b0111011) = 59,
  B00111011 DEPRECATED(0b00111011) = 59,
  B111100 DEPRECATED(0b111100) = 60,
  B0111100 DEPRECATED(0b0111100) = 60,
  B00111100 DEPRECATED(0b00111100) = 60,
  B111101 DEPRECATED(0b111101) = 61,
  B0111101 DEPRECATED(0b0111101) = 61,
  B00111101 DEPRECATED(0b00111101) = 61,
  B111110 DEPRECATED(0b111110) = 62,
  B0111110 DEPRECATED(0b0111110) = 62,
  B00111110 DEPRECATED(0b00111110) = 62,
  B111111 DEPRECATED(0b111111) = 63,
  B0111111 DEPRECATED(0b0111111) = 63,
  B00111111 DEPRECATED(0b00111111) = 63,
  B1000000 DEPRECATED(0b1000000) = 64,
  B01000000 DEPRECATED(0b01000000) = 64,
  B1000001 DEPRECATED(0b1000001) = 65,
  B01000001 DEPRECATED(0b01000001) = 65,
  B1000010 DEPRECATED(0b1000010) = 66,
  B01000010 DEPRECATED(0b01000010) = 66,
  B1000011 DEPRECATED(0b1000011) = 67,
  B01000011 DEPRECATED(0b01000011) = 67,
  B1000100 DEPRECATED(0b1000100) = 68,
  B01000100 DEPRECATED(0b01000100) = 68,
  B1000101 DEPRECATED(0b1000101) = 69,
  B01000101 DEPRECATED(0b01000101) = 69,
  B1000110 DEPRECATED(0b1000110) = 70,
  B01000110 DEPRECATED(0b01000110) = 70,
  B1000111 DEPRECATED(0b1000111) = 71,
  B01000111 DEPRECATED(0b01000111) = 71,
  B1001000 DEPRECATED(0b1001000) = 72,
  B01001000 DEPRECATED(0b01001000) = 72,
  B1001001 DEPRECATED(0b1001001) = 73,
  B01001001 DEPRECATED(0b01001001) = 73,
  B1001010 DEPRECATED(0b1001010) = 74,
  B01001010 DEPRECATED(0b01001010) = 74,
  B1001011 DEPRECATED(0b1001011) = 75,
  B01001011 DEPRECATED(0b01001011) = 75,
  B1001100 DEPRECATED(0b1001100) = 76,
  B01001100 DEPRECATED(0b01001100) = 76,
  B1001101 DEPRECATED(0b1001101) = 77,
  B01001101 DEPRECATED(0b01001101) = 77,
  B1001110 DEPRECATED(0b1001110) = 78,
  B01001110 DEPRECATED(0b01001110) = 78,
  B1001111 DEPRECATED(0b1001111) = 79,
  B01001111 DEPRECATED(0b01001111) = 79,
  B1010000 DEPRECATED(0b1010000) = 80,
  B01010000 DEPRECATED(0b01010000) = 80,
  B1010001 DEPRECATED(0b1010001) = 81,
  B01010001 DEPRECATED(0b01010001) = 81,
  B1010010 DEPRECATED(0b1010010) = 82,
  B01010010 DEPRECATED(0b01010010) = 82,
  B1010011 DEPRECATED(0b1010011) = 83,
  B01010011 DEPRECATED(0b01010011) = 83,
  B1010100 DEPRECATED(0b1010100) = 84,
  B01010100 DEPRECATED(0b01010100) = 84,
  B1010101 DEPRECATED(0b1010101) = 85,
  B01010101 DEPRECATED(0b01010101) = 85,
  B1010110 DEPRECATED(0b1010110) = 86,
  B01010110 DEPRECATED(0b01010110) = 86,
  B1010111 DEPRECATED(0b1010111) = 87,
  B01010111 DEPRECATED(0b01010111) = 87,
  B1011000 DEPRECATED(0b1011000) = 88,
  B01011000 DEPRECATED(0b01011000) = 88,
  B1011001 DEPRECATED(0b1011001) = 89,
  B01011001 DEPRECATED(0b01011001) = 89,
  B1011010 DEPRECATED(0b1011010) = 90,
  B01011010 DEPRECATED(0b01011010) = 90,
  B1011011 DEPRECATED(0b1011011) = 91,
  B01011011 DEPRECATED(0b01011011) = 91,
  B1011100 DEPRECATED(0b1011100) = 92,
  B01011100 DEPRECATED(0b01011100) = 92,
  B1011101 DEPRECATED(0b1011101) = 93,
  B01011101 DEPRECATED(0b01011101) = 93,
  B1011110 DEPRECATED(0b1011110) = 94,
  B01011110 DEPRECATED(0b01011110) = 94,
  B1011111 DEPRECATED(0b1011111) = 95,
  B01011111 DEPRECATED(0b01011111) = 95,
  B1100000 DEPRECATED(0b1100000) = 96,
  B01100000 DEPRECATED(0b01100000) = 96,
  B1100001 DEPRECATED(0b1100001) = 97,
  B01100001 DEPRECATED(0b01100001) = 97,
  B1100010 DEPRECATED(0b1100010) = 98,
  B01100010 DEPRECATED(0b01100010) = 98,
  B1100011 DEPRECATED(0b1100011) = 99,
  B01100011 DEPRECATED(0b01100011) = 99,
  B1100100 DEPRECATED(0b1100100) = 100,
  B01100100 DEPRECATED(0b01100100) = 100,
  B1100101 DEPRECATED(0b1100101) = 101,
  B01100101 DEPRECATED(0b01100101) = 101,
  B1100110 DEPRECATED(0b1100110) = 102,
  B01100110 DEPRECATED(0b01100110) = 102,
  B1100111 DEPRECATED(0b1100111) = 103,
  B01100111 DEPRECATED(0b01100111) = 103,
  B1101000 DEPRECATED(0b1101000) = 104,
  B01101000 DEPRECATED(0b01101000) = 104,
  B1101001 DEPRECATED(0b1101001) = 105,
  B01101001 DEPRECATED(0b01101001) = 105,
  B1101010 DEPRECATED(0b1101010) = 106,
  B01101010 DEPRECATED(0b01101010) = 106,
  B1101011 DEPRECATED(0b1101011) = 107,
  B01101011 DEPRECATED(0b01101011) = 107,
  B1101100 DEPRECATED(0b1101100) = 108,
  B01101100 DEPRECATED(0b01101100) = 108,
  B1101101 DEPRECATED(0b1101101) = 109,
  B01101101 DEPRECATED(0b01101101) = 109,
  B1101110 DEPRECATED(0b1101110) = 110,
  B01101110 DEPRECATED(0b01101110) = 110,
  B1101111 DEPRECATED(0b1101111) = 111,
  B01101111 DEPRECATED(0b01101111) = 111,
  B1110000 DEPRECATED(0b1110000) = 112,
  B01110000 DEPRECATED(0b01110000) = 112,
  B1110001 DEPRECATED(0b1110001) = 113,
  B01110001 DEPRECATED(0b01110001) = 113,
  B1110010 DEPRECATED(0b1110010) = 114,
  B01110010 DEPRECATED(0b01110010) = 114,
  B1110011 DEPRECATED(0b1110011) = 115,
  B01110011 DEPRECATED(0b01110011) = 115,
  B1110100 DEPRECATED(0b1110100) = 116,
  B01110100 DEPRECATED(0b01110100) = 116,
  B1110101 DEPRECATED(0b1110101) = 117,
  B01110101 DEPRECATED(0b01110101) = 117,
  B1110110 DEPRECATED(0b1110110) = 118,
  B01110110 DEPRECATED(0b01110110) = 118,
  B1110111 DEPRECATED(0b1110111) = 119,
  B01110111 DEPRECATED(0b01110111) = 119,
  B1111000 DEPRECATED(0b1111000) = 120,
  B01111000 DEPRECATED(0b01111000) = 120,
  B1111001 DEPRECATED(0b1111001) = 121,
  B01111001 DEPRECATED(0b01111001) = 121,
  B1111010 DEPRECATED(0b1111010) = 122,
  B01111010 DEPRECATED(0b01111010) = 122,
  B1111011 DEPRECATED(0b1111011) = 123,
  B01111011 DEPRECATED(0b01111011) = 123,
  B1111100 DEPRECATED(0b1111100) = 124,
  B01111100 DEPRECATED(0b01111100) = 124,
  B1111101 DEPRECATED(0b1111101) = 125,
  B01111101 DEPRECATED(0b01111101) = 125,
  B1111110 DEPRECATED(0b1111110) = 126,
  B01111110 DEPRECATED(0b01111110) = 126,
  B1111111 DEPRECATED(0b1111111) = 127,
  B01111111 DEPRECATED(0b01111111) = 127,
  B10000000 DEPRECATED(0b10000000) = 128,
  B10000001 DEPRECATED(0b10000001) = 129,
  B10000010 DEPRECATED(0b10000010) = 130,
  B10000011 DEPRECATED(0b10000011) = 131,
  B10000100 DEPRECATED(0b10000100) = 132,
  B10000101 DEPRECATED(0b10000101) = 133,
  B10000110 DEPRECATED(0b10000110) = 134,
  B10000111 DEPRECATED(0b10000111) = 135,
  B10001000 DEPRECATED(0b10001000) = 136,
  B10001001 DEPRECATED(0b10001001) = 137,
  B10001010 DEPRECATED(0b10001010) = 138,
  B10001011 DEPRECATED(0b10001011) = 139,
  B10001100 DEPRECATED(0b10001100) = 140,
  B10001101 DEPRECATED(0b10001101) = 141,
  B10001110 DEPRECATED(0b10001110) = 142,
  B10001111 DEPRECATED(0b10001111) = 143,
  B10010000 DEPRECATED(0b10010000) = 144,
  B10010001 DEPRECATED(0b10010001) = 145,
  B10010010 DEPRECATED(0b10010010) = 146,
  B10010011 DEPRECATED(0b10010011) = 147,
  B10010100 DEPRECATED(0b10010100) = 148,
  B10010101 DEPRECATED(0b10010101) = 149,
  B10010110 DEPRECATED(0b10010110) = 150,
  B10010111 DEPRECATED(0b10010111) = 151,
  B10011000 DEPRECATED(0b10011000) = 152,
  B10011001 DEPRECATED(0b10011001) = 153,
  B10011010 DEPRECATED(0b10011010) = 154,
  B10011011 DEPRECATED(0b10011011) = 155,
  B10011100 DEPRECATED(0b10011100) = 156,
  B10011101 DEPRECATED(0b10011101) = 157,
  B10011110 DEPRECATED(0b10011110) = 158,
  B10011111 DEPRECATED(0b10011111) = 159,
  B10100000 DEPRECATED(0b10100000) = 160,
  B10100001 DEPRECATED(0b10100001) = 161,
  B10100010 DEPRECATED(0b10100010) = 162,
  B10100011 DEPRECATED(0b10100011) = 163,
  B10100100 DEPRECATED(0b10100100) = 164,
  B10100101 DEPRECATED(0b10100101) = 165,
  B10100110 DEPRECATED(0b10100110) = 166,
  B10100111 DEPRECATED(0b10100111) = 167,
  B10101000 DEPRECATED(0b10101000) = 168,
  B10101001 DEPRECATED(0b10101001) = 169,
  B10101010 DEPRECATED(0b10101010) = 170,
  B10101011 DEPRECATED(0b10101011) = 171,
  B10101100 DEPRECATED(0b10101100) = 172,
  B10101101 DEPRECATED(0b10101101) = 173,
  B10101110 DEPRECATED(0b10101110) = 174,
  B10101111 DEPRECATED(0b10101111) = 175,
  B10110000 DEPRECATED(0b10110000) = 176,
  B10110001 DEPRECATED(0b10110001) = 177,
  B10110010 DEPRECATED(0b10110010) = 178,
  B10110011 DEPRECATED(0b10110011) = 179,
  B10110100 DEPRECATED(0b10110100) = 180,
  B10110101 DEPRECATED(0b10110101) = 181,
  B10110110 DEPRECATED(0b10110110) = 182,
  B10110111 DEPRECATED(0b10110111) = 183,
  B10111000 DEPRECATED(0b10111000) = 184,
  B10111001 DEPRECATED(0b10111001) = 185,
  B10111010 DEPRECATED(0b10111010) = 186,
  B10111011 DEPRECATED(0b10111011) = 187,
  B10111100 DEPRECATED(0b10111100) = 188,
  B10111101 DEPRECATED(0b10111101) = 189,
  B10111110 DEPRECATED(0b10111110) = 190,
  B10111111 DEPRECATED(0b10111111) = 191,
  B11000000 DEPRECATED(0b11000000) = 192,
  B11000001 DEPRECATED(0b11000001) = 193,
  B11000010 DEPRECATED(0b11000010) = 194,
  B11000011 DEPRECATED(0b11000011) = 195,
  B11000100 DEPRECATED(0b11000100) = 196,
  B11000101 DEPRECATED(0b11000101) = 197,
  B11000110 DEPRECATED(0b11000110) = 198,
  B11000111 DEPRECATED(0b11000111) = 199,
  B11001000 DEPRECATED(0b11001000) = 200,
  B11001001 DEPRECATED(0b11001001) = 201,
  B11001010 DEPRECATED(0b11001010) = 202,
  B11001011 DEPRECATED(0b11001011) = 203,
  B11001100 DEPRECATED(0b11001100) = 204,
  B11001101 DEPRECATED(0b11001101) = 205,
  B11001110 DEPRECATED(0b11001110) = 206,
  B11001111 DEPRECATED(0b11001111) = 207,
  B11010000 DEPRECATED(0b11010000) = 208,
  B11010001 DEPRECATED(0b11010001) = 209,
  B11010010 DEPRECATED(0b11010010) = 210,
  B11010011 DEPRECATED(0b11010011) = 211,
  B11010100 DEPRECATED(0b11010100) = 212,
  B11010101 DEPRECATED(0b11010101) = 213,
  B11010110 DEPRECATED(0b11010110) = 214,
  B11010111 DEPRECATED(0b11010111) = 215,
  B11011000 DEPRECATED(0b11011000) = 216,
  B11011001 DEPRECATED(0b11011001) = 217,
  B11011010 DEPRECATED(0b11011010) = 218,
  B11011011 DEPRECATED(0b11011011) = 219,
  B11011100 DEPRECATED(0b11011100) = 220,
  B11011101 DEPRECATED(0b11011101) = 221,
  B11011110 DEPRECATED(0b11011110) = 222,
  B11011111 DEPRECATED(0b11011111) = 223,
  B11100000 DEPRECATED(0b11100000) = 224,
  B11100001 DEPRECATED(0b11100001) = 225,
  B11100010 DEPRECATED(0b11100010) = 226,
  B11100011 DEPRECATED(0b11100011) = 227,
  B11100100 DEPRECATED(0b11100100) = 228,
  B11100101 DEPRECATED(0b11100101) = 229,
  B11100110 DEPRECATED(0b11100110) = 230,
  B11100111 DEPRECATED(0b11100111) = 231,
  B11101000 DEPRECATED(0b11101000) = 232,
  B11101001 DEPRECATED(0b11101001) = 233,
  B11101010 DEPRECATED(0b11101010) = 234,
  B11101011 DEPRECATED(0b11101011) = 235,
  B11101100 DEPRECATED(0b11101100) = 236,
  B11101101 DEPRECATED(0b11101101) = 237,
  B11101110 DEPRECATED(0b11101110) = 238,
  B11101111 DEPRECATED(0b11101111) = 239,
  B11110000 DEPRECATED(0b11110000) = 240,
  B11110001 DEPRECATED(0b11110001) = 241,
  B11110010 DEPRECATED(0b11110010) = 242,
  B11110011 DEPRECATED(0b11110011) = 243,
  B11110100 DEPRECATED(0b11110100) = 244,
  B11110101 DEPRECATED(0b11110101) = 245,
  B11110110 DEPRECATED(0b11110110) = 246,
  B11110111 DEPRECATED(0b11110111) = 247,
  B11111000 DEPRECATED(0b11111000) = 248,
  B11111001 DEPRECATED(0b11111001) = 249,
  B11111010 DEPRECATED(0b11111010) = 250,
  B11111011 DEPRECATED(0b11111011) = 251,
  B11111100 DEPRECATED(0b11111100) = 252,
  B11111101 DEPRECATED(0b11111101) = 253,
  B11111110 DEPRECATED(0b11111110) = 254,
  B11111111 DEPRECATED(0b11111111) = 255
};
}  // namespace mjz_ard
#undef DEPRECATED

#endif

#ifndef Arduino

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

typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrParam)(void *);

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
#ifdef EXTENDED_PIN_MODE
// Platforms who want to declare more than 256 pins need to define
// EXTENDED_PIN_MODE globally
typedef uint32_t pin_size_t;
#else
typedef uint8_t pin_size_t;
#endif

#ifndef Arduino

// #define word(...) makeWord(__VA_ARGS__)
}
#endif  // __cplusplus

#else
/*
  #include <Arduino.h>
  #include <ArduinoJson.h>
  #include <FS.h>
  #include <HTTPClient.h>
  #include <HTTPUpdate.h>
  #include <HardwareSerial.h>
  #include <LittleFS.h>
  #include <Update.h>
  #include <WString.h>
  #include <WiFi.h>
  #include <WiFiMulti.h>
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
constexpr inline bool isAlphaNumeric(int c) __attribute__((always_inline));
constexpr inline bool isAlpha(int c) __attribute__((always_inline));
constexpr inline bool isAscii(int c) __attribute__((always_inline));
constexpr inline bool isWhitespace(int c) __attribute__((always_inline));
constexpr inline bool isControl(int c) __attribute__((always_inline));
constexpr inline bool isDigit(int c) __attribute__((always_inline));
constexpr inline bool isGraph(int c) __attribute__((always_inline));
constexpr inline bool isLowerCase(int c) __attribute__((always_inline));
constexpr inline bool isPrintable(int c) __attribute__((always_inline));
constexpr inline bool isPunct(int c) __attribute__((always_inline));
constexpr inline bool isSpace(int c) __attribute__((always_inline));
constexpr inline bool isUpperCase(int c) __attribute__((always_inline));
constexpr inline bool isHexadecimalDigit(int c) __attribute__((always_inline));
constexpr inline int toAscii(int c) __attribute__((always_inline));
constexpr inline int toLowerCase(int c) __attribute__((always_inline));
constexpr inline int toUpperCase(int c) __attribute__((always_inline));

// Checks for an alphanumeric character.
// It is equivalent to (isalpha(c) || isdigit(c)).
constexpr inline bool isAlphaNumeric(int c) {
  return (isalnum(c) == 0 ? false : true);
}

// Checks for an alphabetic character.
// It is equivalent to (isupper(c) || islower(c)).
constexpr inline bool isAlpha(int c) {
  return (isalpha(c) == 0 ? false : true);
}

// Checks whether c is a 7-bit unsigned char value
// that fits into the ASCII character set.
constexpr inline bool isAscii(int c) {
  return ((c & ~0x7f) != 0 ? false : true);
}

// Checks for a blank character,that is,a space or a tab.
constexpr inline bool isWhitespace(int c) {
  return (isblank(c) == 0 ? false : true);
}

// Checks for a control character.
constexpr inline bool isControl(int c) {
  return (iscntrl(c) == 0 ? false : true);
}

// Checks for a digit (0 through 9).
constexpr inline bool isDigit(int c) {
  return (isdigit(c) == 0 ? false : true);
}

// Checks for any printable character except space.
constexpr inline bool isGraph(int c) {
  return (isgraph(c) == 0 ? false : true);
}

// Checks for a lower-case character.
constexpr inline bool isLowerCase(int c) {
  return (islower(c) == 0 ? false : true);
}

// Checks for any printable character including space.
constexpr inline bool isPrintable(int c) {
  return (isprint(c) == 0 ? false : true);
}

// Checks for any printable character which is not a space
// or an alphanumeric character.
constexpr inline bool isPunct(int c) {
  return (ispunct(c) == 0 ? false : true);
}

// Checks for white-space characters. For the avr-libc library,
// these are: space,formfeed ('\f'),newline ('\n'),carriage
// return ('\r'),horizontal tab ('\t'),and vertical tab ('\v').
constexpr inline bool isSpace(int c) {
  return (isspace(c) == 0 ? false : true);
}

// Checks for an uppercase letter.
constexpr inline bool isUpperCase(int c) {
  return (isupper(c) == 0 ? false : true);
}

// Checks for a hexadecimal digits,i.e. one of 0 1 2 3 4 5 6 7
// 8 9 a b c d e f A B C D E F.
constexpr inline bool isHexadecimalDigit(int c) {
  return (isxdigit(c) == 0 ? false : true);
}

// Converts c to a 7-bit unsigned char value that fits into the
// ASCII character set,by clearing the high-order bits.
constexpr inline int toAscii(int c) { return (c & 0x7f); }

// Warning:
// Many people will be unhappy if you use this function.
// This function will convert accented letters into random
// characters.

// Converts the letter c to lower case,if possible.
constexpr inline int toLowerCase(int c) {
  if (!((c > 96) && (c < 123))) c ^= 0x20;
  return c;
}

// Converts the letter c to upper case,if possible.
constexpr inline int toUpperCase(int c) {
  if ((c > 96) && (c < 123)) c ^= 0x20;
  return c;
}

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
#ifndef Arduino
namespace mjz_ard {
// Public Methods //////////////////////////////////////////////////////////////
/* C++ prototypes */
constexpr inline long map(long x, long in_min, long in_max, long out_min,
                          long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
constexpr inline long long map(long long x, long long in_min, long long in_max,
                               long long out_min, long long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
constexpr inline uint16_t makeWord(uint16_t w) { return w; }
constexpr inline uint16_t makeWord(uint8_t h, uint8_t l) {
  return (h << 8) | l;
}

}  // namespace mjz_ard

#endif

#endif  // STRING_SUM_HELPER
#define __mjz_ard_STRINGS__
#ifdef Arduino
#define std__string_view_if_is String
#else
#define std__string_view_if_is std::string_view
#endif
// #define virtual
// use final if want to not be virtualized again
// #define if_override_then_override override
// override
#ifndef if_override_then_override
#define if_override_then_override
#endif  // if_override_then_override
// override
#ifdef Arduino
#define if_ard_then_override override
#else
#define if_ard_then_override
unsigned long millis();
#endif  // Arduino
#ifndef global_mjz_areana_allocator_on
constexpr size_t number_of_global_mjz_areana_allocator_blocks = 64 * 1024;
constexpr size_t size_of_global_mjz_areana_allocator_blocks = 32;
// true
// false
#define global_mjz_areana_allocator_on true
#endif  // !global_mjz_areana_allocator_on
#ifndef global_mjz_areana_allocator_log
#define global_mjz_areana_allocator_log false
#endif
inline uint32_t usteejtgk_millis() { return millis(); }
class __FlashStringHelper;

namespace mjz_ard {

template <class Type>
using mjz_get_value_Type = typename Type::value_type;

template <class Type>
using mjz_get_pointer_Type = typename Type::pointer;
template <class Type>
Type *remove_const(const Type *o) {
  return const_cast<Type *>(o);
}
template <class Type>
Type &remove_const(const Type &o) {
  return *remove_const(std::addressof(o));
}
// iterator_template Class
template <typename Type, bool error_check = 1>
class iterator_template_t {
 protected:
  Type *m_iterator;
  Type *m_iterator_begin_ptr;
  Type *m_iterator_end_ptr;

 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  // using iterator_concept = std::contiguous_iterator_tag;
  constexpr inline iterator_template_t() noexcept
      : iterator_template_t(nullptr, nullptr, (Type *)-1) {}
  // iterator_template(Type *iter ) noexcept: m_iterator { iter } { }
  constexpr inline iterator_template_t(Type *iter, Type *min_end,
                                       Type *max_end) noexcept
      : m_iterator{iter},
        m_iterator_begin_ptr{min_end},
        m_iterator_end_ptr{max_end} {}
  constexpr inline iterator_template_t(Type *arr, size_t len) noexcept
      : m_iterator(arr),
        m_iterator_begin_ptr(arr),
        m_iterator_end_ptr(arr + len) {}
  constexpr inline iterator_template_t(const Type *First_arg,
                                       const Type *Last_arg) noexcept
      : iterator_template_t(First_arg, First_arg, Last_arg) {}
  //  constexpr  inline  //
  // iterator_template(std::initializer_list<Type>list) noexcept :
  // iterator_template((Type *)(list.begin()), (size_t)list.size()) { } this is
  // bad for class types becuse ~list => ~Type => } code... => use after free

  constexpr inline void throw_if_bad(Type *_iterator) const {
    if constexpr (error_check) {
      if (_iterator == (Type *)-1) {
        _iterator = m_iterator;
      }
      if ((m_iterator_begin_ptr <= _iterator) &&
          (_iterator <= m_iterator_end_ptr)) {
        return;
      }
      throw std::out_of_range(
          "bad ptr access : iterator_template::throw_if_bad ");
    }
  }
  constexpr void throw_if_bad() const { throw_if_bad((Type *)-1); }
  constexpr iterator_template_t(const iterator_template_t &p) noexcept
      : m_iterator(p.m_iterator),
        m_iterator_begin_ptr(p.m_iterator_begin_ptr),
        m_iterator_end_ptr(p.m_iterator_end_ptr) {}
  constexpr iterator_template_t(iterator_template_t &&p) noexcept
      : m_iterator(p.m_iterator),
        m_iterator_begin_ptr(p.m_iterator_begin_ptr),
        m_iterator_end_ptr(p.m_iterator_end_ptr) {}
  constexpr iterator_template_t &operator=(Type *iter) {
    m_iterator = iter;
    throw_if_bad();
    return *this;
  }
  constexpr iterator_template_t &operator=(const iterator_template_t &p) {
    m_iterator = (p.m_iterator);
    m_iterator_begin_ptr = p.m_iterator_begin_ptr;
    m_iterator_end_ptr = p.m_iterator_end_ptr;
    throw_if_bad();
    return *this;
  }
  constexpr iterator_template_t &operator=(iterator_template_t &&p) noexcept {
    m_iterator = (p.m_iterator);
    m_iterator_begin_ptr = p.m_iterator_begin_ptr;
    m_iterator_end_ptr = p.m_iterator_end_ptr;
    return *this;
  }
  inline ~iterator_template_t() { m_iterator = 0; }
  constexpr bool operator==(const iterator_template_t &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(const iterator_template_t &other) const noexcept {
    return m_iterator != other.m_iterator;
  }
  constexpr reference operator*() const {
    throw_if_bad();
    return *m_iterator;
  }
  constexpr pointer operator->() const {
    throw_if_bad();
    return m_iterator;
  }
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return operator->()->*my_var;
  }
  constexpr inline const Type *begin() const { return m_iterator_begin_ptr; }
  constexpr inline const Type *end() const { return m_iterator_end_ptr; }
  constexpr inline Type *begin() { return m_iterator_begin_ptr; }
  constexpr inline Type *end() { return m_iterator_end_ptr; }
  constexpr inline iterator_template_t base() {
    return iterator_template_t(m_iterator_begin_ptr, m_iterator_begin_ptr,
                               m_iterator_end_ptr);
  }
  constexpr size_t size() const noexcept {
    return static_cast<size_t>((const uint8_t *)m_iterator_end_ptr -
                               (const uint8_t *)m_iterator_begin_ptr) /
           sizeof(Type);
  }
  constexpr iterator_template_t &operator++() noexcept {
    ++m_iterator;
    return *this;
  }
  constexpr iterator_template_t operator++(int) noexcept {
    iterator_template_t tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr iterator_template_t &operator--() noexcept {
    --m_iterator;
    return *this;
  }
  constexpr iterator_template_t operator--(int) noexcept {
    iterator_template_t tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr iterator_template_t &operator+=(
      const difference_type other) noexcept {
    m_iterator += other;
    return *this;
  }
  constexpr iterator_template_t &operator-=(
      const difference_type other) noexcept {
    m_iterator -= other;
    return *this;
  }
  constexpr iterator_template_t &operator+=(
      const iterator_template_t &other) noexcept {
    m_iterator += other.m_iterator;
    return *this;
  }
  constexpr iterator_template_t &operator-=(
      const iterator_template_t &other) noexcept {
    m_iterator -= other.m_iterator;
    return *this;
  }
  constexpr reference operator[](std::size_t index) const {
    throw_if_bad(m_iterator + index);
    return m_iterator[index];
  }
  constexpr bool operator<(const iterator_template_t &other) const noexcept {
    return m_iterator < other.m_iterator;
  }
  constexpr bool operator>(const iterator_template_t &other) const noexcept {
    return m_iterator > other.m_iterator;
  }
  constexpr bool operator<=(const iterator_template_t &other) const noexcept {
    return m_iterator <= other.m_iterator;
  }
  constexpr bool operator>=(const iterator_template_t &other) const noexcept {
    return m_iterator >= other.m_iterator;
  }
  constexpr operator pointer() {
    throw_if_bad();
    return m_iterator;
  }
  constexpr explicit operator pointer &() {
    throw_if_bad();
    return m_iterator;
  }
  constexpr pointer get_pointer() const {
    throw_if_bad();
    return m_iterator;
  }
  constexpr pointer &get_pointer() {
    throw_if_bad();
    return m_iterator;
  }
  constexpr friend iterator_template_t operator+(
      const iterator_template_t &me, const difference_type other) noexcept {
    return iterator_template_t(me.m_iterator + other, me.m_iterator_begin_ptr,
                               me.m_iterator_end_ptr);
  }
  constexpr friend iterator_template_t operator-(
      const iterator_template_t &me, const difference_type other) noexcept {
    return iterator_template_t(me.m_iterator - other, me.m_iterator_begin_ptr,
                               me.m_iterator_end_ptr);
  }
  constexpr friend iterator_template_t operator+(
      const iterator_template_t &other,
      const iterator_template_t &me) noexcept {
    return iterator_template_t(
        other.m_iterator + me,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  constexpr friend difference_type operator-(
      const iterator_template_t &other,
      const iterator_template_t &me) noexcept {
    return std::distance(other.m_iterator, me.m_iterator);
  }
  constexpr friend void swap(iterator_template_t &lhs,
                             iterator_template_t &rhs) noexcept {
    iterator_template_t lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
};
// iterator_template_ptr_warper Class

template <typename iterator_t, typename Type>
class iterator_template_ptr_warper {
 protected:
  iterator_t m_iterator;

 public:
  using value_type = Type;
  using reference = Type &;
  using pointer = Type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  constexpr inline iterator_template_ptr_warper() noexcept : m_iterator() {}
  constexpr inline iterator_template_ptr_warper(iterator_t iter) noexcept
      : m_iterator(iter) {}
  constexpr iterator_template_ptr_warper(
      const iterator_template_ptr_warper &p) noexcept
      : m_iterator(p.m_iterator) {}
  constexpr iterator_template_ptr_warper(
      iterator_template_ptr_warper &&p) noexcept
      : m_iterator(p.m_iterator) {}
  constexpr iterator_template_ptr_warper &operator=(iterator_t iter) {
    m_iterator = iter;
    return *this;
  }
  constexpr iterator_template_ptr_warper &operator=(
      const iterator_template_ptr_warper &p) {
    m_iterator = (p.m_iterator);
    return *this;
  }
  constexpr iterator_template_ptr_warper &operator=(
      iterator_template_ptr_warper &&p) noexcept {
    m_iterator = (p.m_iterator);
    return *this;
  }
  inline ~iterator_template_ptr_warper() { m_iterator = 0; }
  constexpr bool operator==(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator != other.m_iterator;
  }
  constexpr reference operator*() const { return **m_iterator; }
  constexpr pointer operator->() const { return &(**m_iterator); }
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return operator->()->*my_var;
  }
  constexpr inline const iterator_template_ptr_warper begin() const {
    return iterator_template_ptr_warper(
        {m_iterator.begin(), m_iterator.begin(), m_iterator.end()});
  }
  constexpr inline const iterator_template_ptr_warper end() const {
    return iterator_template_ptr_warper(
        {m_iterator.end(), m_iterator.begin(), m_iterator.end()});
  }
  constexpr inline iterator_template_ptr_warper begin() {
    return iterator_template_ptr_warper(
        {m_iterator.begin(), m_iterator.begin(), m_iterator.end()});
  }
  constexpr inline iterator_template_ptr_warper end() {
    return iterator_template_ptr_warper(
        {m_iterator.end(), m_iterator.begin(), m_iterator.end()});
  }
  constexpr inline iterator_t base() { return m_iterator; }
  constexpr size_t size() const noexcept { return m_iterator.size(); }
  constexpr iterator_template_ptr_warper &operator++() noexcept {
    ++m_iterator;
    return *this;
  }
  constexpr iterator_template_ptr_warper operator++(int) noexcept {
    iterator_template_ptr_warper tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr iterator_template_ptr_warper &operator--() noexcept {
    --m_iterator;
    return *this;
  }
  constexpr iterator_template_ptr_warper operator--(int) noexcept {
    iterator_template_ptr_warper tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr iterator_template_ptr_warper &operator+=(
      const difference_type other) noexcept {
    m_iterator += other;
    return *this;
  }
  constexpr iterator_template_ptr_warper &operator-=(
      const difference_type other) noexcept {
    m_iterator -= other;
    return *this;
  }
  constexpr iterator_template_ptr_warper &operator+=(
      const iterator_template_ptr_warper &other) noexcept {
    m_iterator += other.m_iterator;
    return *this;
  }
  constexpr iterator_template_ptr_warper &operator-=(
      const iterator_template_ptr_warper &other) noexcept {
    m_iterator -= other.m_iterator;
    return *this;
  }
  constexpr reference operator[](std::size_t index) const {
    return *m_iterator[index];
  }
  constexpr bool operator<(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator < other.m_iterator;
  }
  constexpr bool operator>(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator > other.m_iterator;
  }
  constexpr bool operator<=(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator <= other.m_iterator;
  }
  constexpr bool operator>=(
      const iterator_template_ptr_warper &other) const noexcept {
    return m_iterator >= other.m_iterator;
  }
  constexpr operator pointer() { return m_iterator; }
  constexpr explicit operator pointer &() { return m_iterator; }
  constexpr pointer get_pointer() const { return m_iterator; }
  constexpr pointer &get_pointer() { return m_iterator; }
  constexpr friend iterator_template_ptr_warper operator+(
      const iterator_template_ptr_warper &me,
      const difference_type other) noexcept {
    return iterator_template_ptr_warper(me.m_iterator + other);
  }
  constexpr friend iterator_template_ptr_warper operator-(
      const iterator_template_ptr_warper &me,
      const difference_type other) noexcept {
    return iterator_template_ptr_warper(me.m_iterator - other);
  }
  constexpr friend iterator_template_ptr_warper operator+(
      const difference_type other,
      const iterator_template_ptr_warper &me) noexcept {
    return iterator_template_ptr_warper(other + me.m_iterator);
  }
  // friend iterator_template_ptr_warper operator-(const difference_type other,
  // const iterator_template_ptr_warper& me) noexcept { // bad function dont
  // use return iterator_template_ptr_warper(me.m_iterator - (pointer)other);
  // }
  constexpr friend iterator_template_ptr_warper operator+(
      const iterator_template_ptr_warper &other,
      const iterator_template_ptr_warper &me) noexcept {
    return iterator_template_ptr_warper(other.m_iterator + me);
  }
  constexpr friend difference_type operator-(
      const iterator_template_ptr_warper &other,
      const iterator_template_ptr_warper &me) noexcept {
    return operator-(other.m_iterator, me.m_iterator);
  }
  constexpr friend void swap(iterator_template_ptr_warper &lhs,
                             iterator_template_ptr_warper &rhs) noexcept {
    iterator_template_ptr_warper lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
  constexpr friend void swap(reference lhs, reference rhs) noexcept {
    value_type lhsm_ = lhs;
    lhs = rhs;
    rhs = lhsm_;
  }
};
template <typename Type>
bool dummy_iterator_template_filter_warper_filter(const Type &) {
  return true;
}
template <typename Type, typename iterator_t>
class iterator_template_filter_warper {
 public:
  using filter_type = std::function<bool(const Type &)>;

 protected:
  iterator_t m_iterator;
  filter_type m_filter = dummy_iterator_template_filter_warper_filter<Type>;

 public:
  using value_type = Type;
  using reference = Type &;
  using pointer = Type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  constexpr inline iterator_template_filter_warper() noexcept : m_iterator() {}
  constexpr inline iterator_template_filter_warper(iterator_t iter) noexcept
      : m_iterator(iter) {}
  constexpr inline iterator_template_filter_warper(filter_type filter) noexcept
      : m_iterator(), m_filter(filter) {}
  constexpr inline iterator_template_filter_warper(iterator_t iter,
                                                   filter_type filter) noexcept
      : m_iterator(iter), m_filter(filter) {}
  constexpr iterator_template_filter_warper(
      const iterator_template_filter_warper &p) noexcept
      : m_iterator(p.m_iterator), m_filter(p.m_filter) {}
  constexpr iterator_template_filter_warper(
      iterator_template_filter_warper &&p) noexcept
      : m_iterator(p.m_iterator), m_filter(p.m_filter) {}
  constexpr iterator_template_filter_warper(
      const iterator_template_filter_warper &p, filter_type filter) noexcept
      : m_iterator(p.m_iterator), m_filter(filter) {}
  constexpr iterator_template_filter_warper(iterator_template_filter_warper &&p,
                                            filter_type filter) noexcept
      : m_iterator(p.m_iterator), m_filter(filter) {}
  constexpr iterator_template_filter_warper &operator=(Type *iter) {
    m_iterator = iter;
    return *this;
  }
  constexpr inline iterator_template_filter_warper(Type *iter, Type *min_end,
                                                   Type *max_end) noexcept
      : m_iterator(iter, min_end, max_end) {}
  constexpr inline iterator_template_filter_warper(Type *arr,
                                                   size_t len) noexcept
      : m_iterator(arr, len) {}
  constexpr inline iterator_template_filter_warper(
      const Type *First_arg, const Type *Last_arg) noexcept
      : iterator_template_filter_warper(First_arg, First_arg, Last_arg) {}
  constexpr inline iterator_template_filter_warper(Type *iter, Type *min_end,
                                                   Type *max_end,
                                                   filter_type filter) noexcept
      : m_iterator(iter, min_end, max_end), m_filter(filter) {}
  constexpr inline iterator_template_filter_warper(Type *arr, size_t len,
                                                   filter_type filter) noexcept
      : m_iterator(arr, len), m_filter(filter) {}
  constexpr inline iterator_template_filter_warper(const Type *First_arg,
                                                   const Type *Last_arg,
                                                   filter_type filter) noexcept
      : m_iterator(First_arg, First_arg, Last_arg), m_filter(filter) {}
  constexpr iterator_template_filter_warper &operator=(iterator_t iter) {
    m_iterator = iter;
    m_filter = (iter.m_filter);
    return *this;
  }
  constexpr iterator_template_filter_warper &operator=(
      const iterator_template_filter_warper &p) {
    m_iterator = (p.m_iterator);
    m_filter = (p.m_filter);
    return *this;
  }
  constexpr iterator_template_filter_warper &operator=(
      iterator_template_filter_warper &&p) noexcept {
    m_iterator = (p.m_iterator);
    m_filter = (p.m_filter);
    return *this;
  }
  inline ~iterator_template_filter_warper() {}
  constexpr bool operator==(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator != other.m_iterator;
  }
  constexpr reference operator*() {
    go_forward();
    return m_iterator.operator*();
  }
  constexpr pointer operator->() {
    go_forward();
    return m_iterator.operator->();
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) {
    return operator->()->*my_var;
  }
  constexpr inline const iterator_template_filter_warper begin() const {
    iterator_t b = base();
    b = b.begin();
    return iterator_template_filter_warper(b, m_filter);
  }
  constexpr inline const iterator_template_filter_warper end() const {
    iterator_t b = base();
    b = b.end();
    return iterator_template_filter_warper(b, m_filter);
  }
  constexpr inline iterator_template_filter_warper begin() {
    iterator_t b = base();
    b = b.begin();
    return iterator_template_filter_warper(b, m_filter);
  }
  constexpr inline iterator_template_filter_warper end() {
    iterator_t b = base();
    b = b.end();
    return iterator_template_filter_warper(b, m_filter);
  }
  constexpr inline iterator_t base() { return m_iterator; }
  constexpr inline const iterator_t base() const { return m_iterator; }
  constexpr size_t size() const noexcept { return m_iterator.size(); }
  constexpr void go_forward() {
    while ((m_iterator.end() != m_iterator) && !m_filter(*m_iterator)) {
      ++m_iterator;
    }
  }
  constexpr void go_backward() {
    while ((m_iterator.begin() != m_iterator) && !m_filter(*m_iterator)) {
      --m_iterator;
    }
  }
  constexpr static iterator_template_filter_warper go_forward(
      iterator_template_filter_warper &&it) {
    it.go_forward();
    return it;
  }
  constexpr static iterator_template_filter_warper go_backward(
      iterator_template_filter_warper &&it) {
    it.go_backward();
    return it;
  }
  constexpr iterator_template_filter_warper &operator++() noexcept {
    ++m_iterator;
    go_forward();
    return *this;
  }
  constexpr iterator_template_filter_warper operator++(int) noexcept {
    iterator_template_filter_warper tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr iterator_template_filter_warper &operator--() noexcept {
    --m_iterator;
    go_backward();
    return *this;
  }
  constexpr iterator_template_filter_warper operator--(int) noexcept {
    iterator_template_filter_warper tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr iterator_template_filter_warper &operator+=(
      const difference_type other) noexcept {
    m_iterator += other;
    go_forward();
    return *this;
  }
  constexpr iterator_template_filter_warper &operator-=(
      const difference_type other) noexcept {
    m_iterator -= other;
    go_backward();
    return *this;
  }
  constexpr iterator_template_filter_warper &operator+=(
      const iterator_template_filter_warper &other) noexcept {
    m_iterator += other.m_iterator;
    go_forward();
    return *this;
  }
  constexpr iterator_template_filter_warper &operator-=(
      const iterator_template_filter_warper &other) noexcept {
    m_iterator -= other.m_iterator;
    go_backward();
    return *this;
  }
  constexpr reference operator[](std::size_t index) const {
    return m_iterator[index];
  }
  constexpr bool operator<(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator < other.m_iterator;
  }
  constexpr bool operator>(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator > other.m_iterator;
  }
  constexpr bool operator<=(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator <= other.m_iterator;
  }
  constexpr bool operator>=(
      const iterator_template_filter_warper &other) const noexcept {
    return m_iterator >= other.m_iterator;
  }
  constexpr operator pointer() { return m_iterator; }
  constexpr explicit operator pointer &() { return m_iterator; }
  constexpr pointer get_pointer() const { return m_iterator; }
  constexpr pointer &get_pointer() { return m_iterator; }
  constexpr friend iterator_template_filter_warper operator+(
      const iterator_template_filter_warper &me,
      const difference_type other) noexcept {
    return go_forward(iterator_template_filter_warper(me.m_iterator + other));
  }
  constexpr friend iterator_template_filter_warper operator-(
      const iterator_template_filter_warper &me,
      const difference_type other) noexcept {
    return go_backward(iterator_template_filter_warper(me.m_iterator - other));
  }
  constexpr friend iterator_template_filter_warper operator+(
      const difference_type other,
      const iterator_template_filter_warper &me) noexcept {
    return go_forward(iterator_template_filter_warper(other + me.m_iterator));
  }
  // friend iterator_template_filter_warper operator-(const difference_type
  // other, const iterator_template_filter_warper& me) noexcept { // bad
  // function dont use return iterator_template_filter_warper(me.m_iterator -
  // (pointer)other);
  // }
  constexpr friend iterator_template_filter_warper operator+(
      const iterator_template_filter_warper &other,
      const iterator_template_filter_warper &me) noexcept {
    return go_forward(iterator_template_filter_warper(other.m_iterator + me));
  }
  constexpr friend difference_type operator-(
      const iterator_template_filter_warper &other,
      const iterator_template_filter_warper &me) noexcept {
    return operator-(other.m_iterator, me.m_iterator);
  }
  constexpr friend void swap(iterator_template_filter_warper &lhs,
                             iterator_template_filter_warper &rhs) noexcept {
    iterator_template_filter_warper<Type, iterator_t> lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
};
template <class iterator_t>
constexpr iterator_template_filter_warper<mjz_get_value_Type<iterator_t>,
                                          iterator_t>
to_filter_it(
    iterator_t it,
    typename iterator_template_filter_warper<mjz_get_value_Type<iterator_t>,
                                             iterator_t>::filter_type filter) {
  return {it, filter};
}

template <class it_T>
class iterator_warper_template_t {
 protected:
  it_T m_begin;
  it_T m_end;

 public:
  using r_it_T = std::reverse_iterator<it_T>;
  constexpr inline iterator_warper_template_t(iterator_warper_template_t &&obj)
      : m_begin(std::move(obj.m_begin)), m_end(std::move(obj.m_end)) {}
  constexpr inline iterator_warper_template_t(iterator_warper_template_t &obj)
      : m_begin(obj.m_begin), m_end(obj.m_end) {}
  constexpr inline iterator_warper_template_t(
      const iterator_warper_template_t &obj)
      : m_begin(obj.m_begin), m_end(obj.m_end) {}
  constexpr inline iterator_warper_template_t(const it_T &_begin,
                                              const it_T &_end)
      : m_begin(_begin), m_end(_end) {}
  constexpr inline iterator_warper_template_t(it_T &_begin, it_T &_end)
      : m_begin(_begin), m_end(_end) {}
  constexpr inline iterator_warper_template_t(it_T &&_begin, it_T &&_end)
      : m_begin(std::move(_begin)), m_end(std::move(_end)) {}
  ~iterator_warper_template_t() {}
  constexpr inline it_T begin() { return m_begin; }
  constexpr inline it_T end() { return m_end; }
  constexpr inline const it_T begin() const { return m_begin; }
  constexpr inline const it_T end() const { return m_end; }
  constexpr inline const it_T cbegin() const { return m_begin; }
  constexpr inline const it_T cend() const { return m_end; }
  constexpr inline r_it_T rbegin() { return m_end; }
  constexpr inline r_it_T rend() { return m_begin; }
  constexpr inline const r_it_T rbegin() const { return m_end; }
  constexpr inline const r_it_T rend() const { return m_begin; }
  constexpr inline const r_it_T crbegin() const { return m_end; }
  constexpr inline const r_it_T crend() const { return m_begin; }
};

template <class it_T>
constexpr inline iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(iterator_warper_template_t<it_T> &it) {
  return {it.rbegin(), it.rend()};
}

template <class it_T>
constexpr inline const iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(const iterator_warper_template_t<it_T> &it) {
  return {it.rbegin(), it.rend()};
}

template <class it_T>
constexpr inline iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(it_T &it) {
  return to_revurse(it.begin(), it.end());
}

template <class it_T>
constexpr inline const iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(const it_T &it) {
  return to_revurse(it.begin(), it.end());
}

template <class it_T>
constexpr inline const iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(const it_T &_begin, const it_T &_end) {
  return {std::reverse_iterator<it_T>(_end),
          std::reverse_iterator<it_T>(_begin)};
}

template <class it_T>
constexpr inline iterator_warper_template_t<std::reverse_iterator<it_T>>
to_revurse(it_T &_begin, it_T &_end) {
  return {std::reverse_iterator<it_T>(_end),
          std::reverse_iterator<it_T>(_begin)};
}

template <typename Type, class iterator_t>
class mjz_to_iterator_template_warper_t {
 protected:
  iterator_t m_begin;
  iterator_t m_end;
  iterator_t m_iterator;

 public:
  using r_iterator_t_wr_t =
      std::reverse_iterator<mjz_to_iterator_template_warper_t>;
  constexpr inline mjz_to_iterator_template_warper_t(
      mjz_to_iterator_template_warper_t &&obj)
      : m_begin(std::move(obj.m_begin)),
        m_end(std::move(obj.m_end)),
        m_iterator(std::move(obj.m_iterator)) {}
  constexpr inline mjz_to_iterator_template_warper_t(
      mjz_to_iterator_template_warper_t &obj)
      : m_begin(obj.m_begin), m_end(obj.m_end), m_iterator(obj.m_iterator) {}
  constexpr inline mjz_to_iterator_template_warper_t(
      const mjz_to_iterator_template_warper_t &obj)
      : m_begin(obj.m_begin), m_end(obj.m_end), m_iterator(obj.m_iterator) {}
  constexpr inline mjz_to_iterator_template_warper_t(const iterator_t &it,
                                                     const iterator_t &_begin,
                                                     const iterator_t &_end)
      : m_begin(_begin), m_end(_end), m_iterator(it) {}
  constexpr inline mjz_to_iterator_template_warper_t(iterator_t &it,
                                                     iterator_t &_begin,
                                                     iterator_t &_end)
      : m_begin(_begin), m_end(_end), m_iterator(it) {}
  constexpr inline mjz_to_iterator_template_warper_t(iterator_t &&it,
                                                     iterator_t &&_begin,
                                                     iterator_t &&_end)
      : m_begin(std::move(_begin)),
        m_end(std::move(_end)),
        m_iterator(std::move(it)) {}
  constexpr inline mjz_to_iterator_template_warper_t(const iterator_t &_begin,
                                                     const iterator_t &_end)
      : m_begin(_begin), m_end(_end), m_iterator(_begin) {}
  constexpr inline mjz_to_iterator_template_warper_t(iterator_t &_begin,
                                                     iterator_t &_end)
      : m_begin(_begin), m_end(_end), m_iterator(_begin) {}
  constexpr inline mjz_to_iterator_template_warper_t(iterator_t &&_begin,
                                                     iterator_t &&_end)
      : m_begin(_begin),
        m_end(std::move(_end)),
        m_iterator(std::move(_begin)) {}
  constexpr inline ~mjz_to_iterator_template_warper_t() {}
  constexpr inline mjz_to_iterator_template_warper_t &operator=(
      const mjz_to_iterator_template_warper_t &obj) {
    m_begin = obj.m_begin;
    m_end = obj.m_end;
    m_iterator = obj.m_iterator;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator=(
      mjz_to_iterator_template_warper_t &obj) {
    m_begin = obj.m_begin;
    m_end = obj.m_end;
    m_iterator = obj.m_iterator;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator=(
      mjz_to_iterator_template_warper_t &&obj) {
    m_begin = std::move(obj.m_begin);
    m_end = std::move(obj.m_end);
    m_iterator = std::move(obj.m_iterator);
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t begin() {
    return {m_begin, m_begin, m_end};
  }
  constexpr inline mjz_to_iterator_template_warper_t end() {
    return {m_end, m_begin, m_end};
  }
  constexpr inline const mjz_to_iterator_template_warper_t begin() const {
    return {m_begin, m_begin, m_end};
  }
  constexpr inline const mjz_to_iterator_template_warper_t end() const {
    return {m_end, m_begin, m_end};
  }
  constexpr inline const mjz_to_iterator_template_warper_t cbegin() const {
    return begin();
  }
  constexpr inline const mjz_to_iterator_template_warper_t cend() const {
    return end();
  }
  constexpr inline r_iterator_t_wr_t rbegin() { return end(); }
  constexpr inline r_iterator_t_wr_t rend() { return begin(); }
  constexpr inline const r_iterator_t_wr_t rbegin() const { return end(); }
  constexpr inline const r_iterator_t_wr_t rend() const { return begin(); }
  constexpr inline const r_iterator_t_wr_t crbegin() const { return end(); }
  constexpr inline const r_iterator_t_wr_t crend() const { return begin(); }

 public:
  using value_type = Type;
  using reference = Type &;
  using pointer = Type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  constexpr inline bool operator==(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr inline bool operator!=(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator != other.m_iterator;
  }
  constexpr inline reference operator*() const { return *m_iterator; }
  constexpr inline pointer operator->() const { return &operator*(); }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) {
    return operator->()->*my_var;
  }
  constexpr inline iterator_t base() { return m_iterator; }
  constexpr inline size_t size() const noexcept { return m_iterator.size(); }
  constexpr inline mjz_to_iterator_template_warper_t &operator++() noexcept {
    ++m_iterator;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t operator++(int) noexcept {
    mjz_to_iterator_template_warper_t tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator--() noexcept {
    --m_iterator;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t operator--(int) noexcept {
    mjz_to_iterator_template_warper_t tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator+=(
      const difference_type other) noexcept {
    m_iterator += other;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator-=(
      const difference_type other) noexcept {
    m_iterator -= other;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator+=(
      const mjz_to_iterator_template_warper_t &other) noexcept {
    m_iterator += other.m_iterator;
    return *this;
  }
  constexpr inline mjz_to_iterator_template_warper_t &operator-=(
      const mjz_to_iterator_template_warper_t &other) noexcept {
    m_iterator -= other.m_iterator;
    return *this;
  }
  constexpr inline reference operator[](std::size_t index) const {
    return m_iterator[index];
  }
  constexpr inline bool operator<(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator < other.m_iterator;
  }
  constexpr inline bool operator>(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator > other.m_iterator;
  }
  constexpr inline bool operator<=(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator <= other.m_iterator;
  }
  constexpr inline bool operator>=(
      const mjz_to_iterator_template_warper_t &other) const noexcept {
    return m_iterator >= other.m_iterator;
  }
  constexpr inline operator pointer() { return &operator*(); }
  constexpr inline explicit operator pointer &() { return &operator*(); }
  constexpr inline pointer get_pointer() const { return &operator*(); }
  constexpr inline pointer &get_pointer() { return &operator*(); }
  constexpr inline friend mjz_to_iterator_template_warper_t operator+(
      const mjz_to_iterator_template_warper_t &me,
      const difference_type other) noexcept {
    return mjz_to_iterator_template_warper_t(me.m_iterator + other, me.m_begin,
                                             me.m_end);
  }
  constexpr inline friend mjz_to_iterator_template_warper_t operator-(
      const mjz_to_iterator_template_warper_t &me,
      const difference_type other) noexcept {
    return mjz_to_iterator_template_warper_t(me.m_iterator - other, me.m_begin,
                                             me.m_end);
  }
  constexpr inline friend mjz_to_iterator_template_warper_t operator+(
      const difference_type other,
      const mjz_to_iterator_template_warper_t &me) noexcept {
    return mjz_to_iterator_template_warper_t(other + me.m_iterator, me.m_begin,
                                             me.m_end);
  }

  constexpr inline friend difference_type operator-(
      const mjz_to_iterator_template_warper_t &other,
      const mjz_to_iterator_template_warper_t &me) noexcept {
    return operator-(other.m_iterator, me.m_iterator);
  }
  constexpr inline friend void swap(
      mjz_to_iterator_template_warper_t &lhs,
      mjz_to_iterator_template_warper_t &rhs) noexcept {
    mjz_to_iterator_template_warper_t lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
};
template <class iterator_T>
constexpr inline mjz_to_iterator_template_warper_t<
    mjz_get_value_Type<iterator_T>, iterator_T>
to_mjz_it(iterator_T begin, iterator_T end) {
  return {std::move(begin), std::move(end)};
}
template <class T>
constexpr inline mjz_to_iterator_template_warper_t<T, T *> to_mjz_it(T *begin,
                                                                     T *end) {
  return {std::move(begin), std::move(end)};
}
template <class T>
constexpr inline auto to_mjz_it(T &obj) {
  return mjz_to_iterator_template_warper_t<mjz_get_value_Type<T>,
                                           decltype(obj.begin())>{obj.begin(),
                                                                  obj.end()};
}
template <class T>
constexpr inline auto to_mjz_it(const T &obj) {
  return mjz_to_iterator_template_warper_t<mjz_get_value_Type<const T>,
                                           decltype(obj.begin())>{obj.begin(),
                                                                  obj.end()};
}

template <size_t number_of_blocks =
              number_of_global_mjz_areana_allocator_blocks,
          size_t block_length = size_of_global_mjz_areana_allocator_blocks,
          bool KEEP_the_heap_clean = false>
class mjz_arena_allocator_t {
  template <typename T, size_t m_size>
  struct data_buffer {
    T m_data[m_size];
    constexpr inline size_t size() { return m_size; }
    constexpr inline T *begin() { return data(); }
    constexpr inline T *end() { return data() + m_size; }
    constexpr inline T *data() { return m_data; }
    constexpr inline const T &operator[](int64_t i) const { return m_data[i]; }
    constexpr inline T &operator[](int64_t i) { return m_data[i]; }
  };

 public:
  constexpr static size_t not_valid = (((size_t)-1 >> 1) - 1);
  constexpr static char EMPTY_char = 'A';
  struct block_data {
    size_t index_of_begin = not_valid;
    size_t index_of_end = not_valid;
  };

 protected:
  data_buffer<data_buffer<uint8_t, block_length>, number_of_blocks> m_blocks;
  data_buffer<block_data, number_of_blocks> m_block_data;
  constexpr inline void clean_all_data() & {
    if constexpr (KEEP_the_heap_clean)
      memset((char *)m_blocks.begin(), EMPTY_char,
             ((char *)m_blocks.end()) - ((char *)m_blocks.begin()));
  }
  constexpr inline size_t get_index_of_pointer(void *ptr) & {
    if (m_blocks.end() <= ptr || ptr < m_blocks.begin()) return not_valid;
    size_t ptr_diff = (size_t)((size_t)ptr - (size_t)m_blocks.begin());
    if ((ptr_diff % block_length) && ptr_diff) return not_valid;
    return (ptr_diff / block_length);
  }
  constexpr inline size_t get_index_of_pointer_that_is_not_allocated(
      size_t number_of_blocks_in_a_row = 1,
      size_t begin_search_at_index = 0) & {
    block_data *begin_it = m_block_data.begin() + begin_search_at_index;
    block_data *end_it = m_block_data.end();
    while (1) {
      while ((begin_it < end_it) && (begin_it->index_of_end != not_valid))
        begin_it += begin_it->index_of_end - begin_it->index_of_begin;
      auto end_of_row_it = begin_it + number_of_blocks_in_a_row;
      if (end_it < end_of_row_it) return not_valid;
      auto begin_it_buf = begin_it;
      while ((begin_it < end_of_row_it) &&
             (begin_it->index_of_begin == not_valid))
        begin_it++;
      if (end_of_row_it == begin_it)
        return (size_t)(begin_it_buf - m_block_data.begin());
    }
  }
  constexpr inline size_t allocate_block(size_t size,
                                         size_t realloc_index = not_valid) & {
    bool reallocatating = (realloc_index != not_valid);
    size_t index = get_index_of_pointer_that_is_not_allocated(
        reallocatating ? (size - get_size_in_blocks(realloc_index)) : size,
        reallocatating ? m_block_data[realloc_index].index_of_end : 0);
    if (reallocatating) {
      if (index == m_block_data[realloc_index].index_of_end) {
        index = realloc_index;
      } else {
        index = get_index_of_pointer_that_is_not_allocated(size, 0);
      }
    }
    if (index == not_valid) return not_valid;
    set_range_to(index, {index + size});
    return index;
  }
  constexpr inline void set_range_to(size_t ren_beg, size_t ren_end, size_t beg,
                                     size_t end) & {
    auto the_begin_data_ptr = m_block_data.begin();
    auto index_beg_it = the_begin_data_ptr + beg;
    auto index_end_it = the_begin_data_ptr + end;
    while (index_beg_it < index_end_it) {
      (*index_beg_it).index_of_begin = beg;
      (*index_beg_it++).index_of_end = end;
    }
  }
  constexpr inline void set_range_to(size_t ren_beg, size_t ren_end) {
    set_range_to(ren_beg, ren_end, ren_beg, ren_end);
  }
  constexpr inline void deallocate_rage(size_t index_begin, size_t index_end) {
    auto the_begin_data_ptr = m_block_data.begin();
    auto index_beg_it = the_begin_data_ptr + index_begin;
    auto index_end_it = the_begin_data_ptr + index_end;
    while (index_beg_it < index_end_it) {
      (*index_beg_it).index_of_begin = not_valid;
      (*index_beg_it++).index_of_end = not_valid;
    }
  }
  constexpr inline void deallocate_block(size_t index) & {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return;
    if constexpr (KEEP_the_heap_clean) {
      size_t end_i = m_block_data[index].index_of_end;
      size_t beg_i = index;
      deallocate_rage(beg_i, end_i);
      char *beg_p = (char *)(m_blocks[beg_i]).data();
      char *end_p = (char *)(m_blocks[end_i]).data();
      memset(beg_p, EMPTY_char, end_p - beg_p);
    } else {
      deallocate_rage(index, m_block_data[index].index_of_end);
    }
  }
  constexpr inline void *get_ptr(size_t index) & {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_blocks[index]).data();
  }
  constexpr inline size_t get_size_in_blocks(size_t index) & {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_block_data[index].index_of_end - index);
  }
  constexpr inline size_t get_real_size(size_t index) & {
    return get_size_in_blocks(index) * block_length;
  }
  constexpr inline size_t size_to_number_of_blocks(size_t size) & {
    return (size % block_length) ? ((size / block_length) + 1)
                                 : (size / block_length);
  }

 public:
  constexpr inline void *malloc(size_t size) & {
    size = size_to_number_of_blocks(size);
    return get_ptr(allocate_block(size));
  }
  constexpr inline void *realloc(void *ptr, size_t size) & {
    size_t index_of_ptr = get_index_of_pointer(ptr);
    if (index_of_ptr == not_valid) return malloc(size);
    size = size_to_number_of_blocks(size);
    size_t size_of_current_ptr = get_size_in_blocks(index_of_ptr);
    if (size < size_of_current_ptr) {
      deallocate_rage(index_of_ptr + size, index_of_ptr + size_of_current_ptr);
      set_range_to(index_of_ptr, index_of_ptr + size);
      return ptr;
    }
    if (size == size_of_current_ptr) {
      return ptr;
    }
    if (!size) {
      free(ptr);
      return 0;
    }
    size_t index = allocate_block(size, index_of_ptr);
    if (index == not_valid) {
      free(ptr);
      return 0;
    } else if (index == index_of_ptr) {
      return get_ptr(index_of_ptr);
    }
    if (index != not_valid)
      std::memmove(get_ptr(index), ptr, get_real_size(index_of_ptr));
    deallocate_block(index_of_ptr);
    if (index != not_valid) return get_ptr(index);
    return 0;
  }
  constexpr inline void free(void *ptr) & {
    deallocate_block(get_index_of_pointer(ptr));
  }
  constexpr inline size_t get_size(void *ptr) & {
    return get_real_size(get_index_of_pointer(ptr));
  }
  constexpr inline bool is_valid(void *ptr) & {
    if (m_blocks.end() <= ptr || ptr < m_blocks.begin()) return 0;
    return !(((size_t)((size_t)ptr - (size_t)m_blocks.begin())) % block_length);
  }
  constexpr inline mjz_arena_allocator_t() { clean_all_data(); }
  constexpr inline ~mjz_arena_allocator_t() { clean_all_data(); }
  constexpr inline mjz_arena_allocator_t(mjz_arena_allocator_t &&)
      : mjz_arena_allocator_t(){};
  constexpr inline mjz_arena_allocator_t(const mjz_arena_allocator_t &)
      : mjz_arena_allocator_t(){};
  constexpr inline mjz_arena_allocator_t &operator=(mjz_arena_allocator_t &&) &
      {};
  constexpr inline mjz_arena_allocator_t &operator=(
      const mjz_arena_allocator_t &) &
      {};
};

struct arena_allocator : public mjz_arena_allocator_t<1024, 32> {};

template <size_t block_length = size_of_global_mjz_areana_allocator_blocks,
          bool KEEP_the_heap_clean = false, typename size_type = size_t>
class dynamic_mjz_arena_allocator_t {
 private:
  template <typename T, size_type m_size>
  struct data_buffer {
    T m_data[m_size];
    constexpr inline size_type size() { return m_size; }
    constexpr inline T *begin() { return data(); }
    constexpr inline T *end() { return data() + m_size; }
    constexpr inline T *data() { return m_data; }
    constexpr inline const T &operator[](size_type i) const {
      return m_data[i];
    }
    constexpr inline T &operator[](size_type i) { return m_data[i]; }
  };

 public:
  constexpr static size_type not_valid = (((size_type)-1 >> 1) - 1);
  constexpr static char EMPTY_char = 'A';
  struct block_data {
    constexpr inline block_data()
        : index_of_begin(not_valid), index_of_end(not_valid) {}
    constexpr inline void init() {
      index_of_begin = not_valid;
      index_of_end = not_valid;
    }
    size_type index_of_begin = not_valid;
    size_type index_of_end = not_valid;
  };
  static constexpr inline size_type block_required_size =
      block_length + sizeof(block_data);

 protected:
  size_type number_of_blocks{};
  iterator_template_t<data_buffer<uint8_t, block_length>> m_blocks;
  iterator_template_t<block_data> m_block_data;
  constexpr inline void clean_all_data() {
    if constexpr (KEEP_the_heap_clean)
      memset((char *)m_blocks.begin(), EMPTY_char,
             ((char *)m_blocks.end()) - ((char *)m_blocks.begin()));
  }
  constexpr inline size_type get_index_of_pointer(void *ptr) {
    if (m_blocks.end() <= ptr || ptr < m_blocks.begin()) return not_valid;
    size_type ptr_diff =
        (size_type)((size_type)ptr - (size_type)m_blocks.begin());
    if ((ptr_diff % block_length) && ptr_diff) return not_valid;
    return (ptr_diff / block_length);
  }
  constexpr inline size_type get_index_of_pointer_that_is_not_allocated(
      size_type number_of_blocks_in_a_row = 1,
      size_type begin_search_at_index = 0) {
    block_data *begin_it = m_block_data.begin() + begin_search_at_index;
    block_data *end_it = m_block_data.end();
    while (1) {
      while ((begin_it < end_it) && (begin_it->index_of_end != not_valid))
        begin_it += begin_it->index_of_end - begin_it->index_of_begin;
      auto end_of_row_it = begin_it + number_of_blocks_in_a_row;
      if (end_it < end_of_row_it) return not_valid;
      auto begin_it_buf = begin_it;
      while ((begin_it < end_of_row_it) &&
             (begin_it->index_of_begin == not_valid))
        begin_it++;
      if (end_of_row_it == begin_it)
        return (size_type)(begin_it_buf - m_block_data.begin());
    }
  }
  constexpr inline size_type allocate_block(
      size_type size, size_type realloc_index = not_valid) {
    bool reallocatating = (realloc_index != not_valid);
    size_type index = get_index_of_pointer_that_is_not_allocated(
        reallocatating ? (size - get_size_in_blocks(realloc_index)) : size,
        reallocatating ? m_block_data[realloc_index].index_of_end : 0);
    if (reallocatating) {
      if (index == m_block_data[realloc_index].index_of_end) {
        index = realloc_index;
      } else {
        index = get_index_of_pointer_that_is_not_allocated(size, 0);
      }
    }
    if (index == not_valid) return not_valid;
    set_range_to((size_type)index, (size_type)index + (size_type)size);
    return index;
  }
  constexpr inline void set_range_to(size_type ren_beg, size_type ren_end,
                                     size_type beg, size_type end) {
    auto the_begin_data_ptr = m_block_data.begin();
    auto index_beg_it = the_begin_data_ptr + beg;
    auto index_end_it = the_begin_data_ptr + end;
    while (index_beg_it < index_end_it) {
      (*index_beg_it).index_of_begin = beg;
      (*index_beg_it++).index_of_end = end;
    }
  }
  constexpr inline void set_range_to(size_type ren_beg, size_type ren_end) {
    set_range_to(ren_beg, ren_end, ren_beg, ren_end);
  }
  constexpr inline void deallocate_rage(size_type index_begin,
                                        size_type index_end) {
    auto the_begin_data_ptr = m_block_data.begin();
    auto index_beg_it = the_begin_data_ptr + index_begin;
    auto index_end_it = the_begin_data_ptr + index_end;
    while (index_beg_it < index_end_it) {
      (*index_beg_it).index_of_begin = not_valid;
      (*index_beg_it++).index_of_end = not_valid;
    }
  }
  constexpr inline void deallocate_block(size_type index) {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return;
    if constexpr (KEEP_the_heap_clean) {
      size_type end_i = m_block_data[index].index_of_end;
      size_type beg_i = index;
      deallocate_rage(beg_i, end_i);
      char *beg_p = (char *)(m_blocks[beg_i]).data();
      char *end_p = (char *)(m_blocks[end_i]).data();
      memset(beg_p, EMPTY_char, end_p - beg_p);
    } else {
      deallocate_rage(index, m_block_data[index].index_of_end);
    }
  }
  constexpr inline void *get_ptr(size_type index) {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_blocks[index]).data();
  }
  constexpr inline size_type get_size_in_blocks(size_type index) {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_block_data[index].index_of_end - index);
  }
  constexpr inline size_type get_real_size(size_type index) {
    return get_size_in_blocks(index) * block_length;
  }
  constexpr inline size_type size_to_number_of_blocks(size_type size) {
    return (size % block_length) ? ((size / block_length) + 1)
                                 : (size / block_length);
  }
  constexpr inline void initilize_data() {
    for (auto p = m_blocks.get_pointer(), e = p + m_blocks.size(); p < e; p++) {
      new (p) data_buffer<uint8_t, block_length>();
    }
    for (auto p = m_block_data.get_pointer(), e = p + m_block_data.size();
         p < e; p++) {
      new (p) block_data();
    }
  }

 public:
  constexpr inline void *malloc(size_type size) & {
    size = size_to_number_of_blocks(size);
    return get_ptr(allocate_block(size));
  }
  constexpr inline void *realloc(void *ptr, size_type size) & {
    size_type index_of_ptr = get_index_of_pointer(ptr);
    if (index_of_ptr == not_valid) return malloc(size);
    size = size_to_number_of_blocks(size);
    size_type size_of_current_ptr = get_size_in_blocks(index_of_ptr);
    if (size < size_of_current_ptr) {
      deallocate_rage(index_of_ptr + size, index_of_ptr + size_of_current_ptr);
      set_range_to(index_of_ptr, index_of_ptr + size);
      return ptr;
    }
    if (size == size_of_current_ptr) {
      return ptr;
    }
    if (!size) {
      free(ptr);
      return 0;
    }
    size_type index = allocate_block(size, index_of_ptr);
    if (index == not_valid) {
      free(ptr);
      return 0;
    } else if (index == index_of_ptr) {
      return get_ptr(index_of_ptr);
    }
    if (index != not_valid)
      std::memmove(get_ptr(index), ptr, get_real_size(index_of_ptr));
    deallocate_block(index_of_ptr);
    if (index != not_valid) return get_ptr(index);
    return 0;
  }
  constexpr inline void free(void *ptr) & {
    deallocate_block(get_index_of_pointer(ptr));
  }
  constexpr inline size_type get_size(void *ptr) & {
    return get_real_size(get_index_of_pointer(ptr));
  }
  constexpr inline bool is_valid(void *ptr) & {
    if (m_blocks.end() <= ptr || ptr < m_blocks.begin()) return 0;
    return !(((size_type)((size_type)ptr - (size_type)m_blocks.begin())) %
             block_length);
  }
  constexpr inline dynamic_mjz_arena_allocator_t(void *data_mem,
                                                 size_type tatal_size)
      : number_of_blocks(tatal_size / block_required_size),
        m_blocks(((data_buffer<uint8_t, block_length> *)data_mem),
                 number_of_blocks),
        m_block_data(
            (block_data *)(((data_buffer<uint8_t, block_length> *)data_mem) +
                           number_of_blocks),
            number_of_blocks) {
    initilize_data();
    clean_all_data();
  }
  constexpr inline ~dynamic_mjz_arena_allocator_t() { clean_all_data(); }
  constexpr inline void *unsafe_data() & { return m_blocks.get_pointer(); }
  constexpr inline size_t unsafe_size() & {
    return ((char *)m_blocks.end()) - ((char *)m_blocks.begin());
  }
};

struct dynamic_arena_allocator : public dynamic_mjz_arena_allocator_t<64> {
  constexpr inline dynamic_arena_allocator(void *p, size_t cap)
      : dynamic_mjz_arena_allocator_t(p, cap) {}
  constexpr inline ~dynamic_arena_allocator() {}
};
template <size_t block_length = 64, bool KEEP_the_heap_clean = false,
          typename size_type = uint32_t>
struct small_dynamic_arena_allocator_t
    : public dynamic_mjz_arena_allocator_t<block_length, KEEP_the_heap_clean,
                                           size_type> {
  using me = small_dynamic_arena_allocator_t;
  constexpr inline small_dynamic_arena_allocator_t(void *p, size_t cap)
      : dynamic_mjz_arena_allocator_t<block_length, KEEP_the_heap_clean,
                                      size_type>(p, cap) {}
  constexpr inline ~small_dynamic_arena_allocator_t() {}
};
struct small_dynamic_arena_allocator
    : public small_dynamic_arena_allocator_t<64, false, uint32_t> {
  constexpr inline small_dynamic_arena_allocator(void *p, size_t cap)
      : small_dynamic_arena_allocator_t<64, false, uint32_t>(p, cap) {}
  constexpr inline ~small_dynamic_arena_allocator() {}
};
struct tiny_dynamic_arena_allocator
    : public small_dynamic_arena_allocator_t<64, false, uint16_t> {
  constexpr inline tiny_dynamic_arena_allocator(void *p, size_t cap)
      : small_dynamic_arena_allocator_t<64, false, uint16_t>(p, cap) {}
  constexpr inline ~tiny_dynamic_arena_allocator() {}
};
struct vary_tiny_dynamic_arena_allocator
    : public small_dynamic_arena_allocator_t<16, false, uint8_t> {
  constexpr inline vary_tiny_dynamic_arena_allocator(void *p, size_t cap)
      : small_dynamic_arena_allocator_t<16, false, uint8_t>(p, cap) {}
  constexpr inline ~vary_tiny_dynamic_arena_allocator() {}
};
template <bool gsjgskhl>
struct dummy_struct______T {
  static size_t dummy;
  constexpr inline size_t &get() { return dummy; }
};

template <bool B>
size_t dummy_struct______T<B>::dummy{};

template <class my_free_realloc_wrpr_t, typename Type>
struct mjz_reallocator_template_t {
  using my_value_Type_t = Type;
  using value_type = my_value_Type_t;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const my_value_Type_t &;
  using size_type = size_t;
  using propagate_on_container_move_assignment = std::true_type;
  constexpr inline mjz_reallocator_template_t(){};
  constexpr inline ~mjz_reallocator_template_t(){};
  constexpr inline mjz_reallocator_template_t(void *p, size_t n)
      : my_free_realloc_wrpr_obj(p, n) {}
  template <class T>
  constexpr inline mjz_reallocator_template_t(
      const mjz_reallocator_template_t<my_free_realloc_wrpr_t, T> &) noexcept {}
  constexpr [[nodiscard]] inline my_value_Type_t *allocate(size_t n) {
    return reallocate((my_value_Type_t *)0, n);
  }
  constexpr [[nodiscard]] inline my_value_Type_t *allocate(size_t n,
                                                           const void *hint) {
    return reallocate((my_value_Type_t *)hint, n);
  }
  constexpr inline void deallocate(my_value_Type_t *p, size_t n) noexcept {
    free(p);
  }

 protected:
  constexpr [[nodiscard]] inline void *allocate_raw(size_t number_of_bytes) {
    return reallocate_raw((void *)0, number_of_bytes);
  }
  constexpr [[nodiscard]] inline void *allocate_raw(size_t n,
                                                    const void *hint) {
    return reallocate_raw((void *)hint, n);
  }
  constexpr inline void deallocate_raw(void *p,
                                       size_t number_of_bytes) noexcept {
    free(p);
  }

 private:
  constexpr [[nodiscard]] inline my_value_Type_t *reallocate(
      my_value_Type_t *ptr, size_t n) {
    return static_cast<my_value_Type_t *>(
        realloc(ptr, n * sizeof(my_value_Type_t)));
  }
  constexpr [[nodiscard]] inline void *reallocate_raw(void *ptr,
                                                      size_t number_of_bytes) {
    return realloc(ptr, number_of_bytes);
  }
  constexpr size_t &get_size_of_mem(void *ptr) {
    if (ptr == 0) {
      dummy_struct______T<1>().get() = 0;
      return dummy_struct______T<1>().get();
    }
    return *(size_t *)get_real_mem(ptr);
  }
  constexpr inline size_t get_needed_size_of_mem(size_t size) {
    return sizeof(size_t) + size;
  }
  constexpr inline void *get_fake_mem(void *ptr) {
    if (ptr == 0) return ptr;
    return (void *)((size_t *)ptr + 1);
  }
  constexpr inline void *get_real_mem(void *ptr) {
    if (ptr == 0) return ptr;
    return (void *)((size_t *)ptr - 1);
  }
  my_free_realloc_wrpr_t my_free_realloc_wrpr_obj;
  constexpr inline void *realloc(void *ptr, size_t size) {
    void *ptr2{};
    ptr2 = realloc_mem(ptr, size);
    get_size_of_mem(ptr2) = get_needed_size_of_mem(size);
    return ptr2;
  }
  constexpr inline void free(void *ptr) {
    my_free_realloc_wrpr_obj.free(get_real_mem(ptr), get_size_of_mem(ptr));
  }
  constexpr inline void *realloc_mem(void *ptr, size_t len) {
    void *real_ptr = get_real_mem(ptr);
    void *real_reallocated_ptr = my_free_realloc_wrpr_obj.realloc(
        real_ptr, get_size_of_mem(ptr), get_needed_size_of_mem(len));
    return get_fake_mem(real_reallocated_ptr);
  }
};
template <class T1, class T2, class U>
constexpr inline bool operator==(const mjz_reallocator_template_t<U, T1> &,
                                 const mjz_reallocator_template_t<U, T2> &) {
  return true;
}

template <class T1, class T2, class U>
constexpr inline bool operator!=(const mjz_reallocator_template_t<U, T1> &,
                                 const mjz_reallocator_template_t<U, T2> &) {
  return false;
}
template <class T1, class T2, class U1, class U2>
constexpr inline bool operator==(const mjz_reallocator_template_t<U1, T1> &,
                                 const mjz_reallocator_template_t<U2, T2> &) {
  return U1() == U2();
}

template <class T1, class T2, class U1, class U2>
constexpr inline bool operator!=(const mjz_reallocator_template_t<U1, T1> &,
                                 const mjz_reallocator_template_t<U2, T2> &) {
  return U1() != U2();
}
using mjz_global_arena_allocator_type =
    mjz_arena_allocator_t<number_of_global_mjz_areana_allocator_blocks,
                          size_of_global_mjz_areana_allocator_blocks,
                          global_mjz_areana_allocator_on>;
template <bool jfgskgkla>
struct mjz_global_arena_allocator_type_static_holder {
  static mjz_global_arena_allocator_type obj;
  constexpr inline mjz_global_arena_allocator_type &get() { return obj; }
};
template <bool jfgskgkla>
mjz_global_arena_allocator_type
    mjz_global_arena_allocator_type_static_holder<jfgskgkla>::obj{};

constexpr inline mjz_global_arena_allocator_type &my_global_allocator() {
  return mjz_global_arena_allocator_type_static_holder<1>().get();
}
namespace private_dont_use_it {
#if global_mjz_areana_allocator_on
constexpr inline void *realloc(void *p, size_t s) {
  void *p2 = my_global_allocator().realloc(p, s);
  return p2;
}
constexpr inline void free(void *p) { return my_global_allocator().free(p); }
#else
constexpr inline void *realloc(void *p, size_t s) {
  void *p2 = ::realloc(p, s);
  return p2;
}
constexpr inline void free(void *p) { return ::free(p); }
#endif

constexpr bool log_it = global_mjz_areana_allocator_log;
namespace log_functions {
inline void log(const char *const str, size_t n, const void *const in) {
#if global_mjz_areana_allocator_log
  std::cout << '\n' << str << " " << n << " bytes in:" << in << ".\n";
#endif
}
constexpr inline void *get_fake_mem(void *ptr) {
  if (ptr == 0) return ptr;
  return (void *)((size_t *)ptr + 1);
}
constexpr inline void *get_real_mem(void *ptr) {
  if (ptr == 0) return ptr;
  return (void *)((size_t *)ptr - 1);
}
namespace why_are_you_using_me {
static size_t dummy{};
}
constexpr inline size_t &get_size_of_mem(void *ptr) {
  if (ptr == 0) {
    why_are_you_using_me::dummy = 0;
    return why_are_you_using_me::dummy;
  }
  return *(size_t *)get_real_mem(ptr);
}
constexpr inline size_t get_needed_size_of_mem(size_t size) {
  return sizeof(size_t) + size;
}

inline void *realloc_pv(void *p, size_t s) {
  void *p2 = get_fake_mem(realloc(get_real_mem(p), get_needed_size_of_mem(s)));
  get_size_of_mem(p2) = s;
  if (p) {
    log("freed", get_size_of_mem(p), p);
    log("reallocated", s, p2);
  } else {
    log("allocated", s, p2);
  }
  return p2;
}
inline void free_pv(void *p) {
  namespace pv = private_dont_use_it;
  if (p) {
    log("freed", get_size_of_mem(p), p);
    free(get_real_mem(p));
  } else {
    log("freed !(BUG)! ", 0, nullptr);
  }
}
}  // namespace log_functions
namespace no_log_functions {
inline void free_pv(void *p) { return free(p); }

inline void *realloc_pv(void *p, size_t s) { return realloc(p, s); }
}  // namespace no_log_functions

inline void free_pv(void *p) {
  if (log_it) {
    return log_functions::free_pv(p);
  }
  return no_log_functions::free_pv(p);
}

inline void *realloc_pv(void *p, size_t s) {
  if (log_it) {
    return log_functions::realloc_pv(p, s);
  }
  return no_log_functions::realloc_pv(p, s);
}
}  // namespace private_dont_use_it
inline void free(void *p) {
  namespace pv = private_dont_use_it;
  pv::free_pv(p);
}
inline void *realloc(void *p, size_t n) {
  namespace pv = private_dont_use_it;
  return pv::realloc_pv(p, n);
}
class C_realloc_free_package_example {
 public:
  inline void free(void *ptr) { mjz_ard::free(ptr); }
  inline void *realloc(void *ptr, size_t needed_len) {
    return mjz_ard::realloc(ptr, needed_len);
  }
};
template <class C_realloc_free_package_t = C_realloc_free_package_example>
struct C_allocate_free_warpper {
 public:
  inline C_allocate_free_warpper() {}
  inline ~C_allocate_free_warpper() {}
  inline C_allocate_free_warpper(void *p, size_t n) : my_c_allocator(p, n) {}
  inline void free(void *ptr, size_t len) { my_c_allocator.free(ptr); }
  inline void *realloc(void *ptr, size_t, size_t needed_len) {
    return my_c_allocator.realloc(ptr, needed_len);
  }

 private:
  C_realloc_free_package_t my_c_allocator;
};

template <class std_allocator_for_char>
struct std_reallocator_free_realloc_warper {
  using char_type = mjz_get_value_Type<std_allocator_for_char>;
  inline std_reallocator_free_realloc_warper(void *p, size_t n)
      : my_std_allocator_(p, n) {}
  inline std_reallocator_free_realloc_warper() {}
  inline ~std_reallocator_free_realloc_warper() {}

 public:
  inline void free(void *ptr, size_t len) {
    my_std_allocator_.deallocate((char_type *)ptr, len);
  }
  inline void *realloc(void *ptr, size_t len, size_t needed_len) {
    void *ptr2 = my_std_allocator_.allocate(needed_len);
    bool was_not_null = ptr && len;
    if (ptr2 && was_not_null) {
      memmove(ptr2, ptr, min(needed_len, len));
    }
    if (was_not_null) {
      my_std_allocator_.deallocate((char_type *)ptr, len);
    }
    return ptr2;
  }

 private:
  std_allocator_for_char my_std_allocator_;
};

template <class std_allocator_for_char, typename Type>
struct std_reallocator_warper
    : public mjz_reallocator_template_t<
          std_reallocator_free_realloc_warper<std_allocator_for_char>, Type> {
  inline std_reallocator_warper(void *p, size_t n)
      : mjz_reallocator_template_t<
            std_reallocator_free_realloc_warper<std_allocator_for_char>, Type>(
            p, n) {}
  inline std_reallocator_warper() {}
  inline ~std_reallocator_warper() {}
};

template <class C_realloc_free_package_t, typename Type>
struct C_reallocator_warper_t
    : public mjz_reallocator_template_t<
          C_allocate_free_warpper<C_realloc_free_package_t>, Type> {
  inline C_reallocator_warper_t() {}
  inline C_reallocator_warper_t(void *p, size_t n)
      : mjz_reallocator_template_t<
            C_allocate_free_warpper<C_realloc_free_package_t>, Type>(p, n) {}
  inline ~C_reallocator_warper_t() {}
};

template <typename Type>
struct C_reallocator_warper
    : public mjz_reallocator_template_t<
          C_allocate_free_warpper<C_realloc_free_package_example>, Type> {
  inline C_reallocator_warper() {}
  inline ~C_reallocator_warper() {}
  inline C_reallocator_warper(void *p, size_t n)
      : mjz_reallocator_template_t<
            C_allocate_free_warpper<C_realloc_free_package_example>, Type>(p,
                                                                           n) {}
};

template <typename Type,
          size_t number_of_blocks =
              number_of_global_mjz_areana_allocator_blocks,
          size_t block_length = size_of_global_mjz_areana_allocator_blocks,
          bool KEEP_the_heap_clean = false>
struct mjz_arena_allocator_warper
    : public C_reallocator_warper_t<
          mjz_arena_allocator_t<number_of_blocks, block_length,
                                KEEP_the_heap_clean>,
          Type> {
  mjz_arena_allocator_warper() {}
  mjz_arena_allocator_warper(void *p, size_t n)
      : C_reallocator_warper_t<
            mjz_arena_allocator_t<number_of_blocks, block_length,
                                  KEEP_the_heap_clean>,
            Type>(p, n) {}
  ~mjz_arena_allocator_warper() {}
};

template <class Type>
struct basic_mjz_allocator
    : std_reallocator_warper<std::allocator<uint8_t>, Type> {
  inline basic_mjz_allocator(){};
  inline basic_mjz_allocator(void *p, size_t n)
      : std_reallocator_warper<std::allocator<uint8_t>, Type>(p, n){};
  inline ~basic_mjz_allocator(){};
  template <class T>
  inline basic_mjz_allocator(const basic_mjz_allocator<T> &) noexcept {}
};

template <class Type, class U>
constexpr inline bool operator==(const basic_mjz_allocator<Type> &,
                                 const basic_mjz_allocator<U> &) {
  return true;
}

template <class Type, class U>
constexpr inline bool operator!=(const basic_mjz_allocator<Type> &,
                                 const basic_mjz_allocator<U> &) {
  return false;
}

template <typename Type>
struct mjz_obj_manager_template_t {
  using me = mjz_obj_manager_template_t;

 public:
  template <typename... args_t>
  static constexpr inline Type *construct_at(Type *dest,
                                             args_t &&...args) noexcept {
    Type *ptr{};
    try {
      ptr = (new (dest) Type(std::forward<args_t>(args)...));
    } catch (...) {
    }
    return ptr;
  }
  template <typename... args_t>
  static constexpr inline Type *construct_array_at(Type *dest, size_t n,
                                                   args_t &&...args) noexcept {
    Type *ptr{dest};
    Type *end{dest + n - 1};
    try {
      while (ptr < end) construct_at(ptr++, args...);
      end++;
      if (ptr < end) construct_at(ptr, std::forward<args_t>(args)...);
    } catch (...) {
      return nullptr;
    }
    return dest;
  }
  template <typename... args_t>
  static constexpr inline bool destruct_array_at(Type *dest, size_t n,
                                                 args_t &&...args) noexcept {
    Type *r_end = dest;
    Type *ptr = dest + n;
    bool success = true;
    while (r_end < ptr) {
      try {
        while (r_end < ptr) destroy_at(--ptr);
      } catch (...) {
        success = false;
      }
    }
    return success;
  }
  template <typename... args_t>
  [[nodiscard]] static constexpr inline Type obj_constructor(args_t &&...args) {
    return Type(std::forward<args_t>(args)...);
  }
  template <typename... args_t>
  [[nodiscard]] static constexpr inline Type &&obj_constructor_on(
      Type &&uninitilized_object, args_t &&...args) {
    return std::move(
        *construct_at(&uninitilized_object, std::forward<args_t>(args)...));
  }
  template <typename... args_t>
  [[nodiscard]] static constexpr inline Type &obj_constructor_on(
      Type &uninitilized_object, args_t &&...args) {
    return *construct_at(&uninitilized_object, std::forward<args_t>(args)...);
  }
  template <typename... args_t>
  [[nodiscard]] static constexpr inline Type *obj_constructor_on(
      Type *uninitilized_object, args_t &&...args) {
    return construct_at(uninitilized_object, std::forward<args_t>(args)...);
  }
  static constexpr inline void obj_destructor_on(
      Type &&obj_that_will_be_destroyed) {
    destroy_at(obj_that_will_be_destroyed);
  }
  static constexpr inline void obj_destructor_on(
      Type &obj_that_will_be_destroyed) {
    destroy_at(obj_that_will_be_destroyed);
  }
  static constexpr inline void obj_destructor_on(
      Type *obj_that_will_be_destroyed) {
    destroy_at(obj_that_will_be_destroyed);
  }
  union simple_init_obj_wrpr {
    simple_init_obj_wrpr() { obj_constructor_on(address_()); }
    template <class T0, class... Ts>
    simple_init_obj_wrpr(T0 &&arg0, Ts &&...args) {
      obj_constructor_on(address_(), std::forward<T0>(arg0),
                         std::forward<Ts>(args)...);
    }
    ~simple_init_obj_wrpr() { obj_destructor_on(address_()); }
    Type obj;
    Type *address_() { return (Type *)&NO_USE_; }  // union
   private:
    char NO_USE_;
  };
  template <class T, class... Args>
  static constexpr inline void construct(mjz_obj_manager_template_t &a, T *p,
                                         Args &&...args) {
    a.construct_at(p, std::forward<Args>(args)...);
  }
  static constexpr inline Type &obj_move_to_obj(Type &dest, Type &&src) {
    return dest = (std::move(src));
  }
  static constexpr inline Type &obj_copy_to_obj(Type &dest, Type &src) {
    return dest = (src);
  }
  static constexpr inline Type &obj_copy_to_obj(Type &dest, const Type &src) {
    return dest = (src);
  }
  static constexpr inline Type &obj_move_to_obj(Type *dest,
                                                Type *src) {  // src is &&
    return obj_move_to_obj(*dest, std::move(*src));
  }
  static constexpr inline Type &obj_copy_to_obj(Type *dest, Type *src) {
    return obj_copy_to_obj(*dest, *src);
  }
  static constexpr inline Type &obj_copy_to_obj(Type *dest, const Type *src) {
    return obj_copy_to_obj(*dest, *src);
  }
  static constexpr inline Type &obj_go_to_obj(Type &dest, const Type &src) {
    return obj_copy_to_obj(dest, src);
  }
  static constexpr inline Type &obj_go_to_obj(Type &dest, Type &src) {
    return obj_copy_to_obj(dest, src);
  }
  static constexpr inline Type &obj_go_to_obj(Type &dest, Type &&src) {
    return obj_move_to_obj(dest, std::move(src));
  }
  template <typename... Args>
  static constexpr inline Type &obj_equals(Type &dest, Args &&...args) {
    return dest.operator=(std::forward<Args>(args)...);
  }
  friend constexpr inline Type *addressof(Type &obj) { return &obj; }
  friend constexpr inline const Type *addressof(const Type &obj) {
    return &obj;
  }
  static constexpr inline Type *addressof(Type &obj) { return &obj; }
  static constexpr inline const Type *addressof(const Type &obj) {
    return &obj;
  }
  friend constexpr inline Type *to_address(Type *p) noexcept { return p; }
  friend constexpr inline const Type *to_address(const Type &obj) noexcept {
    return &obj;
  }
  friend constexpr inline const Type *to_address(const Type *p) noexcept {
    return p;
  }
  friend constexpr inline Type *to_address(Type &obj) noexcept { return &obj; }
  friend constexpr inline Type *pointer_to(Type &r) noexcept { return &r; }
  static constexpr inline bool destroy_at(Type *ptr) noexcept {
    try {
      ptr->~Type();
    } catch (...) {
      return false;
    }
    return true;
  }
  static constexpr inline bool destroy_at(Type &ptr) noexcept {
    try {
      ptr.~Type();
    } catch (...) {
      return false;
    }
    return true;
  }
  static constexpr inline void destroy(mjz_obj_manager_template_t &a, Type *p) {
    a.destroy_at(p);
  }
  static constexpr inline void destroy(Type *p) { me::destroy_at(p); }
  template <class ForwardIt, class Size>
  static constexpr inline ForwardIt destroy_n(ForwardIt first, Size n) {
    for (; n > 0; (void)++first, --n) me::destroy_at(&(*first));
    return first;
  }
  template <class ForwardIt>
  static constexpr inline void destroy(ForwardIt first, ForwardIt last) {
    for (; first != last; ++first) me::destroy_at(&(*first));
  }
};

template <typename Type,
          class my_constructor = mjz_obj_manager_template_t<Type>>

struct mjz_temp_type_obj_creator_warpper_t : public my_constructor {
  using me = mjz_temp_type_obj_creator_warpper_t;
  constexpr static inline size_t size_of_type() { return sizeof(Type); }
  static constexpr inline void swap(Type &a, Type &b) {
    uint8_t buf[size_of_type()];
    Type *temp_p = (Type *)buf;
    me::construct_at(temp_p, std::move(a));
    me::obj_move_to_obj(a, std::move(b));
    me::obj_move_to_obj(b, std::move(*temp_p));
    me::destroy_at(temp_p);
  }
  static constexpr inline bool obj_destructor_arr(Type *arr, size_t n,
                                                  bool in_reveres = 1) {
    bool was_successful{1};
    if (in_reveres) {
      Type *ptr = arr + n;
      Type *ptr_end = arr - 1;
      while ((--ptr) > ptr_end) {
        was_successful &= my_constructor::destroy_at(ptr);
      }
    } else {
      Type *ptr = arr - 1;
      Type *ptr_end = arr + n;
      while ((++ptr) < ptr_end) {
        was_successful &= my_constructor::destroy_at(ptr);
      }
    }
    return was_successful;
  }
  template <typename... args_t>
  static constexpr inline Type *construct_arr_at(Type *dest, size_t n,
                                                 bool in_reveres,
                                                 args_t &&...args) {
    if (in_reveres) {
      Type *ptr_end = dest - 1;
      Type *ptr = dest + n;
      while ((--ptr) > ptr_end) {
        my_constructor::construct_at(ptr, args...);
      }
      return dest;
    } else {
      return my_constructor::construct_array_at(dest, n,
                                                std::forward<args_t>(args)...);
    }
  }
  static constexpr inline Type *construct_arr_at(Type *dest, size_t n,
                                                 bool in_reveres = 0) {
    if (in_reveres) {
      Type *ptr_end = dest - 1;
      Type *ptr = dest + n;
      while ((--ptr) > ptr_end) {
        my_constructor::construct_at(ptr);
      }
      return dest;
    } else {
      my_constructor::construct_array_at(dest, n);
    }
  }
};

template <typename Type,
          class my_constructor = mjz_obj_manager_template_t<Type>>
struct mjz_temp_type_obj_algorithims_warpper_t
    : public mjz_temp_type_obj_creator_warpper_t<Type, my_constructor> {
  using me = mjz_temp_type_obj_algorithims_warpper_t;
  template <typename TTT>
  static constexpr inline auto addressof(const TTT &obj) {
    return &obj;
  }
  template <typename TTT>
  static constexpr inline auto addressof(const TTT &&obj) = delete;
  template <typename TTT>
  static constexpr inline auto addressof(TTT &obj) {
    return &obj;
  }
  template <class InputIt, class Size, class NoThrowForwardIt>
  static constexpr inline NoThrowForwardIt uninitialized_copy_n(
      InputIt first, Size count, NoThrowForwardIt d_first) {
    using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
    NoThrowForwardIt current = d_first;
    try {
      for (; count > 0; ++first, (void)++current, --count)
        construct_at(addressof(*current), *first);
    } catch (...) {
      for (; d_first != current; ++d_first) d_first->~T();
      throw;
    }
    return current;
  }
  template <class ForwardIt, class T>
  static constexpr inline void uninitialized_fill(ForwardIt first,
                                                  ForwardIt last,
                                                  const T &value) {
    using V = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
      for (; current != last; ++current)
        me::construct_at(addressof(*current), value);
    } catch (...) {
      for (; first != current; ++first) first->~V();
      throw;
    }
  }
  template <class ForwardIt, class Size, class T>
  static constexpr inline ForwardIt uninitialized_fill_n(ForwardIt first,
                                                         Size count,
                                                         const T &value) {
    using V = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
      for (; count > 0; ++current, (void)--count)
        me::construct_at(addressof(*current), value);
      return current;
    } catch (...) {
      for (; first != current; ++first) first->~V();
      throw;
    }
  }
  template <class ForwardIt, class T>
  static constexpr inline void uninitialized_fill(ForwardIt first,
                                                  ForwardIt last, T &&value) {
    using V = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
      if (current != last) {
        construct_at(addressof(*first), value);
        ++current;
        for (; current != last; ++current)
          construct_at(addressof(*current), *first);
      }
    } catch (...) {
      for (; first != current; ++first) first->~V();
      throw;
    }
  }
  template <class ForwardIt, class Size, class T>
  static constexpr inline ForwardIt uninitialized_fill_n(ForwardIt first,
                                                         Size count,
                                                         T &&value) {
    using V = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
      if (count > 0) {
        construct_at(addressof(*first), std::move(value));
        ++current;
        (void)--count;
        for (; count > 0; ++current, (void)--count)
          construct_at(addressof(*current), *first);
      }
      return current;
    } catch (...) {
      for (; first != current; ++first) first->~V();
      throw;
    }
  }
  template <class InputIt, class NoThrowForwardIt>
  static constexpr inline NoThrowForwardIt uninitialized_move(
      InputIt first, InputIt last, NoThrowForwardIt d_first) {
    NoThrowForwardIt current = d_first;
    try {
      for (; first != last; ++first, (void)++current)
        me::construct_at(addressof(*current), std::move(*first));
      return current;
    } catch (...) {
      me::destroy(d_first, current);
      throw;
    }
  }
  template <class InputIt, class Size, class NoThrowForwardIt>
  static constexpr inline std::pair<InputIt, NoThrowForwardIt>
  uninitialized_move_n(InputIt first, Size count, NoThrowForwardIt d_first) {
    NoThrowForwardIt current = d_first;
    try {
      for (; count > 0; ++first, (void)++current, --count)
        me::construct_at(addressof(*current), std::move(*first));
    } catch (...) {
      me::destroy(d_first, current);
      throw;
    }
    return {first, current};
  }
  template <class InputIt, class NoThrowForwardIt>
  static constexpr inline NoThrowForwardIt uninitialized_copy(
      InputIt first, InputIt last, NoThrowForwardIt d_first) {
    NoThrowForwardIt current = d_first;
    try {
      for (; first != last; ++first, (void)++current)
        construct_at(addressof(*current), *first);
      return current;
    } catch (...) {
      for (; d_first != current; ++d_first) d_first->~T();
      throw;
    }
  }
  template <class ForwardIt>
  static constexpr inline void uninitialized_default_construct(ForwardIt first,
                                                               ForwardIt last) {
    ForwardIt current = first;
    try {
      for (; current != last; ++current) {
        construct_at(addressof(*current));
      }
    } catch (...) {
      destroy(first, current);
      throw;
    }
  }
  template <class ForwardIt>
  static constexpr inline void uninitialized_value_construct(ForwardIt first,
                                                             ForwardIt last) {
    ForwardIt current = first;
    try {
      for (; current != last; ++current) construct_at(addressof(*current));
    } catch (...) {
      destroy(first, current);
      throw;
    }
  }
  template <class ForwardIt, class Size>
  static constexpr inline ForwardIt uninitialized_default_construct_n(
      ForwardIt first, Size n) {
    ForwardIt current = first;
    try {
      for (; n > 0; (void)++current, --n) construct_at(addressof(*current));
      return current;
    } catch (...) {
      destroy(first, current);
      throw;
    }
  }
  template <class ForwardIt, class Size>
  static constexpr inline ForwardIt uninitialized_value_construct_n(
      ForwardIt first, Size n) {
    ForwardIt current = first;
    try {
      for (; n > 0; (void)++current, --n) construct_at(addressof(*current));
      return current;
    } catch (...) {
      destroy(first, current);
      throw;
    }
  }
  template <class BidirIt1, class BidirIt2, class BidirIt3>
  static constexpr inline BidirIt3 move_backward(BidirIt1 first, BidirIt2 last,
                                                 BidirIt3 d_last) {
    while (first != last)
      me::obj_move_to_obj(*(--d_last), std::move(*(--last)));
    return d_last;
  }
  template <class BidirIt1, class BidirIt2>
  static constexpr inline BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last,
                                                 BidirIt2 d_last) {
    while (first != last) obj_copy_to_obj(*(--d_last), std::forward(*(--last)));
    return d_last;
  }
  template <class InputIt, class OutputIt>
  static constexpr inline OutputIt copy(InputIt first, InputIt last,
                                        OutputIt d_first) {
    for (; first != last; (void)++first, (void)++d_first)
      obj_copy_to_obj(*d_first, *first);
    return d_first;
  }
  template <class InputIt, class OutputIt, class UnaryPredicate>
  static constexpr inline OutputIt copy_if(InputIt first, InputIt last,
                                           OutputIt d_first,
                                           UnaryPredicate pred) {
    for (; first != last; ++first)
      if (pred(*first)) {
        obj_copy_to_obj(*d_first, *first);
        ++d_first;
      }
    return d_first;
  }
};

template <typename Type,
          class my_constructor = mjz_obj_manager_template_t<Type>,
          class my_reallocator = basic_mjz_allocator<Type>>

struct mjz_temp_type_allocator_warpper_t
    : public mjz_temp_type_obj_algorithims_warpper_t<Type, my_constructor>,
      protected my_reallocator {
  using me = mjz_temp_type_allocator_warpper_t;

 public:
  using my_value_Type_t = Type;
  using value_type = my_value_Type_t;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using const_reference = const my_value_Type_t &;
  using size_type = size_t;
  using propagate_on_container_move_assignment = std::true_type;
  constexpr inline mjz_temp_type_allocator_warpper_t(){};
  constexpr inline mjz_temp_type_allocator_warpper_t(void *p, size_t n)
      : my_reallocator(p, n){};
  constexpr inline ~mjz_temp_type_allocator_warpper_t(){};
  template <class U>
  constexpr inline mjz_temp_type_allocator_warpper_t(
      const mjz_temp_type_allocator_warpper_t<U> &) noexcept {}
  [[nodiscard]] Type *allocate(size_t n) { return allocate(n, (Type *)0); }
  [[nodiscard]] static constexpr inline Type *allocate(
      mjz_temp_type_allocator_warpper_t &a, size_t n, const void *hint) {
    return (Type *)a.allocate(n, hint);
  }
  [[nodiscard]] static constexpr inline Type *allocate(
      mjz_temp_type_allocator_warpper_t &a, size_t n) {
    return (Type *)a.allocate(n);
  }
  static constexpr inline void deallocate(mjz_temp_type_allocator_warpper_t &a,
                                          Type *p, size_t n) noexcept {
    a.deallocate(p, n);
  }
  constexpr inline void deallocate(Type *p, size_t n) noexcept {
    deallocate_pv(p, n);
  }
  constexpr [[nodiscard]] inline Type *allocate(size_t n, const void *hint) {
    return allocate_pv(n, hint);
  }
  template <typename... args_t>
  constexpr [[nodiscard]] inline Type *allocate_obj(args_t &&...args) {
    // new Type(std::forward<args_t>(args)...);
    Type *ptr = allocate_pv(1, 0);
    if (!ptr) return nullptr;
    *get_real_array_ptr<size_t>(ptr) = 1;
    return my_constructor::construct_at(ptr, std::forward<args_t>(args)...);
  }
  template <typename... args_t>
  constexpr [[nodiscard]] inline Type *allocate_obj_array(size_t len,
                                                          bool in_reveres,
                                                          args_t &&...args) {
    // new Type(std::forward<args_t>(args)...)[len];
    Type *ptr = allocate_pv(len, 0);
    if (!ptr) return nullptr;
    *get_real_array_ptr<size_t>(ptr) = len;
    return me::construct_arr_at(ptr, len, in_reveres,
                                std::forward<args_t>(args)...);
  }
  template <typename... args_t>
  constexpr [[nodiscard]] inline Type *allocate_obj_array(size_t len,
                                                          bool in_reveres = 0) {
    Type *ptr = allocate_pv(len, 0);
    if (!ptr) return nullptr;
    *get_real_array_ptr<size_t>(ptr) = len;
    return me::construct_arr_at(ptr, len, in_reveres);
  }
  constexpr inline size_t get_number_of_obj_in_array(Type *ptr) {
    return *get_real_array_ptr<size_t>(ptr);
  }
  constexpr inline size_t size_of_array_with(size_t len) {
    return sizeof(size_t) + this->size_of_type() * len;
  }
  constexpr inline size_t size_of_array_with(Type *ptr) {
    return sizeof(size_t) +
           this->size_of_type() * get_number_of_obj_in_array(ptr);
  }
  constexpr inline bool deallocate_obj(Type *ptr) {
    return deallocate_obj_array(ptr);
  }
  constexpr inline bool deallocate_obj_array(Type *ptr, bool in_reveres = 1) {
    // delete[] dest;
    bool was_successful = this->obj_destructor_arr(
        ptr, get_number_of_obj_in_array(ptr), in_reveres);
    deallocate_pv(ptr, size_of_array_with(ptr), 1);
    return was_successful;
  }

 private:
  template <typename T_as, typename T_from>
  constexpr inline T_as *get_real_array_ptr(T_from *fake) {
    if (fake == 0) return 0;
    return (T_as *)(((size_t *)fake) - 1);
  }
  template <typename T_as, typename T_from>
  constexpr inline T_as *get_fake_array_ptr(T_from *real) {
    if (real == 0 || (void *)real == (void *)sizeof(size_t)) return 0;
    return (T_as *)(((size_t *)real) + 1);
  }
  constexpr [[nodiscard]] inline Type *allocate_pv(size_t n, const void *hint,
                                                   bool raw = 0) {
    return get_fake_array_ptr<Type>(my_reallocator::allocate_raw(
        raw ? n : size_of_array_with(n), get_real_array_ptr<Type>(hint)));
  }
  constexpr inline void deallocate_pv(Type *p, size_t n,
                                      bool raw = 0) noexcept {
    my_reallocator::deallocate_raw(get_real_array_ptr<Type>(p),
                                   raw ? n : size_of_array_with(n));
  }
  template <typename _Type, class _constructor, class _reallocator>
  constexpr inline friend bool operator==(
      mjz_temp_type_allocator_warpper_t<Type, my_constructor, my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _constructor, _reallocator>) {
    return false;
  }
  template <typename _Type, class _constructor, class _reallocator>
  constexpr inline friend bool operator!=(
      mjz_temp_type_allocator_warpper_t<Type, my_constructor, my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _constructor, _reallocator>) {
    return true;
  }
  template <typename _Type, class _constructor>
  constexpr inline friend bool operator==(
      mjz_temp_type_allocator_warpper_t<Type, my_constructor, my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _constructor, my_reallocator>) {
    return true;
  }
  template <typename _Type, class _constructor>
  constexpr inline friend bool operator!=(
      mjz_temp_type_allocator_warpper_t<Type, my_constructor, my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _constructor, my_reallocator>) {
    return false;
  }
};

template <typename Type,
          class my_constructor = mjz_obj_manager_template_t<Type>,
          class C_free_realloc = C_realloc_free_package_example>
struct C_mjz_temp_type_allocator_warpper_t
    : public mjz_temp_type_allocator_warpper_t<
          Type, my_constructor, C_reallocator_warper_t<C_free_realloc, Type>> {
  constexpr inline C_mjz_temp_type_allocator_warpper_t() {}
  constexpr inline ~C_mjz_temp_type_allocator_warpper_t() {}
  constexpr inline C_mjz_temp_type_allocator_warpper_t(void *p, size_t n)
      : mjz_temp_type_allocator_warpper_t<
            Type, my_constructor, C_reallocator_warper_t<C_free_realloc, Type>>(
            p, n) {}
};

template <typename Type, class my_reallocator = basic_mjz_allocator<Type>>
using mjz_allocator_warpper_r_t =
    mjz_temp_type_allocator_warpper_t<Type, mjz_obj_manager_template_t<Type>,
                                      my_reallocator>;
template <class Type>
struct mjz_allocator_warpper : mjz_allocator_warpper_r_t<Type> {
  constexpr mjz_allocator_warpper(){};
  constexpr inline mjz_allocator_warpper(void *p, size_t n)
      : mjz_allocator_warpper_r_t<Type>(p, n){};
  constexpr inline ~mjz_allocator_warpper(){};
  template <class T>
  constexpr mjz_allocator_warpper(const mjz_allocator_warpper<T> &) noexcept {}
};
struct mjz_normal_allocator : public C_mjz_temp_type_allocator_warpper_t<char> {
  constexpr inline mjz_normal_allocator() {}
  constexpr inline ~mjz_normal_allocator() {}
  constexpr inline mjz_normal_allocator(const mjz_normal_allocator &) {}
  constexpr inline mjz_normal_allocator &operator=(
      const mjz_normal_allocator &) {
    return *this;
  }
  constexpr inline mjz_normal_allocator(mjz_normal_allocator &&) {}
  constexpr inline mjz_normal_allocator &operator=(mjz_normal_allocator &&) {
    return *this;
  }
};
#if global_mjz_areana_allocator_on
}
constexpr inline mjz_ard::C_realloc_free_package_example &&
get_CPP_local_global_allocator() {
  return mjz_ard::C_realloc_free_package_example();
}
[[nodiscard]] inline void *operator new(size_t m_size) {
  void *p = get_CPP_local_global_allocator().realloc(0, m_size);
  if (p) return p;
  throw std::runtime_error("no memory left ");
}
[[nodiscard]] inline void *operator new[](size_t m_size) {
  void *p = get_CPP_local_global_allocator().realloc(0, m_size);
  if (p) return p;
  throw std::runtime_error("no memory left ");
}
inline void operator delete(void *p) {
  get_CPP_local_global_allocator().free(p);
}
inline void operator delete[](void *p) {
  get_CPP_local_global_allocator().free(p);
}
inline void operator delete(void *p, size_t) {
  get_CPP_local_global_allocator().free(p);
}
inline void operator delete[](void *p, size_t) {
  get_CPP_local_global_allocator().free(p);
}
namespace mjz_ard {
#endif  // global_mjz_areana_allocator_on

template <class Type, size_t m_Size, bool error_check = 1>
struct mjz_Array {  // fixed size mjz_Array of values
 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  using iterator = iterator_template_t<Type, error_check>;
  using const_iterator = iterator_template_t<const Type, error_check>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  constexpr inline void assign(const Type &_Value) { fill(_Value); }
  constexpr inline void fill(const Type &value) {
    iterator fst = begin();
    iterator lst = end();
    while (fst < lst) *fst++ = value;
  }
  constexpr inline void swap(mjz_Array &other) {
    iterator fst[2] = {begin(), other.begin()};
    iterator lst[2] = {end(), other.end()};
    while ((fst[0] < lst[0]) && (fst[1] < lst[1]))
      std::swap(*fst[0]++, *fst[1]++);
  }
  constexpr [[nodiscard]] inline iterator begin() noexcept {
    return iterator(m_elements, m_elements, m_elements + m_Size);
  }
  constexpr [[nodiscard]] inline const_iterator begin() const noexcept {
    return const_iterator(m_elements, m_elements, m_elements + m_Size);
  }
  constexpr [[nodiscard]] inline iterator end() noexcept {
    return iterator(m_elements + m_Size, m_elements, m_elements + m_Size);
  }
  constexpr [[nodiscard]] inline const_iterator end() const noexcept {
    return const_iterator(m_elements + m_Size, m_elements, m_elements + m_Size);
  }
  constexpr [[nodiscard]] inline reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rbegin()
      const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr [[nodiscard]] inline const_iterator cend() const noexcept {
    return end();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crbegin()
      const noexcept {
    return rbegin();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr [[nodiscard]] inline size_type size() const noexcept {
    return m_Size;
  }
  constexpr [[nodiscard]] inline size_type max_size() const noexcept {
    return m_Size;
  }
  constexpr inline bool empty() const noexcept { return !!m_Size; }
  constexpr [[nodiscard]] inline reference at(size_type _Pos) {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return m_elements[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference at(size_type _Pos) const {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return m_elements[_Pos];
  }
  constexpr [[nodiscard]] inline reference operator[](size_type _Pos) noexcept
  /* strengthened */ {
    if constexpr (error_check) {
      if (_Pos >= m_Size) {
        throw std::out_of_range{"mjz_Array subscript out of range"};
      }
    }
    return m_elements[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference operator[](
      size_type _Pos) const noexcept
  /* strengthened */ {
    if constexpr (error_check) {
      {
        if (_Pos >= m_Size)
          throw std::out_of_range{"mjz_Array subscript out of range"};
      }
    }
    return m_elements[_Pos];
  }
  constexpr [[nodiscard]] inline reference front() noexcept
  /* strengthened */ {
    return m_elements[0];
  }
  constexpr [[nodiscard]] inline const_reference front() const noexcept
  /* strengthened */ {
    return m_elements[0];
  }
  constexpr [[nodiscard]] inline reference back() noexcept
  /* strengthened */ {
    return m_elements[m_Size - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept
  /* strengthened */ {
    return m_elements[m_Size - 1];
  }
  constexpr [[nodiscard]] inline Type *data() noexcept { return m_elements; }
  constexpr [[nodiscard]] inline const Type *data() const noexcept {
    return m_elements;
  }
  [[noreturn]] void invld_throw() const {
    throw std::out_of_range("invalid mjz_Array<T, N> subscript");
  }
  static mjz_temp_type_obj_algorithims_warpper_t<Type> mjz_obj_algorithims;
  constexpr inline mjz_Array() {}
  constexpr inline ~mjz_Array() {}
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array(mjz_Array<Type, O_size, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims.uninitialized_move_n(other.begin(), len, this->begin());
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array(const mjz_Array<Type, O_size, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims.uninitialized_copy_n(other.begin(), len, this->begin());
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      mjz_Array<Type, O_size, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims.move_backward(other.begin(), other.begin() + len,
                                      this->end());
    return *this;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      const mjz_Array<Type, O_size, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims.copy_backward(other.begin(), other.begin() + len,
                                      this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_Array(mjz_Array<Type, m_Size, O_err> &&other) {
    mjz_obj_algorithims.uninitialized_move_n(other.begin(), other.size(),
                                             this->begin());
  }
  template <bool O_err>
  constexpr inline mjz_Array(const mjz_Array<Type, m_Size, O_err> &other) {
    mjz_obj_algorithims.uninitialized_copy_n(other.begin(), other.size(),
                                             this->begin());
  }
  template <bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      mjz_Array<Type, m_Size, O_err> &&other) {
    mjz_obj_algorithims.move_backward(
        other.begin(), other.begin() + this->size(), this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      const mjz_Array<Type, m_Size, O_err> &other) {
    mjz_obj_algorithims.copy_backward(
        other.begin(), other.begin() + this->size(), this->end());
    return *this;
  }
  Type m_elements[m_Size]{};
};

template <class Type, size_t m_Size, bool error_check>

mjz_temp_type_obj_algorithims_warpper_t<Type>
    mjz_Array<Type, m_Size, error_check>::mjz_obj_algorithims;
template <class Type, size_t m_Size,
          class my_obj_constructor =
              mjz_temp_type_obj_algorithims_warpper_t<Type>,
          bool error_check = 1>
struct mjz_obj_Array {  // fixed size mjz_obj_Array of values
  my_obj_constructor my_obj_cntr;

 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  using iterator = iterator_template_t<Type, error_check>;
  using const_iterator = iterator_template_t<const Type, error_check>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  void assign(const Type &_Value) { fill(_Value); }
  constexpr inline void fill(const Type &value) {
    iterator fst = begin();
    iterator lst = end();
    while (fst < lst) my_obj_cntr.obj_copy_to_obj(*fst++, value);
  }
  constexpr inline void swap(mjz_obj_Array &other) {
    iterator fst[2] = {begin(), other.begin()};
    iterator lst[2] = {end(), other.end()};
    while ((fst[0] < lst[0]) && (fst[1] < lst[1]))
      my_obj_cntr.swap(*fst[0]++, *fst[1]++);
  }
  constexpr [[nodiscard]] inline iterator begin() noexcept {
    return iterator(m_elements(), m_elements(), m_elements() + m_Size);
  }
  constexpr [[nodiscard]] inline const_iterator begin() const noexcept {
    return const_iterator(m_elements(), m_elements(), m_elements() + m_Size);
  }
  constexpr [[nodiscard]] inline iterator end() noexcept {
    return iterator(m_elements() + m_Size, m_elements(), m_elements() + m_Size);
  }
  constexpr [[nodiscard]] inline const_iterator end() const noexcept {
    return const_iterator(m_elements() + m_Size, m_elements(),
                          m_elements() + m_Size);
  }
  constexpr [[nodiscard]] inline reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rbegin()
      const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr [[nodiscard]] inline const_iterator cend() const noexcept {
    return end();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crbegin()
      const noexcept {
    return rbegin();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr [[nodiscard]] inline size_type size() const noexcept {
    return m_Size;
  }
  constexpr [[nodiscard]] inline size_type max_size() const noexcept {
    return m_Size;
  }
  constexpr inline bool empty() const noexcept { return !!m_Size; }
  constexpr [[nodiscard]] inline reference at(size_type _Pos) {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference at(size_type _Pos) const {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference operator[](size_type _Pos)
  /* strengthened */ {
    if constexpr (error_check) {
      if (_Pos >= m_Size) {
        throw std::out_of_range{"mjz_obj_Array subscript out of range"};
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference operator[](
      size_type _Pos) const
  /* strengthened */ {
    if constexpr (error_check) {
      {
        if (_Pos >= m_Size)
          throw std::out_of_range{"mjz_obj_Array subscript out of range"};
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference front() noexcept
  /* strengthened */ {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline const_reference front() const noexcept
  /* strengthened */ {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline reference back() noexcept
  /* strengthened */ {
    return m_elements()[m_Size - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept
  /* strengthened */ {
    return m_elements()[m_Size - 1];
  }
  constexpr [[nodiscard]] inline Type *data() noexcept { return m_elements(); }
  constexpr [[nodiscard]] inline const Type *data() const noexcept {
    return m_elements();
  }
  [[noreturn]] void invld_throw() const {
    throw std::out_of_range("invalid mjz_obj_Array<T, N> subscript");
  }
  constexpr inline mjz_obj_Array() { init_data(); }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_obj_Array(
      mjz_obj_Array<Type, O_size, my_obj_constructor, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.uninitialized_move_n(other.begin(), len, this->begin());
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_obj_Array(
      const mjz_obj_Array<Type, O_size, my_obj_constructor, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.uninitialized_copy_n(other.begin(), len, this->begin());
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_obj_Array &operator=(
      mjz_obj_Array<Type, O_size, my_obj_constructor, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.move_backward(other.begin(), other.begin() + len, this->end());
    return *this;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_obj_Array &operator=(
      const mjz_obj_Array<Type, O_size, my_obj_constructor, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.copy_backward(other.begin(), other.begin() + len, this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_obj_Array(
      mjz_obj_Array<Type, m_Size, my_obj_constructor, O_err> &&other) {
    my_obj_cntr.uninitialized_move_n(other.begin(), other.size(),
                                     this->begin());
  }
  template <bool O_err>
  constexpr inline mjz_obj_Array(
      const mjz_obj_Array<Type, m_Size, my_obj_constructor, O_err> &other) {
    my_obj_cntr.uninitialized_copy_n(other.begin(), other.size(),
                                     this->begin());
  }
  template <bool O_err>
  constexpr inline mjz_obj_Array &operator=(
      mjz_obj_Array<Type, m_Size, my_obj_constructor, O_err> &&other) {
    my_obj_cntr.move_backward(other.begin(), other.begin() + this->size(),
                              this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_obj_Array &operator=(
      const mjz_obj_Array<Type, m_Size, my_obj_constructor, O_err> &other) {
    my_obj_cntr.copy_backward(other.begin(), other.begin() + this->size(),
                              this->end());
    return *this;
  }
  constexpr inline ~mjz_obj_Array() { deinit_data(); }

 protected:
  constexpr inline void init_data() {
    my_obj_cntr.construct_arr_at(m_elements(), m_Size);
  }
  constexpr inline void deinit_data() {
    my_obj_cntr.obj_destructor_arr(m_elements(), m_Size);
  }
  constexpr inline Type *m_elements() { return (Type *)m_elements_data; }
  uint8_t m_elements_data[my_obj_constructor::size_of_type() * m_Size];
};

template <typename T, typename U = T>
constexpr inline T exchange(T &obj, U &&new_value) {
  T old_value = std::move(obj);
  obj = std::forward<U>(new_value);
  return old_value;
}

template <typename T, typename Allocator = mjz_allocator_warpper<T>>
struct mjz_Vector {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename Allocator::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  constexpr inline mjz_Vector() : mjz_Vector(Allocator{}) {}
  constexpr inline explicit mjz_Vector(const Allocator &a)
      : m_allocator{a}, m_size{0}, m_capacity{0}, m_data{nullptr} {}
  constexpr inline explicit mjz_Vector(size_type n, const T &val,
                                       const Allocator &a = Allocator{})
      : m_allocator{a},
        m_size{n},
        m_capacity{n},
        m_data{m_allocator.allocate(n)} {
    m_allocator.uninitialized_fill_n(m_data, n, val);
  }
  constexpr inline explicit mjz_Vector(size_type n,
                                       const Allocator &a = Allocator{})
      : m_allocator{a},
        m_size{n},
        m_capacity{n},
        m_data{m_allocator.allocate_obj_array(n)} {}
  template <typename InputIt>
  constexpr inline mjz_Vector(InputIt first, InputIt last,
                              const Allocator &a = Allocator{})
      : mjz_Vector(std::distance(first, last), a) {
    m_allocator.copy(first, last, begin());
  }
  constexpr inline mjz_Vector(const mjz_Vector &other)
      : m_allocator{other.m_allocator},
        m_size{other.m_size},
        m_capacity{other.m_capacity} {
    m_data = m_allocator.allocate(m_capacity);
    m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
  }
  constexpr inline mjz_Vector(const mjz_Vector &other, const Allocator &a)
      : m_allocator{a}, m_size{other.m_size}, m_capacity{other.m_capacity} {
    m_data = m_allocator.allocate(m_capacity);
    m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
  }
  constexpr inline mjz_Vector(mjz_Vector &&other) noexcept
      : m_allocator{std::move(other.m_allocator)},
        m_size{std::exchange(other.m_size, 0)},
        m_capacity{std::exchange(other.m_capacity, 0)},
        m_data{std::exchange(other.m_data, nullptr)} {}
  constexpr inline mjz_Vector(mjz_Vector &&other, const Allocator &a)
      : m_allocator{a},
        m_size{std::exchange(other.m_size, 0)},
        m_capacity{std::exchange(other.m_capacity, 0)},
        m_data{std::exchange(other.m_data, nullptr)} {}
  constexpr inline mjz_Vector(std::initializer_list<T> il,
                              const Allocator &a = Allocator{})
      : mjz_Vector(il.begin(), il.end(), a) {}
  constexpr inline ~mjz_Vector() {
    clear();
    deallocate();
  }
  constexpr inline mjz_Vector &operator=(const mjz_Vector &other) {
    if (this != &other) {
      clear();
      deallocate();
      m_allocator = other.m_allocator;
      m_size = other.m_size;
      m_capacity = other.m_capacity;
      m_data = m_allocator.allocate(m_capacity);
      m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
    }
    return *this;
  }
  constexpr inline mjz_Vector &operator=(mjz_Vector &&other) noexcept {
    if (this != &other) {
      clear();
      deallocate();
      m_allocator = std::move(other.m_allocator);
      m_size = std::exchange(other.m_size, 0);
      m_capacity = std::exchange(other.m_capacity, 0);
      m_data = std::exchange(other.m_data, nullptr);
    }
    return *this;
  }
  constexpr inline mjz_Vector &operator=(std::initializer_list<T> il) {
    *this = mjz_Vector(il);
    return *this;
  }
  constexpr inline reference operator[](size_type pos) { return m_data[pos]; }
  constexpr inline const_reference operator[](size_type pos) const {
    return m_data[pos];
  }
  constexpr inline reference at(size_type pos) {
    check_range(pos);
    return m_data[pos];
  }
  constexpr inline const_reference at(size_type pos) const {
    check_range(pos);
    return m_data[pos];
  }
  constexpr inline reference front() { return *begin(); }
  constexpr inline const_reference front() const { return *begin(); }
  constexpr inline reference back() { return *(end() - 1); }
  constexpr inline const_reference back() const { return *(end() - 1); }
  constexpr inline T *data() noexcept { return m_data; }
  constexpr inline const T *data() const noexcept { return m_data; }
  constexpr inline iterator begin() noexcept { return m_data; }
  constexpr inline const_iterator begin() const noexcept { return m_data; }
  constexpr inline iterator end() noexcept { return m_data + m_size; }
  constexpr inline const_iterator end() const noexcept {
    return m_data + m_size;
  }
  constexpr inline reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr inline const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr inline reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr inline const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr inline const_iterator cbegin() const noexcept { return begin(); }
  constexpr inline const_iterator cend() const noexcept { return end(); }
  constexpr inline const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }
  constexpr inline const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr inline bool empty() const noexcept { return m_size == 0; }
  constexpr inline size_type size() const noexcept { return m_size; }
  constexpr inline size_type max_size() const noexcept {
    return ((((size_type)-1) >> 1) -
            1);  // std::allocator_traits<Allocator>::max_size(m_allocator);
  }
  constexpr inline void reserve(size_type n) {
    if (n > m_capacity) reallocate(n);
  }
  constexpr inline size_type capacity() const noexcept { return m_capacity; }
  constexpr inline void shrink_to_fit() {
    if (m_size < m_capacity) reallocate(m_size);
  }
  constexpr inline void clear() noexcept {
    destroy(begin(), end());
    m_size = 0;
  }
  constexpr inline iterator insert(const_iterator pos, const T &val) {
    return insert(pos, 1, val);
  }
  constexpr inline iterator insert(const_iterator pos, T &&val) {
    emplace(pos, std::move(val));
    return pos;
  }
  constexpr inline iterator insert(const_iterator pos, size_type count,
                                   const T &val) {
    auto offset = pos - cbegin();
    reallocate(m_size + count);
    m_allocator.move_backward(begin() + offset, end() - count, end());
    m_allocator.uninitialized_fill_n(begin() + offset, count, val);
    m_size += count;
    return begin() + offset;
  }
  template <typename InputIt>
  constexpr inline iterator insert(const_iterator pos, InputIt first,
                                   InputIt last) {
    auto offset = pos - cbegin();
    auto count = std::distance(first, last);
    reallocate(m_size + count);
    m_allocator.move_backward(begin() + offset, end() - count, end());
    m_allocator.uninitialized_copy(first, last, begin() + offset);
    m_size += count;
    return begin() + offset;
  }
  constexpr inline iterator insert(const_iterator pos,
                                   std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
  }
  template <typename... Args>
  constexpr inline iterator emplace(const_iterator pos, Args &&...args) {
    auto offset = pos - cbegin();
    reallocate(m_size + 1);
    m_allocator.move_backward(begin() + offset, end() - 1, end());
    m_allocator.construct(m_allocator, m_data + offset,
                          std::forward<Args>(args)...);
    ++m_size;
    return begin() + offset;
  }
  constexpr inline iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  }
  constexpr inline iterator erase(const_iterator first, const_iterator last) {
    auto offset = first - cbegin();
    auto count = last - first;
    destroy(begin() + offset, end());
    std::move(begin() + offset + count, end(), begin() + offset);
    m_size -= count;
    return begin() + offset;
  }
  template <typename... Args>
  constexpr inline reference emplace_back(Args &&...args) {
    if (m_size == m_capacity) {
      reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
    m_allocator.construct_at(m_data + m_size, std::forward<Args>(args)...);
    ++m_size;
    return back();
  }
  constexpr inline void pop_back() {
    if (m_size > 0) {
      --m_size;
      m_allocator.destroy(m_data + m_size);
    }
  }
  constexpr inline bool add_size(size_type n) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    auto it = begin() + size();
    auto it_e = it + n;
    while (it < it_e) {
      m_allocator.construct_at(it);
      ++it;
    }
    m_size += n;
    return 1;
  }
  constexpr inline bool add_size(size_type n, const value_type &value) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    m_allocator.uninitialized_fill_n(begin() + size(), n, value);
    m_size += n;
    return 1;
  }
  constexpr inline bool add_size(size_type n, value_type &&value) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    if ((n - 1) > 0)
      m_allocator.uninitialized_fill_n(begin() + size(), n - 1, value);
    m_allocator.construct(m_allocator, begin() + size() + n - 1,
                          std::move(value));
    m_size += n;
    return 1;
  }
  constexpr inline bool resize(size_type n) {
    if (m_size < n) return add_size(n - m_size);
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, const value_type &value) {
    if (m_size < n) return add_size(n - m_size, std::move(value));
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, value_type &&value) {
    if (m_size < n) return add_size(n - m_size, std::move(value));
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline reference push_back(const T &value) {
    return emplace_back(value);
  }
  constexpr inline reference push_back(T &&value) {
    return emplace_back(std::move(value));
  }

 private:
  allocator_type m_allocator;
  size_type m_size;
  size_type m_capacity;
  pointer m_data;
  // Helper functions

  constexpr inline void deallocate() {
    if (m_data) {
      for (size_type i = 0; i < m_size; ++i) {
        m_allocator.destroy(m_data + i);
      }
      m_allocator.deallocate(m_data, m_capacity);
      m_data = nullptr;
      m_size = 0;
      m_capacity = 0;
    }
  }
  constexpr inline void destroy(iterator first, iterator last) {
    while (first < last) {
      m_allocator.destroy(first++);
    }
  }
  constexpr inline void reallocate(size_type newCapacity) {
    pointer newData = m_allocator.allocate(newCapacity);
    m_allocator.uninitialized_move_n(m_data, m_size, newData);
    deallocate();
    m_data = newData;
    m_capacity = newCapacity;
  }
  constexpr inline void check_range(size_type size) {
    if (m_size < size) throw std::out_of_range(" bad accesses");
  }
};

template <class Type, size_t m_capacity,
          class my_obj_constructor =
              mjz_temp_type_obj_algorithims_warpper_t<Type>,
          bool do_error_check = 1>
struct mjz_static_vector_template_t {
 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  using iterator = iterator_template_t<Type, do_error_check>;
  using const_iterator = iterator_template_t<const Type, do_error_check>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 protected:
  size_type m_length{};
  my_obj_constructor my_obj_cntr;
  constexpr inline bool init_data(size_type n) { return resize(n); }
  constexpr inline void deinit_data() {
    my_obj_cntr.obj_destructor_arr(m_elements(), size());
    m_length = 0;
  }
  constexpr inline Type *m_elements() { return (Type *)m_elements_data; }
  constexpr inline const Type *m_elements() const {
    return (Type *)m_elements_data;
  }
  uint8_t m_elements_data[my_obj_constructor::size_of_type() * m_capacity];
  constexpr inline bool is_full() { return size() >= max_size(); }
  constexpr inline bool is_full(size_type n) {
    return (size() + n) > max_size();
  }
  constexpr inline bool valid_itrator(const_iterator first,
                                      const_iterator last) {
    if (first >= last) return 1;
    if (first < begin() || end() < last) return 1;
    return 0;
  }
  constexpr inline bool error_check() {
    if constexpr (do_error_check) return is_full();
    return 0;
  }
  constexpr inline bool error_check(size_type n) {
    if constexpr (do_error_check) return is_full(n);
    return 0;
  }
  constexpr inline bool error_check(size_type n, const_iterator first,
                                    const_iterator last, bool do_n = true) {
    if constexpr (do_error_check)
      return (do_n && is_full(n)) || valid_itrator(first, last);
    return 0;
  }
  constexpr inline bool error_check(size_type n, const_iterator first,
                                    bool do_n = true) {
    if constexpr (do_error_check)
      return (do_n && is_full(n)) || (first < begin() || end() < first);
    return 0;
  }
  constexpr inline bool error_check(const_iterator first, const_iterator last,
                                    bool do_n = true) {
    if constexpr (do_error_check)
      return (do_n && is_full(std::distance(first, last))) ||
             valid_itrator(first, last);
    return 0;
  }
  constexpr inline bool error_check(const_iterator first, bool do_n = true) {
    if constexpr (do_error_check)
      return (do_n && is_full()) || (first < begin() || end() < first);
    return 0;
  }
  constexpr inline void swap_elms(Type &a, Type &b) { my_obj_cntr.swap(a, b); }

 public:
  constexpr inline void assign(const Type &_Value) { fill(_Value); }
  constexpr inline bool add_to_length(size_type n) {
    return init_data(m_length + n);
  }
  constexpr inline bool remove_from_length(size_type n) {
    if (m_length < n) return 0;
    if (m_length == n) {
      deinit_data();
      return 1;
    }
    return init_data(m_length - n);
  }
  constexpr inline void fill(const Type &value) {
    iterator fst = begin();
    iterator lst = end();
    while (fst < lst) my_obj_cntr.obj_copy_to_obj(*fst++, value);
  }
  constexpr inline void swap(mjz_static_vector_template_t &other) {
    iterator fst[2] = {begin(), other.begin()};
    iterator lst[2] = {end(), other.end()};
    while ((fst[0] < lst[0]) && (fst[1] < lst[1]))
      swap_elms(*fst[0]++, *fst[1]++);
  }
  constexpr [[nodiscard]] inline iterator begin() noexcept {
    return iterator(m_elements(), m_elements(), m_elements() + size());
  }
  constexpr [[nodiscard]] inline const_iterator begin() const noexcept {
    return const_iterator(m_elements(), m_elements(), m_elements() + size());
  }
  constexpr [[nodiscard]] inline iterator end() noexcept {
    return iterator(m_elements() + size(), m_elements(), m_elements() + size());
  }
  constexpr [[nodiscard]] inline const_iterator end() const noexcept {
    return const_iterator(m_elements() + size(), m_elements(),
                          m_elements() + size());
  }
  constexpr [[nodiscard]] inline reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rbegin()
      const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr [[nodiscard]] inline reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr [[nodiscard]] inline const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr [[nodiscard]] inline const_iterator cend() const noexcept {
    return end();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crbegin()
      const noexcept {
    return rbegin();
  }
  constexpr [[nodiscard]] inline const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr [[nodiscard]] inline size_type size() const noexcept {
    return m_length;
  }
  constexpr [[nodiscard]] inline size_type max_size() const noexcept {
    return capacity();
  }
  constexpr inline bool empty() const noexcept { return !!size(); }
  constexpr [[nodiscard]] inline reference at(size_type _Pos) {
    if constexpr (do_error_check) {
      if (size() <= _Pos) {
        invld_throw();
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference at(size_type _Pos) const {
    if constexpr (do_error_check) {
      if (size() <= _Pos) {
        invld_throw();
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference operator[](size_type _Pos)
  /* strengthened */ {
    if constexpr (do_error_check) {
      if (_Pos >= size()) {
        throw std::out_of_range{
            "mjz_static_vector_template_t subscript out of range"};
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference operator[](
      size_type _Pos) const
  /* strengthened */ {
    if constexpr (do_error_check) {
      {
        if (_Pos >= size())
          throw std::out_of_range{
              "mjz_static_vector_template_t subscript out of range"};
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference front() noexcept
  /* strengthened */ {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline const_reference front() const noexcept
  /* strengthened */ {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline reference back() noexcept
  /* strengthened */ {
    return m_elements()[size() - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept
  /* strengthened */ {
    return m_elements()[size() - 1];
  }
  constexpr [[nodiscard]] inline Type *data() noexcept { return m_elements(); }
  constexpr [[nodiscard]] inline const Type *data() const noexcept {
    return m_elements();
  }
  [[noreturn]] void invld_throw() const {
    throw std::out_of_range(
        "invalid mjz_static_vector_template_t<T, N> subscript");
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_static_vector_template_t(
      mjz_static_vector_template_t<Type, O_size, my_obj_constructor, O_err>
          &&other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.uninitialized_move_n(other.begin(), len, this->begin());
    m_length = len;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_static_vector_template_t(
      const mjz_static_vector_template_t<Type, O_size, my_obj_constructor,
                                         O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    my_obj_cntr.uninitialized_copy_n(other.begin(), len, this->begin());
    m_length = len;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_static_vector_template_t &operator=(
      mjz_static_vector_template_t<Type, O_size, my_obj_constructor, O_err>
          &&other) {
    size_t fill_len = min(other.size(), this->max_size());
    auto o_it_b = other.begin();
    if (fill_len < this->size()) {
      my_obj_cntr.move_backward(o_it_b, o_it_b + fill_len, this->end());
      init_data(fill_len);
      return *this;
    }
    size_t un_init_len = fill_len - this->size();
    my_obj_cntr.move_backward(o_it_b, o_it_b + this->size(), this->end());
    o_it_b += this->size();
    if (0 < un_init_len) {
      auto o_it_e = o_it_b + un_init_len;
      my_obj_cntr.uninitialized_move(o_it_b, o_it_e,
                                     this->begin() + this->size());
    }
    m_length = fill_len;
    return *this;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_static_vector_template_t &operator=(
      const mjz_static_vector_template_t<Type, O_size, my_obj_constructor,
                                         O_err> &other) {
    size_t fill_len = min(other.size(), this->max_size());
    auto o_it_b = other.begin();
    if (fill_len < this->size()) {
      my_obj_cntr.copy_backward(o_it_b, o_it_b + fill_len, this->end());
      init_data(fill_len);
      return *this;
    }
    size_t un_init_len = fill_len - this->size();
    my_obj_cntr.copy_backward(o_it_b, o_it_b + this->size(), this->end());
    o_it_b += this->size();
    if (0 < un_init_len) {
      auto o_it_e = o_it_b + un_init_len;
      my_obj_cntr.uninitialized_copy(o_it_b, o_it_e,
                                     this->begin() + this->size());
    }
    m_length = fill_len;
    return *this;
  }
  constexpr inline mjz_static_vector_template_t &operator=(
      std::initializer_list<Type> li) {
    size_t fill_len = min(li.size(), this->max_size());
    auto o_it_b = li.begin();
    if (fill_len < this->size()) {
      my_obj_cntr.copy_backward(o_it_b, o_it_b + fill_len, this->end());
      init_data(fill_len);
      return *this;
    }
    size_t un_init_len = fill_len - this->size();
    my_obj_cntr.copy_backward(o_it_b, o_it_b + this->size(), this->end());
    o_it_b += this->size();
    if (0 < un_init_len) {
      auto o_it_e = o_it_b + un_init_len;
      my_obj_cntr.uninitialized_copy_n(o_it_b, o_it_e,
                                       this->begin() + this->size());
    }
    m_length = fill_len;
    return *this;
  }
  // Constructors
  constexpr inline mjz_static_vector_template_t() noexcept {}
  constexpr inline explicit mjz_static_vector_template_t(size_type count) {
    init_data(count);
  }
  constexpr inline explicit mjz_static_vector_template_t(size_type count,
                                                         const Type &value) {
    my_obj_cntr.uninitialized_fill_n(m_elements(), count, value);
    m_length = count;
  }
  template <typename InputIt>
  constexpr inline mjz_static_vector_template_t(InputIt first, InputIt last) {
    if (init_data(std::distance(first, last)))
      my_obj_cntr.copy(first, last, begin());
  }
  constexpr inline mjz_static_vector_template_t(std::initializer_list<Type> il)
      : mjz_static_vector_template_t(il.begin(), il.end()) {}
  constexpr inline ~mjz_static_vector_template_t() { deinit_data(); }
  constexpr inline void reserve(size_type) {}
  constexpr inline size_type capacity() const noexcept { return m_capacity; }
  constexpr inline void shrink_to_fit() {}
  constexpr inline void clear() noexcept { deinit_data(); }
  constexpr inline iterator insert(const_iterator pos, const Type &val) {
    return insert(pos, 1, val);
  }
  constexpr inline iterator insert(const_iterator pos, Type &&val) {
    emplace(pos, std::move(val));
    return pos;
  }
  constexpr inline iterator insert(const_iterator pos, size_type count,
                                   const Type &val) {
    if (error_check(count, pos)) return {};
    auto offset = pos - cbegin();
    my_obj_cntr.move_backward(begin() + offset, end() - count, end());
    my_obj_cntr.uninitialized_fill_n(begin() + offset, count, val);
    m_length += count;
    return begin() + offset;
  }
  template <typename InputIt>
  constexpr inline iterator insert(const_iterator pos, InputIt first,
                                   InputIt last) {
    if (first >= last) return {};
    auto count = std::distance(first, last);
    if (error_check(count)) return {};
    auto offset = pos - cbegin();
    my_obj_cntr.move_backward(begin() + offset, end() - count, end());
    my_obj_cntr.uninitialized_copy(first, last, begin() + offset);
    m_length += count;
    return begin() + offset;
  }
  constexpr inline iterator insert(const_iterator pos,
                                   std::initializer_list<Type> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
  }
  template <typename... Args>
  constexpr inline iterator emplace(const_iterator pos, Args &&...args) {
    if (error_check(pos)) return {};
    auto offset = pos - cbegin();
    my_obj_cntr.move_backward(begin() + offset, end() - 1, end());
    my_obj_cntr.construct(my_obj_cntr, m_elements() + offset,
                          std::forward<Args>(args)...);
    ++m_length;
    return begin() + offset;
  }
  constexpr inline iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  }
  constexpr inline iterator erase(const_iterator first, const_iterator last) {
    if (error_check(first, last, false)) return {};
    auto offset = first - cbegin();
    auto count = last - first;
    destroy(begin() + offset, end());
    std::move(begin() + offset + count, end(), begin() + offset);
    m_length -= count;
    return begin() + offset;
  }
  template <typename... Args>
  constexpr inline reference emplace_back(Args &&...args) {
    if (error_check()) return *((pointer) nullptr);
    my_obj_cntr.construct(my_obj_cntr, m_elements() + size(),
                          std::forward<Args>(args)...);
    ++m_length;
    return back();
  }
  constexpr inline bool resize(size_type n) {
    if (m_capacity < n) return 0;
    if (m_length < n) {
      my_obj_cntr.construct_arr_at(m_elements() + m_length, n - m_length);
      m_length = n;
    } else if (m_length > n) {
      my_obj_cntr.obj_destructor_arr(m_elements() + n, m_length - n);
      m_length = n;
    } else {
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, const value_type &value) {
    if (m_capacity < n) return 0;
    if (m_length < n) {
      my_obj_cntr.construct_arr_at(m_elements() + m_length, n - m_length, 0,
                                   value);
      m_length = n;
    } else if (m_length > n) {
      my_obj_cntr.obj_destructor_arr(m_elements() + n, m_length - n);
      m_length = n;
    } else {
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, value_type &&value) {
    if (m_capacity < n) return 0;
    if (m_length < n) {
      if ((n - 1) > 0)
        my_obj_cntr.construct_arr_at(m_elements() + m_length, n - m_length - 1,
                                     0, value);
      my_obj_cntr.construct_at(m_elements() + n - 1, std::move(value));
      m_length = n;
    } else if (m_length > n) {
      my_obj_cntr.obj_destructor_arr(m_elements() + n, m_length - n);
      m_length = n;
    } else {
    }
    return 1;
  }
  constexpr inline reference push_back(const Type &value) {
    return emplace_back(value);
  }
  constexpr inline reference push_back(Type &&value) {
    return emplace_back(std::move(value));
  }
  constexpr inline void pop_back() {
    if (size() > 0) {
      --m_length;
      my_obj_cntr.destroy(m_elements() + size());
    }
  }
  constexpr inline void destroy(iterator first, iterator last) {
    my_obj_cntr.destroy(first, last);
  }
};

struct UINT64_X2_32_t {
  union {
    int64_t data64;
    int32_t data32[2];
    int16_t data16[4];
    int8_t data8[8];
    uint8_t data[8];
  };
};
UINT64_X2_32_t mjz_random();
bool get_random_chanch_bool(double chance_var);
inline uint32_t ARD_millis() { return usteejtgk_millis(); }
long long unsigned int mjz_millis();
enum uint8_t_error_level : uint8_t {
  on = 0,
  stack_ovf_prtt_off = MJZ_logic_BIT(0),
  LL_ovf_prtt_off = MJZ_logic_BIT(1),
  len_ovf_prtt_off = MJZ_logic_BIT(2)
};
struct memcmp_data {
  int64_t first_delta_index{-1};
  int first_delta{};
  constexpr inline memcmp_data(int64_t i, int in)
      : first_delta_index(i), first_delta(in) {}
  constexpr inline memcmp_data() : memcmp_data(-1, 0) {}
};
struct MJZ_memcmp_data : public memcmp_data {
  size_t num_delta{};
  constexpr inline MJZ_memcmp_data(int64_t i, int in, size_t n)
      : memcmp_data(i, in), num_delta(n) {}
  constexpr inline MJZ_memcmp_data() : memcmp_data(-1, 0), num_delta(0) {}
};

constexpr inline bool is_blank_characteres_default(char x_char_) {
  char in_CHAR_ = x_char_;
  int num{127};
  if (in_CHAR_ >= '0' && in_CHAR_ <= '9') {
    num = (uint8_t)(in_CHAR_ - '0');
  } else if (in_CHAR_ >= 'A' && in_CHAR_ <= 'Z') {
    num = (uint8_t)(in_CHAR_ - 'A' + 10);
  } else if (in_CHAR_ >= 'a' && in_CHAR_ <= 'z') {
    num = (uint8_t)(in_CHAR_ - 'a' + 10);
  }
  return !((num != 127) || ((x_char_) != '\n' && (x_char_) != '\r' &&
                            (x_char_) != ' ' && mjz_ard::isPrintable(x_char_)));
}
// When compiling programs with this class,the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency,typically
// with little or no increase in code size.
// -felide-constructors
// -std=c++0x
// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper
// *>(PSTR(string_literal)))
// An inherited class for holding the result of a concatenation. These
// result objects are assumed to be writable by subsequent concatenations.

template <typename T>
class StringSumHelper_t;

template <typename T>
class mjz_str_t;
using mjz_Str = mjz_str_t<mjz_normal_allocator>;
using StringSumHelper = StringSumHelper_t<mjz_normal_allocator>;

template <typename T = mjz_normal_allocator>
class extended_mjz_str_t;
using extended_mjz_Str = extended_mjz_str_t<mjz_normal_allocator>;
// Define constants and variables for buffering incoming serial data. We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
#define SERIAL_BUFFER_SIZE 64
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define NO_IGNORE_CHAR '\4'  // a char not found in a valid ASCII numeric field

class static_str_algo {
 public:
  static constexpr int64_t expected_mjz_str_size =
      64;  // set the wanted size in range of [(expected_basic_mjz_str_size +
  // expected_min_stack_obj_buffer_size),+inf]
  static constexpr int64_t expected_basic_mjz_str_size =
      (sizeof(size_t) * 2 + sizeof(void *));
  static constexpr int64_t expected_min_stack_obj_buffer_size =
      sizeof(uint8_t) + sizeof(char);
  static constexpr int64_t stack_buffer_size =
      expected_mjz_str_size -
      (expected_basic_mjz_str_size + expected_min_stack_obj_buffer_size);
  static constexpr int64_t the_reinterpreted_char_cca_size = 17;
  static constexpr int64_t forbiden_chars_cnt_size = 3;
  static size_t constexpr FLT_MAX_DECIMAL_PLACES = 10;
  static size_t constexpr DBL_MAX_DECIMAL_PLACES = 19;
  static constexpr const char *empty_STRING_C_STR = "";
  constexpr static const char forbiden_chars_cnt[forbiden_chars_cnt_size] = {
      '\r', '\n', 0};
  constexpr static const char
      the_in_reinterpreted_char_cca[the_reinterpreted_char_cca_size] = {
          'n', 'r', 'a',  'b',  't',  'v', 'f', 'e',
          's', '@', '\\', '\"', '\'', 'p', 'c', '\0'};
  constexpr static const char
      the_out_reinterpreted_char_cca[the_reinterpreted_char_cca_size] = {
          '\n', '\r', '\a', '\b', '\t', '\v', '\f', 'e',
          ' ',  '@',  '\\', '\"', '\'', '"',  '\\', '\0'};

 public:
  // this will crash the program
  [[noreturn]] static inline void trap_crash(void) {
    *(volatile char *)0 = (volatile char)0;  // address 0 is invalid
    // this will crash the program
  }

 public:
  template <class Type>
  static constexpr inline Type BL_min(Type a, Type b) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(b < a);
    return (BL & b) + ((~BL) & a);
  }
  template <class Type>
  static constexpr inline Type BL_max(Type a, Type b) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(a < b);
    return (BL & b) + ((~BL) & a);
  }
  template <class Type>
  static constexpr inline Type BL_abs(Type x) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(0 < x);
    return (BL & x) + ((~BL) & (-x));
  }

 public:
  static constexpr inline int MJZ_STRnCMP(const char *p1, const char *p2,
                                          size_t length_) {
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;
    bool b{1};
    bool state{1};
    int64_t length = length_;
    while (b) {
      b &= (*s1 != 0 && *s2 != 0);
      b &= (*s1++ == *s2++);
      state = b;
      b &= (0 < length--);
    }
    if (!state) return s1[-1] - s2[-1];
    return 0;
  }
  constexpr inline static int memcmp(const void *str1, const void *str2,
                                     size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    while ((0 < count--) && *s1++ != *s2++)
      ;
    if (*--s1 != *--s2) return *s1 < *s2 ? -1 : 1;
    return 0;
  }
  constexpr inline static int memcmp_rev(const void *str1, const void *str2,
                                         size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2 + count - 1;
    while ((0 < count--) && *s1++ != *s2--)
      ;
    if (*--s1 != *++s2) return *s1 < *s2 ? -1 : 1;
    return 0;
  }
  constexpr inline static memcmp_data memcmp_d(const void *str1,
                                               const void *str2, size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s1beg = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    while ((0 < count--) && *s1++ != *s2++)
      ;
    if (*--s1 != *--s2) return {s1 - s1beg, *s1 < *s2 ? -1 : 1};
    return {};
  }
  constexpr inline static int memcmp(const void *str1, size_t count1,
                                     const void *str2, size_t count2) {
    if (count1 != count2) {
      int cmp = memcmp(str1, str2, min(count1, count2));
      if (cmp == 0) return count1 < count2 ? -1 : 1;
      return cmp;
    }
    if (str1 == str2) {
      return 0;
    }
    return memcmp(str1, str2, count2);
  }
  constexpr inline static memcmp_data memcmp_d(const void *str1, size_t count1,
                                               const void *str2,
                                               size_t count2) {
    if (count1 != count2) {
      size_t min_len = min(count1, count2);
      memcmp_data cmp = memcmp_d(str1, str2, min_len);
      if (cmp.first_delta == 0)
        return {(int64_t)min_len, count1 < count2 ? -1 : 1};
      return cmp;
    }
    if (str1 == str2) {
      return {};
    }
    return memcmp_d(str1, str2, count2);
  }
  constexpr inline static void *memset(void *_Dst, int _Val, size_t _Size) {
    if (!(_Dst && _Size)) return _Dst;
    uint8_t *Dst{static_cast<uint8_t *>(_Dst)};
    uint8_t *Dst_end{Dst + _Size};
    while (Dst < Dst_end) {
      *Dst++ = static_cast<uint8_t>(_Val);
    }
    return _Dst;
  }
  constexpr inline static void *memmove(void *dest, const void *src,
                                        size_t len) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    if (d < s) {
      while (len--) {
        *d++ = *s++;
      }
    } else {
      const char *lasts = s + (len - 1);
      char *lastd = d + (len - 1);
      while (len--) {
        *lastd-- = *lasts--;
      }
    }
    return dest;
  }
  constexpr inline static void *memcpy(void *dest, const void *src,
                                       size_t len) {
    return memmove(dest, src, len);
  }
  constexpr inline static size_t strlen(const char *str) {
    if (!(str && *str)) {
      return 0;
    }
    const char *str_i = str;
    while (*str_i) ++str_i;
    ;
    return str_i - str;
  }
  constexpr inline static void *strncpy(void *dest, const char *src,
                                        size_t len) {
    return memcpy(dest, src, min_macro_(strlen(src) + 1, len));
  }
  constexpr inline static void *strcpy(void *dest, const char *src) {
    return memcpy(dest, src, strlen(src) + 1);
  }
  constexpr inline static const char *strchr(const char *str, size_t len_,
                                             char ch) {
    const char *str_end = str + len_;
    const char *ptr{str};
    while (ptr < str_end && (*ptr != ch)) ++ptr;
    if (ptr != str) return ptr;
    return NULL;
  }
  constexpr inline static const char *strrchr(const char *str, size_t len_,
                                              char ch) {
    const char *str_end{str - 1};
    const char *ptr = str + len_ - 1;
    while (ptr < str_end && (*ptr != ch)) --ptr;
    if (ptr != str) return ptr;
    return NULL;
  }
  static constexpr inline bool mem_equals(const uint8_t *p1, const uint8_t *p2,
                                          size_t count) {
    if (!count) return 1;
    if (p1 == p2) return 1;
    if (*p1 != *p2) return 0;
    constexpr size_t size_of_s_t = sizeof(size_t);
    size_t len_of_big = count / size_of_s_t;
    size_t len_of_small = count % size_of_s_t;
    if (len_of_big) {
      const size_t *P1 = (size_t *)p1;
      const size_t *P2 = (size_t *)p2;
      while (len_of_big-- && *P2++ == *P1++)
        ;
      if (P2[-1] != P1[-1]) return 0;
    }
    if (len_of_small) {
      size_t shift = count - len_of_small;
      p1 += shift;
      p2 += shift;
      while (len_of_small-- && *p2++ == *p1++)
        ;
      if (p2[-1] != p1[-1]) return 0;
    }
    return 1;
  }
  constexpr inline static bool mem_equals(const void *p1, const void *p2,
                                          size_t count) {
    return mem_equals((const uint8_t *)p1, (const uint8_t *)p2, count);
  }
  constexpr inline static const char *r_strstr(const char *const haystack_,
                                               const size_t haystack_len,
                                               const char *const needle_,
                                               const size_t needle_len) {
    if (needle_len > haystack_len) return NULL;
    if (needle_len == haystack_len)
      return mem_equals(haystack_, needle_, haystack_len) ? haystack_ : nullptr;
    if (!(haystack_len && needle_len && needle_ && haystack_)) return NULL;
    if (!(*needle_ && *haystack_)) return NULL;
    const char *const R_haystack_end = haystack_ - 1;
    const char *const haystack_end_protection =
        haystack_ + (haystack_len - needle_len);
    const char *haystack_ptr =
        haystack_ +
        (haystack_len -
         needle_len);  // skipping to first of last potential nedle start
    const char *const needle_end = needle_ + needle_len;
    const char *const needle_at_i_1_ptr = needle_ + 1;
    char first_needle_char = *needle_;
    while (R_haystack_end < haystack_ptr) {
      if (*haystack_ptr != first_needle_char) {
        --haystack_ptr;
        continue;
      }
      if (haystack_end_protection < haystack_ptr) return NULL;
      const char *haystack_ptr_buf = haystack_ptr + 1;
      const char *needle_ptr_buf = needle_at_i_1_ptr;
      while ((*haystack_ptr_buf == *needle_ptr_buf) &&
             (needle_ptr_buf < needle_end)) {
        haystack_ptr_buf++;
        needle_ptr_buf++;
      }
      if (needle_ptr_buf >= needle_end) {
        return haystack_ptr;
      }
      haystack_ptr--;
    }
    return NULL;
  }
  constexpr inline static const char *strstr(const char *const haystack_,
                                             const size_t haystack_len,
                                             const char *const needle_,
                                             const size_t needle_len) {
    if (needle_len > haystack_len) return NULL;
    if (needle_len == haystack_len)
      return mem_equals(haystack_, needle_, haystack_len) ? haystack_ : nullptr;
    if (!(haystack_len && needle_len && needle_ && haystack_)) return NULL;
    if (!(*needle_ && *haystack_)) return NULL;
    const char *const haystack_end = haystack_ + haystack_len;
    const char *haystack_ptr = haystack_;
    const char *const needle_end = needle_ + needle_len;
    const char *const needle_at_i_1_ptr = needle_ + 1;
    const char *const haystack_end_protection =
        haystack_ + (haystack_len - needle_len);
    char first_needle_char = *needle_;
    while (haystack_ptr < haystack_end) {
      if (*haystack_ptr != first_needle_char) {
        ++haystack_ptr;
        continue;
      }
      if (haystack_end_protection < haystack_ptr) return NULL;
      const char *haystack_ptr_buf = haystack_ptr + 1;
      const char *needle_ptr_buf = needle_at_i_1_ptr;
      while ((*haystack_ptr_buf == *needle_ptr_buf) &&
             (needle_ptr_buf < needle_end)) {
        haystack_ptr_buf++;
        needle_ptr_buf++;
      }
      if (needle_ptr_buf >= needle_end) return haystack_ptr;
      haystack_ptr = haystack_ptr_buf;
    }
    return NULL;
  }
  constexpr inline static void *mem_reverse_move(void *dest, const void *src,
                                                 size_t len, void *buffer = 0) {
    char *d = (char *)dest;
    const char *s = ((const char *)src) + len;
    if (dest == src) {
      return mem_reverse_it(dest, len);
    }
    if ((size_t)abs((int64_t)dest - (int64_t)src) <= len) {
      if (!buffer) return 0;
      mem_reverse_move(buffer, src, len);
      return memmove(dest, buffer, len);
    }
    if (d < s)  // for memmove to properly work
      while (len--) {
        *d++ = *--s;
      }
    else {
      const char *last_s = ((const char *)src);
      char *last_d = d + (len - 1);
      while (len--) {
        *last_d-- = *last_s++;
      }
    }
    return dest;
  }
  constexpr inline static void *mem_reverse_it(void *data, size_t len) {
    char *end_p = ((char *)data) + (len - 1);
    char *beg_p = (char *)data;
    while (beg_p < end_p) {
      swap(*end_p, *beg_p);
      end_p--;
      beg_p++;
    }
    return data;
  }
  template <typename cmpr_type>
  static constexpr inline MJZ_memcmp_data MJZ_memcmp(
      const void *ptr_1_, const void *ptr_2_, size_t size_of_data_in_byte) {
    const uint8_t *ptr_1 = (const uint8_t *)ptr_1_;
    const uint8_t *ptr_2 = (const uint8_t *)ptr_2_;
    size_t size_of_data = size_of_data_in_byte / sizeof(cmpr_type);
    uint8_t last_char_cmpr_size = size_of_data_in_byte % sizeof(cmpr_type);
    size_t diff_of_data{};
    const cmpr_type *ptr_1_rf = (const cmpr_type *)ptr_1;
    const cmpr_type *ptr_2_rf = (const cmpr_type *)ptr_2;
    const cmpr_type *ptr_1_rf_end = ptr_1_rf + size_of_data;
    const cmpr_type *ptr_1_rf_begin = ptr_1_rf;
    size_t first_delta_index[2]{};
    while (ptr_1_rf < ptr_1_rf_end) {
      bool is_different{(*ptr_1_rf++ != *ptr_2_rf++)};
      diff_of_data += is_different;
      first_delta_index[0] +=
          MJZ_logic_BL_bit_to_64_bits(is_different && !(first_delta_index[0])) &
          (size_t)(ptr_1_rf - ptr_1_rf_begin - 1);
    }
    size_t first_index_of_last = size_of_data_in_byte - last_char_cmpr_size;
    bool last_diff_of_data{};
    for (const uint8_t *ptr_i[4] = {ptr_1 + first_index_of_last,
                                    ptr_2 + first_index_of_last,
                                    last_char_cmpr_size + ptr_1 +
                                        first_index_of_last,
                                    ptr_1 + first_index_of_last};
         ptr_i[0] < ptr_i[2];) {
      bool is_different = (*ptr_i[0]++ != *ptr_i[1]++);
      last_diff_of_data |= is_different;
      first_delta_index[1] +=
          MJZ_logic_BL_bit_to_64_bits(is_different && !(first_delta_index[1])) &
          (size_t)(ptr_i[0] - ptr_i[3] - 1);
    }
    int64_t first_delta_index_in_8_t{};
    if (first_delta_index[0]) {
      size_t index_8 = first_delta_index[0] * sizeof(cmpr_type);
      const uint8_t *ptr_i[4] = {ptr_1 + index_8, ptr_2 + index_8,
                                 sizeof(cmpr_type) + ptr_1 + index_8, ptr_1};
      for (; ptr_i[0] < ptr_i[2];) {
        bool is_different = (*ptr_i[0]++ != *ptr_i[1]++);
        first_delta_index_in_8_t +=
            MJZ_logic_BL_bit_to_64_bits(is_different &&
                                        !(first_delta_index_in_8_t)) &
            (size_t)(ptr_i[0] - ptr_i[3] - 1);
      }
    } else if (first_delta_index[1]) {
      first_delta_index_in_8_t = first_delta_index[1] + first_index_of_last;
    }
    diff_of_data += last_diff_of_data;
    return {(((first_delta_index_in_8_t == 0) && (*ptr_1 == *ptr_2))
                 ? (-1)
                 : first_delta_index_in_8_t),
            ptr_1[first_delta_index_in_8_t] - ptr_2[first_delta_index_in_8_t],
            diff_of_data};
  }
  template <typename cmpr_type>
  constexpr inline MJZ_memcmp_data MJZ_strcmp(const void *ptr_1,
                                              const void *ptr_2) {
    size_t len_1_ = strlen((const char *)ptr_1);
    size_t len_2_ = strlen((const char *)ptr_2);
    if (len_2_ != len_1_) {
      MJZ_memcmp_data ret =
          MJZ_memcmp<cmpr_type>(ptr_1, ptr_2, min(len_2_, len_1_));
      ret.num_delta += abs((int64_t)len_2_ - (int64_t)len_1_);
      if (ret.first_delta_index == -1) {
        ret.first_delta_index = min(len_2_, len_1_);
        ret.first_delta = ((const char *)ptr_1)[ret.first_delta_index] -
                          ((const char *)ptr_2)[ret.first_delta_index];
      }
      return ret;
    }
    MJZ_memcmp_data ret = MJZ_memcmp<cmpr_type>(ptr_1, ptr_2, len_1_);
    ret.num_delta += (((uint8_t *)ptr_1)[len_1_] != ((uint8_t *)ptr_2)[len_2_]);
    return ret;
  }
  constexpr inline const char static *const alphabett_table_lower =
      "0123456789abcdefghijklmnopqrstuvwxyz";
  constexpr inline const char static *const alphabett_table_upper =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static constexpr inline char GET_CHAR_from_int(uint8_t intager_in,
                                                 bool is_upper_case) {
    const char *const alphabett_table =
        (is_upper_case ? alphabett_table_upper : alphabett_table_lower);
    if (intager_in == 255) {
      return '-';
    } else if (36 < intager_in) {
      return ' ';
    }
    return (char)alphabett_table[intager_in];
  }
  static constexpr inline uint8_t get_num_from_char(uint8_t in_CHAR_,
                                                    bool *to_neg) {
    if (in_CHAR_ >= '0' && in_CHAR_ <= '9') {
      return (uint8_t)(in_CHAR_ - '0');
    } else if (in_CHAR_ >= 'A' && in_CHAR_ <= 'Z') {
      return (uint8_t)(in_CHAR_ - 'A' + 10);
    } else if (in_CHAR_ >= 'a' && in_CHAR_ <= 'z') {
      return (uint8_t)(in_CHAR_ - 'a' + 10);
    } else if ((!!to_neg) && in_CHAR_ == '-') {
      *to_neg = 1;
      return (uint8_t)127;
    } else {
      return (uint8_t)127;  // note : 127 is delete in ascii
    }
  }
  static constexpr inline char *double_to_char_array(double num_double,
                                                     int decimal_place,
                                                     char buf) {
    int num_int = round(num_double * mjz_pow_UINT(10, decimal_place));
    int sign = num_int < 0 ? 1 : 0;
    num_int = abs(num_int);
    if (num_int == 0) {
      char *s = (char *)buf;
      s[0] = '0';
      s[1] = '.';
      for (int i = 2; i < decimal_place + 2; i++) s[i] = '0';
      s[decimal_place + 2] = '\0';
      return s;
    }
    int digit_count = 1;
    int n = num_int;
    if (n >= 100000000) {
      digit_count += 8;
      n /= 100000000;
    }
    if (n >= 10000) {
      digit_count += 4;
      n /= 10000;
    }
    if (n >= 100) {
      digit_count += 2;
      n /= 100;
    }
    if (n >= 10) {
      digit_count++;
    }
    int size = digit_count + 1 + (decimal_place > 0 ? 1 : 0) + sign;
    char *s = (char *)buf;
    for (int i = 0, integer = num_int; integer != 0; integer /= 10) {
      s[size - 2 - i++] = integer % 10 + 48;
      if (decimal_place > 0 && i == decimal_place) s[size - 2 - i++] = '.';
    }
    s[size - 1] = '\0';
    if (sign) s[0] = '-';
    return s;
  }
  struct just_str_view_data {
    constexpr inline ~just_str_view_data() {}
    constexpr inline just_str_view_data(char *p, size_t n)
        : buffer(p), len(n) {}
    char *buffer;
    size_t len;
  };

 private:
  static constexpr const uint64_t pow_of_10s[] = {1,
                                                  10,
                                                  100,
                                                  1000,
                                                  10000,
                                                  100000,
                                                  1000000,
                                                  10000000,
                                                  100000000,
                                                  1000000000,
                                                  10000000000,
                                                  100000000000,
                                                  1000000000000,
                                                  10000000000000,
                                                  100000000000000,
                                                  1000000000000000,
                                                  10000000000000000,
                                                  100000000000000000,
                                                  1000000000000000000,
                                                  10000000000000000000};

  static constexpr inline uint8_t num_digit_fast_minimal_for_dtostrf(
      double __val) {
    uint8_t int_component_log{0};
    uint8_t i{};
    if (__val < 1) return 1;
    while (int_component_log == 0 && i < 21) {
      ++i;
      int_component_log |= MJZ_logic_bit_to_64_bits(__val < pow_of_10s[i]) & i;
    }
    return int_component_log - 1;
  }
  static constexpr inline bool dtostrf_helper(double __val, uint8_t num_digit,
                                              size_t buffer_len, uint8_t __prec,
                                              char *buffer_) {
    char *buffer = buffer_;
    if (__prec == 0) {
      int64_t i_v = round<int64_t>(__val);
      size_t len_buffer_for_arr =
          b_U_lltoa_n(bit_cast<uint64_t>(i_v), buffer_, buffer_len, 10, 1).len;
      if (len_buffer_for_arr > buffer_len) return 0;
      return 1;
    }
    if (__val < 0) {
      __val = -(__val);
      *buffer++ = '-';
    }
    const uint64_t multipliyer = pow_of_10s[__prec];
    double temp__val = __val;
    uint64_t int_component_val = temp__val;
    temp__val -= int_component_val;
    temp__val *= multipliyer;
    uint64_t int_val = temp__val;
    if (int_val + 0.5f < temp__val) {
      int_val++;
      if (int_val >= multipliyer) {
        int_val -= multipliyer;
        int_component_val++;
      }
    }

    uint8_t number_of_number_in_int_component = num_digit;
    bool length_is_small = buffer_len < number_of_number_in_int_component +
                                            __prec + (buffer_ != buffer);
    if (length_is_small) {
      return 0;
    }
    char *the_dot_posiotion = buffer + number_of_number_in_int_component;
    char *ptr = the_dot_posiotion + __prec;
    char *ptr_max = ptr;
    char *ptr_end = buffer - 1;
    while (the_dot_posiotion < ptr) {
      *ptr-- = GET_CHAR_from_int(int_val % 10, 0);
      int_val /= 10;
    }
    // the_dot_posiotion == ptr
    *ptr-- = '.';
    while (ptr_end < ptr) {
      *ptr-- = GET_CHAR_from_int(int_component_val % 10, 0);
      int_component_val /= 10;
    }
    // ptr_end == ptr
    return 1;
  }

 public:
  static constexpr inline just_str_view_data b_U_lltoa_n(
      uint64_t value, char *BFR_buffer,
      size_t BFR_len,  // has to be 66 bytes for all values
                       // from 0 to -1ULL  in base2  to base36
      int radix, bool is_signed, bool force_neg = 0, bool is_upper_case = 1) {
    if (!(BFR_buffer && (2 < BFR_len))) return {0, 0};
    if (value == 0) {
      *BFR_buffer = '0';
      BFR_buffer[1] = '\0';
      return {BFR_buffer, 1};
    }
    constexpr char end_of_transmission_char = 4;
    constexpr char null_char = 0;
    bool IS_NEGITIVE__ = (is_signed && (*((int64_t *)&value) < 0) &&
                          ((radix == 10) || force_neg));
    if (IS_NEGITIVE__) {
      *((int64_t *)&value) =
          ((-1) *
           (*((int64_t *)&value)));  // use a positive insted of the - sign
    }
    char buffer[200]{};
    if ((radix < 2) || (36 < radix)) {
      return {0, 0};
    }
    for (int64_t i = 0; (i < 200); i++) {
      buffer[i] = null_char;
    }
    char buffer_[129]{};
    for (int64_t i = 0; i < 129; i++) {
      buffer_[i] = end_of_transmission_char;
    }
    int64_t number_of_numbers{};
    for (int64_t i = 0; i < 129; i++) {
      buffer_[128 - i] =
          GET_CHAR_from_int((uint8_t)(value % radix), is_upper_case);
      value /= radix;
      number_of_numbers++;
      if (value == null_char) {
        for (int64_t j = 0, k = 0; j < 129; j++) {
          if (buffer_[j] != end_of_transmission_char) {
            buffer[k] = buffer_[j];  // when its done we reverse the string
            k++;
          }
        }
        break;
      }
    }
    buffer[number_of_numbers] = 0;
    if (BFR_len < number_of_numbers) return {0, 0};
    if (IS_NEGITIVE__) {
      BFR_buffer[0] = '-';
      static_str_algo::memmove(BFR_buffer + 1, buffer,
                               number_of_numbers + 1);  //+null
    } else {
      static_str_algo::memmove(BFR_buffer, buffer,
                               number_of_numbers + 1);  //+null
    }
    return {BFR_buffer, (size_t)number_of_numbers};
  }
  static constexpr inline just_str_view_data dtostrf(double __val, int8_t width,
                                                     uint8_t __prec,
                                                     char *buffer_,
                                                     size_t buffer_len) {
    if (!(buffer_ && buffer_len) || 19 < __prec || (width + 1 >= buffer_len))
      return {buffer_, 0};
    uint8_t num_digit = num_digit_fast_minimal_for_dtostrf(abs(__val));
    uint8_t required_len = (__val < 0) + num_digit + !!__prec + __prec +
                           1;  // "-I.P\0" [-] + {I} + {.} + {P} + [\0]

    if (buffer_len < required_len) return {buffer_, 0};
    uint8_t &str_len = --required_len;  // strlen dosent counr \0
    int numberof_spaces = abs(width) - str_len;
    if (width < 0 || numberof_spaces <= 0) {
      width = -width;
      dtostrf_helper(__val, num_digit, buffer_len, __prec, buffer_);

      char *ptr = buffer_ + str_len;
      char *ptr_end = ptr + numberof_spaces;  // it dosent count \0 and if
                                              // isnt  bigger it dose noting
      while (ptr < ptr_end) {
        *ptr++ = ' ';
      }
      uint8_t out_len = max_macro_(str_len, width + 1);
      buffer_[out_len] = '\0';
      return {buffer_, out_len};
    }
    dtostrf_helper(__val, num_digit, buffer_len - numberof_spaces, __prec,
                   buffer_ + numberof_spaces);

    char *ptr = buffer_ + required_len;
    char *ptr_end = ptr + numberof_spaces;
    while (ptr < ptr_end) {
      *ptr++ = ' ';
    }
    uint8_t out_len = width + 1;
    buffer_[out_len] = '\0';
    return {buffer_, out_len};
  }

  static constexpr inline int64_t mjz_pow_UINT(uint32_t base,
                                               uint32_t power_Of_base) {
    int64_t ret_val = 1;
    for (uint32_t i{}; i < power_Of_base; i++) {
      ret_val *= base;
    }
    return ret_val;
  }
  static constexpr inline long long C_STR_to_LL(const char *buffer,
                                                uint8_t buffer_len, int radix,
                                                bool *had_error,
                                                uint8_t error_level) {
    bool dummy;
    if (!had_error) {
      had_error = &dummy;
    }
    const double MAX_LL_IN_D = 9.223372036854775807e+18;
    uint8_t buffer_for_number[70]{};
    for (int64_t i{}; i < 70; i++) {
      buffer_for_number[i] = 127;
    }
    bool is_neg_bool{};
    bool stack_ovf_prtt = !MJZ_logic_BIT_CHECK(error_level, 0);
    bool LL_ovf_prtt = !MJZ_logic_BIT_CHECK(error_level, 1);
    bool len_ovf_prtt =
        (!MJZ_logic_BIT_CHECK(error_level, 2) && stack_ovf_prtt);
    if ((len_ovf_prtt &&
         (std::pow((double)buffer_len - 2, (double)radix) > MAX_LL_IN_D ||
          (buffer_len - 1) > 63)) ||
        radix > 36 || radix < 2 || buffer_len > 65 || !buffer || !buffer_len) {
      *had_error = 1;
      if (stack_ovf_prtt) {
        return 0;
      }
    }
    if (!buffer) {
      return 0;
    }
    uint8_t WHILE_Index_for_int_buf = 0;
    int64_t WHILE_Index_for_buffer = 0;
    uint8_t number_of_empty_spaces = 0;
    while (WHILE_Index_for_buffer < buffer_len) {
      bool is_neg_bool_buf{};
      buffer_for_number[WHILE_Index_for_int_buf] =
          get_num_from_char(buffer[WHILE_Index_for_buffer], &is_neg_bool_buf);
      // if is not a number then add empty space
      number_of_empty_spaces++;
      if (is_neg_bool_buf) {
        is_neg_bool = 1;
      } else if (buffer_for_number[WHILE_Index_for_int_buf] == 127) {
        volatile uint16_t HOLDER_does_not_do_any_thing = 0;
        HOLDER_does_not_do_any_thing +=
            HOLDER_does_not_do_any_thing;  // TODO: V2007
                                           // https://pvs-studio.com/en/docs/warnings/V2007/
        // This expression can be simplified.
        // One of the operands in the '+='
        // operation equals 0x0. Probably it is
        // a mistake.
      } else if (buffer_for_number[WHILE_Index_for_int_buf] >= radix) {
        *had_error = 1;
        if (LL_ovf_prtt) {
          return 0;
        }
      } else {
        number_of_empty_spaces--;
        WHILE_Index_for_int_buf++;
      }
      WHILE_Index_for_buffer++;
    }
    if (len_ovf_prtt && !is_neg_bool) {
      uint8_t number_of_numbers = buffer_len - number_of_empty_spaces;
      double MAX_for_my_int =
          std::pow((double)number_of_numbers - 1, (double)radix);
      if ((MAX_for_my_int > MAX_LL_IN_D || (number_of_numbers) > 63)) {
        *had_error = 1;
        // if (stack_ovf_prtt) is not necessary // stack_ovf_prtt =1
        return 0;
      }
    }
    int64_t my_ret_value_{};
    WHILE_Index_for_buffer = WHILE_Index_for_int_buf;
    size_t while_2_i_max = (size_t)WHILE_Index_for_int_buf + 1;
    WHILE_Index_for_int_buf = 0;
    size_t while_2_i = 0;
    while (while_2_i < while_2_i_max && WHILE_Index_for_buffer >= 0) {
      while_2_i++;
      if (buffer_for_number[WHILE_Index_for_buffer] != 127 &&
          (LL_ovf_prtt || buffer_for_number[WHILE_Index_for_buffer] < radix)) {
        if (!!buffer_for_number[WHILE_Index_for_buffer]) {
          my_ret_value_ += buffer_for_number[WHILE_Index_for_buffer] *
                           mjz_pow_UINT(radix, WHILE_Index_for_int_buf);
        }
        WHILE_Index_for_int_buf++;
      }
      WHILE_Index_for_buffer--;
    }
    if (is_neg_bool) {
      my_ret_value_ *= -1;
    }
    return my_ret_value_;
  }
  static constexpr inline int MJZ_STRCMP(const char *p1, const char *p2) {
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;
    unsigned char c1, c2;
    do {
      c1 = (unsigned char)*s1;
      s1++;
      c2 = (unsigned char)*s2;
      s2++;
      if (c1 == '\0') {
        return c1 - c2;
      }
    } while (c1 == c2);
    return c1 - c2;
  }

  static constexpr inline char *b_U_lltoa(
      uint64_t value, char *BFR_buffer,  // has to be 66 bytes for all values
                                         // from 0 to -1ULL  in base2  to base36
      int radix, bool is_signed, bool force_neg = 0, bool is_upper_case = 0) {
    constexpr char end_of_transmission_char = 4;
    constexpr char null_char = 0;
    bool IS_NEGITIVE__ = (is_signed && (*((int64_t *)&value) < 0) &&
                          ((radix == 10) || force_neg));
    if (IS_NEGITIVE__) {
      *((int64_t *)&value) =
          ((-1) *
           (*((int64_t *)&value)));  // use a positive insted of the - sign
    }
    char buffer[200]{};
    if ((radix < 2) || (36 < radix)) {
      return 0;
    }
    for (int64_t i = 0; (i < 200); i++) {
      buffer[i] = null_char;
    }
    char buffer_[129]{};
    for (int64_t i = 0; i < 129; i++) {
      buffer_[i] = end_of_transmission_char;
    }
    int64_t number_of_numbers{};
    for (int64_t i = 0; i < 129; i++) {
      buffer_[128 - i] =
          GET_CHAR_from_int((uint8_t)(value % radix), is_upper_case);
      value /= radix;
      number_of_numbers++;
      if (value == null_char) {
        for (int64_t j = 0, k = 0; j < 129; j++) {
          if (buffer_[j] != end_of_transmission_char) {
            buffer[k] = buffer_[j];  // when its done we reverse the string
            k++;
          }
        }
        break;
      }
    }
    buffer[number_of_numbers] = 0;
    if (IS_NEGITIVE__) {
      BFR_buffer[0] = '-';
      static_str_algo::memmove(BFR_buffer + 1, buffer,
                               number_of_numbers + 1);  //+null
    } else {
      static_str_algo::memmove(BFR_buffer, buffer,
                               number_of_numbers + 1);  //+null
    }
    return BFR_buffer;
  }

  static constexpr inline char *ulltoa(uint64_t value, char *buffer,
                                       int radix) {
    return b_U_lltoa((uint64_t)value, buffer, radix, 0);
  }
  static constexpr inline char *ultoa(uint32_t value, char *buffer, int radix) {
    return ulltoa((uint64_t)value, buffer, radix);
  }
  static constexpr inline char *utoa(uint32_t value, char *buffer, int radix) {
    return ultoa(value, buffer, radix);
  }
  static constexpr inline char *lltoa(long long value, char *buffer,
                                      int radix) {
    return b_U_lltoa((uint64_t)value, buffer, radix, 1);
  }
  static constexpr inline char *ltoa(long value, char *buffer, int radix) {
    return lltoa((long long)value, buffer, radix);
  }
  static constexpr inline char *itoa(int value, char *buffer, int radix) {
    return lltoa((long long)value, buffer, radix);
  }
  static constexpr inline void Set_nth_bit_andret32(
      void *data, uint64_t nthbt,
      bool set_to) {  // set_to is a bool
    uint64_t &data_in_array = ((uint64_t *)data)[nthbt / 32];
    const uint8_t ofset = nthbt % 32;
    data_in_array &= static_cast<uint64_t>(0) << ofset;
    if (set_to) {
      data_in_array |= (static_cast<uint64_t>(1) << ofset);
    }
  }
  static constexpr inline bool Get_nth_bit_andret32(
      const void *data,
      uint64_t nthbt) {  // set_to is a bool
    uint64_t data_in_array = ((uint64_t *)data)[nthbt / 32];
    const uint8_t ofset = nthbt % 32;
    data_in_array &= static_cast<uint64_t>(1) << ofset;
    if (data_in_array) {
      return 1;
    }
    return 0;
  }
  static constexpr inline void tgl_nth_bit_andret32(
      void *data, uint64_t nthbt) {  // set_to is a bool
    uint64_t *data_in_array = ((uint64_t *)data) + (nthbt / 32);
    const uint8_t ofset = nthbt % 32;
    *data_in_array ^= (static_cast<uint64_t>(1) << ofset);
  }
  static constexpr inline void Set_nth_bit_andret8(
      void *data, uint64_t nthbt,
      bool set_to) {  // set_to is a bool
    uint8_t *data_in_array = ((uint8_t *)data) + (nthbt / 8);
    const uint8_t ofset = nthbt % 8;
    *data_in_array &= (1 << ofset) ^ (-1);  // number &= ~(1UL << ofset);
    if (set_to) {
      *data_in_array = static_cast<uint8_t>(*data_in_array | (1 << ofset));
    }
  }
  static constexpr inline void tgl_nth_bit_andret8(
      void *data,
      uint64_t nthbt) {  // set_to is a bool
    uint8_t *data_in_array = ((uint8_t *)data) + (nthbt / 8);
    const uint8_t ofset = nthbt % 8;
    *data_in_array ^= (1 << ofset);
  }
  static constexpr inline bool Get_nth_bit_andret8(
      const void *data,
      uint64_t nthbt) {  // set_to is a bool
    uint64_t data_in_array = *((uint8_t *)data) + (nthbt / 8);
    const uint8_t ofset = nthbt % 8;
    data_in_array &= (uint64_t)1 << ofset;
    if (data_in_array) {
      return 1;
    }
    return 0;
  }

 private:
  template <class Type>
  struct remove_reference {
    using type = Type;
  };
  template <class Type>
  struct remove_reference<Type &> {
    using type = Type;
  };
  template <class Type>
  struct remove_reference<Type &&> {
    using type = Type;
  };
  template <class Type>
  struct add_reference {
    using type = Type &;
  };
  template <class Type>
  struct add_const {
    using type = const Type;
  };
  template <class Type>
  struct add_temp {
    using type = Type &&;
  };
  template <class Type>
  struct add_ptr {
    using type = Type *;
  };

 public:
  template <class Type>
  using remove_reference_t = typename remove_reference<Type>::type;
  template <class Type>
  using to_reference_t = typename add_reference<remove_reference_t<Type>>::type;
  template <class Type>
  using to_const_t = typename add_const<remove_reference_t<Type>>::type;
  template <class Type>
  using to_temperory_t = typename add_temp<remove_reference_t<Type>>::type;
  template <class Type>
  using to_const_reference_t = typename add_const<to_reference_t<Type>>::type;
  template <class Type>
  using to_pointer_t = typename add_ptr<remove_reference_t<Type>>::type;
  template <class Type>
  using to_pointer_const_t =
      typename add_const<add_ptr<remove_reference_t<Type>>>::type;
  template <class Type>
  using to_const_pointer_t =
      typename add_ptr<add_const<remove_reference_t<Type>>>::type;
  template <class Type>
  using to_const_pointer_const_t =
      typename add_const<add_ptr<add_const<remove_reference_t<Type>>>>::type;
  template <typename Type>
  constexpr static void swap(Type &Left, Type &Right) {
    Type _Tmp = std::move(Left);
    Left = std::move(Right);
    Right = std::move(_Tmp);
  }
  template <class T, class U = T>
  static T exchange(T &obj, U &&new_value) {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
  }
  constexpr static int64_t compare_two_str(const char *rhs, size_t rhs_l,
                                           const char *lhs, size_t lhs_l) {
    if (!rhs || !lhs) {
      if (lhs && lhs_l > 0) {
        return static_cast<int64_t>(0) - *(unsigned char *)lhs;
      }
      if (rhs && rhs_l > 0) {
        return *(unsigned char *)rhs;
      }
      return 0;
    }
    return MJZ_STRnCMP(rhs, lhs, rhs_l);
  }
  constexpr static bool are_two_str_equale(const char *rhs, size_t rhs_l,
                                           const char *lhs, size_t lhs_l) {
    if (rhs_l == 0) {
      //
      return (lhs == NULL || *lhs == 0);
    }
    if (lhs == NULL) {
      //
      return rhs[0] == 0;
    }
    if (lhs_l != rhs_l) {
      return 0;
    }
    return mem_equals(rhs, lhs, rhs_l);
  }
  struct _char_8_a {
    union {
      bool data_b[8]{};
      char data_c[8];
      uint8_t data_u[8];
    };
    constexpr inline _char_8_a() = default;
    constexpr inline bool &operator[](uint8_t i) { return data_b[i]; }
  };
  constexpr inline static _char_8_a char_get_bits(uint8_t val, bool to_char) {
    _char_8_a ret_val{};
    uint8_t i{};
    for (; i < 8; i++) {
      ret_val[i] = !!(val & (1 << (7 - i)));
    }
    if (to_char) {
      for (i = 0; i < 8; i++) {
        ret_val.data_c[i] = ret_val[i] ? '1' : '0';
      }
    }
    return ret_val;
  }
  enum class cpu_endian : uint8_t { little = 0, big = 1 };
  inline static cpu_endian get_cpu_endian() {
    union {
      int x = 1;
      uint8_t a[4];
    };
    return a[0] ? cpu_endian::little : cpu_endian::big;
  }
  inline static char *get_bit_representation(
      char *buffer, size_t buffer_len, const void *data_ptr, size_t len,
      bool in_reverse = (get_cpu_endian() == cpu_endian::little)) {
    if (buffer_len < len * 8) return 0;
    const char *data = (const char *)data_ptr;
    const char *data_end = data + len;
    uint64_t *buffer_ptr = (uint64_t *)buffer;
    _char_8_a arr_buf;
    if (in_reverse)
      for (const char *ptr{data_end - 1}, *ptr_end{data - 1}; ptr_end < ptr;
           ptr--) {
        arr_buf = char_get_bits(*ptr, 1);
        memmove(buffer_ptr++, arr_buf.data_u, 8);
      }
    else
      for (const char *ptr = data; ptr < data_end; ptr++) {
        arr_buf = char_get_bits(*ptr, 1);
        memmove(buffer_ptr++, arr_buf.data_u, 8);
      }
    return buffer;
  }
  template <typename Type>
  inline static char *get_bit_representation(char *buffer, const Type &data) {
    return get_bit_representation(buffer, sizeof(Type) * 8, &data,
                                  sizeof(Type));
  }

 public:
  class stack_str_buf {
    uint8_t STR_is_in_stack{};

   public:
    char stack_buffer[stack_buffer_size + 1]{};  // ptr you're searching for
    constexpr inline stack_str_buf() : STR_is_in_stack(0) {
      stack_buffer[stack_buffer_size] = 0;
    }
    constexpr inline stack_str_buf(const stack_str_buf &) = default;
    constexpr inline stack_str_buf(stack_str_buf &&) = default;
    constexpr inline stack_str_buf &operator=(stack_str_buf &&) = default;
    constexpr inline stack_str_buf &operator=(const stack_str_buf &) = default;
    constexpr inline bool set(bool STR_is_in_stack_) {
      STR_is_in_stack = STR_is_in_stack_;
      if (!STR_is_in_stack_) {
        // static_str_algo::memset(stack_buffer, 0, stack_buffer_size);//
        // SLOOOOW
        stack_buffer[stack_buffer_size] = 0;
      }
      return STR_is_in_stack_;
    }
    constexpr inline bool get() { return !!STR_is_in_stack; }
    // virtual //i don't need it
    constexpr inline ~stack_str_buf() { STR_is_in_stack = 0; }
  };
  template <typename T2, typename T1>
  inline static constexpr T2 bit_cast(const T1 &data) {
    union MyUnion {
      MyUnion() {}
      ~MyUnion() {}
      T2 val;
    };
    MyUnion val;
    memmove(&val, &data, min_macro_(sizeof(val), sizeof(data)));
    return val.val;
  }
  static constexpr inline size_t find_first_of_in_str(const char *hay_stack,
                                                      size_t hay_len,
                                                      const char *needle,
                                                      size_t needle_len) {
    for (const char *ptr_i{hay_stack}, *ptr_i_end{ptr_i + hay_len};
         ptr_i < ptr_i_end; ptr_i++) {
      for (const char *ptr_j{needle}, *ptr_j_end{ptr_j + needle_len};
           ptr_j < ptr_j_end; ptr_j++) {
        if (*ptr_i == *ptr_j) {
          return ptr_i - hay_stack;
        }
      }
    }
    return -1;
  }
  static constexpr inline size_t find_last_of_in_str(const char *hay_stack,
                                                     size_t hay_len,
                                                     const char *needle,
                                                     size_t needle_len) {
    for (const char *ptr_i_end{hay_stack - 1}, *ptr_i{ptr_i_end + hay_len};
         ptr_i_end < ptr_i; ptr_i--) {
      for (const char *ptr_j{needle}, *ptr_j_end{ptr_j + needle_len};
           ptr_j < ptr_j_end; ptr_j++) {
        if (*ptr_i == *ptr_j) {
          return ptr_i - hay_stack;
        }
      }
    }
    return -1;
  }
  static constexpr inline size_t find_first_of_not_in_str(const char *hay_stack,
                                                          size_t hay_len,
                                                          const char *needle,
                                                          size_t needle_len) {
    for (const char *ptr_i{hay_stack}, *ptr_i_end{ptr_i + hay_len};
         ptr_i < ptr_i_end; ptr_i++) {
      for (const char *ptr_j{needle}, *ptr_j_end{ptr_j + needle_len};
           ptr_j < ptr_j_end; ptr_j++) {
        if (*ptr_i != *ptr_j) {
          return ptr_i - hay_stack;
        }
      }
    }
    return -1;
  }
  static constexpr inline size_t find_last_of_not_in_str(const char *hay_stack,
                                                         size_t hay_len,
                                                         const char *needle,
                                                         size_t needle_len) {
    for (const char *ptr_i_end{hay_stack - 1}, *ptr_i{ptr_i_end + hay_len};
         ptr_i_end < ptr_i; ptr_i--) {
      for (const char *ptr_j{needle}, *ptr_j_end{ptr_j + needle_len};
           ptr_j < ptr_j_end; ptr_j++) {
        if (*ptr_i != *ptr_j) {
          return ptr_i - hay_stack;
        }
      }
    }
    return -1;
  }
  template <typename Type, typename function_type>
  static constexpr inline Type *do_operation(Type *buffer_,
                                             function_type function,
                                             const Type *a_arr,
                                             const Type *b_arr, size_t len) {
    const Type *buffer_end = buffer_ + len;
    Type *buffer{buffer_};
    while (buffer < buffer_end) {
      *buffer++ = function(*a_arr++, *b_arr++);
    }
    return buffer_;
  }

 public:
  constexpr static uint8_t number_of_terms = 100;
  static constexpr inline long divide_by_2(long x) { return (x >> 1); }

 private:
  constexpr static float three_halfs = 1.5F;
  inline static constexpr float Q_rsqrt_unsafe_logic(float &y, float x2) {
    constexpr uint32_t magic_constant = 0x5f3759df;
    // long &i = *(long *)&y; // log base 2
    // long i = *(long *)&y;
    long i = bit_cast<long>(y);
    i = magic_constant - divide_by_2(i);  // some magic
                                          // y = *(float *)&i;
    y = bit_cast<float>(i);
    y = y * (three_halfs - (x2 * y * y));  // 1st iteration of newtons method
    return y;
  }
  static constexpr inline bool check_number_for_r_sqrt(float &number) {
    if (number == 0) {
      number = INFINITY;
      return 1;
    }
    if (number == 1) {
      number = 1;
      return 1;
    }
    if (number < 0 || number == NAN) {
      number = NAN;
      return 1;
    }
    return 0;
  }

 public:
  static constexpr float accurate_Q_rsqrt_unsafe(float number) {
    float x2 = number * 0.5F;
    Q_rsqrt_unsafe_logic(number, x2);
    // 2nd iteration, this can be removed
    return number * (three_halfs - (x2 * number * number));
  }
  static constexpr float super_accurate_Q_rsqrt_unsafe(float number_) {
    float x2_ = number_ * 0.5F;
    Q_rsqrt_unsafe_logic(number_, x2_);
    double x2 = x2_;
    double number = number_;
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    long flr = floor(number);
    if (abs(number - (double)flr) < 0.0001) return flr;
    return number;
  }
  static constexpr double scientifically_accurate_Q_rsqrt_unsafe(
      double number) {
    float x2_ = number * 0.5F;
    float number_ = number;
    Q_rsqrt_unsafe_logic(number_, x2_);
    double x2 = x2_;
    number = number_;
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    number *= (1.5 - (x2 * number * number));
    long flr = floor(number);
    if (abs(number - (double)flr) < 0.0001) return flr;
    return number;
  }
  static constexpr float Q_rsqrt_unsafe(float number) {
    float x2 = number * 0.5F;
    return Q_rsqrt_unsafe_logic(number, x2);
  }
  static constexpr float Q_rsqrt(float number) {
    if (check_number_for_r_sqrt(number)) return number;
    return Q_rsqrt_unsafe(number);
  }
  static constexpr float accurate_Q_rsqrt(float number) {
    if (check_number_for_r_sqrt(number)) return number;
    return accurate_Q_rsqrt_unsafe(number);
  }
  static constexpr float super_accurate_Q_rsqrt(float number) {
    if (check_number_for_r_sqrt(number)) return number;
    return super_accurate_Q_rsqrt_unsafe(number);
  }
  static constexpr double scientifically_accurate_Q_rsqrt(double number) {
    if (number == 0.0) {
      return INFINITY;
    }
    if (number == 1.0) {
      return 1;
    }
    if (number < 0.0 || number == NAN) {
      return NAN;
    }
    return scientifically_accurate_Q_rsqrt_unsafe(number);
  }
  constexpr static inline uint64_t floor(double x) { return (uint64_t)x; }
  constexpr static inline double expUL(uint32_t number) {
    double retval{1};
    for (uint32_t i{}; i < number; i++) {
      retval *= EULER;
    }
    return retval;
  }
  constexpr static inline double expUD(double x) {
    uint32_t floor_x = (uint32_t)floor(x);
    double retval_fast_component = expUL(floor_x);
    x -= floor_x;
    double retval{1};
    for (int64_t i = 1; i <= number_of_terms; i++) {
      double term{1};
      for (int64_t j = 1; j <= i; j++) {
        term *= x / j;
      }
      retval += term;
    }
    retval *= retval_fast_component;
    return retval;
  }
  constexpr static inline double exp(int32_t number) {
    return ((0 < number) ? expUL(number) : (double)1 / expUL(-(number)));
  }
  constexpr static inline double exp(double number) {
    return ((0 < number) ? expUD(number) : (double)1 / expUD(-(number)));
  }
  constexpr static inline double log(double x) {
    if (x == 1) return 0;
    if (x == 0) return NAN;
    uint64_t integer_component{0};
    for (; EULER < x; x /= EULER) {
      integer_component++;
    }
    double retval{0};
    double term{1};
    double x_mines_one = (x - 1.0);
    for (int64_t i{1}; i <= number_of_terms; i++) {
      term *= x_mines_one / x;
      retval += term / i;
    }
    return (float)integer_component + retval;
  }
  constexpr static inline double powUD(double base, double exponent) {
    if (base == 0) return 0;
    uint32_t exponent_int_component = (uint32_t)floor(exponent);
    exponent -= exponent_int_component;
    return exp(exponent * log(base)) * powUL(base, exponent_int_component);
  }
  constexpr static inline double powUL(double base, uint32_t exponent) {
    double result{1};
    for (uint64_t i{}; i < exponent; i++) result *= base;
    return result;
  }
  constexpr static inline double pow(double base, double exponent) {
    return ((0 < exponent) ? (powUD(base, exponent))
                           : (1 / powUD(base, -exponent)));
  }
  constexpr static inline double sqrt(double x) {
    return 1 / scientifically_accurate_Q_rsqrt(x);
  }
  template <typename Type>
  constexpr static inline Type sqrtt(Type x) {
    if (!x) return 0;
    return expUt(logt(x) * (Type)0.5);
  }
  constexpr static inline uint64_t ceiling(double x) {
    uint64_t fx = floor(x);
    if ((double)fx == x) return fx;
    return fx + 1;
  }
  template <typename T = uint64_t>
  constexpr static inline T round(double x) {
    T fx = floor(x);
    return ((0.5 < (x - (double)fx)) ? (fx + 1) : (fx));
  }
  template <typename Type>
  constexpr static inline Type expULt(uint32_t number) {
    Type retval{1};
    for (uint32_t i{}; i < number; i++) {
      retval *= (Type)EULER;
    }
    return retval;
  }
  template <typename Type>
  constexpr static inline Type expUt(Type x) {
    Type retval{1};
    for (int64_t i = 1; i <= number_of_terms; i++) {
      Type term{1};
      for (int64_t j = 1; j <= i; j++) {
        term *= x / Type(j);
      }
      retval += term;
    }
    return retval;
  }
  template <typename Type>
  constexpr static inline Type expt(int32_t number) {
    return ((number > Type{0}) ? expULt<Type>(number)
                               : (Type)1 / expULt<Type>(-(number)));
  }
  template <typename Type>
  constexpr static inline Type expt(Type number) {
    return ((number > Type{0}) ? expUt<Type>(number)
                               : (Type)1 / expUt<Type>(-(number)));
  }
  template <typename Type>
  constexpr static inline Type logt(Type x) {
    if (x == (Type)1) return 0;
    if (x == (Type)0) return NAN;
    Type integer_component{0};
    Type EULER_T = EULER;
    for (; EULER_T < x; x /= EULER_T) {
      integer_component += 1;
    }
    Type retval{0};
    Type term{1};
    Type x_mines_one = (x - 1.0f);
    for (int64_t i{1}; i <= number_of_terms; i++) {
      term *= x_mines_one / x;
      retval += term / i;
    }
    return integer_component + retval;
  }
  template <typename Type>
  constexpr static inline Type powUt(Type base, Type exponent) {
    if (base == 0) return 0;
    uint32_t exponent_int_component = (uint32_t)floor(exponent);
    exponent -= exponent_int_component;
    return expt(exponent * logt(base)) * powULt(base, exponent_int_component);
  }
  template <typename Type>
  constexpr static inline Type powULt(Type base, uint32_t exponent) {
    Type result{1};
    for (uint64_t i{}; i < exponent; i++) result *= base;
    return result;
  }
  template <typename Type>
  constexpr static inline Type powt(Type base, Type exponent) {
    return ((0 < exponent) ? (powUt(base, exponent))
                           : (1 / powUt(base, -exponent)));
  }
  constexpr static inline float expULf(uint32_t number) {
    float retval{1};
    for (uint32_t i{}; i < number; i++) {
      retval *= (float)EULER;
    }
    return retval;
  }
  constexpr static inline float expUF(float x) {
    uint32_t floor_x = (uint32_t)floor(x);
    float retval_fast_component = expULf(floor_x);
    x -= floor_x;
    float retval{1};
    for (int64_t i = 1; i <= number_of_terms; i++) {
      float term{1};
      for (int64_t j = 1; j <= i; j++) {
        term *= x / j;
      }
      retval += term;
    }
    retval *= retval_fast_component;
    return retval;
  }
  constexpr static inline float expf(int32_t number) {
    return ((0 < number) ? expULf(number) : (float)1 / expULf(-(number)));
  }
  constexpr static inline float expf(float number) {
    return ((0 < number) ? expUF(number) : (float)1 / expUF(-(number)));
  }
  constexpr static inline float logf(float x) {
    if (x == 1) return 0;
    if (x == 0) return NAN;
    uint64_t integer_component{0};
    for (; EULER < x; x /= (float)EULER) {
      integer_component++;
    }
    float retval{0};
    float term{1};
    float x_mines_one = (x - 1.0f);
    for (int64_t i{1}; i <= number_of_terms; i++) {
      term *= x_mines_one / x;
      retval += term / i;
    }
    return (float)integer_component + retval;
  }
  constexpr static inline float powUF(float base, float exponent) {
    if (base == 0) return 0;
    uint32_t exponent_int_component = (uint32_t)floor(exponent);
    exponent -= exponent_int_component;
    return expf(exponent * logf(base)) * powULf(base, exponent_int_component);
  }
  constexpr static inline float powULf(float base, uint32_t exponent) {
    float result{1};
    for (uint64_t i{}; i < exponent; i++) result *= base;
    return result;
  }
  constexpr static inline float powf(float base, float exponent) {
    return ((0 < exponent) ? (powUF(base, exponent))
                           : (1 / powUF(base, -exponent)));
  }
  constexpr static inline float sqrtf(float x) {
    if (!x) return 0;
    return expf(logf(x) * 0.5f);
  }
  constexpr static float AlphaLeaky = 0.01f;
  constexpr static float AlphaSELU = 1.6733f;
  constexpr static float LamdaSELU = 1.0507f;
  constexpr static float AlphaELU = 1.0f;
  constexpr static inline float Sigmoid(float x) {
    return 1.0f / (1 + expf(-x));
  }
  constexpr static inline float Tanh(float x) {
    return (expf(2 * x) - 1) / (expf(2 * x) + 1);
  }
  constexpr static inline float ReLU(float x) { return (x > 0) ? x : 0; }
  constexpr static inline float LeakyReLU(float x) { return LeakyELU(x); }
  constexpr static inline float LeakyReLUDer(const float fx) {
    return (fx > 0) ? 1 : AlphaLeaky;
  }
  constexpr static inline float LeakyELU(float x) {
    return (x > 0) ? x : AlphaLeaky * x;
  }
  constexpr static inline float ELU(float x) {
    return (x > 0) ? x : AlphaELU * (expf(x) - 1);
  }
  constexpr static inline float SELU(float x) {
    return (x > 0) ? x : AlphaSELU * (expf(x) - 1);
  }
  constexpr static inline double erf(double x) {
    if (x <= 0) return 0.0;
    if (4 < x) return 1.0;
    const constexpr double _2_ovr_sqrt_pi =
        2.0 / 1.7724538509055160272981674833411;
    double retval{0};
    double nag_x_sqr = -(x * x);
    for (uint32_t i{}; i < number_of_terms; i++) {
      double retval_buff = x / ((2 * i) + 1);
      uint32_t j{1};
      for (j = 1; j <= i; j++) {
        retval_buff *= nag_x_sqr / j;
      }
      retval += retval_buff;
    }
    return retval * _2_ovr_sqrt_pi;
  }
  constexpr static inline uint32_t factorial(uint32_t x) {
    uint32_t ret_val{1};
    while (x != 1) {
      ret_val *= x--;
    }
    return ret_val;
  }

 private:
 public:
  constexpr static inline double cos_rad_until_pi_over_2(double x) {
    constexpr double neg_1ovr_fact2 = -1.0 / factorial(2);
    constexpr double _1ovr_fact4 = 1.0 / factorial(4);
    constexpr double neg_1ovr_fact6 = -1.0 / factorial(6);
    constexpr double _1ovr_fact8 = 1.0 / factorial(8);
    constexpr double neg_1ovr_fact10 = -1.0 / factorial(10);
    if (abs(x - HALF_PI) < 0.001) return 0;
    double retval{};
    double x_2 = (x * x);
    retval =
        1.0 +
        x_2 *
            (neg_1ovr_fact2 +
             x_2 *
                 (_1ovr_fact4 +
                  x_2 *
                      neg_1ovr_fact6));  // we dont do all the polonomial just 4
    if (HALF_PI - abs(x) < 0.01) {
      double x_8 = powUL(x_2, 4);
      retval += x_8 * _1ovr_fact8;
      retval += x_8 * x_2 * neg_1ovr_fact10;
    }
    if (1 < retval) return 1;
    if (retval < -1) return -1;
    return retval;
  }

 private:
 public:
  constexpr static inline double sin_rad_until_pi_over_2(double x) {
    constexpr double _1ovr_fact1 = 1.0 / factorial(1);
    constexpr double neg_1ovr_fact3 = -1.0 / factorial(3);
    constexpr double _1ovr_fact5 = 1.0 / factorial(5);
    constexpr double neg_1ovr_fact7 = -1.0 / factorial(7);
    constexpr double _1ovr_fact9 = 1.0 / factorial(9);
    constexpr double neg_1ovr_fact11 = -1.0 / factorial(11);
    if (abs(x - HALF_PI) < 0.001) return 1;
    double retval{};
    double x_2 = (x * x);
    retval = x * (_1ovr_fact1 +
                  x_2 * (neg_1ovr_fact3 +
                         x_2 * (_1ovr_fact5 +
                                x_2 * neg_1ovr_fact7)));  // we dont do all the
                                                          // polonomial just 4

    if (HALF_PI - abs(x) < 0.01) {
      double x_9 = powUL(x_2, 4) * x;
      retval += x_9 * _1ovr_fact9;
      retval += x_9 * x_2 * neg_1ovr_fact11;
    }
    if (1 < retval) return 1;
    if (retval < -1) return -1;
    return retval;
  }
  constexpr static inline double cos_rad_until_pi(double x) {
    return HALF_PI < x ? -cos_rad_until_pi_over_2(PI - x)
                       : cos_rad_until_pi_over_2(x);
  }
  constexpr static inline double sin_rad_until_pi(double x) {
    return HALF_PI < x ? sin_rad_until_pi_over_2(PI - x)
                       : sin_rad_until_pi_over_2(x);
  }
  constexpr static inline double cos_rad_until_2_pi(double x) {
    if (x < PI) return cos_rad_until_pi(x);
    if (PI <= x && x < (PI + HALF_PI)) {
      return -cos_rad_until_pi_over_2(PI - x);
    }
    return cos_rad_until_pi_over_2(TWO_PI - x);
  }
  constexpr static inline double sin_rad_until_2_pi(double x) {
    if (x <= PI) return sin_rad_until_pi(x);
    if (PI < x && x < (PI + HALF_PI)) {
      return -sin_rad_until_pi(x - PI);
    }
    return -sin_rad_until_pi_over_2(TWO_PI - x);
  }
  constexpr static inline double radians_adjust_cos(double x) {
    if (x == NAN) return 0;
    if (x == INFINITY) return 0;
    if (x == -INFINITY) return 0;
    if (x < 0) x = -x;
    while (TWO_PI < x) x -= TWO_PI;
    return x;
  }
  constexpr static inline double radians_adjust_sin(double x) {
    if (x == NAN) return 0;
    if (x == INFINITY) return 0;
    if (x == -INFINITY) return 0;
    bool was_neg{};
    if (x < 0) {
      was_neg = 1;
      x = -x;
    }
    while (TWO_PI < x) x -= TWO_PI;
    return was_neg ? x + PI : x;
  }
  constexpr static inline double cos_rad(double x) {
    return cos_rad_until_2_pi(radians_adjust_cos(x));
  }
  constexpr static inline double sin_rad(double x) {
    return sin_rad_until_2_pi(radians_adjust_sin(x));
  }
  constexpr static inline double tan_rad(double x) {
    return sin_rad(x) / cos_rad(x);
  }
  constexpr static inline double cot_rad(double x) {
    return cos_rad(x) / sin_rad(x);
  }
  constexpr static inline float log(float x) { return (float)log((double)x); }
  constexpr static inline float erf(float x) { return (float)erf((double)x); }
  constexpr static inline double acos(double x) { return fastest_acos(x); }
  template <typename Type>
  constexpr static inline auto abs(const Type &x) -> decltype(x < 0 ? -x : x) {
    return x < 0 ? -x : x;
  }
  constexpr static inline double fastest_normal_acos(double x) {
    x = ((-0.69813170079773212 * x * x - 0.87266462599716477) * x +
         1.5707963267948966);
    x -= 0.0156668 * (0 < x && x < 0.5);
    x += 0.0156668 * (-0.5 < x && x < 0);
    x += 0.0958027 * (0.5 < x && x < 1);
    x -= 0.0958027 * (-1 < x && x < -0.5);
    return x;
  }
  constexpr static inline double fastest_acos(double x) {
    x = ((-0.69813170079773212 * x * x - 0.87266462599716477) * x +
         1.5707963267948966);
    return x;
  }
  constexpr static inline double sl_acos(double x) {
    double negate = double(x < 0);
    x = abs(x);
    double ret = -0.0187293;
    ret = ret * x;
    ret = ret + 0.0742610;
    ret = ret * x;
    ret = ret - 0.2121144;
    ret = ret * x;
    ret = ret + 1.5707288;
    ret = ret * sqrt(1.0 - x);
    ret = ret - 2 * negate * ret;
    return negate * 3.14159265358979 + ret;
  }
  constexpr static inline double meh_acos(double x) {
    double a = 1.43 + 0.59 * x;
    a = (a + (2 + 2 * x) / a) / 2;
    double b = 1.65 - 1.41 * x;
    b = (b + (2 - 2 * x) / b) / 2;
    double c = 0.88 - 0.77 * x;
    c = (c + (2 - a) / c) / 2;
    return (8 * (c + (2 - a) / c) - (b + (2 - 2 * x) / b)) / 6;
  }
  constexpr static inline double slow_acos(double a) {
    const double C = 0.10501094f;
    double r{}, s{}, t{}, u{};
    t = (a < 0) ? (-a) : a;  // handle negative arguments
    u = 1.0f - t;
    s = sqrt(u + u);
    r = C * u * s + s;      // or fmaf (C * u, s, s) if FMA support in hardware
    if (a < 0) r = PI - r;  // handle negative arguments
    return r;
  }
};

constexpr inline char char_to_char_for_reinterpret_fnc_ptr_default(
    char x_char_) {
  for (int64_t i{};
       i < mjz_ard::static_str_algo::the_reinterpreted_char_cca_size; i++) {
    if (x_char_ == mjz_ard::static_str_algo::the_in_reinterpreted_char_cca[i]) {
      return mjz_ard::static_str_algo::the_out_reinterpreted_char_cca[i];
    }
  }
  return x_char_;
}
constexpr inline bool is_forbiden_character_default(char x_char_) {
  for (int64_t i{}; i < mjz_ard::static_str_algo ::forbiden_chars_cnt_size;
       i++) {
    if (x_char_ == mjz_ard::static_str_algo::forbiden_chars_cnt[i]) {
      return 1;
    }
  }
  return 0;
}
template <int N>
class mjz_RingBufferN {
 public:
  uint8_t _aucBuffer[N];
  volatile int _iHead;
  volatile int _iTail;
  volatile int _numElems;

 public:
  mjz_RingBufferN(void) if_override_then_override;
  void store_char(uint8_t c) if_override_then_override;
  void clear() if_override_then_override;
  int read_char() if_override_then_override;
  int available() if_override_then_override;
  int availableForStore() if_override_then_override;
  int peek() if_override_then_override;
  bool isFull() if_override_then_override;

 protected:
  int nextIndex(int index);
  inline bool isEmpty() const { return (_numElems == 0); }
};
// The ptr class

class mjz_Str_DATA_storage_cls;
class mjz_Str_DATA_storage_cls
    : public std::enable_shared_from_this<mjz_Str_DATA_storage_cls> {
 public:
  std::function<bool(char)> is_blank_character = is_blank_characteres_default;
  std::function<char(char)> char_to_char_for_reinterpret_fnc_ptr =
      char_to_char_for_reinterpret_fnc_ptr_default;
  std::function<bool(char)> is_forbiden_character =
      is_forbiden_character_default;  // like bool
  // (*is_forbiden_character)(char)
  // but better
  char reinterpret_char_char = '\\';
  template <typename T>
  friend class mjz_str_t;
  std::shared_ptr<mjz_Str_DATA_storage_cls> getptr() {
    return shared_from_this();
  }
  // No public constructor, only a factory function, // so there's no way to
  // have getptr return nullptr.
  [[nodiscard]] static std::shared_ptr<mjz_Str_DATA_storage_cls> create() {
    // Not using std::make_shared<mjz_Str_DATA_storage_cls> because the c'tor is
    // private.
    return std::make_shared<mjz_Str_DATA_storage_cls>();
  }

  static std::shared_ptr<mjz_Str_DATA_storage_cls> &get_defult() {
    static std::shared_ptr<mjz_Str_DATA_storage_cls>
        main_mjz_Str_DATA_storage_Obj_ptr = mjz_Str_DATA_storage_cls::create();
    return main_mjz_Str_DATA_storage_Obj_ptr;
  }

 private:
  // mjz_Str_DATA_storage_cls() = default;
};
enum Dealocation_state : uint8_t {
  dont_deallocate_on_free = MJZ_logic_BIT(0),
  is_moved = MJZ_logic_BIT(1)
};

template <class T>
constexpr inline const T *begin(iterator_template_t<T> it) noexcept {
  return it.begin();
}

template <class T>
constexpr inline const T *end(iterator_template_t<T> it) noexcept {
  return it.end();
}

template <typename my_iner_Type_, bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1>
struct mjz_stack_obj_warper_template_t
    : private my_obj_creator_t  // for that 1 exetera (compiler added ) memory
{
  using Type = static_str_algo::remove_reference_t<my_iner_Type_>;

 public:
  static constexpr size_t sizeof_Type =
      max_macro_(sizeof(Type), my_obj_creator_t::size_of_type());
  constexpr inline my_obj_creator_t get_obj_creator() {
    return my_obj_creator_t(*((const my_obj_creator_t *)this));
  }

 protected:
  constexpr inline const my_obj_creator_t &m_obj_creator() const & {
    return *this;
  }
  constexpr inline my_obj_creator_t &m_obj_creator() & { return *this; }
  constexpr inline const my_obj_creator_t &&m_obj_creator() const && {
    return temp_me();
  }
  constexpr inline my_obj_creator_t &&m_obj_creator() && { return temp_me(); }

 private:
  struct {
    union {
      bool m_Has_data{false};
      mutable bool mm_Has_data;
    };
    union {
      mutable uint8_t mm_data[sizeof_Type];
      uint8_t m_data[sizeof_Type];
    };
  };
  constexpr inline Type *OP() & { return (Type *)&m_data[0]; }
  constexpr inline Type &O() & { return *OP(); }
  constexpr inline const Type *OP() const & { return (const Type *)&m_data[0]; }
  constexpr inline const Type &O() const & { return *OP(); }
  constexpr inline Type &&O() && { return std::move(*((Type *)&m_data[0])); }
  constexpr inline const Type &&O() const && {
    return std::move(*((const Type *)&m_data[0]));
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type &&src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, std::move(src));
      return place;
    }
    return m_obj_creator().construct_at(place, std::move(src));
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type &src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_obj_creator().construct_at(place, src);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            const Type &src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_obj_creator().construct_at(place, src);
  }
  template <typename... args_t>
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            args_t &&...args) {
    if (plc_has_obj) {
      uint8_t buf[sizeof_Type];
      Type *that = (Type *)buf;
      m_obj_creator().construct_at(that, std::forward<args_t>(args)...);
      m_obj_creator().obj_go_to_obj(*place, std::move(*that));
      m_obj_creator().destroy_at(that);
      return place;
    }
    return m_obj_creator().construct_at(place, std::forward<args_t>(args)...);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj) {
    if (plc_has_obj) {
      m_obj_creator().destroy_at(place);
      m_obj_creator().construct_at(place);
      return place;
    }
    return m_obj_creator().construct_at(place);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type *src) {
    return construct_in_place(place, *src);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            const Type *src) {
    return construct_in_place(place, *src);
  }
  constexpr inline void destroy_at_place(Type *place) {
    m_obj_creator().destroy_at(place);
  }
  template <typename... args_t>
  constexpr inline void construct(args_t &&...args) {
    if (pointer_to_unsafe_data() ==
        construct_in_place(pointer_to_unsafe_data(), m_Has_data,
                           std::forward<args_t>(args)...)) {
      m_Has_data = 1;
    } else {
      m_Has_data = 0;
    }
  }
  constexpr inline void destroy() {
    if (m_Has_data) destroy_at_place(pointer_to_unsafe_data());
    m_Has_data = 0;
  }
  constexpr inline Type *init_with_unsafe_data(bool initialized) {
    m_Has_data = initialized;
    return OP();
  }
  constexpr inline Type *move_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj,
                                std::move(*pointer_to_unsafe_data()));
    destroy();
    return 0;
  }
  constexpr inline Type *copy_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj, *pointer_to_unsafe_data());
    return 0;
  }

 public:
  constexpr inline Type &o() & { return **this; }
  constexpr inline Type *op() & { return this->operator->(); }
  constexpr inline const Type &o() const & { return **this; }
  constexpr inline const Type *op() const & { return this->operator->(); }
  constexpr inline Type &&o() && { return std::move(*temp_me()); }
  constexpr inline Type *op() && = delete;
  constexpr inline const Type &&o() const && { return std::move(*temp_me()); }
  constexpr inline const Type *op() const && = delete;
  // unsafe object functions begin
  constexpr inline Type &uo() &noexcept { return *uop(); }
  constexpr inline Type *uop() &noexcept {
    if (m_Has_data) return OP();
    return nullptr;
  }
  constexpr inline const Type &uo() const &noexcept { return *uop(); }
  constexpr inline const Type *uop() const &noexcept {
    if (m_Has_data) return OP();
    return nullptr;
  }
  constexpr inline Type &&uo() &&noexcept { return std::move(O()); }
  constexpr inline Type *uop() && = delete;
  constexpr inline const Type &&uo() const &&noexcept { return std::move(O()); }
  constexpr inline const Type *uop() const && = delete;
  // unsafe object functions end

  constexpr inline mjz_stack_obj_warper_template_t() {
    if constexpr (construct_obj_on_constructor) {
      construct();
    }
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_t(T0 &&arg0, T_s &&...args) {
    construct(std::move(arg0), std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_t(const T0 &arg0,
                                                   T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_t(T0 &arg0, T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  constexpr inline ~mjz_stack_obj_warper_template_t() { de_init(); }
  constexpr inline mjz_stack_obj_warper_template_t(
      mjz_stack_obj_warper_template_t &&s_obj_w) {
    if (s_obj_w.has_data()) construct(std::move(s_obj_w.operator*()));
  }
  constexpr inline mjz_stack_obj_warper_template_t(
      const mjz_stack_obj_warper_template_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }
  constexpr inline mjz_stack_obj_warper_template_t(
      mjz_stack_obj_warper_template_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }
  constexpr inline mjz_stack_obj_warper_template_t(const Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_t(Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_t(Type &&obj) {
    construct(std::move(obj));
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_t &operator=(Type &&obj) {
    construct(std::move(obj));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator=(Type &obj) {
    construct(obj);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator=(const Type &obj) {
    construct(obj);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator=(
      mjz_stack_obj_warper_template_t &&s_obj_w) {
    if (!!s_obj_w) operator=(std::move(s_obj_w.operator*()));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator=(
      const mjz_stack_obj_warper_template_t &s_obj_w) {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator=(
      mjz_stack_obj_warper_template_t &s_obj_w) {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }

 public:
  constexpr inline void init(const mjz_stack_obj_warper_template_t &obj) & {
    operator=(obj);
  }
  constexpr inline void init(mjz_stack_obj_warper_template_t &obj) & {
    operator=(obj);
  }
  constexpr inline void init(mjz_stack_obj_warper_template_t &&obj) & {
    operator=(std::move(obj));
  }
  template <typename arg_T, typename Type>
  constexpr inline void init(std::initializer_list<arg_T> list) & {
    construct(list);
  }
  template <typename arg_T>
  constexpr inline void init(iterator_template_t<arg_T> list) & {
    construct(list);
  }
  template <typename... arguments_types>
  constexpr inline void init(arguments_types &&...args) & {
    construct(std::forward<arguments_types>(args)...);
  }

 public:
  constexpr inline void de_init() { destroy(); }
  constexpr inline void de_init(int fill_VAL) {
    destroy();
    static_str_algo::memset(m_data, fill_VAL, sizeof_Type);
  }

 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using T = Type;
  using type = Type;

 public:
  constexpr inline Type *pointer_to_unsafe_data_for_unsafe_placement_new(
      Type *ptr) & {
    destroy();
    return pointer_to_unsafe_data();
  }
  constexpr inline Type *init_with_unsafe_placement_new(
      Type *ptr) & {  // placement new "new (ptr) Type();"
    return init_with_unsafe_data(ptr == pointer_to_unsafe_data());
  }

 public:
  constexpr inline Type &if_no_obj_then_create() & {
    if (!m_Has_data) construct();
    return *pointer_to_unsafe_data();
  }
  Type &&if_no_obj_then_create() && {
    if (!m_Has_data) construct();
    return std::move(temp_me().O());
  }
  Type &&if_no_obj_then_create() const && {
    if (!m_Has_data) construct();
    return std::move(temp_me().O());
  }

 public:
  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() & {
    return (uint8_t *)(m_data);
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const & {
    return (uint8_t *)(m_data);
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const && =
      delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() const && = delete;
  constexpr inline Type *pointer_to_unsafe_data() & { return OP(); }
  constexpr inline const Type *pointer_to_unsafe_data() const & { return OP(); }
  constexpr inline const Type *throw_if_no_data_or_give_data() && = delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const && =
      delete;

 public:
  constexpr inline const Type *throw_if_no_data_or_give_data() const & {
    if (!m_Has_data) {
      if constexpr (do_error_check) {
        throw std::out_of_range(
            "mjz_ard::mjz_stack_obj_warper_template_t::pointer_to_data bad "
            "access");
      } else {
        return (const Type *)nullptr;  // :(
      }
    }
    return pointer_to_unsafe_data();
  }
  constexpr inline const Type *pointer_to_data() && = delete;
  constexpr inline const Type *pointer_to_data() const && = delete;
  constexpr inline const Type *pointer_to_data() const & {
    return throw_if_no_data_or_give_data();
  }
  constexpr inline Type *pointer_to_data() & {
    return mjz_ard::remove_const(throw_if_no_data_or_give_data());
  }

 public:
  constexpr inline Type *operator->() & { return pointer_to_data(); }
  constexpr inline Type &&operator->() && {
    if (has_data()) return std::move(temp_me().O());
    throw std::out_of_range{
        "mjz_ard::mjz_stack_obj_warper_template_t::pointer_to_data bad "
        "access"};
  }  // overload dosnt give ptr
  constexpr inline const Type &&operator->() const && {
    if (has_data()) return std::move(temp_me().O());
    throw std::out_of_range{
        "mjz_ard::mjz_stack_obj_warper_template_t::pointer_to_data bad "
        "access"};
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) const & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  constexpr inline void operator->*(my_type my_var) && = delete;
  template <typename my_type>
  constexpr inline void operator->*(my_type my_var) const && = delete;
  constexpr inline Type &&operator*() && {
    return std::move(temp_me().operator->());
  }
  constexpr inline const Type &&operator*() const && {
    return std::move(temp_me().operator->());
  }
  constexpr inline Type &operator*() & { return *operator->(); }
  constexpr inline mjz_stack_obj_warper_template_t *operator&() & {
    return this;
  }  // &obj
  constexpr inline mjz_stack_obj_warper_template_t *operator&() const & {
    return this;
  }
  constexpr inline mjz_stack_obj_warper_template_t *operator&() && = delete;
  constexpr inline mjz_stack_obj_warper_template_t *operator&() const && =
      delete;
  constexpr inline Type *operator&(int) & {
    return pointer_to_data();
  }  // obj&0
  constexpr inline Type *operator&(int) && = delete;
  constexpr inline Type *operator&(int) const && = delete;
  constexpr inline const Type *operator&(int) const & {
    return pointer_to_data();
  }  // obj&0
  constexpr inline const Type *operator->() const & {
    return pointer_to_data();
  }

  constexpr inline mjz_stack_obj_warper_template_t &&temp_me() {
    return std::move(*this);
  }

  constexpr inline const mjz_stack_obj_warper_template_t &&temp_me() const {
    return std::move(*this);
  }
  constexpr inline const Type &operator*() const & { return *operator->(); }
  constexpr inline const Type &operator()() const & { return **this; }
  constexpr inline Type &&operator()() && { return std::move(*temp_me()); }
  constexpr inline const Type &&operator()() const && {
    return std::move(*temp_me());
  }
  constexpr inline Type &operator()() & { return **this; }
  using my_Type_t = mjz_stack_obj_warper_template_t;
  constexpr inline my_Type_t &operator~() & {
    de_init();
    return *this;
  }
  constexpr inline my_Type_t &operator+() & {
    if (has_data()) {
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &operator-() & {
    if (has_data()) {
      de_init();
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &&operator~() && {
    de_init();
    return temp_me();
  }
  constexpr inline my_Type_t &&operator+() && {
    if (has_data()) {
      return temp_me();
    }
    construct();
    return temp_me();
  }
  constexpr inline my_Type_t &&operator-() && {
    if (has_data()) {
      de_init();
      return temp_me();
    }
    construct();
    return temp_me();
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator--() & {
    de_init();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &&operator--() && {
    de_init();
    return temp_me();
  }
  constexpr inline mjz_stack_obj_warper_template_t &&operator--() const && =
      delete;
  constexpr inline mjz_stack_obj_warper_template_t operator--(int) {
    my_Type_t temp(std::move(**this));
    de_init();
    return temp;
  }
  constexpr inline mjz_stack_obj_warper_template_t &operator++() & {
    // de_init();
    construct();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_t &&operator++() && {
    // de_init();
    construct();
    return temp_me();
  }
  constexpr inline mjz_stack_obj_warper_template_t &&operator++() const && =
      delete;
  constexpr inline mjz_stack_obj_warper_template_t operator++(int) {
    my_Type_t temp(std::move(**this));
    // you may want to reuse data soo
    // de_init();
    construct();
    return temp;
  }
  Type *begin() & { return pointer_to_data(); }
  Type *end() & { return begin() + 1; }
  const Type *begin() const & { return pointer_to_data(); }
  const Type *end() const & { return begin() + 1; }
  const Type *cbegin() const & { return begin(); }
  const Type *cend() const & { return end(); }
  const Type *begin() && = delete;
  const Type *end() && = delete;
  const Type *cbegin() && = delete;
  const Type *cend() && = delete;
  const Type *begin() const && = delete;
  const Type *end() const && = delete;
  const Type *cbegin() const && = delete;
  const Type *cend() const && = delete;
  constexpr inline static size_t size() { return 1; }  // for iterator
  constexpr inline static size_t my_size() { return sizeof_Type; }
  constexpr inline static size_t size_T() { return my_size(); }
  constexpr inline Type &operator()(Type &&moved) & {
    return *this = std::move(moved);
  }
  constexpr inline Type &operator()(Type &moved) & { return *this = moved; }
  constexpr inline Type &operator()(const Type &moved) & {
    return *this = moved;
  }
  constexpr inline Type &operator()(Type &&moved) const && = delete;
  constexpr inline Type &operator()(Type &moved) const && = delete;
  constexpr inline Type &operator()(const Type &moved) const && = delete;
  constexpr inline Type &operator()(Type &&moved) && = delete;
  constexpr inline Type &operator()(Type &moved) && = delete;
  constexpr inline Type &operator()(const Type &moved) && = delete;
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) && {
    return f(std::move(*temp_me()), std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const && {
    return f(std::move(*temp_me()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) & {
    if (has_data()) f(O(), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) && {
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) const && {
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) const & {
    if (has_data()) f(O(), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) const & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) && {
    if (do_throw) {
      f(std::move(*temp_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) const && {
    if (do_throw) {
      f(std::move(*temp_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(temp_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(temp_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(temp_me().O()),
                        std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(temp_me().O()),
                        std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }

 public:
  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      mjz_stack_obj_warper_template_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_stack_obj_warper_template_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      mjz_stack_obj_warper_template_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_stack_obj_warper_template_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      mjz_stack_obj_warper_template_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_stack_obj_warper_template_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool operator==(mjz_stack_obj_warper_template_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(mjz_stack_obj_warper_template_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(mjz_stack_obj_warper_template_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(mjz_stack_obj_warper_template_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(mjz_stack_obj_warper_template_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(mjz_stack_obj_warper_template_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool has_data() const { return m_Has_data; }
  constexpr inline bool operator!() const noexcept { return !m_Has_data; }
  constexpr inline operator Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() const & { return pointer_to_data(); }
  constexpr inline operator Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() const & {
    return *pointer_to_data();
  }
  constexpr inline operator Type &&() && { return std::move(*temp_me()); }
  constexpr inline operator const Type &&() && { return std::move(*temp_me()); }
  constexpr inline operator const Type &&() const && {
    return std::move(*temp_me());
  }
  constexpr inline operator Type() & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() const & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() && {
    return get_obj_creator().obj_constructor(std::move(*temp_me()));
  }
  constexpr inline operator Type() const && {
    return get_obj_creator().obj_constructor(std::move(*temp_me()));
  }
  constexpr explicit operator bool() const noexcept { return has_data(); }
  constexpr bool has_value() const noexcept { return has_data(); }
  template <class... Args>
  T &emplace(Args &&...args) & {
    construct(std::forward<Args>(args)...);
  }
  template <class U, class... Args>
  T &emplace(std::initializer_list<U> ilist, Args &&...args) & {
    construct(ilist, std::forward<Args>(args)...);
  }
  void reset() noexcept {
    try {
      ~(*this);
    } catch (...) {
    }
  }
  template <class F>
  mjz_stack_obj_warper_template_t or_else(F &&f) const & {
    return has_data() ? *this : std::forward<F>(f)();
  }
  template <class F>
  mjz_stack_obj_warper_template_t or_else(F &&f) && {
    return has_data() ? temp_me() : std::forward<F>(f)();
  }
  constexpr Type &&value() && { return *temp_me(); }
  constexpr const Type &value() const & { return **this; }
  constexpr Type &value() & { return **this; }
  template <class U>
  constexpr Type value_or(U &&default_value) const & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) && {
    return has_data() ? std::move(*temp_me())
                      : static_cast<T>(std::forward<U>(default_value));
  }
  template <class F>
  constexpr auto transform(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(O()))>(
          std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(O()))>(
          std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().O())))>(
          std::forward<F>(f)(std::move(temp_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().O())))>(
          std::forward<F>(f)(std::move(temp_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(O()))>(
          std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(O()))>(
          std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().O())))>(
          std::forward<F>(f)(std::move(temp_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().O())))>(
          std::forward<F>(f)(std::move(temp_me().O())));
    }
    return {};
  }

 public:
  constexpr inline Type *copy_to(Type *dest, bool dest_has_obj) & {
    return copy_to_place(dest, dest_has_obj);
  }
  constexpr inline Type *move_to(Type *dest, bool dest_has_obj) {
    return move_to_place(dest, dest_has_obj);
  }
  constexpr inline Type &copy_to(Type &dest, bool dest_has_obj) & {
    return *copy_to(&dest, dest_has_obj);
  }
  constexpr inline Type &move_to(Type &dest, bool dest_has_obj) {
    return *move_to(&dest, dest_has_obj);
  }
  constexpr inline mjz_stack_obj_warper_template_t &copy_to(
      mjz_stack_obj_warper_template_t &dest) & {
    if (this != &dest)
      dest.init_with_unsafe_placement_new(
          copy_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_t &move_to(
      mjz_stack_obj_warper_template_t &dest) {
    if (this != &dest)
      dest.init_with_unsafe_placement_new(
          move_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_t *copy_to(
      mjz_stack_obj_warper_template_t *dest) & {
    return &copy_to(*dest);
  }
  constexpr inline mjz_stack_obj_warper_template_t *move_to(
      mjz_stack_obj_warper_template_t *dest) {
    return &move_to(*dest);
  }

 public:  // unsafe may cuse undefined behavior
  mjz_stack_obj_warper_template_t &remove_const() const & {
    return *mjz_ard::remove_const(this);
  }
  mjz_stack_obj_warper_template_t &&remove_const() const && {
    return std::move(*mjz_ard::remove_const(this));
  }
  mjz_stack_obj_warper_template_t &remove_const() & { return *this; }
  mjz_stack_obj_warper_template_t &&remove_const() && {
    return std::move(*this);
  }
  Type &remove_const_obj() const & { return *((Type *)mm_data); }
  Type &&remove_const_obj() const && { return std::move(*((Type *)mm_data)); }
  Type &remove_const_obj() & { return *((Type *)mm_data); }
  Type &&remove_const_obj() && { return std::move(*((Type *)mm_data)); }
  const Type &&move() const & { return std::move(o()); }
  const Type &&move() const && { return std::move(o()); }
  Type &&move() & { return std::move(o()); }
  Type &&move() && { return std::move(o()); }
};

template <class Type, const size_t m_Size, bool error_check = 1,
          bool construct_obj_on_constructor = 1,
          class my_obj_creator = mjz_temp_type_obj_algorithims_warpper_t<
              Type>>        // i promise that their is no allocation
class extended_mjz_Array {  // fixed size extended_mjz_Array of values
 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;
  using const_reference = const Type &;
  using size_type = size_t;
  using my_stack_obj_buffer_t =
      mjz_stack_obj_warper_template_t<Type, construct_obj_on_constructor,
                                      my_obj_creator>;
  using iterator_r = iterator_template_t<my_stack_obj_buffer_t, error_check>;
  using const_iterator_r =
      iterator_template_t<const my_stack_obj_buffer_t, error_check>;
  using iterator = iterator_template_ptr_warper<iterator_r, Type>;
  using const_iterator = iterator_template_ptr_warper<const_iterator_r, Type>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  constexpr inline void assign(const Type &_Value) { fill(_Value); }
  constexpr inline void fill(const Type &value) {
    iterator fst = begin();
    iterator lst = end();
    while (fst < lst) *fst++ = value;
  }
  constexpr inline void swap(extended_mjz_Array &other) {
    iterator fst[2] = {begin(), other.begin()};
    iterator lst[2] = {end(), other.end()};
    while ((fst[0] < lst[0]) && (fst[1] < lst[1]))
      std::swap(*fst[0]++, *fst[1]++);
  }
  [[nodiscard]] iterator begin() noexcept { return base(); }
  constexpr inline [[nodiscard]] const_iterator begin() const noexcept {
    return base();
  }
  constexpr inline [[nodiscard]] iterator end() noexcept {
    return base() + m_Size;
  }
  constexpr inline [[nodiscard]] const_iterator end() const noexcept {
    return base() + m_Size;
  }
  constexpr inline [[nodiscard]] reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr inline [[nodiscard]] const_reverse_iterator rbegin()
      const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr inline [[nodiscard]] reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr inline [[nodiscard]] const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr inline [[nodiscard]] const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr inline [[nodiscard]] const_iterator cend() const noexcept {
    return end();
  }
  constexpr inline [[nodiscard]] const_reverse_iterator crbegin()
      const noexcept {
    return rbegin();
  }
  constexpr inline [[nodiscard]] const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr inline [[nodiscard]] size_type size() const noexcept {
    return m_Size;
  }
  constexpr inline [[nodiscard]] size_type max_size() const noexcept {
    return m_Size;
  }
  constexpr inline bool empty() const noexcept { return !!m_Size; }
  constexpr inline [[nodiscard]] reference at(size_type _Pos) {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] const_reference at(size_type _Pos) const {
    if constexpr (error_check) {
      if (m_Size <= _Pos) {
        invld_throw();
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] reference operator[](size_type _Pos)
  /* strengthened */ {
    if constexpr (error_check) {
      if (_Pos >= m_Size) {
        throw std::out_of_range{"extended_mjz_Array subscript out of range"};
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] const_reference operator[](
      size_type _Pos) const noexcept
  /* strengthened */ {
    if constexpr (error_check) {
      {
        if (_Pos >= m_Size)
          throw std::out_of_range{"extended_mjz_Array subscript out of range"};
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] reference front() noexcept
  /* strengthened */ {
    return *m_elements[0];
  }
  constexpr inline [[nodiscard]] const_reference front() const noexcept
  /* strengthened */ {
    return *m_elements[0];
  }
  constexpr inline [[nodiscard]] reference back() noexcept
  /* strengthened */ {
    return *m_elements[m_Size - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept
  /* strengthened */ {
    return *m_elements[m_Size - 1];
  }
  constexpr inline [[nodiscard]] my_stack_obj_buffer_t *data() noexcept {
    return m_elements;
  }
  constexpr inline [[nodiscard]] const my_stack_obj_buffer_t *data()
      const noexcept {
    return m_elements;
  }
  constexpr [[nodiscard]] inline iterator_r get_wrapper_it() noexcept {
    return {m_elements, m_Size};
  }
  constexpr inline [[nodiscard]] const_iterator_r get_wrapper_it()
      const noexcept {
    return {m_elements, m_Size};
  }
  [[noreturn]] void invld_throw() const {
    throw std::out_of_range("invalid extended_mjz_Array<T, N> subscript");
  }
  constexpr inline iterator_r base() { return iterator_r(m_elements, m_Size); }
  constexpr inline const_iterator_r base() const {
    return const_iterator_r(m_elements, m_Size);
  }
  my_stack_obj_buffer_t m_elements[m_Size];
};
template <typename my_iner_Type_, bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>>
class mjz_heap_obj_warper_template_t {
 public:
  using Type = static_str_algo::remove_reference_t<my_iner_Type_>;
  using container_Type =
      mjz_stack_obj_warper_template_t<Type, construct_obj_on_constructor,
                                      my_obj_creator_t>;

  class my_unique_ptr_of_container_Type
      : private mjz_allocator_warpper<container_Type> {
    constexpr inline mjz_allocator_warpper<container_Type> &get_obj_creator() {
      return *this;
    }
    mutable container_Type
        *m_obj{};  // sorry for <mutable> but it is what it is
    constexpr inline container_Type *get_then_null() {
      container_Type *t = m_obj;
      m_obj = 0;
      return t;
    }

   public:
    constexpr inline my_unique_ptr_of_container_Type(
        my_unique_ptr_of_container_Type &&otr) {
      m_obj = otr.get_then_null();
    }

    constexpr inline my_unique_ptr_of_container_Type(
        const my_unique_ptr_of_container_Type &&otr) {
      m_obj = otr.get_then_null();
    }

    constexpr inline my_unique_ptr_of_container_Type(
        container_Type *valid_heap_object) {
      m_obj = valid_heap_object;
    }

    constexpr inline my_unique_ptr_of_container_Type &operator=(
        my_unique_ptr_of_container_Type &&otr) {
      if (&otr == this) return *this;
      m_obj = otr.get_then_null();
      return *this;
    }

    constexpr inline my_unique_ptr_of_container_Type &operator=(
        const my_unique_ptr_of_container_Type &&otr) {
      if (&otr == this) return *this;
      m_obj = otr.get_then_null();
      return *this;
    }
    my_unique_ptr_of_container_Type(const my_unique_ptr_of_container_Type &) =
        delete;
    my_unique_ptr_of_container_Type &operator=(
        const my_unique_ptr_of_container_Type &) = delete;

    my_unique_ptr_of_container_Type(my_unique_ptr_of_container_Type &) = delete;
    my_unique_ptr_of_container_Type &operator=(
        my_unique_ptr_of_container_Type &) = delete;

    constexpr inline ~my_unique_ptr_of_container_Type() {
      if (m_obj) {
        get_obj_creator().destroy_at(m_obj);
        get_obj_creator().deallocate(m_obj, 1);
        m_obj = 0;
      }
    }
    template <typename... args_t>

    constexpr static inline my_unique_ptr_of_container_Type make_unique(
        args_t &&...args) {
      my_unique_ptr_of_container_Type ret;
      ret.m_obj = (ret.get_obj_creator().allocate(1));
      if (!ret.m_obj) return ret;
      ret.get_obj_creator().construct_at(ret.m_obj,
                                         std::forward<args_t>(args)...);
      return ret;
    }

    constexpr inline container_Type &operator*() & { return *m_obj; }
    constexpr inline const container_Type &operator*() const & {
      return *m_obj;
    }
    constexpr inline container_Type &&operator*() && { return *m_obj; }
    constexpr inline const container_Type &&operator*() const && {
      return *m_obj;
    }

    constexpr inline Type &operator()() & { return **m_obj; }
    constexpr inline const Type &operator()() const & { return **m_obj; }
    constexpr inline Type &&operator()() && { return *std::move(*m_obj); }
    constexpr inline const Type &&operator()() const && {
      return *std::move(*m_obj);
    }

    constexpr inline container_Type *operator->() { return m_obj; }
    constexpr inline const container_Type *operator->() const { return m_obj; }

    constexpr inline container_Type *get_ptr() & { return m_obj; }
    constexpr inline const container_Type *get_ptr() const & { return m_obj; }
    constexpr inline container_Type *get_ptr() && { return m_obj; }
    constexpr inline const container_Type *get_ptr() const && { return m_obj; }
    // u before name menes it prevents deallocation  and is unsafe !
    constexpr inline container_Type *uget_ptr() && { return get_then_null(); }
    constexpr inline const container_Type *uget_ptr() const && {
      return get_then_null();
    }
    constexpr inline operator bool() const { return !!m_obj; }
    constexpr inline bool operator!() const { return !m_obj; }

   private:
    constexpr inline my_unique_ptr_of_container_Type() : m_obj(0) {}
  };
  using container_Type_ptr =
      my_unique_ptr_of_container_Type;  // std::unique_ptr<container_Type>;
 private:
  container_Type_ptr m_ptr;
  static constexpr size_t sizeof_Type = container_Type::sizeof_Type;
  container_Type &&base_mv() { return std::move(*this).base(); }

 public:
  constexpr inline ~mjz_heap_obj_warper_template_t() {}
  constexpr inline mjz_heap_obj_warper_template_t()
      : m_ptr(container_Type_ptr::make_unique()) {}
  constexpr inline mjz_heap_obj_warper_template_t(
      mjz_heap_obj_warper_template_t &&temp)
      : m_ptr(std::move(temp.m_ptr)) {}
  constexpr inline mjz_heap_obj_warper_template_t(
      const mjz_heap_obj_warper_template_t &&temp)
      : m_ptr(std::move(temp.m_ptr)) {}
  constexpr inline mjz_heap_obj_warper_template_t(
      mjz_heap_obj_warper_template_t &temp)
      : m_ptr(container_Type_ptr::make_unique(*temp.m_ptr)) {}
  constexpr inline mjz_heap_obj_warper_template_t(
      const mjz_heap_obj_warper_template_t &temp)
      : m_ptr(container_Type_ptr::make_unique(*temp.m_ptr)) {}
  template <typename T0, typename... Ts>
  constexpr inline mjz_heap_obj_warper_template_t(T0 &&arg0, Ts &&...args)
      : m_ptr(container_Type_ptr::make_unique(std::forward<T0>(arg0),
                                              std::forward<Ts>(args)...)) {}
  constexpr inline container_Type &base() & { return *m_ptr; }
  constexpr inline const container_Type &base() const & { return *m_ptr; }
  constexpr inline container_Type &&base() && { return std::move(*m_ptr); }
  constexpr inline const container_Type &&base() const && {
    return std::move(*m_ptr);
  }

 private:
  constexpr inline container_Type_ptr &ptr() & { return m_ptr; }
  constexpr inline const container_Type_ptr &ptr() const & { return m_ptr; }
  constexpr inline container_Type_ptr &&ptr() && { return std::move(m_ptr); }
  constexpr inline const container_Type_ptr &&ptr() const && {
    return std::move(m_ptr);
  }

 public:
  constexpr inline container_Type_ptr get_ptr_then_iam_invalid() && {
    return std::move(m_ptr);
  }
  constexpr inline container_Type_ptr uptr() && { return std::move(m_ptr); }
  mjz_heap_obj_warper_template_t(container_Type_ptr &&my_ptr)
      : m_ptr(std::move(my_ptr)) {
    if (!m_ptr) throw " NO DATA GIVEN ";
  }

  constexpr inline container_Type_ptr get_ptr_then_iam_invalid() const && {
    return std::move(m_ptr);
  }
  constexpr inline container_Type_ptr uptr() const && {
    return std::move(m_ptr);
  }
  mjz_heap_obj_warper_template_t(const container_Type_ptr &&my_ptr)
      : m_ptr(std::move(my_ptr)) {
    if (!m_ptr) throw " NO DATA GIVEN ";
  }

  constexpr inline mjz_heap_obj_warper_template_t &operator=(
      mjz_heap_obj_warper_template_t &&obj) {
    base() = std::move(obj.base());
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator=(
      mjz_heap_obj_warper_template_t &obj) {
    base() = obj.base();
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator=(
      const mjz_heap_obj_warper_template_t &&obj) {
    base() = std::move(obj.base());
    return *this;
  }
  template <typename T0>
  constexpr inline mjz_heap_obj_warper_template_t &operator=(T0 &&arg0) {
    base().operator=(std::forward<T0>(arg0));
    return *this;
  }
  constexpr inline my_obj_creator_t get_obj_creator() {
    return base().get_obj_creator();
  }

 private:
  constexpr inline Type &O() &noexcept { return base().uo(); }
  constexpr inline Type *Op() &noexcept { return base().uop(); }
  constexpr inline const Type &O() const &noexcept { return base().uo(); }
  constexpr inline const Type *Op() const &noexcept { return base().uop(); }
  constexpr inline Type &&O() &&noexcept { return base_mv().uo(); }
  constexpr inline Type *Op() && = delete;
  constexpr inline const Type &&O() const &&noexcept { return base_mv().uo(); }
  constexpr inline const Type *Op() const && = delete;

 public:
  constexpr inline Type &o() & { return base().o(); }
  constexpr inline Type *op() & { return base().o(); }
  constexpr inline const Type &o() const & { return base().o(); }
  constexpr inline const Type *op() const & { return base().op(); }
  constexpr inline Type &&o() && { return base_mv().o(); }
  constexpr inline Type *op() && = delete;
  constexpr inline const Type &&o() const && { return base_mv().o(); }
  constexpr inline const Type *op() const && = delete;
  // unsafe object functions begin
  constexpr inline Type &uo() &noexcept { return base().uo(); }
  constexpr inline Type *uop() &noexcept { return base().uop(); }
  constexpr inline const Type &uo() const &noexcept { return base().uo(); }
  constexpr inline const Type *uop() const &noexcept { return base().uop(); }
  constexpr inline Type &&uo() &&noexcept { return base_mv().uo(); }
  constexpr inline Type *uop() && = delete;
  constexpr inline const Type &&uo() const &&noexcept { return base_mv().uo(); }
  constexpr inline const Type *uop() const && = delete;
  // unsafe object functions end

 public:
  constexpr inline mjz_heap_obj_warper_template_t &operator=(Type &&obj) {
    base().operator=(std::move(obj));
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator=(Type &obj) {
    base().operator=(obj);
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator=(const Type &obj) {
    base().operator=(obj);
    return *this;
  }

 public:
  constexpr inline void init(const mjz_heap_obj_warper_template_t &obj) & {
    return base().init(obj);
  }
  constexpr inline void init(mjz_heap_obj_warper_template_t &obj) & {
    return base().init(obj);
  }
  constexpr inline void init(mjz_heap_obj_warper_template_t &&obj) & {
    return base().init(obj);
  }
  template <typename arg_T, typename Type>
  constexpr inline void init(std::initializer_list<arg_T> list) & {
    return base().init(list);
  }
  template <typename arg_T>
  constexpr inline void init(iterator_template_t<arg_T> list) & {
    return base().init(list);
  }
  template <typename... arguments_types>
  constexpr inline void init(arguments_types &&...args) & {
    return base().init(std::forward<arguments_types>(args)...);
  }

 public:
  constexpr inline void de_init() { return base().de_init(); }
  constexpr inline void de_init(int fill_VAL) {
    return base().de_init(fill_VAL);
  }

 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using T = Type;
  using type = Type;

 public:
  constexpr inline Type *pointer_to_unsafe_data_for_unsafe_placement_new(
      Type *ptr) & {
    return base().pointer_to_unsafe_data_for_unsafe_placement_new(ptr);
  }
  constexpr inline Type *init_with_unsafe_placement_new(Type *ptr) & {
    return base().init_with_unsafe_placement_new(ptr);
  }

 public:
  constexpr inline Type &if_no_obj_then_create() & {
    return base().if_no_obj_then_create();
  }
  constexpr inline Type &&if_no_obj_then_create() && {
    return base_mv().if_no_obj_then_create();
  }
  constexpr inline Type &&if_no_obj_then_create() const && {
    return base_mv().if_no_obj_then_create();
  }

 public:
  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() & {
    return base().pointer_to_unsafe_data_buffer();
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const & {
    return base().pointer_to_unsafe_data_buffer();
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const && =
      delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() const && = delete;
  constexpr inline Type *pointer_to_unsafe_data() & {
    return base().pointer_to_unsafe_data();
  }
  constexpr inline const Type *pointer_to_unsafe_data() const & {
    return base().pointer_to_unsafe_data();
  }
  constexpr inline const Type *throw_if_no_data_or_give_data() && = delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const && =
      delete;

 public:
  constexpr inline const Type *throw_if_no_data_or_give_data() const & {
    return base().throw_if_no_data_or_give_data();
  }
  constexpr inline const Type *pointer_to_data() && = delete;
  constexpr inline const Type *pointer_to_data() const && = delete;
  constexpr inline const Type *pointer_to_data() const & {
    return base().pointer_to_data();
  }
  constexpr inline Type *pointer_to_data() & {
    return base().pointer_to_data();
  }

 public:
  constexpr inline Type *operator->() & { return base().operator->(); }
  constexpr inline Type &&operator->() && {
    return base_mv().operator->();
  }  // overload dosnt give ptr
  constexpr inline const Type &&operator->() const && {
    return base_mv().operator->();
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) const & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  inline void operator->*(my_type my_var) && = delete;
  template <typename my_type>
  inline void operator->*(my_type my_var) const && = delete;
  constexpr inline Type &&operator*() && { return base_mv().operator*(); }
  constexpr inline const Type &&operator*() const && {
    return base_mv().operator*();
  }
  constexpr inline Type &operator*() & { return base().operator*(); }
  constexpr inline mjz_heap_obj_warper_template_t *operator&() & {
    return this;
  }  // &obj
  constexpr inline mjz_heap_obj_warper_template_t *operator&() const & {
    return this;
  }
  constexpr inline mjz_heap_obj_warper_template_t *operator&() && = delete;
  constexpr inline mjz_heap_obj_warper_template_t *operator&() const && =
      delete;
  constexpr inline Type *operator&(int) & {
    return pointer_to_data();
  }  // obj&0
  constexpr inline Type *operator&(int) && = delete;
  constexpr inline Type *operator&(int) const && = delete;
  constexpr inline const Type *operator&(int) const & {
    return pointer_to_data();
  }  // obj&0
  constexpr inline const Type *operator->() const & {
    return pointer_to_data();
  }
  constexpr inline mjz_heap_obj_warper_template_t &&temp_me() {
    return std::move(*this);
  }
  constexpr inline const mjz_heap_obj_warper_template_t &&temp_me() const {
    return std::move(*this);
  }
  constexpr inline const Type &operator*() const & { return *operator->(); }
  constexpr inline const Type &operator()() const & { return **this; }
  constexpr inline Type &&operator()() && { return std::move(*temp_me()); }
  constexpr inline const Type &&operator()() const && {
    return std::move(*temp_me());
  }
  constexpr inline Type &operator()() & { return **this; }
  using my_Type_t = mjz_heap_obj_warper_template_t;
  constexpr inline my_Type_t &operator~() & {
    base().operator~();
    return *this;
  }
  constexpr inline my_Type_t &operator+() & {
    base().operator+();
    return *this;
  }
  constexpr inline my_Type_t &operator-() & {
    base().operator-();
    return *this;
  }
  constexpr inline my_Type_t &&operator~() && {
    base_mv().operator~();
    return *this;
  }
  constexpr inline my_Type_t &&operator+() && {
    base_mv().operator+();
    return *this;
  }
  constexpr inline my_Type_t &&operator-() && {
    base_mv().operator~();
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator--() & {
    de_init();
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &&operator--() && {
    de_init();
    return temp_me();
  }
  constexpr inline mjz_heap_obj_warper_template_t &&operator--() const && =
      delete;
  constexpr inline mjz_heap_obj_warper_template_t operator--(int) {
    my_Type_t temp(std::move(**this));
    de_init();
    return temp;
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator++() & {
    // de_init();
    init();
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &&operator++() && {
    // de_init();
    init();
    return temp_me();
  }
  constexpr inline mjz_heap_obj_warper_template_t &&operator++() const && =
      delete;
  constexpr inline mjz_heap_obj_warper_template_t operator++(int) {
    my_Type_t temp(std::move(**this));
    // you may want to reuse data soo
    // de_init();
    init();
    return temp;
  }
  constexpr inline Type *begin() & { return pointer_to_data(); }
  constexpr inline Type *end() & { return begin() + 1; }
  constexpr inline const Type *begin() const & { return pointer_to_data(); }
  constexpr inline const Type *end() const & { return begin() + 1; }
  constexpr inline const Type *cbegin() const & { return begin(); }
  constexpr inline const Type *cend() const & { return end(); }
  const Type *begin() && = delete;
  const Type *end() && = delete;
  const Type *cbegin() && = delete;
  const Type *cend() && = delete;
  const Type *begin() const && = delete;
  const Type *end() const && = delete;
  const Type *cbegin() const && = delete;
  const Type *cend() const && = delete;
  constexpr inline static size_t size() { return 1; }  // for iterator
  constexpr inline static size_t my_size() {
    return sizeof_Type + sizeof(container_Type_ptr);
  }
  constexpr inline static size_t size_T() { return my_size(); }
  constexpr inline Type &operator()(Type &&moved) & {
    return *this = std::move(moved);
  }
  constexpr inline Type &operator()(Type &moved) & { return *this = moved; }
  constexpr inline Type &operator()(const Type &moved) & {
    return *this = moved;
  }
  inline Type &operator()(Type &&moved) const && = delete;
  inline Type &operator()(Type &moved) const && = delete;
  inline Type &operator()(const Type &moved) const && = delete;
  inline Type &operator()(Type &&moved) && = delete;
  inline Type &operator()(Type &moved) && = delete;
  inline Type &operator()(const Type &moved) && = delete;
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) && {
    return f(std::move(*temp_me()), std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class function_type, typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const && {
    return f(std::move(*temp_me()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) & {
    if (has_data()) f(O(), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) && {
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) const && {
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void operator()(function_type f, args_t &&...args) const & {
    if (has_data()) f(O(), std::forward<args_t>(args)...);
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) const & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) && {
    if (do_throw) {
      f(std::move(*temp_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) const && {
    if (do_throw) {
      f(std::move(*temp_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(temp_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(temp_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(temp_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = auto, class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(temp_me().O()),
                        std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline void do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(temp_me().O()),
                        std::forward<args_t>(args)...);
      return;
    }
    no_data_function(std::forward<args_t>(args)...);
    return;
  }

 public:
  constexpr inline bool operator==(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_heap_obj_warper_template_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_heap_obj_warper_template_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      mjz_heap_obj_warper_template_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_heap_obj_warper_template_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(mjz_heap_obj_warper_template_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_heap_obj_warper_template_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(mjz_heap_obj_warper_template_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_heap_obj_warper_template_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_heap_obj_warper_template_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      const mjz_heap_obj_warper_template_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_heap_obj_warper_template_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(const mjz_heap_obj_warper_template_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_heap_obj_warper_template_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(const mjz_heap_obj_warper_template_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_heap_obj_warper_template_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_heap_obj_warper_template_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool operator==(mjz_heap_obj_warper_template_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(mjz_heap_obj_warper_template_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(mjz_heap_obj_warper_template_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(mjz_heap_obj_warper_template_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(mjz_heap_obj_warper_template_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(mjz_heap_obj_warper_template_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_heap_obj_warper_template_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool has_data() const { return base().has_data(); }
  constexpr inline bool operator!() const noexcept { return !has_data(); }
  constexpr inline operator Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() const & { return pointer_to_data(); }
  constexpr inline operator Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() const & {
    return *pointer_to_data();
  }
  constexpr inline operator Type &&() && { return std::move(*temp_me()); }
  constexpr inline operator const Type &&() && { return std::move(*temp_me()); }
  constexpr inline operator const Type &&() const && {
    return std::move(*temp_me());
  }
  constexpr inline operator Type() & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() const & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() && {
    return get_obj_creator().obj_constructor(std::move(*temp_me()));
  }
  constexpr inline operator Type() const && {
    return get_obj_creator().obj_constructor(std::move(*temp_me()));
  }
  constexpr explicit operator bool() const noexcept { return has_data(); }
  constexpr bool has_value() const noexcept { return has_data(); }
  template <class... Args>
  T &emplace(Args &&...args) & {
    construct(std::forward<Args>(args)...);
  }
  template <class U, class... Args>
  T &emplace(std::initializer_list<U> ilist, Args &&...args) & {
    construct(ilist, std::forward<Args>(args)...);
  }
  void reset() noexcept {
    try {
      ~(*this);
    } catch (...) {
    }
  }
  template <class F>
  mjz_heap_obj_warper_template_t or_else(F &&f) const & {
    return has_data() ? *this : std::forward<F>(f)();
  }
  template <class F>
  mjz_heap_obj_warper_template_t or_else(F &&f) && {
    return has_data() ? temp_me() : std::forward<F>(f)();
  }
  constexpr Type &&value() && { return *temp_me(); }
  constexpr const Type &value() const & { return **this; }
  constexpr Type &value() & { return **this; }
  template <class U>
  constexpr Type value_or(U &&default_value) const & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) && {
    return has_data() ? std::move(*temp_me())
                      : static_cast<T>(std::forward<U>(default_value));
  }
  template <class F>
  constexpr auto transform(F &&f) & {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          base()))>(std::forward<F>(f)(base()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const & {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          base()))>(std::forward<F>(f)(base()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().base())))>(
          std::forward<F>(f)(std::move(temp_me().base())));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().base())))>(
          std::forward<F>(f)(std::move(temp_me().base())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) & {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          base()))>(std::forward<F>(f)(base()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const & {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          base()))>(std::forward<F>(f)(base()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().base())))>(
          std::forward<F>(f)(std::move(temp_me().base())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(temp_me().base())))>(
          std::forward<F>(f)(std::move(temp_me().base())));
    }
    return {};
  }

 public:
  constexpr inline Type *copy_to(Type *dest, bool dest_has_obj) & {
    return base().copy_to();
  }
  constexpr inline Type *move_to(Type *dest, bool dest_has_obj) {
    return base().move_to();
  }
  constexpr inline Type &copy_to(Type &dest, bool dest_has_obj) & {
    return *copy_to(&dest, dest_has_obj);
  }
  constexpr inline Type &move_to(Type &dest, bool dest_has_obj) {
    return *move_to(&dest, dest_has_obj);
  }
  constexpr inline mjz_heap_obj_warper_template_t &copy_to(
      mjz_heap_obj_warper_template_t &dest) & {
    if (this != &dest)
      dest.init_with_unsafe_placement_new(
          copy_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  constexpr inline mjz_heap_obj_warper_template_t &move_to(
      mjz_heap_obj_warper_template_t &dest) {
    if (this != &dest)
      dest.init_with_unsafe_placement_new(
          move_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  constexpr inline mjz_heap_obj_warper_template_t *copy_to(
      mjz_heap_obj_warper_template_t *dest) & {
    return &copy_to(*dest);
  }
  constexpr inline mjz_heap_obj_warper_template_t *move_to(
      mjz_heap_obj_warper_template_t *dest) {
    return &move_to(*dest);
  }

 public:  // unsafe may cuse undefined behavior
  constexpr inline mjz_heap_obj_warper_template_t &remove_const() const & {
    return *mjz_ard::remove_const(this);
  }
  constexpr inline mjz_heap_obj_warper_template_t &&remove_const() const && {
    return std::move(*mjz_ard::remove_const(this));
  }
  constexpr inline mjz_heap_obj_warper_template_t &remove_const() & {
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &&remove_const() && {
    return std::move(*this);
  }
  constexpr inline Type &remove_const_obj() const & {
    return base().remove_const_obj();
  }
  constexpr inline Type &&remove_const_obj() const && {
    return base().remove_const_obj();
  }
  constexpr inline Type &remove_const_obj() & {
    return base_mv().remove_const_obj();
  }
  constexpr inline Type &&remove_const_obj() && {
    return base_mv().remove_const_obj();
  }
  constexpr inline const Type &&move() const & { return std::move(o()); }
  constexpr inline const Type &&move() const && { return std::move(o()); }
  constexpr inline Type &&move() & { return std::move(o()); }
  constexpr inline Type &&move() && { return std::move(o()); }
};
template <class my_reallocator>
class mjz_temp_malloc_wrapper_t {
  constexpr inline mjz_temp_malloc_wrapper_t &move(
      mjz_temp_malloc_wrapper_t &otr) {
    if (otr.is_moved_state()) {
      if (otr.m_data_ptr && otr.m_cap_size) {
        m_data_ptr = malloc(otr.m_cap_size);
        if (m_data_ptr)
          static_str_algo::memmove(m_data_ptr, otr.m_data_ptr, otr.m_cap_size);
      }
      return *this;
    }
    otr.obj_is_moved();
    return move(otr.m_data_ptr, otr.m_cap_size);
  };
  constexpr inline mjz_temp_malloc_wrapper_t &move(void *data_ptr,
                                                   size_t size_of_ptr) {
    free();
    if (size_of_ptr) {
      m_data_ptr = data_ptr;
      m_cap_size = size_of_ptr;
    }
    return *this;
  };

 protected:
  void *m_data_ptr{};
  size_t m_cap_size{};
  uint8_t m_Deallocation_state{};
  static my_reallocator my_allocator;
  constexpr inline void obj_is_moved() {
    m_Deallocation_state |= Dealocation_state::is_moved |
                            Dealocation_state::dont_deallocate_on_free;
  }

 public:
  constexpr inline bool do_deallocation_on_free_state() {
    return !(m_Deallocation_state & Dealocation_state::dont_deallocate_on_free);
  }
  constexpr inline bool is_moved_state() {
    return !!(m_Deallocation_state & Dealocation_state::is_moved);
  }
  constexpr inline void dont_do_deallocation_on_free() {
    m_Deallocation_state |= Dealocation_state::dont_deallocate_on_free;
  }
  constexpr inline void do_deallocation_on_free() {
    m_Deallocation_state &=
        ((!(m_Deallocation_state & Dealocation_state::is_moved))
             ? ~Dealocation_state::dont_deallocate_on_free
             : Dealocation_state::dont_deallocate_on_free);
  }
  constexpr inline mjz_temp_malloc_wrapper_t(size_t size_of_ptr = 0) {
    m_data_ptr = malloc(size_of_ptr);
  };
  constexpr inline mjz_temp_malloc_wrapper_t(size_t size_of_ptr, int VAl_) {
    m_data_ptr = malloc(size_of_ptr);
    memset(VAl_);
  };
  constexpr inline mjz_temp_malloc_wrapper_t(void *data_ptr,
                                             size_t size_of_ptr) {
    move(data_ptr, size_of_ptr);
  }
  constexpr inline mjz_temp_malloc_wrapper_t(void *data_ptr, size_t size_of_ptr,
                                             int VAl_) {
    move(data_ptr, size_of_ptr).memset(VAl_);
  }
  constexpr inline mjz_temp_malloc_wrapper_t &change_data_ptr(
      void *data_ptr, size_t size_of_ptr) {
    return move(data_ptr, size_of_ptr);
  }
  constexpr inline mjz_temp_malloc_wrapper_t &change_data_ptr(
      mjz_temp_malloc_wrapper_t &&otr) {
    return move(otr);
  }
  constexpr inline ~mjz_temp_malloc_wrapper_t() { free(); }
  mjz_temp_malloc_wrapper_t(mjz_temp_malloc_wrapper_t &)
      : mjz_temp_malloc_wrapper_t() {}
  constexpr inline mjz_temp_malloc_wrapper_t(
      mjz_temp_malloc_wrapper_t &&otr) noexcept {
    move(otr);
  }
  mjz_temp_malloc_wrapper_t(const mjz_temp_malloc_wrapper_t &)
      : mjz_temp_malloc_wrapper_t(){};
  mjz_temp_malloc_wrapper_t &operator=(mjz_temp_malloc_wrapper_t &){};
  constexpr inline mjz_temp_malloc_wrapper_t &operator=(
      mjz_temp_malloc_wrapper_t &&otr) noexcept {
    return move(otr);
  };
  mjz_temp_malloc_wrapper_t &operator=(const mjz_temp_malloc_wrapper_t &){};
  template <typename Type = void>
  constexpr inline Type *get_ptr_as() {
    return (Type *)m_data_ptr;
  }
  template <typename Type = void>
  constexpr inline Type *operator->() {
    return (Type *)m_data_ptr;
  }
  template <typename Type>
  constexpr inline Type &operator*() {
    return *((Type *)m_data_ptr);
  }
  template <typename Type = void>
  constexpr inline const Type *get_ptr_as() const {
    return (Type *)m_data_ptr;
  }
  template <typename Type = void>
  constexpr inline const Type *operator->() const {
    return (Type *)m_data_ptr;
  }
  template <typename Type>
  constexpr inline const Type &operator*() const {
    return *((Type *)m_data_ptr);
  }
  constexpr inline void *get_ptr() { return m_data_ptr; }
  constexpr inline size_t get_size() { return m_cap_size; }
  constexpr inline const void *get_ptr() const { return m_data_ptr; }
  constexpr inline const size_t get_size() const { return m_cap_size; }
  void free() {
    if (do_deallocation_on_free_state() && m_cap_size) {
      my_allocator.deallocate((mjz_get_value_Type<my_reallocator> *)m_data_ptr,
                              m_cap_size);
    }
    m_data_ptr = 0;
    m_cap_size = 0;
  }
  void *malloc(size_t size_of_ptr) {
    free();
    if (size_of_ptr) {
      m_data_ptr = my_allocator.allocate(size_of_ptr);
      if (m_data_ptr) {
        m_cap_size = size_of_ptr;
      }
    }
    return get_ptr();
  }
  constexpr inline void *realloc(size_t size_of_ptr) {
    // free();
    if (size_of_ptr) {
      m_data_ptr = my_allocator.allocate(
          size_of_ptr, do_deallocation_on_free_state() ? m_data_ptr : 0);
      if (m_data_ptr) {
        m_cap_size = size_of_ptr;
      }
    }
    return get_ptr();
  }
  constexpr inline void *operator()(size_t size_of_ptr) {
    return malloc(size_of_ptr);
  }
  constexpr inline void *operator()() { return get_ptr(); }
  constexpr inline void memset(int _Val) {
    if (m_cap_size) {
      static_str_algo::memset(get_ptr(), _Val, m_cap_size);
    }
  }
  constexpr inline mjz_temp_malloc_wrapper_t(void *data_ptr, size_t cap_size,
                                             uint8_t DO_deallocate)
      : m_data_ptr(data_ptr),
        m_cap_size(cap_size),
        m_Deallocation_state(DO_deallocate) {}
};
template <typename my_reallocator>
my_reallocator mjz_temp_malloc_wrapper_t<my_reallocator>::my_allocator{};
using malloc_wrapper = mjz_temp_malloc_wrapper_t<mjz_normal_allocator>;
/*********************************************************************
Filename: sha256.h
 Author: Brad Conte (brad AT bradconte.com)
Copyright:
Disclaimer: This code is presented "as is" without any guarantees.
Details: Defines the API for the corresponding SHA1 implementation.
*********************************************************************/
#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32  // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef char BYTE;          // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

struct SHA256_CTX {
 private:
  static constexpr inline void paste_str(const char *str, char *&ptr) {
    size_t l = static_str_algo::strlen(str);
    static_str_algo::memmove(ptr, str, l);
    ptr += l - 1;
  }

 protected:
  constexpr inline char *to_c_string(char *buf_) const {
    char *buf = buf_;
    auto str_left = [&]() { return 1024 - (size_t)(buf - buf_); };
    paste_str("const char ", buf_);
    paste_str("hash", buf_);
    paste_str(" [] = { ", buf_);
    static_str_algo::itoa((int)hashed_data[0], buf, 10);
    if ((uint8_t)hashed_data[0] > 100) {
      buf += 3;
    } else if ((uint8_t)hashed_data[0] > 10) {
      buf += 2;
    } else {
      buf += 1;
    }
    for (int64_t i = 1; i < NumberOf(hashed_data); i++) {
      paste_str(",", buf_);
      static_str_algo::itoa((int)hashed_data[i], buf, 10);
      if ((uint8_t)hashed_data[i] > 100) {
        buf += 3;
      } else if ((uint8_t)hashed_data[i] > 10) {
        buf += 2;
      } else {
        buf += 1;
      }
    }
    paste_str(" };\n", buf);
    return buf_;
  }

 public:
  constexpr inline char *copy_to_c_str(char *buf, size_t len) const {
    if (len < 2 * 1024) {
      return 0;
    }
    static_str_algo::memset(buf, 0, len);
    return to_c_string(buf);
  }

  union {
    BYTE data[64]{};
    BYTE hashed_data[SHA256_BLOCK_SIZE];
  };
  WORD datalen{};
  unsigned long long bitlen{};
  WORD state[8]{};
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> to_string() const;
  friend std::ostream &operator<<(std::ostream &CIN, const SHA256_CTX &obj);
  constexpr inline static int compare_hash(const void *rhs,
                                           const SHA256_CTX &lhs) {
    return SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  static constexpr inline int compare_hash(const SHA256_CTX &rhs,
                                           const void *lhs) {
    return SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend constexpr inline int operator!=(const void *rhs,
                                         const SHA256_CTX &lhs) {
    return SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  friend constexpr inline bool operator!=(const SHA256_CTX &rhs,
                                          const void *lhs) {
    if (lhs == 0) return 1;
    return SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend constexpr inline bool operator==(const void *rhs,
                                          const SHA256_CTX &lhs) {
    return !SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  friend constexpr inline bool operator==(const SHA256_CTX &rhs,
                                          const void *lhs) {
    return !SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend constexpr inline bool operator!=(const SHA256_CTX &rhs,
                                          const SHA256_CTX &lhs) {
    return !!SHA256_CTX::compare_hash(rhs.hashed_data, lhs.hashed_data);
  }
  friend constexpr inline bool operator==(const SHA256_CTX &rhs,
                                          const SHA256_CTX &lhs) {
    return !SHA256_CTX::compare_hash(rhs.hashed_data, lhs.hashed_data);
  }
  constexpr inline static int real_compare_hash(const void *rhs,
                                                const void *lhs) {
    return !static_str_algo::mem_equals(rhs, lhs, SHA256_BLOCK_SIZE);
  }
  static constexpr inline int compare_hash(const void *rhs, const void *lhs) {
    return real_compare_hash(rhs, lhs);
  }
#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
 private:
  /**************************** VARIABLES *****************************/
  static constexpr const WORD k_KEY_ENCRIPTER[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
  /*********************** FUNCTION DEFINITIONS ***********************/
  static constexpr void sha256_transform(SHA256_CTX *ctx, const BYTE data[]) {
    WORD a{}, b{}, c{}, d{}, e{}, f{}, g{}, h{}, i{}, j{}, t1{}, t2{}, m[64]{};
    for (i = 0, j = 0; i < 16; ++i, j += 4)
      m[i] = (WORD)((data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) |
                    (data[j + 3]));
    for (; i < 64; ++i) {
      m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
    }
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];
    for (i = 0; i < 64; ++i) {
      t1 = h + EP1(e) + CH(e, f, g) + k_KEY_ENCRIPTER[i] + m[i];
      t2 = EP0(a) + MAJ(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
  }
  static constexpr void sha256_init(SHA256_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
  }
  static constexpr void sha256_update(SHA256_CTX *ctx, const BYTE data[],
                                      size_t len) {
    WORD i{};
    for (i = 0; i < len; ++i) {
      ctx->data[ctx->datalen] = data[i];
      ctx->datalen++;
      if (ctx->datalen == 64) {
        sha256_transform(ctx, ctx->data);
        ctx->bitlen += 512;
        ctx->datalen = 0;
      }
    }
  }
  static constexpr void sha256_final(SHA256_CTX *ctx, BYTE hash[]) {
    WORD i{};
    i = ctx->datalen;
    // Pad whatever data is left in the buffer.
    if (ctx->datalen < 56) {
      ctx->data[i++] = 0x80;
      while (i < 56) {
        ctx->data[i++] = 0x00;
      }
    } else {
      ctx->data[i++] = 0x80;
      while (i < 64) {
        ctx->data[i++] = 0x00;
      }
      sha256_transform(ctx, ctx->data);
      static_str_algo::memset(ctx->data, 0, 64);
    }
    // Append to the padding the total message's length in bits and transform.
    ctx->bitlen += static_cast<uint64_t>(ctx->datalen) * 8;
    ctx->data[63] = static_cast<BYTE>(ctx->bitlen);
    ctx->data[62] = static_cast<BYTE>(ctx->bitlen >> 8);
    ctx->data[61] = static_cast<BYTE>(ctx->bitlen >> 16);
    ctx->data[60] = static_cast<BYTE>(ctx->bitlen >> 24);
    ctx->data[59] = static_cast<BYTE>(ctx->bitlen >> 32);
    ctx->data[58] = static_cast<BYTE>(ctx->bitlen >> 40);
    ctx->data[57] = static_cast<BYTE>(ctx->bitlen >> 48);
    ctx->data[56] = static_cast<BYTE>(ctx->bitlen >> 56);
    sha256_transform(ctx, ctx->data);
    // Since this implementation uses little endian byte ordering and SHA uses
    // big endian, reverse all the bytes when copying the final state to the
    // output hash.
    for (i = 0; i < 4; ++i) {
      hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
  }

 public:
  constexpr size_t get_as_64bit_hash() const {
    return *((size_t *)hashed_data);
  }
  static constexpr void sha256_the_string(SHA256_CTX *hash_out,
                                          const char *c_str, size_t len) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const BYTE *>(c_str), len);
    sha256_final(&ctx, hash_out->data);
    return;
  }
  constexpr void sha256_the_string(const char *c_str, size_t len) {
    return sha256_the_string(this, c_str, len);
  }
  static constexpr SHA256_CTX hash_msg_to_sha_512(
      const char *dev_passwoed, const size_t dev_passwoedLength) {
    SHA256_CTX rtrn{};
    sha256_the_string(&rtrn, dev_passwoed, dev_passwoedLength);
    return rtrn;
  }
  static constexpr SHA256_CTX hash_msg_to_sha_512_n(
      const char *dev_passwoed, const size_t dev_passwoedLength,
      uint8_t n) {  // intended copy
    SHA256_CTX ret = hash_msg_to_sha_512(dev_passwoed, dev_passwoedLength);
    for (int64_t i{}; i < n; i++) {
      ret = hash_msg_to_sha_512((char *)ret.hashed_data, SHA256_BLOCK_SIZE);
    }
    return ret;
  }
  static constexpr inline size_t hash_c_str(const char *const p, size_t n) {
    size_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < n; ++i) {
      hash ^= static_cast<size_t>(p[i]);
      hash *= 1099511628211ULL;
    }
    return hash;
  }
};

typedef SHA256_CTX hash_sha256;
/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ctx, BYTE hash[]);
#endif  // SHA256_H

#ifndef SHA1_H
#define SHA1_H

/****************************** MACROS ******************************/
#define SHA1_BLOCK_SIZE 20  // SHA1 outputs a 20 byte digest

struct SHA1_CTX : public SHA256_CTX {
  WORD k[4]{};

 protected:
  static constexpr void sha1_transform(SHA1_CTX *ctx, const BYTE data[]) {
    WORD a{}, b{}, c{}, d{}, e{}, i{}, j{}, t{}, m[80]{};
    for (i = 0, j = 0; i < 16; ++i, j += 4)
      m[i] = (WORD)((data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8) +
                    (data[j + 3]));
    for (; i < 80; ++i) {
      m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
      m[i] = (m[i] << 1) | (m[i] >> 31);
    }
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    for (i = 0; i < 20; ++i) {
      t = ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + ctx->k[0] + m[i];
      e = d;
      d = c;
      c = ROTLEFT(b, 30);
      b = a;
      a = t;
    }
    for (; i < 40; ++i) {
      t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[1] + m[i];
      e = d;
      d = c;
      c = ROTLEFT(b, 30);
      b = a;
      a = t;
    }
    for (; i < 60; ++i) {
      t = ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + ctx->k[2] + m[i];
      e = d;
      d = c;
      c = ROTLEFT(b, 30);
      b = a;
      a = t;
    }
    for (; i < 80; ++i) {
      t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[3] + m[i];
      e = d;
      d = c;
      c = ROTLEFT(b, 30);
      b = a;
      a = t;
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
  }  // namespace mjz_ard

  static constexpr void sha1_init(SHA1_CTX *ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->k[0] = 0x5a827999;
    ctx->k[1] = 0x6ed9eba1;
    ctx->k[2] = 0x8f1bbcdc;
    ctx->k[3] = 0xca62c1d6;
  }
  static constexpr void sha1_update(SHA1_CTX *ctx, const BYTE data[],
                                    size_t len) {
    size_t i{};
    for (i = 0; i < len; ++i) {
      ctx->data[ctx->datalen] = data[i];
      ctx->datalen++;
      if (ctx->datalen == 64) {
        sha1_transform(ctx, ctx->data);
        ctx->bitlen += 512;
        ctx->datalen = 0;
      }
    }
  }
  static constexpr void sha1_final(SHA1_CTX *ctx, BYTE hash[]) {
    WORD i{};
    i = ctx->datalen;
    // Pad whatever data is left in the buffer.
    if (ctx->datalen < 56) {
      ctx->data[i++] = 0x80;
      while (i < 56) {
        ctx->data[i++] = 0x00;
      }
    } else {
      ctx->data[i++] = 0x80;
      while (i < 64) {
        ctx->data[i++] = 0x00;
      }
      sha1_transform(ctx, ctx->data);
      static_str_algo::memset(ctx->data, 0, 64);
    }
    // Append to the padding the total message's length in bits and transform.
    ctx->bitlen += static_cast<unsigned long long>(ctx->datalen) * 8;
    ctx->data[63] = static_cast<BYTE>(ctx->bitlen);
    ctx->data[62] = static_cast<BYTE>(ctx->bitlen >> 8);
    ctx->data[61] = static_cast<BYTE>(ctx->bitlen >> 16);
    ctx->data[60] = static_cast<BYTE>(ctx->bitlen >> 24);
    ctx->data[59] = static_cast<BYTE>(ctx->bitlen >> 32);
    ctx->data[58] = static_cast<BYTE>(ctx->bitlen >> 40);
    ctx->data[57] = static_cast<BYTE>(ctx->bitlen >> 48);
    ctx->data[56] = static_cast<BYTE>(ctx->bitlen >> 56);
    sha1_transform(ctx, ctx->data);
    // Since this implementation uses little endian byte ordering and MD uses
    // big endian, reverse all the bytes when copying the final state to the
    // output hash.
    for (i = 0; i < 4; ++i) {
      hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
      hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
    }
  }

 public:
  constexpr SHA1_CTX() = default;
  static constexpr SHA1_CTX SHA_1(const void *ptr, size_t len) {
    SHA1_CTX ctx{}, buffer{};
    sha1_init(&ctx);
    sha1_update(&ctx, (const BYTE *)ptr, len);
    sha1_final(&ctx, buffer.hashed_data);
    return buffer;
  }
  static constexpr SHA1_CTX SHA_1(const BYTE *ptr, size_t len) {
    SHA1_CTX ctx{}, buffer{};
    sha1_init(&ctx);
    sha1_update(&ctx, ptr, len);
    sha1_final(&ctx, buffer.hashed_data);
    return buffer;
  }
};
#endif  // SHA1_Hstatic
class basic_mjz_Str_view : protected static_str_algo {
 public:
  constexpr static size_t npos = -1;
  constexpr inline const char *begining_of_str_ptr() const { return m_buffer; }
  constexpr inline const char *ending_of_str_ptr() const {
    return m_buffer + m_length;
  }
  constexpr inline char *begining_of_str_ptr() { return m_buffer; }
  constexpr inline char *ending_of_str_ptr() { return m_buffer + m_length; }

 protected:  // the actual char array
  char *m_buffer;
  // the ptr length (not counting the '\0')
  size_t m_length;

 public:
  constexpr basic_mjz_Str_view(char *buffer, size_t length)
      : m_buffer(buffer), m_length(length) {}
  constexpr inline basic_mjz_Str_view(const char *c_string)
      : basic_mjz_Str_view(c_string, strlen(c_string)) {}
  constexpr basic_mjz_Str_view()
      : m_buffer(const_cast<char *>(empty_STRING_C_STR)), m_length(0) {}
  constexpr basic_mjz_Str_view(const char *buffer, size_t length)
      : basic_mjz_Str_view(const_cast<char *>(buffer), length) {}
  constexpr inline ~basic_mjz_Str_view() = default;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view &operator=(const basic_mjz_Str_view &) = default;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view(const basic_mjz_Str_view &) = default;

 public:
  constexpr inline char *&buffer_ref() { return m_buffer; }
  constexpr inline const char *buffer_ref() const { return m_buffer; }
  constexpr bool is_blank() const {
    size_t i{};
    while (i < m_length) {
      if (!is_blank_characteres_default(m_buffer[i])) {
        return 0;
      }
      i++;
    }
    return 1;
  }
  constexpr bool empty() const {
    return (!m_length || m_buffer == empty_STRING_C_STR || m_buffer == nullptr);
  }
  constexpr inline size_t length(void) const { return m_length; }
  constexpr inline char *data() { return m_buffer; }
  constexpr inline const char *data() const { return m_buffer; }

 public:
  constexpr inline const char *c_str() const & { return buffer_ref(); }
  constexpr inline const char *c_str() & { return buffer_ref(); }
  constexpr inline char *C_str() & { return m_buffer; }

 public:
  constexpr explicit operator const bool() const { return !is_blank(); }
  constexpr char operator[](int64_t index_) const {
    size_t index = signed_index_to_unsigned(index_);
    if ((size_t)index >= m_length || !m_buffer) {
      return 0;
    }
    return m_buffer[index];
  }
  constexpr char operator[](size_t index) const {
    if (index >= m_length || !m_buffer) {
      return 0;
    }
    return m_buffer[index];
  }
  constexpr bool operator!() const { return is_blank(); }
  // parsing/conversion
  constexpr long long toLL(void) const {
    if (m_buffer) {
      return to_LL();
    }
    return 0;
  }
  constexpr long long to_LL(int radix = 10, bool *had_error = 0,
                            uint8_t error_level = 0) const {
    if (m_buffer) {
      return C_STR_to_LL(c_str(), (uint8_t)min(length(), (uint64_t)255), radix,
                         had_error, error_level);
    }
    return 0;
  }
  constexpr long toInt(void) const { return (long)toLL(); }
  constexpr float toFloat(void) const {
    //
    return float(toDouble());
  }
  constexpr double toDouble(void) const {
    if (m_buffer) {
      char *ptr{};
      return strtod(m_buffer, &ptr);
    }
    return 0;
  }
  constexpr friend bool operator==(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return a.equals(b);
  }
  constexpr friend bool operator==(const basic_mjz_Str_view &a, const char *b) {
    return a.equals(b);
  }
  constexpr friend bool operator==(const char *a, const basic_mjz_Str_view &b) {
    return b == a;
  }
  constexpr friend bool operator<(const basic_mjz_Str_view &a,
                                  const basic_mjz_Str_view &b) {
    return a.compareTo(b) < 0;
  }
  constexpr friend bool operator<(const basic_mjz_Str_view &a, const char *b) {
    return a.compareTo(b) < 0;
  }
  constexpr friend bool operator<(const char *a, const basic_mjz_Str_view &b) {
    return b.compareTo(a) > 0;
  }
  constexpr friend bool operator!=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(a == b);
  }
  constexpr friend bool operator!=(const basic_mjz_Str_view &a, const char *b) {
    return !(a == b);
  }
  constexpr friend bool operator!=(const char *a, const basic_mjz_Str_view &b) {
    return !(a == b);
  }
  constexpr friend bool operator>(const basic_mjz_Str_view &a,
                                  const basic_mjz_Str_view &b) {
    return b < a;
  }
  constexpr friend bool operator>(const basic_mjz_Str_view &a, const char *b) {
    return b < a;
  }
  constexpr friend bool operator>(const char *a, const basic_mjz_Str_view &b) {
    return b < a;
  }
  constexpr friend bool operator<=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(b < a);
  }
  constexpr friend bool operator<=(const basic_mjz_Str_view &a, const char *b) {
    return !(b < a);
  }
  constexpr friend bool operator<=(const char *a, const basic_mjz_Str_view &b) {
    return !(b < a);
  }
  constexpr friend bool operator>=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(a < b);
  }
  constexpr friend bool operator>=(const basic_mjz_Str_view &a, const char *b) {
    return !(a < b);
  }
  constexpr friend bool operator>=(const char *a, const basic_mjz_Str_view &b) {
    return !(a < b);
  }
  friend std::ostream &operator<<(std::ostream &COUT,
                                  const basic_mjz_Str_view &s) {
    COUT.write(s.data(), s.length());
    return COUT;
  }
  // hash function
  constexpr hash_sha256 mjz_hash(uint8_t n = 0) const {
    return SHA256_CTX::hash_msg_to_sha_512_n(c_str(), length(), n);
  }
  constexpr hash_sha256 hash() const { return mjz_hash(0); }
  constexpr int64_t compareTo(const basic_mjz_Str_view &s) const {
    return compare_two_str(m_buffer, m_length, s.m_buffer, s.m_length);
  }
  constexpr int64_t compareTo(const char *cstr) const {
    return compare_two_str(m_buffer, m_length, cstr, strlen(cstr));
  }
  constexpr inline int compare(const basic_mjz_Str_view &str) const {
    return compareTo(str);
  }
  constexpr inline int compare(size_t pos, size_t len,
                               const basic_mjz_Str_view &str) const {
    return substr_view_beg_n(pos, len).compareTo(str);
  }
  constexpr inline int compare(size_t pos, size_t len,
                               const basic_mjz_Str_view &str, size_t subpos,
                               size_t sublen) const {
    return substr_view_beg_n(pos, len).compareTo(
        str.substr_view_beg_n(subpos, sublen));
  }
  constexpr inline int compare(const char *s, size_t n) const {
    return compare_two_str(m_buffer, m_length, s, n);
  }
  constexpr inline int compare(size_t pos, size_t len, const char *s,
                               size_t n) const {
    return substr_view_beg_n(pos, len).compare(s, n);
  }
  constexpr inline int compare(size_t pos, size_t len, const char *s) const {
    return compare(pos, len, s, strlen(s));
  }
  constexpr inline int compare(const char *s) const {
    return compare(s, strlen(s));
  }
  constexpr inline bool starts_with(const char *s, size_t n) const {
    return substr_view(0ULL, n).equals(s, n);
  }
  constexpr inline bool starts_with(const basic_mjz_Str_view &s) const {
    return starts_with(s.c_str(), s.length());
  }
  constexpr inline bool starts_with(char c) const { return c == *c_str(); }
  constexpr inline bool starts_with(const char *s) const {
    return starts_with(s, strlen(s));
  }
  constexpr inline bool ends_with(const char *s, size_t n) const {
    return substr_view(length() - n, length()).equals(s, n);
  }
  constexpr inline bool ends_with(const basic_mjz_Str_view &s) const {
    return ends_with(s.c_str(), s.length());
  }
  constexpr inline bool ends_with(char c) const {
    return c == c_str()[length() - 1];
  }
  constexpr inline bool ends_with(const char *s) const {
    return ends_with(s, strlen(s));
  }
  constexpr inline int64_t indexOf(const basic_mjz_Str_view &s2) const {
    return indexOf(s2, 0);
  }
  constexpr inline int64_t indexOf(const basic_mjz_Str_view &s2,
                                   size_t fromIndex) const {
    return indexOf_cstr(s2.data(), s2.length(), fromIndex);
  }
  constexpr inline int64_t lastIndexOf(const basic_mjz_Str_view &s2) const {
    return lastIndexOf(s2, m_length - s2.m_length);
  }
  constexpr inline int64_t lastIndexOf(const basic_mjz_Str_view &s2,
                                       size_t fromIndex) const {
    return lastIndexOf_cstr(s2.c_str(), s2.length(), fromIndex);
  }
  constexpr inline size_t find(const basic_mjz_Str_view &str,
                               size_t pos = 0) const {
    return indexOf(str, pos);
  }
  constexpr inline size_t find(const char *s, size_t pos = 0) const {
    return indexOf_cstr(s, strlen(s), pos);
  }
  constexpr inline size_t find(const char *s, size_t pos, size_t n) const {
    return indexOf_cstr(s, n, pos);
  }
  constexpr inline size_t find(char c, size_t pos = 0) const {
    return indexOf(c, pos);
  }
  constexpr inline size_t rfind(const basic_mjz_Str_view &str,
                                size_t pos = npos) const {
    return lastIndexOf(str, pos);
  }
  constexpr inline size_t rfind(const char *s, size_t pos = npos) const {
    return lastIndexOf_cstr(s, strlen(s), pos);
  }
  constexpr inline size_t rfind(const char *s, size_t pos, size_t n) const {
    return lastIndexOf_cstr(s, n, pos);
  }
  constexpr inline size_t rfind(char c, size_t pos = npos) const {
    return lastIndexOf(c, pos);
  }
  constexpr inline size_t find_first_of_in_str(const char *s, size_t n) const {
    return static_str_algo::find_first_of_in_str(m_buffer, m_length, s, n);
  }
  constexpr inline size_t find_last_of_in_str(const char *s, size_t n) const {
    return static_str_algo::find_last_of_in_str(m_buffer, m_length, s, n);
  }
  constexpr inline size_t find_first_of_in_str(
      const basic_mjz_Str_view &str) const {
    return find_first_of_in_str(str.m_buffer, str.m_length);
  }
  constexpr inline size_t find_first_of(const basic_mjz_Str_view &str,
                                        size_t pos = 0) const {
    return substr_view(pos).find_first_of_in_str(str);
  }
  constexpr inline size_t find_first_of(const char *s, size_t pos,
                                        size_t n) const {
    return substr_view(pos).find_first_of_in_str(s, n);
  }
  constexpr inline size_t find_first_of(const char *s, size_t pos = 0) const {
    return find_first_of(s, pos, strlen(s));
  }
  constexpr inline size_t find_first_of(char c, size_t pos = 0) const {
    return find_first_of(&c, pos, 1);
  }
  constexpr inline size_t find_last_of(const basic_mjz_Str_view &str,
                                       size_t pos = npos) const {
    return substr_view(pos).find_last_of_in_str(str.data(), str.length());
  }
  constexpr inline size_t find_last_of(const char *s, size_t pos = npos) const {
    return substr_view(pos).find_last_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_last_of(const char *s, size_t pos,
                                       size_t n) const {
    return substr_view(pos).find_last_of_in_str(s, n);
  }
  constexpr inline size_t find_last_of(char c, size_t pos = npos) const {
    return find_last_of(&c, pos, 1);
  }
  constexpr inline size_t find_last_not_of_in_str(const char *s,
                                                  size_t n) const {
    return static_str_algo::find_last_of_not_in_str(m_buffer, m_length, s, n);
  }
  constexpr inline size_t find_last_not_of(const basic_mjz_Str_view &str,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(str.c_str(), str.length());
  }
  constexpr inline size_t find_last_not_of(const char *s,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_last_not_of(const char *s, size_t pos,
                                           size_t n) const {
    return substr_view(pos).find_last_not_of_in_str(s, n);
  }
  constexpr inline size_t find_last_not_of(char c, size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(&c, 1);
  }
  constexpr inline size_t find_first_not_of_in_str(const char *s,
                                                   size_t n) const {
    return static_str_algo::find_first_of_not_in_str(m_buffer, m_length, s, n);
  }
  constexpr inline size_t find_first_not_of(const basic_mjz_Str_view &str,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(str.c_str(), str.length());
  }
  constexpr inline size_t find_first_not_of(const char *s,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_first_not_of(const char *s, size_t pos,
                                            size_t n) const {
    return substr_view(pos).find_first_not_of_in_str(s, n);
  }
  constexpr inline size_t find_first_not_of(char c, size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(&c, 1);
  }
  constexpr bool equals(const char *cstr, size_t cstr_len) const {
    return are_two_str_equale(m_buffer, m_length, cstr, cstr_len);
  }
  constexpr bool equals(const basic_mjz_Str_view &s2) const {
    //
    return equals(s2.data(), s2.length());
  }
  constexpr inline bool equals(const char *cstr) const {
    return equals(cstr, strlen(cstr));
  }
  constexpr inline size_t signed_index_to_unsigned(int64_t input) const {
    size_t index{length()};
    if (input < 0) {
      index = (size_t)(input + (int64_t)length());
    } else if (input <= (int64_t)length()) {
      index = (size_t)input;
    }
    return index;
  }
  constexpr size_t UN_ORDERED_compare(const basic_mjz_Str_view &s) const {
    return UN_ORDERED_compare(s.m_buffer, s.m_length);
  }
  constexpr size_t UN_ORDERED_compare(const char *s, size_t s_len) const {
    const unsigned char *ucs_ = (const unsigned char *)s;
    const unsigned char *ucbuffer_ = (const unsigned char *)this->m_buffer;
    size_t number_of_not_right{};
    size_t NUMBER_OF_EACH_char_array[2][256]{};
    for (size_t i{}; i < s_len; i++) {
      NUMBER_OF_EACH_char_array[0][ucs_[i]]++;
    }
    for (size_t i{}; i < this->length(); i++) {
      NUMBER_OF_EACH_char_array[1][ucbuffer_[i]]++;
    }
    for (size_t i{}; i < 256; i++) {
      number_of_not_right +=
          (size_t)abs((int64_t)NUMBER_OF_EACH_char_array[1][i] -
                      (int64_t)NUMBER_OF_EACH_char_array[0][i]);
    }
    return number_of_not_right;
  }
  constexpr bool equalsIgnoreCase(const basic_mjz_Str_view &s2) const {
    if (this == &s2) {
      //
      return true;
    }
    if (m_length != s2.m_length) {
      //
      return false;
    }
    if (m_length == 0) {
      //
      return true;
    }
    const char *p1 = m_buffer;
    const char *p2 = s2.m_buffer;
    const char *end_p1 = m_buffer + length();
    while (p1 < end_p1) {
      if (tolower(*p1++) != tolower(*p2++)) {
        //
        return false;
      }
    }
    //
    return true;
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2) const {
    if (m_length < s2.m_length) {
      //
      return false;
    }
    //
    return startsWith(s2, 0);
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2, size_t offset) const {
    if (offset > m_length - s2.m_length || !m_buffer || !s2.m_buffer) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&m_buffer[offset], s2.m_buffer, s2.m_length) == 0;
  }
  constexpr bool endsWith(const basic_mjz_Str_view &s2) const {
    if (m_length < s2.m_length || !m_buffer || !s2.m_buffer) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&m_buffer[m_length - s2.m_length], s2.m_buffer,
                       s2.m_length) == 0;
  }
  // Function that return the length
  constexpr size_t size() const { return length(); }
  constexpr char charAt(int64_t loc) const {
    return operator[](signed_index_to_unsigned(loc));
  }
  constexpr char charAt(size_t loc) const {
    //
    return operator[](loc);
  }
  constexpr void getBytes(unsigned char *buf, size_t bufsize,
                          size_t index = 0) const {
    if (!bufsize || !buf) {
      //
      return;
    }
    if (index >= m_length) {
      buf[0] = 0;
      //
      return;
    }
    size_t n = bufsize - 1;
    if (n > m_length - index) {
      n = m_length - index;
    }
    memmove((char *)buf, m_buffer + index, min_macro_(n, m_length));
    buf[n] = 0;
    //
  }
  constexpr inline void toCharArray(char *buf, size_t bufsize,
                                    size_t index = 0) const {
    getBytes((unsigned char *)buf, bufsize, index);
  }
  constexpr int64_t indexOf(char c) const { return indexOf(c, 0); }
  constexpr int64_t indexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= m_length) {
      return -1;
    }
    const char *temp = strchr(m_buffer + fromIndex, length() + fromIndex, ch);
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - m_buffer);
  }
  constexpr int64_t indexOf_cstr(const char *c_str_, size_t len_str,
                                 size_t fromIndex) const {
    if (fromIndex >= m_length) {
      return -1;
    }
    const char *found =
        strstr(m_buffer + fromIndex, length() - fromIndex, c_str_, len_str);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - m_buffer);
  }
  constexpr int64_t lastIndexOf_cstr(const char *cstr__, size_t length__,
                                     size_t fromIndex) const {
    if (fromIndex >= m_length) {
      return -1;
    }
    const char *found =
        r_strstr(m_buffer + fromIndex, length() - fromIndex, cstr__, length__);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - m_buffer);
  }
  constexpr inline int64_t lastIndexOf_cstr(const char *cstr__,
                                            size_t length__) const {
    return lastIndexOf_cstr(cstr__, length__, 0);
  }
  constexpr int64_t lastIndexOf(char theChar) const {
    return lastIndexOf(theChar, m_length - 1);
  }
  constexpr int64_t lastIndexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= m_length) {
      return -1;
    }
    char tempchar = m_buffer[fromIndex + 1];
    m_buffer[fromIndex + 1] = '\0';
    char *temp = const_cast<char *>(strrchr(m_buffer, length(), ch));
    m_buffer[fromIndex + 1] = tempchar;
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - m_buffer);
  };
  constexpr const char *substring_give_ptrULL(size_t left, size_t right,
                                              const char *&c_str_out,
                                              size_t &len_out) const {
    if (left > right) {
      size_t temp = right;
      right = left;
      left = temp;
    }
    if (left >= m_length) {
      //
      len_out = 0;
      return (c_str_out = 0);
    }
    if (right > m_length) {
      right = m_length;
    }
    c_str_out = m_buffer + left;
    len_out = right - left;
    return c_str_out;
  }
  constexpr const char *substring_give_ptr(int64_t left, int64_t right,
                                           const char *&c_str_out,
                                           size_t &len_out) const {
    return substring_give_ptrULL(signed_index_to_unsigned(left),
                                 signed_index_to_unsigned(right), c_str_out,
                                 len_out);
  }
  friend class mjz_str_view;
  constexpr const char &at(int64_t i) const & { return operator[](i); }
  constexpr const char &back() const & { return buffer_ref()[0]; }
  constexpr const char &front() const & { return operator[]((int64_t)-1); }
  constexpr inline basic_mjz_Str_view substr_view(size_t beginIndex) {
    return substr_view(beginIndex, length() - beginIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view(size_t beginIndex,
                                                  size_t endIndex) const {
    const char *out_ptr{};
    size_t out_len{};
    substring_give_ptrULL(beginIndex, endIndex, out_ptr, out_len);
    return out_len ? basic_mjz_Str_view(out_ptr, out_len)
                   : basic_mjz_Str_view();
  }
  constexpr inline basic_mjz_Str_view substr_view(int64_t beginIndex,
                                                  int64_t endIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(endIndex));
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  constexpr inline basic_mjz_Str_view substr_view(int64_t beginIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex));
  }
  constexpr inline basic_mjz_Str_view substr_view(int beginIndex,
                                                  int endIndex) const {
    return substr_view((int64_t)beginIndex, (int64_t)endIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int beginIndex,
                                                        int number) const {
    return substr_view_beg_n((int64_t)beginIndex, (size_t)number);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(size_t beginIndex,
                                                        size_t number) const {
    return substr_view(beginIndex, number + beginIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view(size_t beginIndex) const {
    return substr_view(beginIndex, length() - beginIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  constexpr inline size_t max_size() const { return (((size_t)(-1)) >> 1) - 1; }

 public:
  template <typename T = mjz_normal_allocator>
  std::pair<hash_sha256, mjz_str_t<T>> hash_with_output(uint8_t n = 0) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(size_t beginIndex);
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(size_t beginIndex, size_t endIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring_beg_n(size_t beginIndex, size_t number) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substr(size_t pos = 0, size_t len = npos) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(int64_t beginIndex, int64_t endIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(int64_t beginIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(int beginIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(int beginIndex, int endIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring_beg_n(int beginIndex, int number) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring(size_t beginIndex) const;
  template <typename T = mjz_normal_allocator>
  mjz_str_t<T> substring_beg_n(int64_t beginIndex, size_t number) const;
};

class mjz_str_view;
class basic_mjz_String : public basic_mjz_Str_view {
 protected:
  template <typename T>
  friend class mjz_str_t;
  // the array length minus one (for the '\0')
  size_t m_capacity;

 public:
  constexpr inline basic_mjz_String(char *bfr, size_t cap, size_t len)
      : basic_mjz_Str_view(bfr, len), m_capacity(cap){};
  constexpr inline basic_mjz_String(const char *bfr, size_t cap, size_t len)
      : basic_mjz_String(const_cast<char *>(bfr), cap, len){};
  constexpr inline basic_mjz_String()
      : basic_mjz_String(empty_STRING_C_STR, 0, 0) {}
  constexpr inline ~basic_mjz_String() = default;
  ;
  basic_mjz_String(basic_mjz_String &&) = delete;
  basic_mjz_String(const basic_mjz_String &) = delete;
  basic_mjz_String(basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &&) = delete;
  basic_mjz_String &operator=(const basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &) = delete;
  constexpr inline basic_mjz_Str_view sv() const {
    return basic_mjz_Str_view{*((basic_mjz_Str_view *)this)};
  }

 public:
};

template <typename T>
mjz_str_t<T> &&operator_plus(mjz_str_t<T> &&lhs, const basic_mjz_Str_view &rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(mjz_str_t<T_t> &&lhs,
                                 const basic_mjz_Str_view &rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(const basic_mjz_Str_view &lhs,
                                 const basic_mjz_Str_view &rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(mjz_str_t<T_t> &&lhs,
                                 const StringSumHelper_t<T_t> &rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(const basic_mjz_Str_view &lhs,
                                 const StringSumHelper_t<T_t> &rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 const char *cstr);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, char c);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 unsigned char num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, int num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 unsigned int num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, long num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 unsigned long num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, long long num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 unsigned long long num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, float num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, double num);
template <typename T_t>
StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                 const __FlashStringHelper *rhs);
template <typename T_t>
StringSumHelper_t<T_t> operator+(const mjz_str_view &lhs,
                                 const StringSumHelper_t<T_t> &rhs);

template <typename T>
#ifndef Arduino
class mjz_str_t : public basic_mjz_String {
#if 0

}
#endif
#else
class mjz_Str : public basic_mjz_String,
                public Stream {  //
#endif  // Arduino
 protected:
  // void (mjz_Str<T>::*update_event_F_p)( void); //departed
  // (object_ptr->*pointer_name)(arguments)//like (this->*update_event_F_p)();
  stack_str_buf stack_obj_buf;
  friend class StringSumHelper_t<T>;
  // use a function pointer to allow for "if (s)" without the
  // complications of an operator bool(). for more information,see:
  // http://www.artima.com/cppsource/safebool.html
  typedef void (mjz_str_t<T>::*StringIfHelperType)() const;
  void StringIfHelper() const {}
  inline bool realloc_helper_is_in_stack(void *ptr);

 private:
  // copy and move
  mjz_str_t<T> &copy(const char *cstr, size_t length) {
    return copy(cstr, length, 0);
  }
  mjz_str_t<T> &copy(const __FlashStringHelper *pstr, size_t length);
  void move(mjz_str_t<T> &rhs);
  [[nodiscard]] void *realloc_pv(void *ptr, size_t new_size, bool constructor);
  void free_pv(void *&ptr, bool constructor);
  void free_pv(void *const &ptr, bool constructor);
  mjz_str_t<T> &copy(const char *cstr, size_t length, bool constructor);

 protected:
  [[nodiscard]] void *realloc(void *ptr, size_t new_size) {
    return T().allocate(new_size, ptr);
  }
  void free(void *ptr) { return T().deallocate((char *)ptr, m_capacity + 1); }

 public:
  static int8_t char_to_int_for_string(char c_char);
  // stream
 protected:
  // This enumeration provides the lookahead options for parseInt(), //
  // parseFloat() The rules set out here are used until either the first valid
  // character is found or a time out occurs due to lack of input.
  enum LookaheadMode : uint8_t {
    SKIP_ALL,   // All invalid characters are ignored.
    SKIP_NONE,  // Nothing is skipped, and the stream is not touched unless the
                // first waiting character is valid.
    SKIP_WHITESPACE  // Only tabs, spaces, line feeds & carriage returns are
                     // skipped.
  };
  int64_t parseInt(char ignore) { return parseInt(SKIP_ALL, ignore); }
  float parseFloat(char ignore) { return parseFloat(SKIP_ALL, ignore); }
  // These overload exists for compatibility with any class that has derived
  // mjz_Str<T> and used parseFloat/Int with a custom ignore character. To keep
  // the public API simple, these overload remains protected.
  struct MultiTarget {
    const char *str;  // string you're searching for
    size_t len;       // length of string you're searching for
    size_t index;     // index used by the search routine.
  };
  // This allows you to search for an arbitrary number of strings.
  // Returns index of the target that is found first or -1 if timeout occurs.
  int findMulti(struct MultiTarget *targets, int tCount);
  size_t printNumber(unsigned long, uint8_t);
  size_t printULLNumber(unsigned long long, uint8_t);
  size_t printFloat(double, int);
  int timedRead();  // private method to read stream with timeout
  int timedPeek();  // private method to peek stream with timeout
  int peekNextDigit(LookaheadMode lookahead,
                    bool detectDecimal);  // returns the next numeric digit in

 public:
  // default to zero, meaning "a single write may block"
  // should be overridden by subclasses with buffering
  int availableForWrite() if_ard_then_override {
    return (int)availableForWriteLL();
  }
  int64_t availableForWriteLL() if_ard_then_override { return 1; }
  size_t print(const __FlashStringHelper *) if_override_then_override;
  size_t print(const char[]) if_override_then_override;
  size_t print(char) if_override_then_override;
  size_t print(unsigned char, int = DEC) if_override_then_override;
  size_t print(int, int = DEC) if_override_then_override;
  size_t print(unsigned int, int = DEC) if_override_then_override;
  size_t print(long, int = DEC) if_override_then_override;
  size_t print(unsigned long, int = DEC) if_override_then_override;
  size_t print(long long, int = DEC) if_override_then_override;
  size_t print(unsigned long long, int = DEC) if_override_then_override;
  size_t print(double, int = 2) if_override_then_override;
  size_t print(const mjz_str_t<T> &) if_override_then_override;
  size_t println(const __FlashStringHelper *) if_override_then_override;
  size_t println(const char[]) if_override_then_override;
  size_t println(char) if_override_then_override;
  size_t println(unsigned char, int = DEC) if_override_then_override;
  size_t println(int, int = DEC) if_override_then_override;
  size_t println(unsigned int, int = DEC) if_override_then_override;
  size_t println(long, int = DEC) if_override_then_override;
  size_t println(unsigned long, int = DEC) if_override_then_override;
  size_t println(long long, int = DEC) if_override_then_override;
  size_t println(unsigned long long, int = DEC) if_override_then_override;
  size_t println(double, int = 2) if_override_then_override;
  size_t println(const mjz_str_t<T> &) if_override_then_override;
  size_t println(void)
      if_override_then_override;  // TODO: V1071
                                  // https://pvs-studio.com/en/docs/warnings/V1071/
  // Consider inspecting the 'println' function.
  // The return value is not always used. Total
  // calls: 13, discarded results: 1.
  // parsing methods

  bool find_in_stream(
      const char *target);  // reads data from the stream until the target
  // string is found
  bool find_in_stream(const uint8_t *target) {
    return find_in_stream((const char *)target);
  }
  // returns true if target string is found, false if timed out (see setTimeout)
  bool find_in_stream(
      const char *target,
      size_t length);  // reads data from the stream until the target
  // string of given length is found
  bool find_in_stream(const uint8_t *target, size_t length) {
    return find_in_stream((const char *)target, length);
  }
  // returns true if target string is found, false if timed out
  bool find_in_stream(char target) { return find_in_stream(&target, 1); }
  bool find_in_stream_Until(
      const char *target,
      const char *terminator);  // as find but search ends if the
  // terminator string is found
  bool find_in_stream_Until(const uint8_t *target, const char *terminator) {
    return find_in_stream_Until((const char *)target, terminator);
  }
  bool find_in_stream_Until(
      const char *target, size_t targetLen, const char *terminate,
      size_t termLen);  // as above but search ends if the terminate
  // string is found
  bool find_in_stream_Until(const uint8_t *target, size_t targetLen,
                            const char *terminate, size_t termLen) {
    return find_in_stream_Until((const char *)target, targetLen, terminate,
                                termLen);
  }
  long parseInt(LookaheadMode lookahead = SKIP_ALL,
                char ignore = NO_IGNORE_CHAR);
  // returns the first valid (long) integer value from the current position.
  // lookahead determines how parseInt looks ahead in the stream.
  // See LookaheadMode enumeration at the top of the file.
  // Lookahead is terminated by the first character that is not a valid part of
  // an integer. Once parsing commences, 'ignore' will be skipped in the stream.
  float parseFloat(LookaheadMode lookahead = SKIP_ALL,
                   char ignore = NO_IGNORE_CHAR);
  // float version of parseInt
  // terminates if length characters have been read or timeout (see setTimeout)
  // returns the number of characters placed in the buffer (0 means no valid
  // data found)
  size_t readBytesUntil(char terminator, char *buffer, size_t length)
      if_override_then_override;  // as readBytes with terminator character
  size_t readBytesUntil(char terminator, uint8_t *buffer_,
                        size_t length) if_override_then_override {
    return readBytesUntil(terminator, (char *)buffer_, length);
  }
  // terminates if length characters have been read, timeout, or if the
  // terminator character detected returns the number of characters placed in
  // the buffer (0 means no valid data found)
  // Arduino mjz_Str<T> functions to be added here
  mjz_str_t<T> read_mjz_Str();
  mjz_str_t<T> read_mjz_Str_Until(char terminator);
  size_t write(const char *buf, size_t m_size);
  size_t write(const char *buf);
  size_t write(uint8_t) if_ard_then_override;
  size_t write(char cr);
  bool reserve(size_t m_size, bool just_size = 0, bool constructor = 0);
  bool addto_length(size_t addition_tolen, bool just_size = 0);
  size_t write(const uint8_t *buf, size_t m_size) if_ard_then_override;
  int64_t availableLL() if_ard_then_override;
  int available() if_ard_then_override { return (int)availableLL(); }
  int read() if_ard_then_override;
  int peek() if_ard_then_override;
  void flush() if_ard_then_override;
  size_t read(uint8_t *buf, size_t m_size);
  size_t readBytes(char *buffer_, size_t length) {
    return read((uint8_t *)buffer_, length);
  }
  size_t readBytes(uint8_t *buffer_, size_t length) {
    return readBytes((char *)buffer_, length);
  }
  void begin(unsigned long);
  void begin(unsigned long, uint16_t);
  // void end(); used
  // stream

  // new and delete
  [[nodiscard]] static void *operator new(size_t m_size);
  [[nodiscard]] static void *operator new[](size_t m_size);
  static void operator delete(void *p);
  static void operator delete[](void *ptr);
  static void operator delete(void *p, size_t);
  static void operator delete[](void *ptr, size_t);
  inline static void *operator new(size_t, void *where) { return where; }
  inline static void operator delete(void *, void *) {}
  inline static void operator delete(void *, size_t, void *) {}
  inline static mjz_str_t<T> &replace_with_new_str(mjz_str_t<T> &where) {
    // obj.mjz_Str<T>::~mjz_Str<T>();//bad not calling virtually
    // obj->~mjz_Str<T>(); // good calls the most derived constructor
    where.~mjz_str_t<T>();  // end lifetime
    new (&where)
        mjz_str_t<T>;  // create an object in the place of the peivios one
    return where;
  }
  inline static mjz_str_t<T> *replace_with_new_str(mjz_str_t<T> *where) {
    where->~mjz_str_t<T>();  // end lifetime
    new (where)
        mjz_str_t<T>;  // create an object in the place of the peivios one
    return where;
  }
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return this->*my_var;
  }
  template <typename my_type>
  inline auto operator->*(my_type my_var) const {
    return this->*my_var;
  }
  mjz_str_t<T> &assign_range(std::initializer_list<const char> list);
  mjz_str_t<T> &assign_range(iterator_template_t<const char> list);
#ifdef Arduino
  mjz_Str<T>(String &x)
      : mjz_Str<T>(((x.c_str() != 0) ? (x.c_str())
                                     : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str<T>(String &&x)
      : mjz_Str<T>(((x.c_str() != 0) ? (x.c_str())
                                     : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str<T>(const String &x)
      : mjz_Str<T>(((x.c_str() != 0) ? (x.c_str())
                                     : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str<T> &operator=(String &x) { return operator=(x.c_str()); }
  mjz_Str<T> &operator=(const String &x) { return operator=(x.c_str()); }
  mjz_Str<T> &operator=(String &&x) { return operator=(x.c_str()); }
  mjz_Str<T> &operator+=(String &x) { return operator+=(x.c_str()); }
  mjz_Str<T> &operator+=(const String &x) { return operator+=(x.c_str()); }
  mjz_Str<T> &operator+=(String &&x) { return operator+=(x.c_str()); }
#endif
  mjz_str_t<T> &operator=(basic_mjz_String &x) {
    return (*this)(x.c_str(), x.length());
  }
  mjz_str_t<T> &operator=(basic_mjz_String &&x) {
    return (*this)(x.c_str(), x.length());
  }
  mjz_str_t<T> &operator+=(basic_mjz_Str_view &x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_str_t<T> &operator+=(basic_mjz_Str_view &&x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_str_t<T> &operator+=(basic_mjz_String &x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_str_t<T> &operator+=(basic_mjz_String &&x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  ~mjz_str_t(void);  // make all drived destructors called
  void adjust_cap();
  mjz_str_t<T> &operator-=(const mjz_str_t<T> &othr_);
  mjz_str_t<T> &operator-=(const basic_mjz_Str_view &othr_);
  mjz_str_t<T> &operator-=(basic_mjz_Str_view &&othr_) {
    return operator-=(othr_);
  }
  friend mjz_str_t<T> operator-(const mjz_str_t<T> &rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(rhs);
    obj -= lhs;
    return obj;
  }
  friend mjz_str_t<T> operator-(mjz_str_t<T> &&rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(std::move(rhs));
    obj -= lhs;
    return obj;
  }
  mjz_str_t<T> &operator%=(const mjz_str_t<T> &othr_);
  mjz_str_t<T> &operator%=(const basic_mjz_Str_view &othr_);
  mjz_str_t<T> &operator%=(basic_mjz_Str_view &&othr_) {
    return operator%=(othr_);
  }
  friend mjz_str_t<T> operator%(mjz_str_t<T> &&rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(std::move(rhs));
    obj %= lhs;
    return obj;
  }
  friend mjz_str_t<T> operator%(const mjz_str_t<T> &rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(rhs);
    obj %= lhs;
    return obj;
  }
  mjz_str_t<T> &operator*=(unsigned int number_of_it);
  mjz_str_t<T> operator*(unsigned int number_of_it);
  mjz_str_t<T> &operator/=(const basic_mjz_Str_view &othr_);
  mjz_str_t<T> &operator/=(basic_mjz_Str_view &&othr_) {
    return operator/=(othr_);
  }
  mjz_str_t<T> &operator/=(const mjz_str_t<T> &othr_);
  friend mjz_str_t<T> operator/(const mjz_str_t<T> &rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(rhs);
    obj /= lhs;
    return obj;
  }
  friend mjz_str_t<T> operator/(mjz_str_t<T> &&rhs,
                                const basic_mjz_Str_view &lhs) {
    mjz_str_t<T> obj(std::move(rhs));
    obj /= lhs;
    return obj;
  }
  mjz_str_t<T> &operator++();  // print empty line
  mjz_str_t<T> operator++(int);
  mjz_str_t<T> &operator--();        // read one character
  StringSumHelper_t<T> operator-();  // copy
  inline mjz_str_t<T> &operator~() { return operator()(); }
  inline mjz_str_t<T> &operator+() { return ++(*this); }
  mjz_str_t<T> operator--(int);
  inline mjz_str_t<T> &operator()() {
    operator()(empty_STRING_C_STR, (size_t)0);
    return *this;
  };
  inline mjz_str_t<T> &operator()(const mjz_str_t<T> &other) {
    this->operator=(other);
    return *this;
  };
  inline mjz_str_t<T> &operator()(const char *other) {
    operator()(other, (size_t)strlen(other));
    return *this;
  };
  inline mjz_str_t<T> &operator()(const char *other, size_t size_len);
  inline mjz_str_t<T> &operator()(mjz_str_t<T> &&other) {
    this->operator=(std::move(other));
    return *this;
  };
  // memory management
  // return true on success,false on failure (in which case,the string
  // is left unchanged). reserve(0),if successful,will validate an
  // invalid string (i.e.,"if (s)" will be true afterwards)
  constexpr inline const char *c_str() const & { return m_buffer; }
  constexpr inline char *C_str() & { return m_buffer; }
  explicit operator char *() & { return buffer_ref(); }
  explicit operator const uint8_t *() const & {
    return (const uint8_t *)buffer_ref();
  }
  explicit operator uint8_t *() & { return (uint8_t *)buffer_ref(); }
  explicit operator void *() & { return (void *)buffer_ref(); }
  explicit operator uint16_t *() & { return (uint16_t *)buffer_ref(); }
  explicit operator uint32_t *() & { return (uint32_t *)buffer_ref(); }
  explicit operator uint64_t *() & { return (uint64_t *)buffer_ref(); }
  explicit operator const void *() const & { return (void *)buffer_ref(); }
  explicit operator const uint16_t *() const & {
    return (uint16_t *)buffer_ref();
  }
  explicit operator const uint32_t *() const & {
    return (uint32_t *)buffer_ref();
  }
  explicit operator const uint64_t *() const & {
    return (uint64_t *)buffer_ref();
  }
  explicit operator const size_t() const & { return m_length; }
  explicit operator size_t() const & { return m_length; }
  explicit operator size_t &() & { return (size_t &)m_length; }
  explicit operator const long() const { return toInt(); }
  explicit operator const long long() const { return toLL(); }
  explicit operator const float() const { return toFloat(); }
  explicit operator const double() const { return toDouble(); }
  operator const char *() const & { return buffer_ref(); }
  char &operator[](size_t index) &;
  char &operator[](int64_t index) &;
  // creates a copy of the assigned value. if the value is null or
  // invalid,or if the memory allocation fails,the string will be
  // marked as invalid ("if (s)" will be false).
  mjz_str_t<T> &operator=(const mjz_str_t<T> &rhs);
  mjz_str_t<T> &operator=(const char *cstr);
  mjz_str_t<T> &operator=(const __FlashStringHelper *str);
  mjz_str_t<T> &operator=(mjz_str_t<T> &&rval) noexcept;
  // mjz_str_t<T> &operator=(const mjz_str_t<T> *rval) { return
  // operator=(*rval); } mjz_Str<T> &operator=( const mjz_Str<T> *&&rval) {
  // return operator=(std::move(*rval));
  // } // this will give me headaches in the long run so i dont move it
  // concatenate (works w/ built-in types)
  // returns true on success,false on failure (in which case,the string
  // is left unchanged). if the argument is null or invalid,the
  // concatenation is considered unsuccessful.
  bool concat(const mjz_str_t<T> &str);
  bool concat(
      const char
          *cstr);  // TODO: V1071 https://pvs-studio.com/en/docs/warnings/V1071/
  // Consider inspecting the 'concat' function. The return value
  // is not always used. Total calls: 11, discarded results: 1.
  bool concat(const char *cstr, size_t length);
  inline bool concat(const basic_mjz_Str_view &str) {
    return concat(str.c_str(), str.length());
  }
  bool concat(const uint8_t *cstr, size_t length) {
    return concat((const char *)cstr, length);
  }
  bool concat(char c);
  bool concat(unsigned char num);
  bool concat(int num);
  bool concat(unsigned int num);
  bool concat(long num);
  bool concat(unsigned long num);
  bool concat(long long num);
  bool concat(unsigned long long num);
  bool concat(float num);
  bool concat(double num);
  bool concat(const __FlashStringHelper *str);
  mjz_str_t<T> &append(const mjz_str_t<T> &str) {
    concat(str);
    return *this;
  }
  mjz_str_t<T> &append(const mjz_str_t<T> &str, size_t subpos, size_t sublen);
  mjz_str_t<T> &append(const char *s) {
    concat(s);
    return *this;
  }
  mjz_str_t<T> &append(const char *s, size_t n) {
    concat(s, n);
    return *this;
  }
  mjz_str_t<T> &append(size_t n, char c) {
    if (!addto_length(n)) return *this;
    memset(m_buffer + length() - n, c, n);
    return *this;
  }
  template <class InputIterator>
  mjz_str_t<T> &append(InputIterator first, InputIterator last) {
    size_t newlen = last - first;
    if (!addto_length(newlen)) return *this;
    char *ptr_{m_buffer + length() - newlen};
    for (; first != last; first++) {  // i don't like != but :(
      *ptr_++ = *first;
    }
    return *this;
  }
  // if there's not enough memory for the concatenated value,the string
  // will be left unchanged (but this isn't signalled in any way)
  mjz_str_t<T> &operator+=(const mjz_str_t<T> &rhs) {
    concat(rhs);
    return (*this);
  }
  mjz_str_t<T> &operator+=(mjz_str_t<T> &&rhs) {
    concat(rhs);
    return (*this);
  }
  mjz_str_t<T> &operator+=(const char *cstr) {
    concat(cstr);
    return (*this);
  }
  mjz_str_t<T> &operator+=(char c) {
    concat(c);
    return (*this);
  }
  mjz_str_t<T> &operator+=(unsigned char num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(int num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(unsigned int num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(long num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(unsigned long num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(long long num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(unsigned long long num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(float num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(double num) {
    concat(num);
    return (*this);
  }
  mjz_str_t<T> &operator+=(const __FlashStringHelper *str) {
    concat(str);
    return (*this);
  }
  mjz_str_t<T> &operator+=(const basic_mjz_Str_view &str) {
    concat(str);
    return (*this);
  }
  template <typename T_t>
  friend mjz_str_t<T_t> &&operator_plus(mjz_str_t<T_t> &&lhs,
                                        const basic_mjz_Str_view &rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(mjz_str_t<T_t> &&lhs,
                                          const basic_mjz_Str_view &rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(const basic_mjz_Str_view &lhs,
                                          const basic_mjz_Str_view &rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(mjz_str_t<T_t> &&lhs,
                                          const StringSumHelper_t<T_t> &rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(const basic_mjz_Str_view &lhs,
                                          const StringSumHelper_t<T_t> &rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          const char *cstr);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs, char c);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          unsigned char num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          int num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          unsigned int num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          long num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          unsigned long num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          long long num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          unsigned long long num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          float num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          double num);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(StringSumHelper_t<T_t> &&lhs,
                                          const __FlashStringHelper *rhs);
  template <typename T_t>
  friend StringSumHelper_t<T_t> operator+(const mjz_str_view &lhs,
                                          const StringSumHelper_t<T_t> &rhs);
  typedef void *(*function_ptr)(const mjz_str_t<T> &, void *);
  operator StringIfHelperType() const {
    return buffer_ref() ? &mjz_str_t<T>::StringIfHelper : 0;
  }
  void *do_this_for_me(function_ptr, void *x = 0);
  // comparison (only works w/ Strings and "strings")
  [[nodiscard]] static mjz_str_t<T> create_mjz_Str_char_array(size_t m_size,
                                                              char filler = 0,
                                                              bool do_fill = 1);
  [[nodiscard]] static mjz_str_t<T> create_mjz_Str_2D_char_array(
      size_t size_col, size_t size_row, char filler = 0, bool do_fill = 1);
  friend class STRINGSerial;
  // character access
  void setCharAt(size_t index, char c);
  // character access
  void setCharAt(int64_t index, char c);
  inline char *begin_c_str() & { return m_buffer; }
  char *end_c_str() & { return m_buffer + length(); }
  char *endAST_c_str() & { return end_c_str(); }
  const char *begin_c_str() const & { return m_buffer; }
  const char *endAST_c_str() const & { return end_c_str(); }
  const char *end_c_str() const & { return m_buffer + length(); }
  // Iterator Class
  using const_iterator = iterator_template_t<const char>;
  using iterator = iterator_template_t<char>;
  const_iterator begin() const { return cbegin(); }
  inline const_iterator end() const { return cend(); }
  const_iterator cbegin() const {
    return const_iterator(begin_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  const_iterator cend() const {
    return const_iterator(end_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  iterator begin() {
    return iterator(begin_c_str(), begining_of_str_ptr(), ending_of_str_ptr());
  }
  iterator end() {
    return iterator(end_c_str(), begining_of_str_ptr(), ending_of_str_ptr());
  }
  typedef std::reverse_iterator<iterator> rev_iterator;
  typedef std::reverse_iterator<const_iterator> const_rev_iterator;
  rev_iterator rbegin() {
    return rev_iterator(
        iterator{end(), begining_of_str_ptr(), ending_of_str_ptr()});
  }
  rev_iterator rend() {
    return rev_iterator(
        iterator{begin(), begining_of_str_ptr(), ending_of_str_ptr()});
  }
  const_rev_iterator crend() const {
    return const_rev_iterator(
        const_iterator{begin(), begining_of_str_ptr(), ending_of_str_ptr()});
  };
  const_rev_iterator crbegin() const {
    return const_rev_iterator(
        const_iterator{end(), begining_of_str_ptr(), ending_of_str_ptr()});
  };
  inline const_rev_iterator rend() const { return crend(); };
  inline const_rev_iterator rbegin() const { return crbegin(); };
  // erase
  mjz_str_t<T> &erase(size_t pos_ = 0, size_t len_ = 0);
  iterator erase(iterator p);
  iterator erase(iterator first, iterator last);
  mjz_str_t<T> &erase_from_f_to_l(size_t first, size_t last);
  // search
  // modification
  void find_and_replace(char find, char replace_);
  void find_and_replace(const basic_mjz_Str_view &find,
                        const basic_mjz_Str_view &replace_);
  void find_and_replace(const char *find, size_t find_count,
                        const char *replace_, size_t replace_count);
  mjz_str_t<T> &replace(size_t pos, size_t len, const basic_mjz_Str_view &str);
  mjz_str_t<T> &replace_ll(int64_t pos, size_t len,
                           const basic_mjz_Str_view &str);
  mjz_str_t<T> &replace(iterator i1, iterator i2,
                        const basic_mjz_Str_view &str);
  mjz_str_t<T> &replace(size_t pos, size_t len, const basic_mjz_Str_view &str,
                        size_t subpos, size_t sublen);
  mjz_str_t<T> &replace(size_t pos, size_t len, const char *s);
  mjz_str_t<T> &replace(size_t pos, size_t len, const char *s, size_t n);
  mjz_str_t<T> &replace(iterator i1, iterator i2, const char *s, size_t n);
  mjz_str_t<T> &replace(iterator i1, iterator i2, const char *s) {
    return replace(i1, i2, s, strlen(s));
  }
  mjz_str_t<T> &replace(size_t pos, size_t len, size_t n, char c);
  mjz_str_t<T> &replace(iterator i1, iterator i2, size_t n, char c);
  template <class InputIterator>
  mjz_str_t<T> &replace(iterator i1, iterator i2, InputIterator first,
                        InputIterator last) {
    mjz_str_t<T> str(first, last);
    replace(i1, i2, str.c_str(), str.length());
    return *this;
  }
  void remove(size_t index);
  void remove(size_t index, size_t count);
  void toLowerCase(void);
  void toUpperCase(void);
  void trim(void);
  static mjz_str_t<T> ULL_LL_to_str(uint64_t value, int radix, bool is_signed,
                                    bool force_neg) {
    mjz_str_t<T> ret_var;
    ret_var.reserve(70, 1);
    char *ptr_ = b_U_lltoa(value, (char *)ret_var, radix, is_signed, force_neg);
    if (!ptr_) {
      return ret_var;
    }
    ret_var.addto_length((uint64_t)strlen(ptr_), 1);
    return ret_var;
  }
  mjz_str_t<T> &ULL_LL_to_str_add(uint64_t value, int radix, bool is_signed,
                                  bool force_neg = 0) {
    this->operator+=(ULL_LL_to_str(value, radix, is_signed, force_neg));
    return *this;
  }
  mjz_str_t<T> &ULL_LL_to_str_rep(uint64_t value, int radix, bool is_signed,
                                  bool force_neg = 0) {
    operator=(empty_STRING_C_STR);
    reserve(70, 1);
    char *ptr_ = b_U_lltoa(value, buffer_ref(), radix, is_signed, force_neg);
    if (!ptr_) {
      return *this;
    }
    addto_length((size_t)strlen(ptr_), 1);
    return *this;
  }
  friend void swap(mjz_str_t<T> &lhs, mjz_str_t<T> &rhs) { lhs.swap(rhs); }
  void swap(mjz_str_t<T> &rhs) { std::swap(*this, rhs); }
  char &at(int64_t i) & { return operator[](i); }
  char &front() & { return operator[]((int64_t)-1); }
  char &back() & { return buffer_ref()[0]; }
  mjz_str_t<T> &push_back(char cr_) {
    write(cr_);
    return *this;
  }
  mjz_str_t<T> &insert(size_t pos, const basic_mjz_Str_view &other);
  mjz_str_t<T> &insert(size_t pos, const basic_mjz_Str_view &other,
                       size_t subpos, size_t sublen = -1);
  mjz_str_t<T> &insert(size_t pos, const char *s, size_t n);
  mjz_str_t<T> &insert(size_t pos, const char *other) {
    return insert(pos, other, strlen(other));
  }
  mjz_str_t<T> &pop_back() {
    if (m_length) {
      buffer_ref()[--m_length] = 0;
    }
    return *this;
  }
  mjz_str_t<T> &insert(size_t pos, size_t n, char c);
  void insert(iterator p, size_t n, const char *s) {
    if (p.end() != end()) return;
    insert((size_t)(p.get_pointer() - m_buffer), s, n);
  }
  void insert(iterator p, size_t n, char c) {
    if (p.end() != end()) return;
    insert((size_t)(p.get_pointer() - m_buffer), c, n);
  }
  iterator insert(iterator p, char c) {
    insert(p, 1, c);
    return p;
  }
  // slow(becuse of compatibility) dont run
  template <class InputIterator>
  void insert(iterator p, InputIterator first, InputIterator last) {
    mjz_str_t<T> str(first, last);
    insert(p, str.length(), str.c_str());
  }
  friend class basic_mjz_Str_view;
  constexpr size_t capacity() const { return m_capacity; }
  void clear() { operator()(); }
  void shrink_to_fit() {
    mjz_str_t<T> str = create_mjz_Str_char_array(length(), 0, 1);
    memmove(str.C_str(), c_str(), str.length());
    *this = std::move(str);
  }
  inline bool empty() const { return is_blank(); }

 protected:
  // void update_event();
  // void update_event_ard_string();
  void init(bool constructor = 1);
  void invalidate(bool constructor = 1);
  bool changeBuffer(size_t maxStrLen, bool constructor);

 public:
  // easy quality of life
  inline mjz_str_t<T> *operator->() { return this; }
  inline const mjz_str_t<T> *operator->() const { return this; }
  inline mjz_str_t<T> &operator*() { return *this; }
  inline const mjz_str_t<T> &operator*() const { return *this; }
  // templated
  template <typename TYPE_>
  mjz_str_t<T> substring(TYPE_ beginIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex);
  };
  template <typename TYPE_, typename TYPE_2>
  mjz_str_t<T> substring(TYPE_ beginIndex, TYPE_2 endIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex, (int64_t)endIndex);
  }
  template <typename TYPE_, typename TYPE_2>
  mjz_str_t<T> substring_beg_n(TYPE_ beginIndex, TYPE_2 number) const {
    return basic_mjz_String::substring_beg_n((int64_t)beginIndex,
                                             (int64_t)number);
  }
  template <typename... arguments_types>
  int scanf(const char *format, arguments_types &&...arguments_arr) {
    int ret = sscanf((char *)buffer_ref(), format,
                     std::forward<arguments_types>(arguments_arr)...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf(const basic_mjz_Str_view &format,
            arguments_types &&...arguments_arr) {
    int ret = sscanf((char *)buffer_ref(), format.buffer_ref(),
                     std::forward<arguments_types>(arguments_arr)...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const char *format, arguments_types &&...arguments_arr) {
    int ret = sscanf_s((char *)buffer_ref(), format,
                       std::forward<arguments_types>(arguments_arr)...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const basic_mjz_Str_view &format,
              arguments_types &&...arguments_arr) {
    int ret = sscanf_s((char *)buffer_ref(), format.buffer_ref(),
                       std::forward<arguments_types>(arguments_arr)...);
    return ret;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
                         arguments_types &&...arguments_arr) {
    your__function_(*this, std::forward<arguments_types>(arguments_arr)...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
                               arguments_types &&...arguments_arr) const {
    your__function_(*this, std::forward<arguments_types>(arguments_arr)...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &&...arguments_arr) {
    return your__function_(*this,
                           std::forward<arguments_types>(arguments_arr)...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &&...arguments_arr) const {
    return your__function_(*this,
                           std::forward<arguments_types>(arguments_arr)...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str<T>
  // * , ... something)),...something)
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &&...arguments_arr) {
    return your__function_(*this,
                           std::forward<arguments_types>(arguments_arr)...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &&...arguments_arr) const {
    return your__function_(*this,
                           std::forward<arguments_types>(arguments_arr)...);
  }

 public:
  // constructors
  // creates a copy of the initial value.
  // if the initial value is null or invalid,or if memory allocation
  // fails,the string will be marked as invalid (i.e. "if (s)" will
  // be false).
  mjz_str_t(const char *cstr, size_t len_) {
    init();
    if (cstr) {
      copy(cstr, len_,
           1);  // TODO: V1053 https://pvs-studio.com/en/docs/warnings/V1053/
      // Calling the 'free' virtual function indirectly in the
      // constructor may lead to unexpected result at runtime. Check
      // lines: 'mjzString.cpp:358', 'mjzString.hpp:734'.
    }
  }
  mjz_str_t(const mjz_str_t<T> &value) {
    init();
    *this = value;
  }
  mjz_str_t(std::initializer_list<const char> list) {
    init();
    size_t newlen = list.size();
    addto_length(newlen);
    char *ptr_{m_buffer};
    for (const char cr : list) {
      *ptr_++ = cr;
    }
  }
  mjz_str_t(const char *c_str_) : mjz_str_t(basic_mjz_Str_view(c_str_)) {}
  mjz_str_t(mjz_ard::iterator_template_t<const char> list) {
    init();
    size_t newlen = list.size();
    addto_length(newlen);
    char *ptr_{m_buffer};
    for (const char cr : list) {
      *ptr_++ = cr;
    }
  }
  mjz_str_t(const __FlashStringHelper *pstr) {
    init();
    *this = pstr;
  }
  mjz_str_t(unsigned char value, unsigned char base = 10) {
    init();
    char buf[1 + 8 * sizeof(unsigned char)];
    utoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(int value, unsigned char base = 10) {
    init();
    char buf[2 + 8 * sizeof(int)];
    itoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(unsigned int value, unsigned char base = 10) {
    init();
    char buf[1 + 8 * sizeof(unsigned int)];
    utoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(long value, unsigned char base = 10) {
    init();
    char buf[2 + 8 * sizeof(long)];
    ltoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(unsigned long value, unsigned char base = 10) {
    init();
    char buf[1 + 8 * sizeof(unsigned long)];
    ultoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(long long int value, unsigned char base = 10) {
    init();
    char buf[2 + 8 * sizeof(long long)];
    lltoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(long long unsigned int value, unsigned char base = 10) {
    init();
    char buf[1 + 8 * sizeof(unsigned long long)];
    ulltoa(value, buf, base);
    *this = buf;
  }
  mjz_str_t(const void *const ptr, unsigned char base = 16) {
    init();
    char buf[1 + 8 * sizeof(const void *const)];
    ulltoa(reinterpret_cast<uint64_t>(ptr), buf, base);
    *this = buf;
  }
  mjz_str_t(float value, unsigned char decimalPlaces = 2) {
    static size_t const FLOAT_BUF_SIZE = FLT_MAX_10_EXP +
                                         FLT_MAX_DECIMAL_PLACES + 1 /* '-' */ +
                                         1 /* '.' */ + 1 /* '\0' */;
    init();
    char buf[FLOAT_BUF_SIZE];
    decimalPlaces = min_macro_(decimalPlaces, FLT_MAX_DECIMAL_PLACES);
    auto ret =
        dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf, FLOAT_BUF_SIZE);
    *this = basic_mjz_Str_view(ret.buffer, ret.len);
  }
  mjz_str_t(double value, unsigned char decimalPlaces = 2) {
    static size_t const DOUBLE_BUF_SIZE = DBL_MAX_10_EXP +
                                          DBL_MAX_DECIMAL_PLACES + 1 /* '-' */ +
                                          1 /* '.' */ + 1 /* '\0' */;
    init();
    char buf[DOUBLE_BUF_SIZE];
    decimalPlaces = min_macro_(decimalPlaces, DBL_MAX_DECIMAL_PLACES);
    auto ret = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf,
                       DOUBLE_BUF_SIZE);
    *this = basic_mjz_Str_view(ret.buffer, ret.len);
  }
  inline mjz_str_t(const uint8_t *cstr, size_t length)
      : mjz_str_t((const char *)cstr, length) {}
  template <class InputIterator>
  mjz_str_t(InputIterator first, InputIterator last) {
    init();
    size_t newlen{};
    InputIterator begin_it = first;
    while (begin_it !=
           last) {  // not using last-first for compatibility //TODO: V776
                    // https://pvs-studio.com/en/docs/warnings/V776/ Potentially
                    // infinite loop. The variable in the loop exit condition
                    // 'begin_it != last' does not change its value between
                    // iterations.
      begin_it++;
      newlen++;
    }
    if (!addto_length(newlen, 1)) return;
    char *ptr_{m_buffer};
    while (first != last) {
      *ptr_++ = *first++;
    }
  }
  mjz_str_t(mjz_ard::mjz_str_t<T> &&rval) noexcept
      : mjz_ard::basic_mjz_String(
            (rval.stack_obj_buf.get() ? stack_obj_buf.stack_buffer
                                      : rval.m_buffer),
            (rval.m_capacity), (rval.m_length)),
        stack_obj_buf(rval.stack_obj_buf) {
    // update_event_F_p = &mjz_ard::mjz_str_t<T>::update_event;//departed
    if (rval.stack_obj_buf.get()) {
      rval.free_pv(rval.m_buffer, 1);
    }
    rval.m_buffer = NULL;
    rval.m_capacity = 0;
    rval.m_length = 0;
    rval.stack_obj_buf.set(0);
  }
  mjz_str_t(char c) {
    init();
    char buf[2] = {c, 0};
    *this = buf;
  }
  // explicit mjz_Str<T>(const mjz_Str<T> *&&rval) :
  // mjz_Str<T>(std::move(*rval)) {
  // } // this will give me headaches in the long run so i dont move it
  // inline explicit mjz_str_t(const mjz_str_t<T> *rval) : mjz_str_t(*rval)
  // { }
  inline mjz_str_t() : mjz_str_t((const char *)empty_STRING_C_STR, 0) {}
  mjz_str_t(const basic_mjz_String &otr)
      : mjz_str_t(otr.c_str(), otr.length()) {}
  mjz_str_t(const basic_mjz_Str_view &otr)
      : mjz_str_t(otr.c_str(), otr.length()) {}
};
/*
please dont use mjz_str_view with temporary strings
 if the string that it references goes out of scope (delete , ~obj , } ,free
 ,...)the string_view will have undefined behavior use this obj like a
 std::string_view not like std::string
*/
class mjz_str_view : public basic_mjz_Str_view {
 protected:
  constexpr char *&buffer_ref(void) & { return m_buffer; }
  using basic_mjz_Str_view::buffer_ref;
  using basic_mjz_Str_view::C_str;
  using basic_mjz_Str_view::c_str;
  constexpr mjz_str_view &copy(const basic_mjz_Str_view &s) {
    m_buffer = const_cast<char *>(s.c_str());
    m_length = s.length();
    return *this;
  };
  constexpr inline const char *begin_c_str() const & { return c_str(); }
  constexpr inline const char *end_c_str() const & {
    return c_str() + length();
  }

 public:
  constexpr inline const char *data() const && { return m_buffer; }
  constexpr inline const char *data() && { return m_buffer; }
  constexpr inline const char *data() & { return m_buffer; }
  constexpr inline const char *data() const & { return m_buffer; }
  template <typename T>
  mjz_str_view(const mjz_str_t<T> &s) : mjz_str_view(s.c_str(), s.length()) {}
  constexpr mjz_str_view(const basic_mjz_Str_view &str)
      : basic_mjz_Str_view(str) {}
  constexpr mjz_str_view(const char *cstr_, size_t len)
      : basic_mjz_Str_view(cstr_, len) {}
  constexpr mjz_str_view(const uint8_t *cstr_, size_t len)
      : mjz_str_view((char *)(cstr_), len) {}
  constexpr mjz_str_view(const uint8_t *cstr_)
      : mjz_str_view((char *)(cstr_)) {}
  constexpr mjz_str_view(const char *cstr_)
      : mjz_str_view(cstr_, strlen(cstr_)) {}
  constexpr mjz_str_view() : mjz_str_view(empty_STRING_C_STR, 0) {}
  mjz_str_view(mjz_str_view &&s) =
      default;  // we are string views and not strings
  mjz_str_view &operator=(mjz_str_view &&) = default;
  template <typename T>
  constexpr mjz_str_view &operator=(mjz_str_t<T> &&s) {
    return copy(s);
  }
  template <typename T>
  mjz_str_view(mjz_str_t<T> &&s) : mjz_str_view(s) {}
  constexpr mjz_str_view(const mjz_str_view &s)
      : mjz_str_view(s.c_str(), s.length()) {}
  constexpr mjz_str_view(mjz_str_view &s)
      : mjz_str_view(s.c_str(), s.length()) {}
  constexpr mjz_str_view &operator=(const mjz_str_view &s) { return copy(s); };
  constexpr mjz_str_view &operator=(mjz_str_view &s) { return copy(s); }
  inline ~mjz_str_view() = default;
  using iterator_template_CC = iterator_template_t<const char>;
  using const_iterator = iterator_template_CC;
  inline const_iterator begin() const {
    return const_iterator(begin_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  inline const_iterator end() const {
    return const_iterator(end_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  typedef std::reverse_iterator<const_iterator> const_rev_iterator;
  inline const_rev_iterator rbegin() const {
    return const_rev_iterator(end());
  };
  inline const_rev_iterator rend() const {
    return const_rev_iterator(begin());
  };

 public:
  constexpr inline void remove_prefix(size_t n) {
    m_buffer += n;
    remove_suffix(n);
  }
  constexpr inline void remove_suffix(size_t n) { m_length -= n; }
  constexpr friend void swap(mjz_str_view &lhs, mjz_str_view &rhs) {
    lhs.swap(rhs);
  }
  constexpr void swap(mjz_str_view &rhs) {
    static_str_algo::swap(m_length, rhs.m_length);
    static_str_algo::swap(m_buffer, rhs.m_buffer);
  }
  constexpr size_t copy(char *dest, size_t count, size_t pos = 0) const {
    if (length() <= pos) {
      return 0;
    }
    size_t amount = min(count, length() - pos);
    memmove(dest, m_buffer + pos, amount);
    return amount;
  }
  constexpr size_t copy(char *dest, size_t count, int64_t pos) const {
    return copy(dest, count, mjz_str_view::signed_index_to_unsigned(pos));
  }
  friend std::ostream &operator<<(std::ostream &COUT, const mjz_str_view &rhs) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
  friend std::ostream &operator>>(const mjz_str_view &rhs, std::ostream &COUT) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT, mjz_str_view &rhs) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
  friend std::ostream &operator>>(mjz_str_view &rhs, std::ostream &COUT) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
  friend std::ostream &operator>>(mjz_str_view &&rhs, std::ostream &COUT) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT, mjz_str_view &&rhs) {
    COUT.write(rhs.c_str(), rhs.length());
    return COUT;
  }
};

class mjz_virtual_string_view : public mjz_str_view {
  template <typename T>
  mjz_virtual_string_view(const mjz_str_t<T> &s)
      : mjz_virtual_string_view(s.c_str(), s.length()) {}
  mjz_virtual_string_view(const char *cstr_, size_t len)
      : mjz_str_view(cstr_, len) {}
  mjz_virtual_string_view(const char *cstr_)
      : mjz_virtual_string_view(cstr_, strlen(cstr_)) {}
  mjz_virtual_string_view() : mjz_virtual_string_view(empty_STRING_C_STR, 0) {}
  mjz_virtual_string_view(mjz_virtual_string_view &&s) =
      default;  // we are ptr views and not strings
  mjz_virtual_string_view &operator=(mjz_virtual_string_view &&) = default;
  template <typename T>
  mjz_virtual_string_view &operator=(mjz_str_t<T> &&s) {
    copy(s);
    return *this;
  }
  template <typename T>
  mjz_virtual_string_view(mjz_str_t<T> &&s) : mjz_str_view(s) {}
  mjz_virtual_string_view(const mjz_virtual_string_view &s)
      : mjz_virtual_string_view(s.c_str(), s.length()) {}
  mjz_virtual_string_view(mjz_virtual_string_view &s)
      : mjz_virtual_string_view(s.c_str(), s.length()) {}
  mjz_virtual_string_view &operator=(const mjz_virtual_string_view &s) {
    copy(s);
    return *this;
  };
  mjz_virtual_string_view &operator=(mjz_virtual_string_view &s) {
    copy(s);
    return *this;
  }
  virtual inline ~mjz_virtual_string_view(){};
};
template <typename T>
class extended_mjz_str_t : public mjz_str_t<T> {
 public:
  extended_mjz_str_t() : mjz_str_t<T>() {}
  extended_mjz_str_t(const basic_mjz_String &s) : mjz_str_t<T>(s) {}
  extended_mjz_str_t(basic_mjz_String &&s) : extended_mjz_str_t(s) {}
  extended_mjz_str_t(const basic_mjz_Str_view &otr) : mjz_str_t<T>(otr) {}
  extended_mjz_str_t(const mjz_str_view &s) : mjz_str_t<T>(s) {}
  extended_mjz_str_t(mjz_str_view &&s) : extended_mjz_str_t(s) {}
  extended_mjz_str_t(const mjz_str_t<T> &s) : mjz_str_t<T>(s) {}
  extended_mjz_str_t(mjz_str_t<T> &&s) : mjz_str_t<T>(std::move(s)) {}
  extended_mjz_str_t(const char *p) : mjz_str_t<T>(p) {}
  extended_mjz_str_t(const char *p, size_t n) : mjz_str_t<T>(p, n) {}
  extended_mjz_str_t(char c) : mjz_str_t<T>(c) {}
  extended_mjz_str_t(unsigned char num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(int num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(unsigned int num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(long num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(unsigned long num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(long long num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(unsigned long long num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(float num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(double num) : mjz_str_t<T>(num) {}
  extended_mjz_str_t(const extended_mjz_str_t &s) : mjz_str_t<T>(s) {}
  extended_mjz_str_t(extended_mjz_str_t &&s) : mjz_str_t<T>(std::move(s)) {}
  extended_mjz_str_t &operator=(const extended_mjz_str_t &s) {
    mjz_str_t::operator=(s);
    return *this;
  }
  extended_mjz_str_t &operator=(extended_mjz_str_t &&s) {
    mjz_str_t::operator=(std::move(s));
    return *this;
  }
  extended_mjz_str_t &operator=(const mjz_str_t<T> &s) {
    mjz_str_t::operator=(s);
    return *this;
  }
  extended_mjz_str_t &operator=(mjz_str_t<T> &&s) {
    mjz_str_t::operator=(std::move(s));
    return *this;
  }
  extended_mjz_str_t &operator=(const basic_mjz_Str_view &s) {
    mjz_str_t::operator=(s);
    return *this;
  }
  virtual ~extended_mjz_str_t() = default;
  friend std::istream &operator<<(extended_mjz_str_t<T> &rhs,
                                  std::istream &CIN) {
    return helper__op_shift_input_(rhs, CIN, rhs.get_s_shift_op_l_s());
  }
  friend std::istream &operator>>(std::istream &CIN,
                                  extended_mjz_str_t<T> &rhs) {
    return helper__op_shift_input_(rhs, CIN, rhs.get_shift_op_r_s());
  }
  friend std::ostream &operator<<(std::ostream &COUT,
                                  const extended_mjz_str_t<T> &rhs) {
    COUT.write(rhs.get_shift_op_l_sc().c_str(),
               rhs.get_shift_op_l_sc().length());
    return COUT;
  }
  friend std::ostream &operator>>(const extended_mjz_str_t<T> &rhs,
                                  std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_sc().c_str(),
               rhs.get_s_shift_op_r_sc().length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT,
                                  extended_mjz_str_t<T> &rhs) {
    COUT.write(rhs.get_shift_op_l_s().c_str(), rhs.get_shift_op_l_s().length());
    return COUT;
  }
  friend std::ostream &operator>>(extended_mjz_str_t<T> &rhs,
                                  std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_s().c_str(),
               rhs.get_s_shift_op_r_s().length());
    return COUT;
  }
  friend std::ostream &operator>>(extended_mjz_str_t<T> &&rhs,
                                  std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_sc().c_str(),
               rhs.get_s_shift_op_r_sc().length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT,
                                  extended_mjz_str_t<T> &&rhs) {
    COUT.write(rhs.get_shift_op_l_sc().c_str(),
               rhs.get_shift_op_l_sc().length());
    return COUT;
  }
  const extended_mjz_str_t<T> &operator>>(extended_mjz_str_t<T> &typing) const;
  const extended_mjz_str_t<T> &operator>>(extended_mjz_str_t<T> *typing) const;
  extended_mjz_str_t<T> &operator>>(extended_mjz_str_t<T> &typing);
  extended_mjz_str_t<T> &operator>>(extended_mjz_str_t<T> *typing);
  const extended_mjz_str_t<T> &operator>>=(
      extended_mjz_str_t<T> &typing) const {
    (typing)();
    return operator>>(typing);
  }
  const extended_mjz_str_t<T> &operator>>=(
      extended_mjz_str_t<T> *typing) const {
    (*typing)();
    return operator>>(typing);
  }
  extended_mjz_str_t<T> &operator>>=(extended_mjz_str_t<T> &typing) {
    (typing)();
    return operator>>(typing);
  }
  extended_mjz_str_t<T> &operator>>=(extended_mjz_str_t<T> *typing) {
    (*typing)();
    return operator>>(typing);
  }
  extended_mjz_str_t<T> &operator<<(extended_mjz_str_t<T> &typing);
  extended_mjz_str_t<T> &operator<<(extended_mjz_str_t<T> *typing);
  extended_mjz_str_t<T> &operator<<(const extended_mjz_str_t<T> &typing);
  extended_mjz_str_t<T> &operator<<(extended_mjz_str_t<T> &&typing);
  extended_mjz_str_t<T> &operator<<=(extended_mjz_str_t<T> &typing) {
    if (&typing != this) {
      (*this)();
      return operator<<(typing);
    }
    extended_mjz_str_t<T> new_temp = typing;
    (*this)();
    return operator<<(new_temp);
  }
  extended_mjz_str_t<T> &operator<<=(const extended_mjz_str_t<T> &typing) {
    if (&typing != this) {
      (*this)();
      return operator<<(typing);
    }
    extended_mjz_str_t<T> new_temp = typing;
    (*this)();
    return operator<<(new_temp);
  }
  extended_mjz_str_t<T> &operator<<=(extended_mjz_str_t<T> &&typing) {
    if (&typing != this) {
      (*this)();
      return operator<<(typing);
    }
    extended_mjz_str_t<T> new_temp = typing;
    (*this)();
    return operator<<(new_temp);
  }
  extended_mjz_str_t<T> &operator>>(char &var) {
    get_s_shift_op_r().scanf_s("%c", &var, 1);
    return get_s_shift_op_r();
  }
  extended_mjz_str_t<T> &operator>>(int &var) {
    get_s_shift_op_r().scanf_s("%d", &var);
    return get_s_shift_op_r();
  }
  extended_mjz_str_t<T> &operator>>(double &var) {
    var = (double)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  extended_mjz_str_t<T> &operator>>(float &var) {
    var = (float)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  extended_mjz_str_t<T> string_do_interpret();
  void string_do_interpret(extended_mjz_str_t<T> &instr);

 public:
  inline const std::function<char(char)>
      &get_char_to_char_for_reinterpret_fnc_ptr_function() const {
    return drived_mjz_Str_DATA_storage_Obj_ptr
        ->char_to_char_for_reinterpret_fnc_ptr;
  }
  inline const std::function<bool(char)> &get_is_forbiden_character_function()
      const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->is_forbiden_character;
  }
  inline const std::function<bool(char)> &get_is_blank_character_function()
      const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->is_blank_character;
  }

 public:
  friend class basic_mjz_String;
  void set_realloc_free_functions(
      std::function<void(void *)> free_,
      std::function<void *(void *, size_t)> realloc_) {
    if (free_ && realloc_) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->realloc_fnctn = realloc_;
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->free_fnctn = free_;
    }
  }
  void change_is_blank_character_function(std::function<bool(char)> fnction) {
    if (fnction) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->is_blank_character = fnction;
    }
  }
  void change_char_to_char_for_reinterpret_fnc_ptr_function(
      std::function<char(char)> fnction) {
    if (fnction) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()
          ->char_to_char_for_reinterpret_fnc_ptr = fnction;
    }
  }
  void change_is_forbiden_character_function(
      std::function<bool(char)> fnction) {
    if (fnction) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->is_forbiden_character =
          fnction;
    }
  }
  bool is_blank() const;
  bool is_forbiden(char) const;
  bool change_reinterpret_char_char(char);
  bool char_to_char_for_reinterpret(char &c_char) const;
  char get_reinterpret_char_char() const;

 protected:
  uint8_t did_drived_mjz_Str_DATA_storage_Obj_ptr_set{0};
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      drived_mjz_Str_DATA_storage_Obj_ptr =
          mjz_Str_DATA_storage_cls::get_defult();
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      &drived_mjz_Str_DATA_storage_Obj_ptr_set();
  const extended_mjz_str_t<T> &get_shift_op_rc() const;
  extended_mjz_str_t<T> &get_shift_op_r();
  const extended_mjz_str_t<T> &get_shift_op_lc() const;
  extended_mjz_str_t<T> &get_shift_op_l();
  const extended_mjz_str_t<T> &get_shift_op_r_sc() const;
  extended_mjz_str_t<T> &get_shift_op_r_s();
  const extended_mjz_str_t<T> &get_shift_op_l_sc() const;
  extended_mjz_str_t<T> &get_shift_op_l_s();
  const extended_mjz_str_t<T> &get_s_shift_op_rc() const;
  extended_mjz_str_t<T> &get_s_shift_op_r();
  const extended_mjz_str_t<T> &get_s_shift_op_lc() const;
  extended_mjz_str_t<T> &get_s_shift_op_l();
  const extended_mjz_str_t<T> &get_s_shift_op_r_sc() const;
  extended_mjz_str_t<T> &get_s_shift_op_r_s();
  const extended_mjz_str_t<T> &get_s_shift_op_l_sc() const;
  extended_mjz_str_t<T> &get_s_shift_op_l_s();
};
template <typename T>
class StringSumHelper_t : public mjz_str_t<T> {
 public:
  StringSumHelper_t(const basic_mjz_String &s) : mjz_str_t<T>(s) {}
  StringSumHelper_t(basic_mjz_String &&s) : StringSumHelper_t(s) {}
  StringSumHelper_t(const basic_mjz_Str_view &otr) : mjz_str_t<T>(otr) {}
  StringSumHelper_t(const mjz_str_view &s) : mjz_str_t<T>(s) {}
  StringSumHelper_t(mjz_str_view &&s) : StringSumHelper_t(s) {}
  StringSumHelper_t(const mjz_str_t<T> &s) : mjz_str_t<T>(s) {}
  StringSumHelper_t(const char *p) : mjz_str_t<T>(p) {}
  StringSumHelper_t(char c) : mjz_str_t<T>(c) {}
  StringSumHelper_t(unsigned char num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(int num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(unsigned int num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(long num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(unsigned long num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(long long num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(unsigned long long num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(float num) : mjz_str_t<T>(num) {}
  StringSumHelper_t(double num) : mjz_str_t<T>(num) {}
  // non virtual
  ~StringSumHelper_t() = default;
};
template <typename TYPE_>
class type_cmp_class {
 public:
  bool operator()(const TYPE_ &a, const TYPE_ &b) { return cmp(a, b); }
  bool cmp(const TYPE_ &a, const TYPE_ &b) { return a < b; }
};
template <typename TYPE_, class cmpr_function>
class type_cmp_fn_class {
 public:
  bool operator()(const TYPE_ &a, const TYPE_ &b) {
    return cmpr_function(a, b);
  }
};
template <class _function>
class type_fn_class {
 public:
  template <class Type>
  bool operator()(const Type &x) {
    return _function(x);
  }
};
template <typename T>
mjz_str_t<T> &getline(mjz_str_t<T> &is, mjz_str_t<T> &str, char delim);
template <typename T>
mjz_str_t<T> &getline(mjz_str_t<T> &is, mjz_str_t<T> &str);
template <typename T, size_t buffer_len = 2048>
std::istream &getline(std::istream &is, mjz_str_t<T> &str, char delim) {
  char buffer[buffer_len]{};
  is.getline(buffer, buffer_len, delim);
  uint32_t available_len = static_str_algo::strlen(buffer);
  str.append(buffer, available_len);
  return is;
}
template <typename T, size_t buffer_len = 2048>
std::istream &getline(std::istream &is, mjz_str_t<T> &str) {
  return getline<T, buffer_len>(is, str, '\n');
}
template <typename Type>
inline mjz_ard::mjz_Str get_bit_representation(const Type &data) {
  mjz_ard::mjz_Str buffer;
  buffer.addto_length(sizeof(Type) * 8);
  static_str_algo::get_bit_representation(buffer.C_str(), buffer.length(),
                                          &data, sizeof(Type));
  return buffer;
}

inline mjz_ard::mjz_Str operator"" _m_str(const char *p) {
  return mjz_ard::mjz_Str(p);
}

inline mjz_ard::mjz_Str operator"" _m_str(char c) {
  return mjz_ard::mjz_Str(c);
}
inline mjz_ard::mjz_Str operator"" _m_str(unsigned long long num) {
  return mjz_ard::mjz_Str(num);
}
inline mjz_ard::mjz_Str operator"" _m_str(long double num) {
  return mjz_ard::mjz_Str((double)num);
}

inline mjz_ard::mjz_Str operator"" _m_pstr(const char *initilizer) {
  return mjz_ard::extended_mjz_Str(initilizer).string_do_interpret();
}
inline mjz_ard::mjz_Str operator""_m_str(const char *initilizer,
                                         size_t length_) {
  return mjz_ard::mjz_Str(initilizer, length_);
}
inline mjz_ard::mjz_Str operator"" _m_pstr(const char *initilizer,
                                           size_t length_) {
  return mjz_ard::extended_mjz_Str(initilizer, length_).string_do_interpret();
}
inline mjz_str_view operator""_m_strv(const char *initilizer, size_t length_) {
  return mjz_str_view(initilizer, length_);
}
inline mjz_str_view operator"" _m_strv(const char *p) {
  return operator""_m_strv(p, static_str_algo::strlen(p));
}

namespace short_string_convestion_operators {
inline mjz_str_view operator""_msv(const char *initilizer, size_t length_) {
  return operator""_m_strv(initilizer, length_);
}
inline mjz_str_view operator"" _msv(const char *p) {
  return operator""_m_strv(p, static_str_algo::strlen(p));
}
inline mjz_str_view operator""_sv(const char *initilizer, size_t length_) {
  return operator""_m_strv(initilizer, length_);
}
inline mjz_str_view operator""_sv(const char *p) { return operator""_msv(p); }
inline mjz_ard::mjz_Str operator"" _s(const char *p) {
  return mjz_ard::mjz_Str(p);
}
inline mjz_ard::mjz_Str operator"" _s(char c) { return mjz_ard::mjz_Str(c); }
inline mjz_ard::mjz_Str operator"" _s(unsigned long long num) {
  return mjz_ard::mjz_Str(num);
}
inline mjz_ard::mjz_Str operator"" _s(long double num) {
  return mjz_ard::mjz_Str((double)num);
}
inline mjz_ard::mjz_Str operator"" _ps(const char *initilizer) {
  return mjz_ard::mjz_Str(initilizer);
}
inline mjz_ard::mjz_Str operator""_s(const char *initilizer, size_t length_) {
  return mjz_ard::mjz_Str(initilizer, length_);
}
inline mjz_ard::mjz_Str operator"" _ps(const char *initilizer, size_t length_) {
  return mjz_ard::mjz_Str(initilizer, length_);
}
inline mjz_str_view operator""_v(const char *initilizer, size_t length_) {
  return mjz_str_view(initilizer, length_);
}

inline mjz_str_view operator""_mv(const char *initilizer, size_t length_) {
  return operator""_v(initilizer, length_);
}
inline mjz_str_view operator"" _mv(const char *p) {
  return operator""_v(p, static_str_algo::strlen(p));
}
inline mjz_str_view operator"" _v(const char *p) { return operator""_mv(p); }
}  // namespace short_string_convestion_operators

/************************************************************
Author: Charlie Murphy
 Email: tm507211@ohio.edu

Date: July 20, 2015

Description: Implementation of mjz_vector classes
************************************************************/
}  // namespace mjz_ard

namespace mjz_ard {
/***************************************************************************************
Vector2 -- 2-D mjz_vector class
***************************************************************************************/
template <class Type>
class Vector2 {
 public:
  union {
    struct {
      Type m_x, m_y;
    };
    struct {
      Type x, y;
    };
  };
  inline ~Vector2() {
    m_x.~Type();  // the
    // https://stackoverflow.com/questions/40106941/is-a-union-members-destructor-called
    m_y.~Type();
  };
  constexpr inline Vector2 &operator=(const Vector2 &v) {
    m_x = (v.m_x);
    m_y = (v.m_y);
    return *this;
  }
  constexpr inline Vector2 &operator=(Vector2 &v) {
    return operator=((const Vector2<Type> &)v);
  }
  constexpr inline Vector2 &operator=(Vector2 &&v) { return operator=(v); }
  constexpr inline Vector2 &operator()(Vector2 &obj) { return *this = obj; };
  constexpr inline Vector2 &operator()(Vector2 &&obj) { return *this = obj; };
  constexpr inline Vector2 &operator()(const Vector2 &obj) {
    return *this = obj;
  };
  constexpr inline Vector2 &operator()(const Type &x, const Type &y) {
    m_x = (x);
    m_y = (y);
    return *this;
  };
  template <typename FNC_T>
  inline auto operator()(FNC_T your_function) {
    return your_function(*this);
  };
  template <typename FNC_T>
  inline auto operator()(FNC_T your_function) const {
    return your_function(*this);
  };
  template <typename FNC_T>
  inline Vector2 operator()(int do_save, FNC_T your_function) {
    auto v = Vector2(your_function(x()), your_function(y()));
    if (do_save) return (*this)(v);
    return v;
  };
  template <typename FNC_T>
  inline Vector2 operator()(int, FNC_T your_function) const {
    return Vector2(your_function(x()), your_function(y()));
  };
  template <typename FNC_T, typename FNC_T2>
  inline auto operator()(FNC_T2 your_function_returning, FNC_T your_function) {
    return your_function_returning(operator()(0, your_function));
  };
  template <typename FNC_T, typename FNC_T2>
  inline auto operator()(FNC_T2 your_function_returning,
                         FNC_T your_function) const {
    return your_function_returning(operator()(0, your_function));
  };
  constexpr inline Vector2() : m_x{}, m_y{} {}
  constexpr inline Vector2(const Type &s) : m_x(s), m_y(s) {}
  constexpr inline Vector2(const Type &x, const Type &y) : m_x(x), m_y(y) {}
  constexpr inline Vector2(const Vector2<Type> &v) : m_x(v.m_x), m_y(v.m_y) {}
  constexpr inline Vector2(Vector2 &v) : Vector2((const Vector2<Type> &)v){};
  constexpr inline Vector2(Vector2 &&v) : Vector2(v){};
  constexpr inline bool operator==(const Vector2<Type> &v) const {
    return abs(m_x - v.m_x) < 0.01 && abs(m_y - v.m_y) < 0.01;
  }
  constexpr inline bool operator!=(const Vector2<Type> &v) const {
    return !(operator==(v));
  }
  /**********************************************
  Indexing operator
   **********************************************/
  constexpr inline Type &operator[](int i) { return *(&m_x + i); }
  constexpr inline const Type operator[](int i) const { return *(&m_x + i); }
  /*********************************************
  Non modifying math operators
   *********************************************/
  constexpr inline Vector2<Type> operator-() const {
    return Vector2<Type>(-m_x, -m_y);
  }
  constexpr inline Vector2<Type> operator+(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x + v.m_x, m_y + v.m_y);
  }
  constexpr inline Vector2<Type> operator-(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x - v.m_x, m_y - v.m_y);
  }
  constexpr inline Vector2<Type> operator*(const Type &s) const {
    return Vector2<Type>(m_x * s, m_y * s);
  }
  constexpr inline Vector2<Type> operator*(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x * v.m_x, m_y * v.m_y);
  }
  constexpr inline Vector2<Type> operator/(const Type &s) const {
    return Vector2<Type>(m_x / s, m_y / s);
  }
  /*******************************************
  Modifying Math Operators
   *******************************************/
  constexpr inline Vector2<Type> &operator+=(const Vector2<Type> &v) {
    m_x += v.m_x;
    m_y += v.m_y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator-=(const Vector2<Type> &v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator*=(const Type &s) {
    m_x *= s;
    m_y *= s;
    return *this;
  }
  constexpr inline Vector2<Type> &operator*=(const Vector2<Type> &v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator/=(const Type &s) {
    m_x /= s;
    m_y /= s;
    return *this;
  }
  /*******************************************
  Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  constexpr inline operator const Type *() const {
    return static_cast<Type *>(&m_x);
  }
  constexpr inline operator Type *() { return static_cast<Type *>(&m_x); }
  /********************************************
  Useful Vector Operations
   ********************************************/
  constexpr inline Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(m_x * m_x + m_y * m_y);
  }
  constexpr inline Type lengthSq() const { return m_x * m_x + m_y * m_y; }
  constexpr inline Vector2<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Vector2<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Type dot(const Vector2<Type> &v) const {
    return m_x * v.m_x + m_y * v.m_y;
  }
  constexpr inline Type cross(
      const Vector2<Type> &v) const {  // 3-D cross product with z assumed 0
    return m_x * v.m_y +
           v.m_x * m_y;  // return magnitude of resulting mjz_vector
  }
  friend std::ostream &operator<<(std::ostream &outs, const Vector2<Type> &v) {
    outs << "<" << v.m_x << ", " << v.m_y << ">";
    return outs;
  }
  friend std::istream &operator>>(std::istream &ins, Vector2<Type> &v) {
    ins >> v.m_x >> v.m_y;
    return ins;
  }
  friend constexpr inline Vector2<Type> operator*(Type s,
                                                  const Vector2<Type> &v) {
    return Vector2<Type>(s * v.m_x, s * v.m_y);
  }
  /********************************************************
  Basic Trig functions of angle between vectors
   ********************************************************/
  friend constexpr inline Type cos(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return dot(v1, v2) / v1.length() / v2.length();
  }
  friend constexpr inline Type sin(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return cross(v1, v2) / v1.length() / v2.length();
  }
  friend constexpr inline Type tan(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return sin(v1, v2) / cos(v1, v2);
  }
  friend constexpr inline Type angle(const Vector2<Type> &v1,
                                     const Vector2<Type> &v2) {
    return std::acos(cos(v1, v2));
  }
  friend constexpr inline Type dot(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return v1.dot(v2);
  }
  friend constexpr inline Type cross(const Vector2<Type> &v1,
                                     const Vector2<Type> &v2) {
    return v1.cross(v2);
  }
  constexpr inline Type *begin() { return &m_x; }
  constexpr inline Type *end() { return begin() + size(); }
  constexpr inline const Type *begin() const { return &m_x; }
  constexpr inline const Type *end() const { return begin() + size(); }
  constexpr inline const Type *cbegin() const { return begin(); }
  constexpr inline const Type *cend() const { return end(); }
  constexpr inline static const size_t size() { return 2; }
  constexpr inline bool operator!() const { return !m_x && !m_y; }
  constexpr inline operator bool() const { return !!*this; }
};
/*********************************************************************************
Vector3 -- 3D mjz_vector
*********************************************************************************/

template <class Type>
class Vector3 {
 public:
  union {
    struct {
      Type m_x, m_y, m_z;
    };
    struct {
      Type x, y, z;
    };
  };
  inline ~Vector3() {  // https://stackoverflow.com/questions/40106941/is-a-union-members-destructor-called
    m_x.~Type();
    m_y.~Type();
    m_z.~Type();
  };
  constexpr inline Vector3 &operator()(Vector3 &obj) { return *this = obj; };
  constexpr inline Vector3 &operator()(Vector3 &&obj) { return *this = obj; };
  constexpr inline Vector3 &operator()(const Vector3 &obj) {
    return *this = obj;
  };
  constexpr inline Vector3 &operator()(const Type &x, const Type &y,
                                       const Type &z) {
    m_x = (x);
    m_x = (y);
    m_x = (z);
    return *this;
  };
  template <typename FNC_T>
  inline auto operator()(FNC_T your_function) {
    return your_function(*this);
  };
  template <typename FNC_T>
  inline auto operator()(FNC_T your_function) const {
    return your_function(*this);
  };
  template <typename FNC_T>
  inline Vector3 operator()(int do_save, FNC_T your_function) {
    auto v =
        Vector3(your_function(x()), your_function(y()), your_function(z()));
    if (do_save) return (*this)(v);
    return v;
  };
  template <typename FNC_T>
  inline Vector3 operator()(int, FNC_T your_function) const {
    return Vector3(your_function(x()), your_function(y()), your_function(z()));
  };
  template <typename FNC_T, typename FNC_T2>
  inline auto operator()(FNC_T2 your_function_returning, FNC_T your_function) {
    return your_function_returning(operator()(0, your_function));
  };
  template <typename FNC_T, typename FNC_T2>
  inline auto operator()(FNC_T2 your_function_returning,
                         FNC_T your_function) const {
    return your_function_returning(operator()(0, your_function));
  };
  constexpr inline Vector3() : m_x{}, m_y{}, m_z{} {}
  constexpr inline Vector3(const Type &s) : m_x(s), m_y(s), m_z(s) {}
  constexpr inline Vector3(const Type &x, const Type &y, const Type &z)
      : m_x(x), m_y(y), m_z(z) {}
  constexpr inline Vector3(const Vector2<Type> &v, const Type &s = Type())
      : m_x(v.m_x), m_y(v.m_y), m_z(s) {}
  constexpr inline Vector3(const Vector3<Type> &v)
      : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) {}
  constexpr inline Vector3(Vector3 &v) : Vector3((const Vector3<Type> &)v){};
  constexpr inline Vector3(Vector3 &&v) : Vector3(v){};
  constexpr inline Vector3 &operator=(const Vector3 &v) {
    m_x = (v.m_x);
    m_y = (v.m_y);
    m_z = (v.m_z);
    return *this;
  }
  constexpr inline Vector3 &operator=(Vector3 &v) {
    return operator=((const Vector3<Type> &)v);
  }
  constexpr inline Vector3 &operator=(Vector3 &&v) { return operator=(v); }
  constexpr inline bool operator==(const Vector3<Type> &v) const {
    return abs(m_x - v.m_x) < 0.01 && abs(m_y - v.m_y) < 0.01 &&
           abs(m_z - v.m_z) < 0.01;
  }
  constexpr inline bool operator!=(const Vector3<Type> &v) const {
    return !operator==(v);
  }
  /**********************************************
  Indexing operator
   **********************************************/
  constexpr inline Type &operator[](int i) { return *(&m_x + i); }
  const Type operator[](int i) const { return *(&m_x + i); }
  /**********************************************
  Itrator
   **********************************************/
  constexpr inline Type *begin() { return &m_x; }
  constexpr inline Type *end() { return begin() + size(); }
  constexpr inline const Type *begin() const { return &m_x; }
  constexpr inline const Type *end() const { return begin() + size(); }
  constexpr inline const Type *cbegin() const { return begin(); }
  constexpr inline const Type *cend() const { return end(); }
  constexpr inline static const size_t size() { return 3; }
  /*********************************************
  Non modifying math operators
   *********************************************/
  constexpr inline Vector3<Type> operator-() const {
    return Vector3<Type>(-m_x, -m_y, -m_z);
  }
  constexpr inline Vector3<Type> operator+(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
  }
  constexpr inline Vector3<Type> operator-(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
  }
  constexpr inline Vector3<Type> operator*(const Type &s) const {
    return Vector3<Type>(m_x * s, m_y * s, m_z * s);
  }
  constexpr inline Vector3<Type> operator*(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
  }
  constexpr inline Vector3<Type> operator/(const Type &s) const {
    return Vector3<Type>(m_x / s, m_y / s, m_z / s);
  }
  constexpr inline Vector3<Type> operator/(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
  }
  /*******************************************
  Modifying Math Operators
   *******************************************/
  constexpr inline Vector3<Type> &operator+=(const Vector3<Type> &v) {
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator-=(const Vector3<Type> &v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_y -= v.m_z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator*=(const Type &s) {
    m_x *= s;
    m_y *= s;
    m_z *= s;
    return *this;
  }
  constexpr inline Vector3<Type> &operator*=(const Vector3<Type> &v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator/=(const Vector3<Type> &v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    m_z /= v.m_z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator/=(const Type &s) {
    m_x /= s;
    m_y /= s;
    m_z /= s;
    return *this;
  }
  /*******************************************
  Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  constexpr inline operator const Type *() const {
    return static_cast<Type *>(&m_x);
  }
  constexpr inline bool operator!() const { return !m_x && !m_y && !m_z; }
  constexpr inline operator bool() const { return !!*this; }
  constexpr inline operator Type *() { return static_cast<Type *>(&m_x); }
  /********************************************
  Useful Vector Operations
   ********************************************/
  constexpr inline Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(lengthSq());
  }
  constexpr inline Type lengthSq() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
  }
  constexpr inline Vector3<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Vector3<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Type dot(const Vector3<Type> &v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
  }
  constexpr inline Vector3<Type> cross_with_this(
      const Vector3<Type> &v) { /* NOTE this function modifies the
mjz_vector unlike 2D and non-member versions */
    Type x_(m_x), y_(m_y), z_(m_z);
    m_x = y_ * v.m_z - z_ * v.m_y;
    m_y = z_ * v.m_x - x_ * v.m_z;
    m_z = x_ * v.m_y - y_ * v.m_x;
    return *this;
  }
  friend std::ostream &operator<<(std::ostream &outs, const Vector3<Type> &v) {
    outs << "<" << v.m_x << ", " << v.m_y << ", " << v.m_z << ">";
    return outs;
  }
  friend std::istream &operator>>(std::istream &ins, Vector3<Type> &v) {
    ins >> v.m_x >> v.m_y >> v.m_z;
    return ins;
  }
  constexpr inline friend Vector3<Type> operator*(Type s,
                                                  const Vector3<Type> &v) {
    return Vector3<Type>(s * v.m_x, s * v.m_y, s * v.m_z);
  }
  /********************************************************
  Basic Trig functions of angle between vectors
   ********************************************************/
  constexpr inline friend Type cos(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return dot(v1, v2) / v1.length() / v2.length();
  }
  constexpr inline friend Type sin(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return cross(v1, v2).length() / v1.length() / v2.length();
  }
  constexpr inline friend Type tan(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return sin(v1, v2) / cos(v1, v2);
  }
  constexpr inline friend Type angle(const Vector3<Type> &v1,
                                     const Vector3<Type> &v2) {
    return std::acos(cos(v1, v2));
  }
  constexpr inline friend Type dot(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return v1.dot(v2);
  }
  constexpr inline friend Vector3<Type> cross(const Vector3<Type> &v1,
                                              const Vector3<Type> &v2) {
    return Vector3<Type>(v1.m_y * v2.m_z - v1.m_z * v2.m_y,
                         v1.m_z * v2.m_x - v1.m_x * v2.m_z,
                         v1.m_x * v2.m_y - v1.m_y * v2.m_x);
  }
};
/******************************************************************************************
Author: Charlie Murphy
 Email: tm507211@ohio.edu

Date: Augost 7, 2015

Description: Defines point class.
******************************************************************************************/

template <class Type>
class Point3D {
 public:
  Type m_x;
  Type m_y;
  Type m_z;
  Type m_w;
  constexpr inline Point3D(const Type &s = Type())
      : m_x(s), m_y(s), m_z(s), m_w(s) {}
  constexpr inline Point3D(const Type &x, const Type &y, const Type &z,
                           const Type &w)
      : m_x(x), m_y(y), m_z(z), m_w(w) {}
  constexpr inline Point3D(const Point3D<Type> &p)
      : m_x(p.m_x), m_y(p.m_y), m_z(p.m_z), m_w(p.m_w) {}
  constexpr inline Point3D(const Point3D<Type> &p, const Vector3<Type> &v)
      : m_x(p.m_x + v.m_x),
        m_y(p.m_y + v.m_y),
        m_z(p.m_z + v.m_z),
        m_w(p.m_w) {}
  constexpr inline Point3D(Point3D &p) : Point3D((const Point3D<Type> &)p){};
  constexpr inline Point3D(Point3D &&p) : Point3D(p){};
  constexpr inline Point3D &operator=(const Point3D &p) {
    m_x = (p.m_x);
    m_y = (p.m_y);
    m_z = (p.m_z);
    m_w = (p.m_w);
    return *this;
  }
  constexpr inline Point3D &operator=(Point3D &p) {
    return operator=((const Point3D<Type> &)p);
  }
  constexpr inline Point3D &operator=(Point3D &&p) { return operator=(p); }
};
//
// mjz_vector.h
// Vector
//
// Created by Emilis Baliukonis on 05/05/2019.
// Copyright   2019 Emilis Baliukonis. All rights reserved.
//

namespace have_mjz_ard_removed {
using mjz_Str = mjz_Str;
using mjz_str = mjz_Str;

using mjz_estr = extended_mjz_Str;
using mjz_eStr = extended_mjz_Str;
using malloc_wrpr = malloc_wrapper;
using algorithm = static_str_algo;
template <typename T, size_t size>
using ex_Array = extended_mjz_Array<T, size>;
template <typename T, size_t size>
using ex_array = extended_mjz_Array<T, size>;
template <typename T, size_t size>
using Array = mjz_Array<T, size>;
template <typename T>
using Vector = mjz_Vector<T>;
template <typename T, size_t size>
using array = mjz_Array<T, size>;

template <typename T, class C_free_realloc>
using mjz_C_allocator_warpper =
    C_mjz_temp_type_allocator_warpper_t<T, mjz_obj_manager_template_t<T>,
                                        C_free_realloc>;
template <class C_free_realloc>
using C_char_allocator_warpper = mjz_C_allocator_warpper<char, C_free_realloc>;
template <typename T, size_t max_size>
using static_vector = mjz_static_vector_template_t<T, max_size>;

template <typename T>
using iterator = iterator_template_t<T>;
template <typename T>
using iterator_template = iterator_template_t<T>;
template <typename T>
using filter_iterator =
    iterator_template_filter_warper<T, iterator_template_t<T>>;
template <typename T>
using filter_iterator_template =
    iterator_template_filter_warper<T, iterator_template_t<T>>;
template <typename it_T>
using iterator_warper = iterator_warper_template_t<it_T>;
template <typename it_T>
using iterator_wrpr = iterator_warper<it_T>;
template <typename T, class it_T>
using mjz_to_it_warper_t = mjz_to_iterator_template_warper_t<T, it_T>;
template <typename T, class it_T>
using mjz_to_it_wrpr_t = mjz_to_iterator_template_warper_t<T, it_T>;
template <class it_T>
using mjz_to_it_warper =
    mjz_to_iterator_template_warper_t<mjz_get_value_Type<it_T>, it_T>;
template <class it_T>
using mjz_to_it_wrpr =
    mjz_to_iterator_template_warper_t<mjz_get_value_Type<it_T>, it_T>;

using arena_allocator = arena_allocator;
template <typename T>
using vector = mjz_Vector<T>;
template <typename T>
template <typename T, bool init = true>
using obj_warper = mjz_stack_obj_warper_template_t<T, init>;
template <typename T, bool init = true>
using mjz_stack_obj_warper = mjz_stack_obj_warper_template_t<T, init>;
template <typename T, bool init = true>
using hobj_warper = mjz_heap_obj_warper_template_t<T, init>;

template <typename T>
using optional = mjz_stack_obj_warper_template_t<T, false>;
template <typename T>
using mjz_optional = mjz_stack_obj_warper_template_t<T, false>;

template <typename T>
using H_optional = mjz_heap_obj_warper_template_t<T, false>;
template <typename T>
using mjz_H_optional = mjz_heap_obj_warper_template_t<T, false>;

template <typename T>
using H_optional_ptr =
    typename mjz_heap_obj_warper_template_t<T, false>::container_Type_ptr;
template <typename T>
using H_optional_base =
    typename mjz_heap_obj_warper_template_t<T, false>::container_Type;

template <typename T, bool init = true>
using mjz_heap_obj_warper = mjz_heap_obj_warper_template_t<T, init>;
template <typename T>
using allocator = mjz_allocator_warpper<T>;
template <typename T>
using allocator_warpper = mjz_allocator_warpper<T>;
template <typename T>
using mjz_allocator = mjz_allocator_warpper<T>;
template <typename T>
using mjz_allocator_warpper = mjz_allocator_warpper<T>;
using mlc_wrp = malloc_wrapper;
using mjz_StringSumHelper = StringSumHelper_t<mjz_normal_allocator>;
using StringSumHelper = StringSumHelper_t<mjz_normal_allocator>;
using mjz_str_view = mjz_str_view;
using mstrview = mjz_str_view;
using mstrv = mjz_str_view;
using mjz_Vectorf3 = Vector3<float>;
using mjz_Vectorf2 = Vector2<float>;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard
namespace std {
template <>
struct hash<mjz_ard::basic_mjz_Str_view> {
  constexpr size_t operator()(const mjz_ard::basic_mjz_Str_view &k) const {
    return mjz_ard::SHA1_CTX::hash_c_str(k.data(), k.length());
  }
};  // namespace std::hash

template <typename T>
struct hash<mjz_ard::mjz_str_t<T>> {
  size_t operator()(const mjz_ard::mjz_str_t<T> &k) const {
    return mjz_ard::SHA1_CTX::hash_c_str(k.c_str(), k.length());
  }
};  // namespace std::hash
template <>
struct hash<mjz_ard::mjz_str_view> {
  size_t operator()(const mjz_ard::mjz_str_view &k) const {
    return mjz_ard::SHA1_CTX::hash_c_str(k.data(), k.length());
  }
};  // namespace std::hash
template <>
struct hash<mjz_ard::mjz_virtual_string_view> {
  size_t operator()(const mjz_ard::mjz_virtual_string_view &k) const {
    return mjz_ard::SHA1_CTX::hash_c_str(k.data(), k.length());
  }
};  // namespace std::hash

template <typename Type>
struct hash<mjz_ard::Vector3<Type>> {
  size_t operator()(const mjz_ard::Vector3<Type> &k) const {
    return hash<mjz_ard::mjz_str_view>()(mjz_ard::mjz_str_view(
        (const char *)&k, sizeof(mjz_ard::Vector3<Type>)));
  }
};
template <typename Type>
struct hash<mjz_ard::Vector2<Type>> {
  size_t operator()(const mjz_ard::Vector2<Type> &k) const {
    return hash<mjz_ard::mjz_str_view>()(mjz_ard::mjz_str_view(
        (const char *)&k, sizeof(mjz_ard::Vector2<Type>)));
  }
};
template <typename Type>
inline mjz_ard::Vector3<Type> sqrt(mjz_ard::Vector3<Type> v) {
  return mjz_ard::Vector3<Type>(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
}

template <typename Type>
struct hash<mjz_ard::iterator_template_t<Type>> {
  size_t operator()(const mjz_ard::iterator_template_t<Type> &k) const {
    return hash()(k.get_pointer());
  }
};

template <typename... Types>
struct hash<mjz_ard::iterator_template_filter_warper<Types...>> {
  size_t operator()(
      const mjz_ard::iterator_template_filter_warper<Types...> &k) const {
    return hash()(k.get_pointer());
  }
};
template <typename... Types>
struct hash<mjz_ard::iterator_warper_template_t<Types...>> {
  size_t operator()(
      const mjz_ard::iterator_warper_template_t<Types...> &k) const {
    return hash()(mjz_ard::Vector2<size_t>(hash()(k.m_begin), hash()(k.m_end)));
  }
};
template <typename... Types>
struct hash<mjz_ard::mjz_to_iterator_template_warper_t<Types...>> {
  size_t operator()(
      const mjz_ard::mjz_to_iterator_template_warper_t<Types...> &k) const {
    return hash()(k.get_pointer());
  }
};

template <size_t number_of_blocks, size_t block_length,
          bool KEEP_the_heap_clean>
struct hash<mjz_ard::mjz_arena_allocator_t<number_of_blocks, block_length,
                                           KEEP_the_heap_clean>> {
  size_t operator()(
      const mjz_ard::mjz_arena_allocator_t<number_of_blocks, block_length,
                                           KEEP_the_heap_clean> &k) const {
    return hash()(&k);
  }
};
template <typename T, bool COOC_t, class MOC_t, bool DEC_t>
struct hash<mjz_ard::mjz_stack_obj_warper_template_t<T, COOC_t, MOC_t, DEC_t>> {
  inline size_t operator()(
      const mjz_ard::mjz_stack_obj_warper_template_t<T, COOC_t, MOC_t, DEC_t>
          &k) const {
    return hash<T>()(*k);
  }
};
template <typename... Types>
struct hash<mjz_ard::mjz_heap_obj_warper_template_t<Types...>> {
  size_t operator()(
      const mjz_ard::mjz_heap_obj_warper_template_t<Types...> &k) const {
    return hash()(*k);
  }
};
template <typename... Types>
struct hash<mjz_ard::mjz_temp_malloc_wrapper_t<Types...>> {
  size_t operator()(
      const mjz_ard::mjz_temp_malloc_wrapper_t<Types...> &k) const {
    return hash()(k.get_ptr());
  }
};
template <>
struct hash<mjz_ard::hash_sha256> {
  size_t operator()(const mjz_ard::hash_sha256 &k) const {
    return *((size_t *)k.data);
  }
};
}  // namespace std

namespace mjz_ard {

template <typename T>
[[nodiscard]] void *mjz_ard::mjz_str_t<T>::operator new(size_t m_size) {
  void *p = T().allocate(sizeof(size_t) + m_size);
  *((size_t *)p) = sizeof(size_t) + m_size;
  return ((size_t *)p) + 1;
}
template <typename T>
[[nodiscard]] void *mjz_ard::mjz_str_t<T>::operator new[](size_t m_size) {
  void *p = T().allocate(sizeof(size_t) + m_size);
  *((size_t *)p) = sizeof(size_t) + m_size;
  return ((size_t *)p) + 1;
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete(void *p) {
  size_t *real = (((size_t *)p) - 1);
  size_t m_size = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, m_size);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete[](void *p) {
  size_t *real = (((size_t *)p) - 1);
  size_t m_size = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, m_size);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete(void *p, size_t) {
  size_t *real = (((size_t *)p) - 1);
  size_t m_size = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, m_size);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete[](void *p, size_t) {
  size_t *real = (((size_t *)p) - 1);
  size_t m_size = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, m_size);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::assign_range(
    std::initializer_list<const char> list) {
  (*this)();
  size_t newlen = list.size();
  addto_length(newlen);
  char *ptr_{m_buffer};
  for (const char cr : list) {
    *ptr_++ = cr;
  }
  return *this;
}

template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::assign_range(
    iterator_template_t<const char> list) {
  (*this)();
  size_t newlen = list.size();
  addto_length(newlen);
  char *ptr_{m_buffer};
  for (const char cr : list) {
    *ptr_++ = cr;
  }
  return *this;
}

template <typename T>
mjz_ard::mjz_str_t<T>::~mjz_str_t(void) {
  invalidate(
      1);  // TODO: V1053 https://pvs-studio.com/en/docs/warnings/V1053/ Calling
           // the 'free' virtual function indirectly in the destructor may lead
           // to unexpected result at runtime. Check lines: 'mjzString.cpp:462',
  // 'mjzString.cpp:547', 'mjzString.cpp:646', 'mjzString.cpp:640',
  // 'mjzString.hpp:734'.
}  // i don't need to do this but this is explained in stackoverfllow . the
// vtable of the derived free override gets destroyed when
// ~mjz_ard::mjz_str_t<T>() gets called so mjz_ard::mjz_str_t<T>::free should be
// called and i do it explicitly its not necessary but i do it see
// https://stackoverflow.com/questions/41732051/when-is-a-vtable-destroy-in-c
/*
Can I call a virtual function in the destructor of a base class?
Calling virtual functions from destructors or constructors is a bad practice.
See the standard (emphasis mine):
12.7 Construction and destruction
 ....
Member functions, including virtual functions (10.3),
can be called during construction or destruction (12.6.2).
When a virtual function is called directly or indirectly from a constructor or
 from a destructor, including during the construction or destruction of the
 class’s non-static data members, and the object to which the call applies is
 the object (call it x) under construction or destruction, the function called
 is the final overrider in the constructor’s or destructor’s class and not
 one overriding it in a more-derived class. If the virtual function call
 uses an explicit class member access (5.2.5) and the object expression
 refers to the complete object of x or one of that object’s base class
 sub objects but not x or one of its base class sub objects, the behavior
 is undefined.
You can find this recommendation in many sources, including Scott Meyers'
Effective C++: 55 Specific Ways to Improve Your Programs and Designs (Item 9:
Never call virtual functions during construction or destruction.)
or Herb Sutter's
 C++ Coding Standards: 101 Rules, Guidelines, and Best Practices (49. Avoid
 calling virtual functions in constructors and destructors).
Yes, the object will have a pointer to its vtable when you call the
 destructor. The standard explicitly says it is possible to call virtual
 functions in the destructor, and says what happens. There is agreement that
 even though it is allowed it is a bad practice, because it is inherently
 brittle code that leads to surprises with apparently innocent changes. If you
 have that DerDer inherits from Der, which inherits from Base, all of them
 override member function void member(), and you are in the destructor of Der,
and call member(), you are calling Der::member(), not DerDer::member(),
because the DerDer part of your object is GONE, DESTROYED already. A base
 class can refer inadvertently to data in a derived class, for example: struct
 Base { int *ip; Base(int *ip): ip(ip) { } virtual void useInt() {
std::cout << *ip << std::endl; } ~Base() { useInt(); } } ; struct Der:
Base { int theInt; Der(): Base(&theInt) { } void useIntPointer() override {
std::cout << theInt << std::endl; } } ; When an object of type Der gets
 deleted, there is "undefined behavior": First the implicit destructor of Der is
 called, then the explicit destructor of Base, Base::~Base. At that point
 Base::ip is referring to a member of Der that has already been destroyed. Share
 Edit Follow answered Jan 19, 2017 at 6:48 TheCppZoo's user avatar TheCppZoo
 1,21977 silver badges1212 bronze badges
 Your answer could be quite nice if you worked it over a little bit. First, in
 your text you're referring to stuff which just isn't there in the source code
 (like void member(), class DerDer, etc.). Second, the thing you point out
 would be problematic even without useInt() being virtual, but just because
 you let ip point to a member of Der, i.e. Der::theInt. So, it doesn't
 illustrate the point. Why don't you just override useInt in Der and access
 theInt from there, or even better access some heap allocated resource
 which has already been deleted? I think would be much nicer. – Elmar
 Zander Apr 5, 2022 at 12:57
*/
/*********************************************/
/* Memory Management */
/*********************************************/
template <typename T>
void mjz_ard::mjz_str_t<T>::init(bool) {
  m_buffer = NULL;
  // update_event_F_p = &mjz_ard::mjz_str_t<T>::update_event;//departed
  ;
  m_capacity = 0;
  stack_obj_buf.set(0);
  m_length = 0;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::invalidate(bool constructor) {
  if (!m_buffer) {
    // goto _end__;
    m_capacity = m_length = 0;
    return;
  }
  free_pv(m_buffer, constructor);
  stack_obj_buf.set(0);
  m_buffer = NULL;
  //_end__:
  m_capacity = m_length = 0;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::reserve(size_t m_size, bool just_size,
                                    bool constructor) {
  int64_t different_of_size_and_cap = (int64_t)m_size - (int64_t)m_capacity;
  if (!(just_size || different_of_size_and_cap < 0)) {
    if (m_size < stack_buffer_size) {
      m_size = stack_buffer_size;
    } else {
      int64_t minimumcapadd = min_macro_(m_capacity / 5, stack_buffer_size);
      m_size +=
          (size_t)(static_cast<int64_t>(5) * (different_of_size_and_cap < 5) +
                   minimumcapadd * (different_of_size_and_cap < minimumcapadd));
    }
  }
  if (m_buffer && m_capacity >= m_size) {
    return 1;
  }
  if (changeBuffer(m_size, constructor)) {
    if (m_length == 0) {
      m_buffer[0] = 0;
    }
    return true;
  }
  return false;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::addto_length(size_t addition_tolen,
                                         bool just_size) {
  bool ret = 1;
  if ((addition_tolen + m_length) > m_capacity) {
    ret = reserve(addition_tolen + m_length + 1, just_size);
  }
  if (ret) {
    m_length += addition_tolen;
    m_buffer[m_length] = 0;
  }
  return ret;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::realloc_helper_is_in_stack(void *ptr) {
  return (stack_obj_buf.get() || stack_obj_buf.stack_buffer == (char *)ptr);
}
template <typename T>
[[nodiscard]] void *mjz_ard::mjz_str_t<T>::realloc_pv(void *ptr,
                                                      size_t new_size,
                                                      bool constructor) {
  bool ptr_is_in_stack = realloc_helper_is_in_stack(ptr);
  bool ptr_is_buffer = !(ptr_is_in_stack || (0 == ptr));  //(buffer == ptr);
  bool ptr_Can_set_to_stack = (new_size <= (stack_buffer_size + 1));
  if (ptr_Can_set_to_stack) {
    if (ptr_is_buffer) {
      size_t the__length = m_capacity;
      if (the__length)
        memmove(stack_obj_buf.stack_buffer, ptr,
                min_macro_(the__length, stack_buffer_size));
      free_pv(ptr,
              constructor);  // because of my custom free 0 and stack_buffer are
      // ignored and i reset STR_is_in_stack in next line
    }
    stack_obj_buf.set(1);
    return (void *)stack_obj_buf.stack_buffer;
  }
  if (ptr_is_in_stack) {
    ptr = (constructor ? mjz_ard::mjz_str_t<T>::realloc(0, new_size)
                       : this->realloc(0, new_size));
    if (!ptr) {
      stack_obj_buf.set(0);
      return 0;
    }
    size_t the__length = m_capacity;
    if (the__length) {
      memmove(ptr, stack_obj_buf.stack_buffer,
              min_macro_(the__length, stack_buffer_size));
    }
    stack_obj_buf.set(0);
    return ptr;
  }
  return (constructor ? mjz_ard::mjz_str_t<T>::realloc(ptr, new_size)
                      : this->realloc(ptr, new_size));
}
template <typename T>
void mjz_ard::mjz_str_t<T>::free_pv(void *&ptr, bool constructor) {
  if (stack_obj_buf.get() || stack_obj_buf.stack_buffer == ptr) {
    ptr = 0;
    stack_obj_buf.set(0);
    return;
  }
  if (ptr) {
    if (constructor) {
      mjz_ard::mjz_str_t<T>::free(ptr);
    } else {
      this->free(ptr);
    }
  }
  ptr = 0;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::free_pv(void *const &ptr,
                                    bool constructor) {  // TODO: V835
  // https://pvs-studio.com/en/docs/warnings/V835/
  // Passing cheap-to-copy argument by reference may lead
  // to decreased performance. To avoid this, replace the
  // first argument 'void*const & ptr' with 'void*const
  // ptr'. //TODO: V659
  // https://pvs-studio.com/en/docs/warnings/V659/
  // Declarations of functions with 'mjz_ard::mjz_str_t<T>::free_pv'
  // name differ in the 'const' keyword only, but the
  // bodies of these functions have different
  // composition. This is suspicious and can possibly be
  // an error. Check lines: 631, 645.
  void *ptr_ = ptr;
  free_pv(ptr_, constructor);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::changeBuffer(size_t maxStrLen, bool constructor) {
  char *newbuffer{};
  newbuffer = (char *)realloc_pv(m_buffer, maxStrLen + 1, constructor);
  if (newbuffer) {
    m_buffer = newbuffer;
    m_capacity = maxStrLen;
    return true;
  }
  return false;
}
/*********************************************/
/* Copy and Move */
/*********************************************/

template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::copy(const char *cstr,
                                                   size_t length, bool) {
  if (!reserve(length, 0, 1)) {
    invalidate();
    return *this;
  }
  m_length = length;
  memmove(m_buffer, cstr, length);
  m_buffer[m_length] = '\0';
  return *this;
}
#define PGM_P const char *

template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::copy(
    const __FlashStringHelper *pstr, size_t length) {
  if (!reserve(length)) {
    invalidate();
    return *this;
  }
  m_length = length;
  memmove(m_buffer, (PGM_P)pstr, length);
  return *this;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::move(mjz_ard::mjz_str_t<T> &rhs) {
  if (this != &rhs) {
    free_pv(m_buffer, 0);
    stack_obj_buf = rhs.stack_obj_buf;
    m_buffer =
        (rhs.stack_obj_buf.get() ? stack_obj_buf.stack_buffer : rhs.m_buffer);
    if (rhs.stack_obj_buf.get()) {
      rhs.free_pv(rhs.m_buffer, 0);
    }
    m_length = rhs.m_length;
    m_capacity = rhs.m_capacity;
    rhs.stack_obj_buf.set(0);
    rhs.m_buffer = NULL;
    rhs.m_length = 0;
    rhs.m_capacity = 0;
  }
}

template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator=(
    const mjz_ard::mjz_str_t<T> &rhs) {
  if (this == &rhs) {
    return *this;
  }
  if (rhs.m_buffer) {
    copy(rhs.m_buffer, rhs.m_length);
  } else {
    invalidate();
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator=(
    mjz_ard::mjz_str_t<T> &&rval) noexcept {
  move(rval);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator=(const char *cstr) {
  operator()(cstr);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator()(const char *other,
                                                         size_t size_len) {
  if (other) {
    copy(other, size_len);
  } else {
    invalidate();
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator=(
    const __FlashStringHelper *pstr) {
  if (pstr) {
    copy(pstr, (size_t)strlen((PGM_P)pstr));
  } else {
    invalidate();
  }
  return *this;
}
/*********************************************/
/* concat */
/*********************************************/ template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(const mjz_ard::mjz_str_t<T> &s) {
  return concat(s.m_buffer, s.m_length);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::append(
    const mjz_ard::mjz_str_t<T> &str, size_t subpos, size_t sublen) {
  concat(str.substr_view_beg_n(subpos, sublen));
  return *this;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(const char *cstr, size_t length) {
  size_t newlen = m_length + length;
  if (!cstr) {
    return false;
  }
  if (length == 0) {
    return true;
  }
  if (!reserve(newlen)) {
    return false;
  }
  memmove(m_buffer + m_length, cstr, length);
  m_length = newlen;
  m_buffer[m_length] = '\0';
  return true;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(const char *cstr) {
  if (!cstr) {
    return false;
  }
  return concat(cstr, (size_t)strlen(cstr));
}
template <typename T>
inline bool mjz_ard::mjz_str_t<T>::concat(char c) {
  return concat(&c, 1);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(unsigned char num) {
  char buf[1 + 3 * sizeof(unsigned char)];
  itoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(int num) {
  char buf[2 + 3 * sizeof(int)];
  itoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(unsigned int num) {
  char buf[1 + 3 * sizeof(unsigned int)];
  utoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(long num) {
  char buf[2 + 3 * sizeof(long)];
  ltoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(unsigned long num) {
  char buf[1 + 3 * sizeof(unsigned long)];
  ultoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(long long num) {
  char buf[2 + 3 * sizeof(long long)];
  lltoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(unsigned long long num) {
  char buf[1 + 3 * sizeof(unsigned long long)];
  ulltoa(num, buf, 10);
  return concat(buf);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(float num) {
  char buf[20];
  auto ret = dtostrf(num, 4, 2, buf, 20);
  return concat(ret.buffer, ret.len);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(double num) {
  char buf[23];
  auto ret = dtostrf(num, 4, 4, buf, 23);
  return concat(ret.buffer, ret.len);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(const __FlashStringHelper *str) {
  if (!str) {
    return false;
  }
  size_t length = (size_t)strlen((const char *)str);
  if (length == 0) {
    return true;
  }
  size_t newlen = m_length + length;
  if (!reserve(newlen)) {
    return false;
  }
  memmove(m_buffer + m_length, (const char *)str, newlen);
  m_length = newlen;
  return true;
}
/*********************************************/
/* Concatenate */
/*********************************************/
template <typename T>

mjz_ard::mjz_str_t<T> &&operator_plus(mjz_ard::mjz_str_t<T> &&lhs,
                                      const mjz_ard::basic_mjz_Str_view &rhs) {
  if (!lhs.concat(rhs.data(), rhs.length())) {
    lhs.invalidate();
  }
  return std::move(lhs);
}

template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    const mjz_ard::basic_mjz_Str_view &rhs,
    const mjz_ard::basic_mjz_Str_view &lhs) {
  return operator_plus(mjz_ard::mjz_Str(rhs), lhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    mjz_ard::mjz_str_t<T> &&lhs, const mjz_ard::basic_mjz_Str_view &rhs) {
  return operator_plus((mjz_ard::mjz_str_t<T>)std::move(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        char const *cstr) {
  return operator_plus((mjz_ard::mjz_str_t<T>)lhs, mjz_ard::mjz_str_view(cstr));
}

template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(const mjz_ard::basic_mjz_String &lhs,
                                        const mjz_ard::basic_mjz_String &rhs) {
  return operator_plus(mjz_ard::mjz_str_t<T>(lhs),
                       (const mjz_ard::basic_mjz_String &)rhs);
}

template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    mjz_ard::mjz_str_t<T> lhs, const mjz_ard::StringSumHelper_t<T> &rhs) {
  return operator_plus((mjz_ard::mjz_str_t<T>)std::move(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}

template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    const mjz_ard::basic_mjz_Str_view &lhs,
    const mjz_ard::StringSumHelper_t<T> &rhs) {
  return operator_plus(mjz_ard::mjz_str_t<T>(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    mjz_ard::StringSumHelper_t<T> lhs, const mjz_ard::basic_mjz_Str_view &rhs) {
  return operator_plus(mjz_ard::mjz_str_t<T>(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    const mjz_ard::mjz_str_view &lhs, const mjz_ard::basic_mjz_Str_view &rhs) {
  return operator_plus(mjz_ard::mjz_str_t<T>(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(
    const mjz_ard::mjz_str_view &lhs,
    const mjz_ard::StringSumHelper_t<T> &rhs) {
  return operator_plus(mjz_ard::mjz_str_t<T>(lhs),
                       (const mjz_ard::basic_mjz_Str_view &)rhs);
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        char c) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(c)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        unsigned char num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        int num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        unsigned int num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        long num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        unsigned long num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        long long num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        unsigned long long num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        float num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        double num) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
template <typename T>
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::StringSumHelper_t<T> &&lhs,
                                        const __FlashStringHelper *rhs) {
  mjz_ard::StringSumHelper_t<T> &a =
      const_cast<mjz_ard::StringSumHelper_t<T> &>(lhs);
  if (!a.concat(rhs)) {
    a.invalidate();
  }
  return a;
}
/*********************************************/
/* Comparison */
/*********************************************/
/*********************************************/
/* Character Access */
/*********************************************/

template <typename T>
void mjz_ard::mjz_str_t<T>::setCharAt(size_t loc, char c) {
  if (loc < m_length) {
    m_buffer[loc] = c;
  }
}

template <typename T>
void mjz_ard::mjz_str_t<T>::setCharAt(int64_t loc, char c) {
  loc = signed_index_to_unsigned(loc);
  if ((size_t)loc < m_length) {
    m_buffer[loc] = c;
  }
}
template <typename T>
char &mjz_ard::mjz_str_t<T>::operator[](size_t index) & {
  static char dummy_writable_char;
  if (index >= m_length || !m_buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return m_buffer[index];
}
template <typename T>
char &mjz_ard::mjz_str_t<T>::operator[](int64_t index_) & {
  size_t index = signed_index_to_unsigned(index_);
  static char dummy_writable_char;
  if (index >= m_length || !m_buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return m_buffer[index];
}
/*********************************************/
/* Modification */
/*********************************************/
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::insert(
    size_t pos, const mjz_ard::basic_mjz_Str_view &other) {
  return insert(pos, other, 0, length());
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::insert(
    size_t pos, const mjz_ard::basic_mjz_Str_view &other, size_t subpos,
    size_t sublen) {
  return insert(pos, other.c_str() + subpos, sublen);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::insert(size_t pos, const char *s,
                                                     size_t n) {
  mjz_ard::mjz_str_t<T> buffer_str_ = substring(0, pos);
  buffer_str_.write(s, n);
  buffer_str_ += substr_view(pos, length());
  return (*this = std::move(buffer_str_));
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::insert(size_t pos, size_t n,
                                                     char c) {
  if (length() < pos) {
    size_t NumberOf_not_acounted_elems = pos - length();
    addto_length(n + NumberOf_not_acounted_elems);
    memset(m_buffer + pos - NumberOf_not_acounted_elems, c,
           NumberOf_not_acounted_elems + n);
    return *this;
  }
  if (length() == pos) {
    addto_length(n);
    memset(m_buffer + pos, c, n);
    return *this;
  }
  addto_length(n);
  memmove(m_buffer + n + pos, m_buffer + pos, length() - pos);
  memset(m_buffer + pos, c, n);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace_ll(
    int64_t pos, size_t len, const mjz_ard::basic_mjz_Str_view &str) {
  return replace(signed_index_to_unsigned(pos), len, str);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(size_t pos, size_t len,
                                                      const char *s, size_t n) {
  return replace(pos, len, mjz_ard::mjz_str_view(s, n));
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(size_t pos, size_t len,
                                                      const char *s) {
  return replace(pos, len, s, strlen(s));
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(
    size_t pos, size_t len, const mjz_ard::basic_mjz_Str_view &str,
    size_t subpos, size_t sublen) {
  return replace(pos, len,
                 mjz_ard::mjz_str_view(str.c_str(), str.length())
                     .substr_view_beg_n(subpos, sublen));
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(size_t pos, size_t len,
                                                      size_t n, char c) {
  if (length() <= pos) return *this;
  if (len == n) {
    memset(m_buffer + pos, c, len);
    return *this;
  }
  if (n < len) {
    memset(m_buffer + pos, c, n);
    remove(pos + n, len - n);
  }
  if (n > len) {
    memset(m_buffer + pos, c, len);
    insert(pos + len, n - len, c);
    memset(m_buffer + pos + len, c, n - len);
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(
    mjz_ard::mjz_str_t<T>::iterator i1, mjz_ard::mjz_str_t<T>::iterator i2,
    const char *s, size_t n) {
  return replace(i1, i2, mjz_ard::mjz_str_view(s, n));
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(
    mjz_ard::mjz_str_t<T>::iterator i1, mjz_ard::mjz_str_t<T>::iterator i2,
    const mjz_ard::basic_mjz_Str_view &str) {
  if (i1.end() != end() || i2.end() != end()) return *this;
  return replace(i1.get_pointer() - m_buffer, i2 - i1, str);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(
    mjz_ard::mjz_str_t<T>::iterator i1, mjz_ard::mjz_str_t<T>::iterator i2,
    size_t n, char c) {
  if (i1.end() != end() || i2.end() != end()) return *this;
  return replace(i1.get_pointer() - m_buffer, i2 - i1, n, c);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::replace(
    size_t pos, size_t len, const mjz_ard::basic_mjz_Str_view &str) {
  if (length() <= pos) return *this;
  if (len == str.length()) {
    memmove(m_buffer + pos, str.c_str(), len);
    return *this;
  }
  if (str.length() < len) {
    memmove(m_buffer + pos, str.c_str(), str.length());
    remove(pos + str.length(), len - str.length());
  }
  if (str.length() > len) {
    size_t new_cap = m_length + str.length() - len;
    if (!reserve(new_cap + 1)) {
      return *this;
    }
    size_t length_left = str.length();
    const char *data_left = str.c_str();
    char *m_buf_ptr = m_buffer + pos;
    memmove(m_buf_ptr, data_left, len);
    length_left -= len;
    data_left += len;
    m_buf_ptr += len;
    memmove(m_buf_ptr + length_left, m_buf_ptr,
            m_buffer + new_cap - m_buf_ptr - length_left);
    memmove(m_buf_ptr, data_left, length_left);
    m_length = new_cap;
    m_buffer[new_cap] = '\0';
  }
  return *this;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::find_and_replace(char find, char replace_) {
  if (!m_buffer) {
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    if (*p == find) {
      *p = replace_;
    }
  }
}
// namespace mjz_ard
template <typename T>
void mjz_ard::mjz_str_t<T>::find_and_replace(
    const basic_mjz_Str_view &find, const basic_mjz_Str_view &replace_) {
  find_and_replace(find.buffer_ref(), find.length(), replace_.buffer_ref(),
                   replace_.length());
}
template <typename T>
void mjz_ard::mjz_str_t<T>::remove(size_t index) {
  // Pass the biggest integer as the count. The remove method
  // below will take care of truncating it at the end of the
  // string.
  remove(index, (size_t)-1);
}
template <typename T>
void mjz_ard::mjz_str_t<T>::remove(size_t index, size_t count) {
  if (index >= m_length) {
    return;
  }
  if (count <= 0) {
    return;
  }
  if (count > m_length - index) {
    count = m_length - index;
  }
  char *writeTo = m_buffer + index;
  m_length = m_length - count;
  memmove(writeTo, m_buffer + index + count, m_length - index);
  m_buffer[m_length] = 0;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::erase(size_t pos_, size_t len_) {
  if (len_ <= pos_ || length() < (len_ + pos_)) {
    return *this;
  }
  remove((size_t)pos_, (size_t)min(len_, length() - pos_));
  return *this;
}
template <typename T>
iterator_template_t<char> mjz_ard::mjz_str_t<T>::erase(
    iterator_template_t<char> p) {
  if (p < begin() || end() < p) {
    return begin();
  }
  size_t index_ = p - begin();
  remove(index_, 1);
  return p;
}
template <typename T>
iterator_template_t<char> mjz_ard::mjz_str_t<T>::erase(
    iterator_template_t<char> first, iterator_template_t<char> last) {
  if ((last <= first) || (end() < last) || (first < begin())) {
    return begin();
  }
  size_t index_first = first - begin();
  size_t index_last = last - begin();
  remove(index_first, index_last - index_first);
  return first;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::erase_from_f_to_l(size_t first,
                                                                size_t last) {
  first = signed_index_to_unsigned(first);
  last = signed_index_to_unsigned(last);
  if ((last <= first) || (length() < last)) {
    return *this;
  }
  remove(first, last - first);
  return *this;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::toLowerCase(void) {
  if (!m_buffer) {
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    *p = (char)tolower(*p);
  }
}
template <typename T>
void mjz_ard::mjz_str_t<T>::toUpperCase(void) {
  if (!m_buffer) {
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    *p = (char)toupper(*p);
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(char cr) {
  return concat(cr);
}
template <typename T>
void mjz_ard::mjz_str_t<T>::trim(void) {
  if (!m_buffer || m_length == 0) {
    return;
  }
  char *begin = m_buffer;
  while (isspace(*begin)) {
    begin++;
  }
  char *end = m_buffer + m_length - 1;
  while (isspace(*end) && end >= begin) {
    end--;
  }
  m_length = (size_t)(end + 1 -
                      begin);  // i dont care if you make bad sizing for strings

  if (begin > m_buffer) {
    memmove(m_buffer, begin, m_length);
  }
  m_buffer[m_length] = 0;
}
//////////////////////////////////////////////////////////////
template <typename T>
void *mjz_ard::mjz_str_t<T>::do_this_for_me(function_ptr function_ptr_,
                                            void *x) {
  return function_ptr_(*this, x);
}
/*********************************************/
/* Parsing / Conversion */
/*********************************************/

template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_rc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_r() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_lc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_l() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_r_sc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_r_s() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_l_sc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_shift_op_l_s() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_rc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_r() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_lc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_l() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_r_sc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_r_s() {
  return *this;
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_l_sc() const {
  //
  return (const mjz_ard::extended_mjz_str_t<T> &)*this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::get_s_shift_op_l_s() {
  return *this;
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator>>(
    mjz_ard::extended_mjz_str_t<T> &typing) {
  helper__op_shift_input_(*this, get_s_shift_op_r(), typing.get_shift_op_r());
  return get_s_shift_op_r();
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator>>(
    mjz_ard::extended_mjz_str_t<T> *typing) {
  helper__op_shift_input_(*this, get_s_shift_op_r(), typing->get_shift_op_r());
  return get_s_shift_op_r();
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::operator>>(
        mjz_ard::extended_mjz_str_t<T> &typing) const {
  helper__op_shift_input_(*this, get_s_shift_op_rc(), typing.get_shift_op_r());
  //
  return get_s_shift_op_rc();
}
template <typename T>
const mjz_ard::extended_mjz_str_t<T>
    &mjz_ard::extended_mjz_str_t<T>::operator>>(
        mjz_ard::extended_mjz_str_t<T> *typing) const {
  helper__op_shift_input_(*this, get_s_shift_op_rc(), typing->get_shift_op_r());
  //
  return get_s_shift_op_rc();
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator<<(
    mjz_ard::extended_mjz_str_t<T> &typing) {
  helper__op_shift_input_(*this, typing.get_shift_op_l(), get_s_shift_op_l());
  return get_s_shift_op_l();
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator<<(
    mjz_ard::extended_mjz_str_t<T> *typing) {
  helper__op_shift_input_(*this, typing->get_shift_op_l(), get_s_shift_op_l());
  return get_s_shift_op_l();
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator<<(
    const mjz_ard::extended_mjz_str_t<T> &typing) {
  helper__op_shift_input_(*this, typing.get_shift_op_lc(), get_s_shift_op_l());
  return get_s_shift_op_l();
}
template <typename T>
mjz_ard::extended_mjz_str_t<T> &mjz_ard::extended_mjz_str_t<T>::operator<<(
    mjz_ard::extended_mjz_str_t<T> &&typing) {
  helper__op_shift_input_(*this, typing.get_shift_op_lc(), get_s_shift_op_l());
  return get_s_shift_op_l();
}

template <typename T>
void str_helper__op_shift_input_(mjz_ard::extended_mjz_str_t<T> &rhs,
                                 mjz_ard::extended_mjz_str_t<T> &CIN) {
  helper__op_shift_input_(rhs, CIN, rhs);
}
template <typename T>
mjz_ard::extended_mjz_str_t<T>
mjz_ard::extended_mjz_str_t<T>::string_do_interpret() {
  mjz_ard::extended_mjz_str_t<T> out_str;
  str_helper__op_shift_input_(out_str, *this);
  return out_str;
}
template <typename T>
void mjz_ard::extended_mjz_str_t<T>::string_do_interpret(
    mjz_ard::extended_mjz_str_t<T> &instr) {
  str_helper__op_shift_input_(*this, instr);
}
/**********************************************************************/
// stream stuff
template <typename T>
void mjz_ard::mjz_str_t<T>::adjust_cap() {
  if (!m_capacity && !m_buffer) {
    return;
  }
  if (m_capacity < (m_length + 1)) {
    reserve(m_capacity + 1, 1);
  }
  if (m_buffer) {
    m_buffer[m_length] = '\0';
  }
  return;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(uint8_t c) {
  mjz_ard::mjz_str_t<T>::operator+=(c);
  return 1;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(const uint8_t *buf, size_t m_size) {
  mjz_ard::mjz_str_t<T>::operator+=(mjz_ard::mjz_str_t<T>(buf, (size_t)m_size));
  return m_size;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(const char *buf, size_t m_size) {
  mjz_ard::mjz_str_t<T>::operator+=(mjz_ard::mjz_str_t<T>(buf, (size_t)m_size));
  return m_size;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(const char *buf) {
  mjz_ard::mjz_str_t<T>::operator+=(buf);
  return strlen(buf);
}
template <typename T>
int64_t mjz_ard::mjz_str_t<T>::availableLL() {
  return length();
}
template <typename T>
int mjz_ard::mjz_str_t<T>::read() {
  if (available()) {
    char x = charAt(0ULL);
    remove(0, 1);
    return x;
  }
  return -1;
}
template <typename T>
int mjz_ard::mjz_str_t<T>::peek() {
  int64_t len_ = length();
  if (len_ == 0) {
    return -1;
  } else {
    return operator[](0ULL);
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::read(uint8_t *buf, size_t m_size) {
  if (!available()) {
    return (size_t)-1;
  }
  if (available() < m_size) {
    return (size_t)-1;
  }
  if (!buf) {
    return (size_t)-1;
  }
  memmove(buf, m_buffer, m_size);
  buf[m_size] = 0;
  remove(0, (size_t)m_size);
  return m_size;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::flush() {}
template <typename T>
void mjz_ard::mjz_str_t<T>::begin(unsigned long) {}
template <typename T>
void mjz_ard::mjz_str_t<T>::begin(unsigned long, uint16_t) {}
template <typename T>
bool mjz_ard::extended_mjz_str_t<T>::is_blank() const {
  for (size_t index_i{}; index_i < mjz_str_t<T>::length(); index_i++) {
    if (!drived_mjz_Str_DATA_storage_Obj_ptr->is_blank_character(
            mjz_str_t<T>::c_str()[index_i])) {
      return 0;
    }
  }
  return 1;
}
template <typename T>
int8_t mjz_ard::mjz_str_t<T>::char_to_int_for_string(char c_char) {
  if (c_char < '0' || c_char > '9') {
    return -1;
  }
  return (int8_t)(c_char - '0');
}
template <typename T>
bool mjz_ard::extended_mjz_str_t<T>::change_reinterpret_char_char(char x) {
  if (x == 0) {
    return 0;
  }
  drived_mjz_Str_DATA_storage_Obj_ptr_set()->reinterpret_char_char = x;
  return 1;
}
template <typename T>
char mjz_ard::extended_mjz_str_t<T>::get_reinterpret_char_char() const {
  return drived_mjz_Str_DATA_storage_Obj_ptr->reinterpret_char_char;
}
template <typename T>
bool mjz_ard::extended_mjz_str_t<T>::char_to_char_for_reinterpret(
    char &c_char) const {
  if (!drived_mjz_Str_DATA_storage_Obj_ptr) return 1;
  if (c_char == drived_mjz_Str_DATA_storage_Obj_ptr->reinterpret_char_char) {
    return drived_mjz_Str_DATA_storage_Obj_ptr->reinterpret_char_char;
  }
  char c_char_reinterpreted =
      drived_mjz_Str_DATA_storage_Obj_ptr->char_to_char_for_reinterpret_fnc_ptr(
          c_char);
  if (c_char == c_char_reinterpreted) {
    return 0;
  }
  c_char = c_char_reinterpreted;
  return 1;
}
template <typename T>
bool mjz_ard::extended_mjz_str_t<T>::is_forbiden(char x) const {
  if (!x) {
    return 1;
  }
  return drived_mjz_Str_DATA_storage_Obj_ptr->is_forbiden_character(x);
}

template <typename T>
[[nodiscard]] mjz_ard::mjz_str_t<T>
mjz_ard::mjz_str_t<T>::create_mjz_Str_char_array(size_t m_size, char filler,
                                                 bool do_fill) {
  mjz_ard::mjz_str_t<T> ret_val;
  ret_val.addto_length(m_size, 1);
  char *ret_val_bufer = (char *)ret_val;
  if (do_fill)
    for (size_t i{}; i < m_size; i++) {
      ret_val_bufer[i] = filler;
    }
  return ret_val;
}
template <typename T>
[[nodiscard]] mjz_ard::mjz_str_t<T>
mjz_ard::mjz_str_t<T>::create_mjz_Str_2D_char_array(size_t size_col,
                                                    size_t size_row,
                                                    char filler, bool do_fill) {
  size_t size_of_my_ptrs = (size_t)sizeof(void *) * size_col;
  size_t total_size_ = (size_row + 1) * size_col + size_of_my_ptrs;
  mjz_ard::mjz_str_t<T> my_buufer_ =
      create_mjz_Str_char_array(total_size_ + 1, filler, do_fill);
  char **my_buffer_ptr = (char **)my_buufer_.buffer_ref();
  char *array_base_ptr = my_buufer_.buffer_ref() + size_of_my_ptrs;
  for (size_t i{}; i < size_col; i++) {
    *(my_buffer_ptr + i) = array_base_ptr;
    array_base_ptr += size_row;
    *array_base_ptr = 0;
    array_base_ptr++;
  }
  return my_buufer_;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::find_and_replace(const char *find_cstr,
                                             size_t find_count,
                                             const char *replace_cstr,
                                             size_t replace_count) {
  if (length() == 0 || find_count == 0) {
    return;
  }
  size_t i{length()};
  for (;;) {
    i = (substr_view(0ULL, i).rfind(find_cstr, 0, find_count));
    if (bit_cast<int64_t>(i) == -1) break;
    replace(i, find_count, replace_cstr, replace_count);
  }
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator-=(
    const mjz_ard::mjz_str_t<T> &othr_) {
  int64_t index_of_remove = lastIndexOf(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator-=(
    const basic_mjz_Str_view &othr_) {
  int64_t index_of_remove = lastIndexOf(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator%=(
    const mjz_ard::mjz_str_t<T> &othr_) {
  int64_t index_of_remove = find_first_of(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator%=(
    const basic_mjz_Str_view &othr_) {
  int64_t index_of_remove = find_first_of(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator*=(
    unsigned int number_of_it) {
  mjz_str_t<T> temperory_str_view(*this);
  for (size_t i{1}; i < number_of_it; i++) {
    concat(temperory_str_view);
  }
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> mjz_ard::mjz_str_t<T>::operator*(
    unsigned int number_of_it) {
  mjz_ard::mjz_str_t<T> lhs = mjz_ard::mjz_str_t<T>(*this);
  return lhs.operator*=(number_of_it);
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator/=(
    const mjz_ard::mjz_str_t<T> &othr_) {
  find_and_replace(othr_.data(), othr_.length(), empty_STRING_C_STR, 0);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator/=(
    const basic_mjz_Str_view &othr_) {
  find_and_replace(othr_.data(), othr_.length(), empty_STRING_C_STR, 0);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator++() {
  this->println();
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> mjz_ard::mjz_str_t<T>::operator++(int) {
  mjz_ard::mjz_str_t<T> tmp(*this);
  ++(*this);
  return tmp;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator--() {
  this->read();
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> mjz_ard::mjz_str_t<T>::operator--(int) {
  mjz_ard::mjz_str_t<T> tmp(*this);
  --(*this);
  return tmp;
}
template <typename T>
mjz_ard::hash_sha256 hash_msg_to_sha_512_with_output(
    const char *dev_passwoed, const size_t dev_passwoedLength,
    mjz_ard::mjz_str_t<T> &output_name_in_output_out) {
  hash_sha256 rtrn{};
  uint8_t(&shaResult)[SHA256_BLOCK_SIZE] = rtrn.hashed_data;
  rtrn.sha256_the_string(dev_passwoed, dev_passwoedLength);
  if (output_name_in_output_out != "Null") {
    output_name_in_output_out =
        (operator_plus<T>(operator_plus<T>(mjz_str_t<T>("const char "),
                                           (output_name_in_output_out.sv())),
                          basic_mjz_Str_view(" [] = { ", 8)));
    output_name_in_output_out.print((int)shaResult[0]);
    for (int64_t i = 1; i < sizeof(shaResult); i++) {
      output_name_in_output_out.print(",");
      output_name_in_output_out.print((int)shaResult[i]);
    }
    output_name_in_output_out.println(" } ;");
  }
  return rtrn;
}
template <typename T>
mjz_ard::hash_sha256 hash_msg_to_sha_512_n_with_output(
    const char *dev_passwoed, const size_t dev_passwoedLength, uint8_t n,
    mjz_ard::mjz_str_t<T> &output_name) {  // intended copy
  if (n == 0) {
    return hash_msg_to_sha_512_with_output(dev_passwoed, dev_passwoedLength,
                                           output_name);
  }
  static

      hash_sha256 ret =
          hash_sha256::hash_msg_to_sha_512(dev_passwoed, dev_passwoedLength);
  for (int64_t i{}; i < ((int64_t)n - 1); i++) {
    ret = hash_sha256::hash_msg_to_sha_512((char *)ret.hashed_data,
                                           SHA256_BLOCK_SIZE);
  }
  return hash_msg_to_sha_512_with_output((char *)ret.hashed_data,
                                         SHA256_BLOCK_SIZE, output_name);
}

template <typename T>
std::shared_ptr<mjz_ard::mjz_Str_DATA_storage_cls>
    &mjz_ard::extended_mjz_str_t<T>::drived_mjz_Str_DATA_storage_Obj_ptr_set() {
  if (!did_drived_mjz_Str_DATA_storage_Obj_ptr_set) {
    drived_mjz_Str_DATA_storage_Obj_ptr = mjz_Str_DATA_storage_cls::create();
    did_drived_mjz_Str_DATA_storage_Obj_ptr_set = 1;
  }
  return drived_mjz_Str_DATA_storage_Obj_ptr;
}
// stream part
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(const __FlashStringHelper *ifsh) {
  return print(reinterpret_cast<const char *>(ifsh));
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(const mjz_ard::mjz_str_t<T> &s) {
  return write(s.c_str(), s.length());
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(const char *str) {
  return write(str);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(char c) {
  return write(c);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(unsigned char b, int base) {
  return print((unsigned long)b, base);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(int n, int base) {
  return print((long)n, base);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(unsigned int n, int base) {
  return print((unsigned long)n, base);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(long n, int base) {
  if (base == 0) {
    return write((uint8_t)n);
  } else if (base == 10) {
    if (n < 0) {
      size_t t = (size_t)print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, static_cast<uint8_t>(base));
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(unsigned long n, int base) {
  if (base == 0) {
    return write((uint8_t)n);
  } else {
    return printNumber(n, static_cast<uint8_t>(base));
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(long long n, int base) {
  if (base == 0) {
    return write((uint8_t)n);
  } else if (base == 10) {
    if (n < 0) {
      size_t t = (size_t)print('-');
      n = -n;
      return printULLNumber(n, 10) + t;
    }
    return printULLNumber(n, 10);
  } else {
    return printULLNumber(n, static_cast<uint8_t>(base));
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(unsigned long long n, int base) {
  if (base == 0) {
    return write((uint8_t)n);
  } else {
    return printULLNumber(n, static_cast<uint8_t>(base));
  }
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::print(double n, int digits) {
  return printFloat(n, digits);
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(const __FlashStringHelper *ifsh) {
  size_t n = print(ifsh);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println() {
  return write("\r\n");
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(const mjz_ard::mjz_str_t<T> &s) {
  size_t n = print(s);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(const char *c) {
  size_t n = print(c);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(char c) {
  size_t n = print(c);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(unsigned char b, int base) {
  size_t n = print(b, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(int num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(unsigned int num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(unsigned long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(long long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(unsigned long long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::println(double num, int digits) {
  size_t n = print(num, digits);
  n += println();
  return n;
}
// Private Methods /////////////////////////////////////////////////////////////
template <typename T>
size_t mjz_ard::mjz_str_t<T>::printNumber(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]{};  // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];
  *str = '\0';
  // prevent crash if called with base == 1
  if (base < 2) {
    base = 10;
  }
  do {
    char c = n % base;
    n /= base;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);
  return write(str);
}
// REFERENCE IMPLEMENTATION FOR ULL
// size_t mjz_ard::mjz_str_t<T>::printULLNumber(unsigned long long n, uint8_t
// base)
// {
// // if limited to base 10 and 16 the bufsize can be smaller
// char buf[65];
// char *str = &buf[64];
// *str = '\0';
// // prevent crash if called with base == 1
// if (base < 2) base = 10;
// do {
// unsigned long long t = n / base;
// char c = n - t * base; // faster than c = n%base;
// n = t;
// *--str = c < 10 ? c + '0' : c + 'A' - 10;
// } while(n);
// return write(str);
// }
// FAST IMPLEMENTATION FOR ULL
template <typename T>
size_t mjz_ard::mjz_str_t<T>::printULLNumber(unsigned long long n64,
                                             uint8_t base) {
  // if limited to base 10 and 16 the bufsize can be 20
  char buf[64]{};
  uint8_t i = 0;
  uint8_t innerLoops = 0;
  // Special case workaround
  // https://github.com/arduino/ArduinoCore-API/issues/178
  if (n64 == 0) {
    write('0');
    return 1;
  }
  // prevent crash if called with base == 1
  if (base < 2) {
    base = 10;
  }
  // process chunks that fit in "16 bit math".
  uint16_t top = 0xFFFF / base;
  uint16_t th16 = 1;
  while (th16 < top) {
    th16 *= base;
    innerLoops++;
  }
  while (n64 > th16) {
    // 64 bit math part
    size_t q = n64 / th16;
    uint16_t r = (uint16_t)(n64 - q * th16);
    n64 = q;
    // 16 bit math loop to do remainder. (note buffer is filled reverse)
    for (uint8_t j = 0; j < innerLoops; j++) {
      uint16_t qq = r / base;
      buf[i++] = static_cast<char>(r - qq * base);
      r = qq;
    }
  }
  uint16_t n16 = (uint16_t)n64;
  while (n16 > 0) {
    uint16_t qq = n16 / base;
    buf[i++] = static_cast<char>(n16 - qq * base);
    n16 = qq;
  }
  size_t bytes = i;
  for (; i > 0; i--) {
    write((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
  }
  return bytes;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::printFloat(double number, int digits) {
  if (digits < 0) {
    digits = 2;
  }
  size_t n = 0;
  if (isnan(number)) {
    return print("nan");
  }
  if (isinf(number)) {
    return print("inf");
  }
  if (number > 4294967040.0) {
    return print("ovf");
  }  // constant determined empirically

  if (number < -4294967040.0) {
    return print("ovf");
  }  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0) {
    n += print('-');
    number = -number;
  }
  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i) {
    rounding /= 10.0;
  }
  number += rounding;
  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);
  // mjz_ard::mjz_str_t<T> the decimal point, but only if there are digits
  // beyond
  if (digits > 0) {
    n += print(".");
  }
  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)remainder;
    n += print(toPrint);
    remainder -= toPrint;
  }
  return n;
}
// private method to read stream with timeout
template <typename T>
int mjz_ard::mjz_str_t<T>::timedRead() {
  // unsigned long _startMillis; // used for timeout measurement
  int c;
  // _startMillis = millis();
  // do {
  c = read();
  if (c >= 0) {
    return c;
  }
  // }
  // while (
  // millis() - _startMillis <
  // drived_mjz_Str_DATA_storage_Obj_ptr->_timeout
  //);
  return -1;  // -1 indicates timeout
}
// private method to peek stream with timeout
template <typename T>
int mjz_ard::mjz_str_t<T>::timedPeek() {
  // unsigned long _startMillis; // used for timeout measurement
  int c;
  // _startMillis = millis();
  // do {
  c = peek();
  if (c >= 0) {
    return c;
  }
  // }
  // while ( millis() - _startMillis <
  // drived_mjz_Str_DATA_storage_Obj_ptr->_timeout );
  return -1;  // -1 indicates timeout
}
// returns peek of the next digit in the stream or -1 if timeout
// discards non-numeric characters
template <typename T>
int mjz_ard::mjz_str_t<T>::peekNextDigit(LookaheadMode lookahead,
                                         bool detectDecimal) {
  int c;
  while (1) {
    c = timedPeek();
    if (c < 0 || c == '-' || (c >= '0' && c <= '9') ||
        (detectDecimal && c == '.')) {
      return c;
    }
    switch (lookahead) {
      case SKIP_NONE:
        return -1;  // Fail code.
      case SKIP_WHITESPACE:
        switch (c) {
          case ' ':
          case '\t':
          case '\r':
          case '\n':
            break;
          default:
            return -1;  // Fail code.
        }
      case SKIP_ALL:
        break;
    }
    read();  // discard non-numeric
  }
}
// Public Methods
//////////////////////////////////////////////////////////////
// find returns true if the target string is found
template <typename T>
bool mjz_ard::mjz_str_t<T>::find_in_stream(const char *target) {
  return find_in_stream_Until(target, strlen(target), NULL, 0);
}
// reads data from the stream until the target string of given length is found
// returns true if target string is found, false if timed out
template <typename T>
bool mjz_ard::mjz_str_t<T>::find_in_stream(const char *target, size_t length) {
  return find_in_stream_Until(target, length, NULL, 0);
}
// as find but search ends if the terminator string is found
template <typename T>
bool mjz_ard::mjz_str_t<T>::find_in_stream_Until(const char *target,
                                                 const char *terminator) {
  return find_in_stream_Until(target, strlen(target), terminator,
                              strlen(terminator));
}
// reads data from the stream until the target string of the given length is
// found search terminated if the terminator string is found returns true if
// target string is found, false if terminated or timed out
template <typename T>
bool mjz_ard::mjz_str_t<T>::find_in_stream_Until(const char *target,
                                                 size_t targetLen,
                                                 const char *terminator,
                                                 size_t termLen) {
  if (terminator == NULL) {
    MultiTarget t[1] = {{target, targetLen, 0}};
    return findMulti(t, 1) == 0;
  } else {
    MultiTarget t[2] = {{target, targetLen, 0}, {terminator, termLen, 0}};
    return findMulti(t, 2) == 0;
  }
}
// returns the first valid (long) integer value from the current position.
// lookahead determines how parseInt looks ahead in the stream.
// See LookaheadMode enumeration at the top of the file.
// Lookahead is terminated by the first character that is not a valid part of an
// integer. Once parsing commences, 'ignore' will be skipped in the stream.
template <typename T>
long mjz_ard::mjz_str_t<T>::parseInt(
    mjz_ard::mjz_str_t<T>::LookaheadMode lookahead, char ignore) {
  bool isNegative = false;
  long value = 0;
  int c;
  c = peekNextDigit(lookahead, false);
  // ignore non numeric leading characters
  if (c < 0) {
    return 0;
  }  // zero returned if timeout

  do {
    if ((char)c == ignore)
      ;  // ignore this character
    else if (c == '-') {
      isNegative = true;
    } else if (c >= '0' && c <= '9') {  // is c a digit?
      value = value * 10 + c - '0';
    }
    read();  // consume the character we got with peek
    c = timedPeek();
  } while ((c >= '0' && c <= '9') || (char)c == ignore);
  if (isNegative) {
    value = -value;
  }
  return value;
}
// as parseInt but returns a floating point value
template <typename T>
float mjz_ard::mjz_str_t<T>::parseFloat(LookaheadMode lookahead, char ignore) {
  bool isNegative = false;
  bool isFraction = false;
  double value = 0.0;
  int c{};
  double fraction = 1.0;
  c = peekNextDigit(lookahead, true);
  // ignore non numeric leading characters
  if (c < 0) {
    return 0;
  }  // zero returned if timeout

  do {
    if ((char)c == ignore)
      ;  // ignore
    else if (c == '-') {
      isNegative = true;
    } else if (c == '.') {
      isFraction = true;
    } else if (c >= '0' && c <= '9') {  // is c a digit?
      if (isFraction) {
        fraction *= 0.1;
        value = value + fraction * ((char)c - '0');
      } else {
        value = value * 10 + c - '0';
      }
    }
    read();  // consume the character we got with peek
    c = timedPeek();
  } while ((c >= '0' && c <= '9') || (c == '.' && !isFraction) ||
           (char)c == ignore);
  if (isNegative) {
    value = -value;
  }
  return (float)value;
}
// read characters from stream into buffer
// terminates if length characters have been read, or timeout (see setTimeout)
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
/*
size_t mjz_ard::mjz_str_t<T>::readBytes(char *buffer, size_t length)
{
size_t count = 0;
while (count < length) {
int c = timedRead();
if (c < 0) break;
buffer++ = (char)c;
count++;
}
return count;
} */
// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator
// character detected returns the number of characters placed in the buffer (0
// means no valid data found)
template <typename T>
size_t mjz_ard::mjz_str_t<T>::readBytesUntil(char terminator, char *buffer_,
                                             size_t length) {
  size_t index = 0;
  while (index < length) {
    int c = timedRead();
    if (c < 0 || (char)c == terminator) {
      break;
    }
    *buffer_++ = (char)c;
    index++;
  }
  return index;  // return number of characters, not including null terminator
}
template <typename T>
mjz_ard::mjz_str_t<T> mjz_ard::mjz_str_t<T>::read_mjz_Str() {
  /*
  mjz_ard::mjz_str_t<T> ret;
  int c = timedRead();
  while (c >= 0)
  {
  ret += (char)c;
  c = timedRead();
  }
  return ret;
  */
  mjz_ard::mjz_str_t<T> ret = std::move(*this);  // be careful
  replace_with_new_str(this);
  return ret;
}
template <typename T>
mjz_ard::mjz_str_t<T> mjz_ard::mjz_str_t<T>::read_mjz_Str_Until(
    char terminator) {
  mjz_ard::mjz_str_t<T> ret;
  int c = timedRead();
  while (c >= 0 && (char)c != terminator) {
    ret += (char)c;
    c = timedRead();
  }
  return ret;
}
template <typename T>
int mjz_ard::mjz_str_t<T>::findMulti(
    struct mjz_ard::mjz_str_t<T>::MultiTarget *targets, int tCount) {
  // any zero length target string automatically matches and would make
  // a mess of the rest of the algorithm.
  for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
    if (t->len <= 0) {
      return (int)(t - targets);
    }
  }
  while (1) {
    int c = timedRead();
    if (c < 0) {
      return -1;
    }
    for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
      // the simple case is if we match, deal with that first.
      if ((char)c == t->str[t->index]) {
        if (++t->index == t->len) {
          return (int)(t - targets);
        } else {
          continue;
        }
      }
      // if not we need to walk back and see if we could have matched further
      // down the stream (ie '1112' doesn't match the first position in '11112'
      // but it will match the second position so we can't just reset the
      // current index to 0 when we find a mismatch.
      if (t->index == 0) {
        continue;
      }
      int origIndex = (int)(t->index);
      do {
        --t->index;
        // first check if current char works against the new current index
        if ((char)c != t->str[t->index]) {
          continue;
        }
        // if it's the only char then we're good, nothing more to check
        if (t->index == 0) {
          t->index++;
          break;
        }
        // otherwise we need to check the rest of the found string
        int diff = (int)(origIndex - t->index);
        size_t i;
        for (i = 0; i < t->index; ++i) {
          if (t->str[i] != t->str[i + diff]) {
            break;
          }
        }
        // if we successfully got through the previous loop then our current
        // index is good.
        if (i == t->index) {
          t->index++;
          break;
        }
        // otherwise we just try the next index
      } while (t->index);
    }
  }
  // unreachable
  return -1;
}

template <typename T>
std::istream &helper__op_shift_input_(
    const mjz_ard::extended_mjz_str_t<T> &rhs, std::istream &CIN,
    mjz_ard::extended_mjz_str_t<T> &get_shift_op_s) {
  char bfr[2050]{};
  int8_t is_reinterpreted{};
  constexpr uint8_t is_reinterpreted_and_is_int = 2;
  int8_t value_reinterpreted_and_is_int{};
  for (uint16_t i{0}; i < 2047; i++) {
    uint8_t is_reinterpreted_do_not_forbid{};
    if (i == 0) {
      CIN >> bfr[i];
    } else {
      CIN.get(bfr[i]);
    }
    if (is_reinterpreted == is_reinterpreted_and_is_int) {
      int8_t vlal_bf = rhs.char_to_int_for_string(bfr[i]);
      if (vlal_bf == -1) {
        char bfffr = bfr[i];
        bfr[i] = value_reinterpreted_and_is_int;
        i++;
        bfr[i] = bfffr;
        is_reinterpreted = 0;
      } else {
        value_reinterpreted_and_is_int *= 10;
        value_reinterpreted_and_is_int += vlal_bf;
        i--;
        continue;
      }
    } else if (is_reinterpreted == 1) {
      int8_t vlal_bf = rhs.char_to_int_for_string(bfr[i]);
      if (vlal_bf == -1) {
        is_reinterpreted = 0;
        if (!rhs.char_to_char_for_reinterpret(bfr[i])) {
          i--;
          continue;
        } else {
          is_reinterpreted_do_not_forbid = 1;
        }
      } else {
        value_reinterpreted_and_is_int = vlal_bf;
        is_reinterpreted = is_reinterpreted_and_is_int;
        i--;
        continue;
      }
    } else if (rhs.get_reinterpret_char_char() == bfr[i]) {
      bfr[i] = '\0';
      i--;
      is_reinterpreted = 1;
      continue;
    }
    if (!is_reinterpreted_do_not_forbid && rhs.is_forbiden(bfr[i])) {
      bfr[i] = '\0';
      break;
    }
  }
  bfr[2049] = '\0';
  bfr[2048] = '\0';
  bfr[2047] = '\0';
  get_shift_op_s += (const char *)bfr;
  return CIN;
}

template <typename T>
const mjz_ard::extended_mjz_str_t<T> &helper__op_shift_input_(
    const mjz_ard::extended_mjz_str_t<T> &rhs,
    const mjz_ard::extended_mjz_str_t<T> &CIN,
    mjz_ard::extended_mjz_str_t<T> &get_shift_op_s) {
  if (CIN.is_blank()) {
    return CIN;
  }
  const char *CIN_c_str = CIN.data();
  size_t CURunt_index_{};
  size_t my_bfr_obj_length = CIN.length() + 4;
  mjz_temp_malloc_wrapper_t<T> my_bfr_obj_ptr(my_bfr_obj_length + 5, 0);
  char *bfr = (char *)my_bfr_obj_ptr.get_ptr();
  uint8_t is_reinterpreted{};
  constexpr uint8_t is_reinterpreted_and_is_int = 2;
  int8_t value_reinterpreted_and_is_int{};
  char reinterpret_char_char_ref = rhs.get_reinterpret_char_char();
  auto continue_event_is_reinterpreted_and_is_int = [&](uint16_t &i) -> bool {
    if (bfr[i] == reinterpret_char_char_ref) {
      bfr[i] = value_reinterpreted_and_is_int;
      is_reinterpreted = 0;
    } else {
      int8_t value_buffer = rhs.char_to_int_for_string(bfr[i]);
      if (value_buffer == -1) {
        char bfffr = bfr[i];
        bfr[i] = value_reinterpreted_and_is_int;
        i++;
        bfr[i] = bfffr;
        is_reinterpreted = 0;
      } else {
        value_reinterpreted_and_is_int *= 10;
        value_reinterpreted_and_is_int += value_buffer;
        i--;
        return 1;  // continue;
      }
    }
    return 0;
  };
  auto continue_event_is_reinterpreted =
      [&](uint16_t &i, uint8_t &is_reinterpreted_do_not_forbid) -> bool {
    int8_t value_buffer_ = rhs.char_to_int_for_string(bfr[i]);
    if (value_buffer_ == -1) {
      is_reinterpreted = 0;
      if (!rhs.char_to_char_for_reinterpret(bfr[i])) {
        i--;
        return 1;  // continue;
      } else {
        is_reinterpreted_do_not_forbid = 1;
      }
    } else {
      value_reinterpreted_and_is_int = value_buffer_;
      is_reinterpreted = is_reinterpreted_and_is_int;
      i--;
      return 1;  // continue;
    }
    return 0;
  };
  for (uint16_t i{0}; i < my_bfr_obj_length - 3; i++) {
    uint8_t is_reinterpreted_do_not_forbid{};
    bfr[i] = CIN_c_str[CURunt_index_];
    CURunt_index_++;
    if (is_reinterpreted == is_reinterpreted_and_is_int) {
      if (continue_event_is_reinterpreted_and_is_int(i)) {
        continue;
      }
    } else if (is_reinterpreted == 1) {
      if (continue_event_is_reinterpreted(i, is_reinterpreted_do_not_forbid)) {
        continue;
      }
    } else if (reinterpret_char_char_ref == bfr[i]) {
      bfr[i] = '\0';
      i--;
      is_reinterpreted = 1;
      continue;
    }
    if (!is_reinterpreted_do_not_forbid && rhs.is_forbiden(bfr[i])) {
      bfr[i] = '\0';
      break;
    }
  }
  bfr[my_bfr_obj_length - 1] = '\0';
  bfr[my_bfr_obj_length - 2] = '\0';
  bfr[my_bfr_obj_length - 3] = '\0';
  get_shift_op_s += (const char *)my_bfr_obj_ptr.get_ptr();
  return CIN;
}

template <typename T>
mjz_str_t<T> &getline(mjz_str_t<T> &is, mjz_str_t<T> &str, char delim) {
  size_t index_of_delim = is.find_first_of(delim);
  str = is.substr_view(0ULL, index_of_delim);
  is.erase_from_f_to_l(0, index_of_delim);
  return is;
}
template <typename T>
mjz_str_t<T> &getline(mjz_str_t<T> &is, mjz_str_t<T> &str) {
  return getline(is, str, '\n');
}

template <typename T>
std::pair<mjz_ard::hash_sha256, mjz_str_t<T>>
mjz_ard::basic_mjz_Str_view::hash_with_output(uint8_t n) const {
  mjz_ard::hash_sha256 hash_;
  mjz_str_t<T> output(*this);
  hash_ = hash_msg_to_sha_512_n_with_output(c_str(), length(), n, output);
  return {hash_, output};
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring_beg_n(size_t beginIndex,
                                                          size_t number) const {
  size_t endIndex = beginIndex + number;
  if (!number || length() < endIndex) {
    return mjz_str_t<T>();
  }
  return substring(beginIndex, beginIndex + number);
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(size_t beginIndex) const {
  return substring(beginIndex, m_length);
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring_beg_n(int beginIndex,
                                                          int number) const {
  return substring_beg_n((int64_t)beginIndex, (int64_t)number);
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(int beginIndex,
                                                    int endIndex) const {
  return substring((int64_t)beginIndex, (int64_t)endIndex);
};
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(int beginIndex) const {
  return substring((int64_t)beginIndex);
}

template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring_beg_n(int64_t beginIndex,
                                                          size_t number) const {
  return substring_beg_n(signed_index_to_unsigned(beginIndex), number);
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(int64_t beginIndex,
                                                    int64_t endIndex) const {
  return substring(signed_index_to_unsigned(beginIndex),
                   signed_index_to_unsigned(endIndex));
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(int64_t beginIndex) const {
  beginIndex = signed_index_to_unsigned(beginIndex);
  return substring((size_t)beginIndex, length());
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring(size_t left,
                                                    size_t right) const {
  const char *c_str_out{};
  size_t len_out{};
  if (!substring_give_ptr(left, right, c_str_out, len_out)) {
    return mjz_str_t<T>();
  }
  mjz_str_t<T> out;
  out.copy(c_str_out, len_out);
  //
  return out;
}
template <typename T>
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substr(size_t pos, size_t len) const {
  if (len == npos) {
    len = length();
  }
  return substring_beg_n(pos, len);
}
template <typename T>
mjz_str_t<T> SHA256_CTX::to_string() const {
  char buffer[2 * 1024 + 1]{};
  return mjz_str_t<T>(SHA256_CTX::to_c_string(buffer));
}

template <typename T = mjz_normal_allocator>
mjz_str_t<T> float_get_bits_interpretation(float x) {
  auto bits = mjz_ard::get_bit_representation<float>(x);
  mjz_str_t<T> str(" ");
  for (auto cr : bits) {
    str.print((char)cr);
    str.write(' ');
  }
  str.print(
      "\n |- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -|\n "
      "|s| exp | mantissa |\n\n");
  return str;
}

template <typename T>
StringSumHelper_t<T> mjz_str_t<T>::operator-() {
  return *this;
}
template <typename TO>
class mv_to_T2 {
 public:
  template <typename... Types>
  TO operator()(Types &&...obj) {
    return TO(std::forward<Types>(obj)...);
  }
};
template <typename Tc1, typename Tc2, typename T1, typename T2>
inline StringSumHelper operator+(T1 lhs, T2 rhs) {
  return operator_plus(Tc1()(lhs), Tc2()(rhs));
}
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const mjz_Str &,
                                           const mjz_str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const mjz_Str &,
                                           const StringSumHelper &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const mjz_Str &,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(mjz_Str &&,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(mjz_Str &&, const StringSumHelper &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(mjz_Str &&,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(StringSumHelper &&,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(StringSumHelper &&,
                                           const StringSumHelper &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(StringSumHelper &&,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const StringSumHelper &,
                                           const basic_mjz_Str_view &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const StringSumHelper &,
                                           const StringSumHelper &);
template StringSumHelper operator+
    <mv_to_T2<mjz_Str>, mv_to_T2<mjz_Str>>(const StringSumHelper &,
                                           const basic_mjz_Str_view &);

typedef mjz_RingBufferN<SERIAL_BUFFER_SIZE> RingBuffer;
template <int N>
mjz_RingBufferN<N>::mjz_RingBufferN(void) {
  static_str_algo::memset(_aucBuffer, 0, N);
  clear();
}
template <int N>
void mjz_RingBufferN<N>::store_char(uint8_t c) {
  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (!isFull()) {
    _aucBuffer[_iHead] = c;
    _iHead = nextIndex(_iHead);
    _numElems = _numElems + 1;
  }
}
template <int N>
void mjz_RingBufferN<N>::clear() {
  _iHead = 0;
  _iTail = 0;
  _numElems = 0;
}
template <int N>
int mjz_RingBufferN<N>::read_char() {
  if (isEmpty()) {
    return -1;
  }
  uint8_t value = _aucBuffer[_iTail];
  _iTail = nextIndex(_iTail);
  _numElems = _numElems - 1;
  return value;
}
template <int N>
int mjz_RingBufferN<N>::available() {
  return _numElems;
}
template <int N>
int mjz_RingBufferN<N>::availableForStore() {
  return (N - _numElems);
}
template <int N>
int mjz_RingBufferN<N>::peek() {
  if (isEmpty()) {
    return -1;
  }
  return _aucBuffer[_iTail];
}
template <int N>
int mjz_RingBufferN<N>::nextIndex(int index) {
  return (uint32_t)(index + 1) % N;
}
template <int N>
bool mjz_RingBufferN<N>::isFull() {
  return (_numElems == N);
}
}  // namespace mjz_ard
namespace mjz_ard_types = mjz_ard::have_mjz_ard_removed;
namespace mjz = mjz_ard;
namespace mjzt = mjz_ard_types;
#undef NO_IGNORE_CHAR

namespace mjz_ard {
inline void randomSeed(unsigned long seed) {
  if (seed != 0) {
    std::srand(seed);
  }
}
inline long random(long howbig) {
  if (howbig == 0) {
    return 0;
  }

  return std::rand() % howbig;
}
inline long random(long howsmall, long howbig) {
  if (howsmall >= howbig) {
    return howsmall;
  }

  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

inline long long unsigned int mjz_millis() {
  static uint64_t my_time[2] = {0, 0};
  uint64_t ARD_millis_vr = mjz_ard::ARD_millis();

  if (my_time[0] > ARD_millis_vr) {
    my_time[1] += 1;
  }

  my_time[0] = ARD_millis_vr;
  return *((uint64_t *)my_time);
}

/*********************************************/
/* Search */
/*********************************************/
inline std::ostream &operator<<(std::ostream &CIN,
                                const mjz_ard::SHA256_CTX &obj) {
  char buffer[2 * 1025]{};
  CIN << obj.copy_to_c_str(buffer, 2 * 1024);
  return CIN;
}

inline mjz_ard::UINT64_X2_32_t mjz_random() {
  mjz_ard::UINT64_X2_32_t ret{};
  ret.data32[0] = (int32_t)esp_random();
  ret.data32[1] = (int32_t)esp_random();
  return ret;
}
inline bool get_random_chanch_bool(double chance_var) {
  double random_var = (double)(abs(mjz_random().data64) % 1000001);
  random_var /= 1000000;
  return random_var <= chance_var;
}

}  // namespace mjz_ard
#endif  // __mjz_ard_STRINGS__
#endif  // __cplusplus

#ifndef FREAMEWORK_FILE_HEDER
#define FREAMEWORK_FILE_HEDER

/*

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
*/

namespace mjz_ard {

class speed_Timer {
 public:
  speed_Timer() { Reset(); }

  void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }

  long double Elapsednano() {
    return (long double)std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now() - m_Start)
        .count();
  }
  long double Elapsed() { return Elapsednano() * 0.001f * 0.001f * 0.001f; }

  long double ElapsedMillis() { return Elapsednano() / 1000.0f / 1000.0f; }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
class tiny_scoped_timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
  const char *str = "";

 public:
  tiny_scoped_timer(const char *s)
      : m_Start(std::chrono::high_resolution_clock::now()), str(s) {}
  ~tiny_scoped_timer() {
    size_t time = (std::chrono::duration_cast<std::chrono::nanoseconds>(
                       std::chrono::high_resolution_clock::now() - m_Start)
                       .count());
    std::cout << str << " : " << time << " ns\n";
  }
};
struct timer_info {
  long double Time_ns;
  long double atempt_num;
};
inline bool operator<(timer_info rhs, timer_info lhs) {
  return (rhs.Time_ns / rhs.atempt_num) < (lhs.Time_ns / lhs.atempt_num);
}
enum timer_cmd : uint8_t {
  NONE = 0,
  Force_log = MJZ_logic_BIT(0),
  NO_Stop = MJZ_logic_BIT(1),
  just_Stop = MJZ_logic_BIT(2),
};
class Scoped_speed_Timer {
 public:
  void set_list(std::shared_ptr<std::map<std::string, timer_info>> vect_) {
    m_uomp = std::move(vect_);
  }
  Scoped_speed_Timer(const std::string &name, bool glob = 1) : m_Name(name) {
    if (glob) {
      m_uomp = get_global_map_rf();
    }
  }
  Scoped_speed_Timer(const char *name, size_t size_, bool glob = 1)
      : m_Name(name, size_) {
    if (glob) {
      m_uomp = get_global_map_rf();
    }
  }
  Scoped_speed_Timer(const char *name, bool glob = 1)
      : m_Name(name, strlen(name)) {
    if (glob) {
      m_uomp = get_global_map_rf();
    }
  }
  void Stop(timer_cmd cmd_ = timer_cmd::NONE) {
    long double time = m_Timer.Elapsednano();

    if ((stoped && !(cmd_ & timer_cmd::Force_log)) ||
        !!(cmd_ & timer_cmd::just_Stop)) {
    } else {
      if (m_uomp) {
        timer_info &ti_ = (*m_uomp)[m_Name];
        ti_.atempt_num++;
        ti_.Time_ns += time;
      } else {
        std::cout << "[TIMER] " << m_Name << " - " << time << "ns\n";
      }
    }

    if (!(cmd_ & timer_cmd::NO_Stop)) {
      stoped = 1;
    }

    return;
  }
  speed_Timer &get_timer() { return m_Timer; }
  ~Scoped_speed_Timer() { Stop(); }
  static inline bool operator_less_than(
      const std::pair<std::string, timer_info> &rhs,
      const std::pair<std::string, timer_info> &lhs) {
    return !operator<(rhs.second, lhs.second);
  }
  static std::string show_analysis(
      std::shared_ptr<std::map<std::string, timer_info>> map_ptr,
      const std::string &timer_signuchure_name = "_timer_",
      std::function<bool(const std::pair<std::string, timer_info> &,
                         const std::pair<std::string, timer_info> &)>
          cmpr_fnction = operator_less_than) {
    std::string ret_str;
    auto &_timer_info = (*map_ptr)[timer_signuchure_name];
    long double time_defult = _timer_info.Time_ns / _timer_info.atempt_num;
    std::vector<std::pair<std::string, timer_info>> vect_analis(
        map_ptr->size());
    {
      size_t i{};

      for (auto &obj : *map_ptr) {
        if (obj.first != timer_signuchure_name)
          obj.second.Time_ns =
              (obj.second.Time_ns) - (obj.second.atempt_num * time_defult);

        obj.second.Time_ns = floor(obj.second.Time_ns);
        vect_analis[i++] = obj;
      }

      std::sort(vect_analis.begin(), vect_analis.end(), cmpr_fnction);
      i = 0;
      long double time_token_total{};

      for (auto &obj : vect_analis) {
        long double time_token = (obj.second.Time_ns) / obj.second.atempt_num;
        ret_str += std::to_string(++i);
        ret_str += ". ";
        ret_str += obj.first;
        ret_str += ":";
        ret_str += std::to_string(time_token);
        ret_str += " ns (";
        ret_str += std::to_string(obj.second.Time_ns);
        ret_str += " / ";
        ret_str += std::to_string(obj.second.atempt_num);
        ret_str += ")\n";
        obj.second.Time_ns = time_token;
        time_token_total += time_token;
        obj.second.atempt_num = 1;
      }

      i = 0;
      time_token_total /= 100;

      for (auto &obj : vect_analis) {
        ret_str += std::to_string(++i);
        ret_str += ". ";
        ret_str += obj.first;
        ret_str += ":";
        ret_str += std::to_string((obj.second.Time_ns) / time_token_total);
        ret_str += " %\n";
      }
    }
    return ret_str;
  }

  Scoped_speed_Timer &operator=(const Scoped_speed_Timer &name) {
    (*this)(name.m_Name);

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }

    return *this;
  }
  Scoped_speed_Timer(const Scoped_speed_Timer &name) {
    (*this)(name.m_Name);

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }
  }
  Scoped_speed_Timer &operator=(Scoped_speed_Timer &&name) noexcept {
    (*this)(name.m_Name);
    m_Timer = name.m_Timer;

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }

    return *this;
  }
  Scoped_speed_Timer(Scoped_speed_Timer &&name) noexcept {
    (*this)(name.m_Name);
    m_Timer = name.m_Timer;

    if (name.m_uomp) {
      m_uomp = name.m_uomp;
    }
  }

  static inline void set_global_map(
      std::shared_ptr<std::map<std::string, timer_info>> g_map_) {
    get_global_map_rf() = g_map_;
  }
  static inline std::shared_ptr<std::map<std::string, timer_info>>
  get_global_map() {
    return get_global_map_rf();
  }
  inline Scoped_speed_Timer &operator()(const std::string &name,
                                        const bool non_matter = 0) {
    Stop();
    stoped = 0;
    m_Name = name;
    m_Timer.Reset();
    return *this;
  }

 protected:
  static inline std::shared_ptr<std::map<std::string, timer_info>>
      &get_global_map_rf() {
    static std::shared_ptr<std::map<std::string, timer_info>> glob_map;
    return glob_map;
  }
  bool stoped{};
  std::string m_Name;
  std::shared_ptr<std::map<std::string, timer_info>> m_uomp;
  speed_Timer m_Timer;
};
class vr_Scoped_speed_Timer : public Scoped_speed_Timer {
  vr_Scoped_speed_Timer(const std::string &name) : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(const Scoped_speed_Timer &name)
      : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(Scoped_speed_Timer &&name)
      : Scoped_speed_Timer(std::move(name)) {}
  vr_Scoped_speed_Timer(const vr_Scoped_speed_Timer &name)
      : Scoped_speed_Timer(name) {}
  vr_Scoped_speed_Timer(vr_Scoped_speed_Timer &&name) noexcept
      : Scoped_speed_Timer(std::move(name)) {}
  vr_Scoped_speed_Timer &operator=(const vr_Scoped_speed_Timer &s_tmr) {
    *((Scoped_speed_Timer *)this) = s_tmr;
    return *this;
  }
  vr_Scoped_speed_Timer &operator=(vr_Scoped_speed_Timer &&s_tmr) noexcept {
    *((Scoped_speed_Timer *)this) = std::move(s_tmr);
    return *this;
  }
  vr_Scoped_speed_Timer &operator=(const Scoped_speed_Timer &s_tmr) {
    *((Scoped_speed_Timer *)this) = s_tmr;
    return *this;
  }
  vr_Scoped_speed_Timer &operator=(Scoped_speed_Timer &&s_tmr) {
    *((Scoped_speed_Timer *)this) = std::move(s_tmr);
    return *this;
  }

  virtual ~vr_Scoped_speed_Timer() = default;
};
class std_stream_class_warper {
  std_stream_class_warper() {}

 public:
  template <typename T>
  inline friend std_stream_class_warper &operator<<(
      std_stream_class_warper &COUT, T &&obj) {
    std::cout << std::forward<T>(obj);
    return COUT;
  }

  inline std_stream_class_warper &write(const char *s, size_t l) {
    std::cout.write(s, l);
    return *this;
  }
  static inline std_stream_class_warper &get() {
    static std_stream_class_warper singleton;
    return singleton;
  }
};

template <class counter_class, class my_stream = std_stream_class_warper>
class mjz_class_operation_reporter_t {
  static counter_class index;
  template <typename... argT>
  mjz_class_operation_reporter_t &println(argT &&...args) const {
    print_c_str(std::forward<argT>(args)...);
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &println_wf(
      const mjz_class_operation_reporter_t &obj, argT &&...args) const {
    print(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    print(UUID());
    print_c_str(" from ID:");
    print(obj.UUID());
    print('\n');

    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &println_w(argT &&...args) const {
    print_c_str(std::forward<argT>(args)...);
    print_c_str(" with ID: ");
    print(UUID());
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &println_wi(const counter_class &index,
                                             argT &&...args) const {
    print_c_str(std::forward<argT>(args)...);
    print(index);
    print_c_str(" with ID: ");
    print(UUID());
    print('\n');
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &printsv(argT &&...args) const {
    std::initializer_list<std::string_view> list = {
        std::forward<argT>(args)...};
    for (auto &s : list) print(s);
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &print_c_str(argT &&...args) const {
    std::initializer_list<const char *> list = {std::forward<argT>(args)...};
    for (auto &s : list) print(s);
    return none();
  }
  mjz_class_operation_reporter_t &print_c_str(const char *s) const {
    print_c_str_len_1(s, strlen(s));
    return none();
  }
  template <typename... argT>
  mjz_class_operation_reporter_t &print(argT &&...args) const {
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          print(std::forward<argT>(args));
          return;
        }...};  // do all tasks in thr rigth order
    for (auto &f : list) f();
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t &print(T &&arg) const {
    my_stream::get() << std::forward<T>(arg);
    return none();
  }
  mjz_class_operation_reporter_t &print_c_str_len_1(const char *s,
                                                    size_t l) const {
    my_stream::get().write(s, l);
    return none();
  }

  template <typename T>
  mjz_class_operation_reporter_t &println_o(T &&arg) const {
    println_w("operator", std::forward<T>(arg), "()");
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t &println_oi(T &&arg) const {
    println_w("operator", std::forward<T>(arg), "(int)");
    return none();
  }
  template <typename T>
  mjz_class_operation_reporter_t &println_obj(
      const mjz_class_operation_reporter_t &obj,
      T &&arg) const {  // oobj
    print_c_str("  ID: (");
    print(UUID());
    print(" ) operator", std::forward<T>(arg), "(const obj&)", "  ID: ( ");
    print(obj.UUID());
    print(" )\n");
    return none();
  }

 public:
  mutable char filler = '|';
  inline const void *UUID() const { return this; };
  mjz_class_operation_reporter_t() { println_wi(index++, " created : "); }
  mjz_class_operation_reporter_t(int i) : filler(i) {
    print_c_str(" created with int :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(float i) : filler(i) {
    print_c_str(" created with float :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(double i) : filler(i) {
    print_c_str(" created with double :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(size_t i) : filler(i) {
    print_c_str(" created with size_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(int64_t i) : filler(i) {
    print_c_str(" created with int64_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(void *i) : filler(i) {
    print_c_str(" created with size_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(char c) : filler(c) {
    print_c_str(" created with char :(");
    print(c);
    println_wi(index++, ")  ");
  }
  mjz_class_operation_reporter_t(const char *s) {
    print_c_str(" created with str : \"");
    println_wi(index++, s, "\"   ");
  }

  mjz_class_operation_reporter_t(std::initializer_list<const char *> args) {
    print_c_str(" created with c_str list : { ");
    auto it = args.begin(), end = args.end();
    if (it == end) {
      println_wi(index++, " }   ");
      return;
    }
    for (;;) {
      print("\"", *it, "\"");
      it++;
      if (it < end) {
        print_c_str_len_1(" , ", 3);
        continue;
      }
      break;
    }
    println_wi(index++, " }   ");
  }
  mjz_class_operation_reporter_t(std::initializer_list<std::string_view> args) {
    print_c_str(" created with string_view list : { ");
    auto it = args.begin(), end = args.end();
    if (it == end) {
      println_wi(index++, " }   ");
      return;
    }
    for (;;) {
      print("\"", *it, "\"");
      it++;
      if (it < end) {
        print_c_str_len_1(" , ", 3);
        continue;
      }
      break;
    }
    println_wi(index++, " }   ");
  }

  mjz_class_operation_reporter_t(const char *s, size_t len) {
    print_c_str(" created with str , len  : \"");
    print_c_str_len_1(s, len);
    println_wi(index++, "\"   ");
  }
  ~mjz_class_operation_reporter_t() { println_wi(--index, " destroyed : "); }
  mjz_class_operation_reporter_t(mjz_class_operation_reporter_t &&obj) {
    index++;
    println_wf(obj, " move constructed ");
  }
  mjz_class_operation_reporter_t &operator=(
      mjz_class_operation_reporter_t &&obj) {
    println_wf(obj, " moved to me  ");
    return none();
  }
  mjz_class_operation_reporter_t(const mjz_class_operation_reporter_t &obj) {
    index++;
    println_wf(obj, " copy constructed ");
  }
  mjz_class_operation_reporter_t &operator=(
      const mjz_class_operation_reporter_t &obj) const {
    println_wf(obj, " copied to me   ");
    return none();
  }
  mjz_class_operation_reporter_t &operator()() const { return println_o("()"); }
  mjz_class_operation_reporter_t &operator()(std::string_view v) const {
    print("operator()(string_view) : (");
    print(v);
    return print_c_str_len_1(") ", 2);
  }
  mjz_class_operation_reporter_t &operator()(const char *s) const {
    print("operator()(c_str) : (");
    print(s);
    return print_c_str_len_1(") ", 2);
  }
  mjz_class_operation_reporter_t &operator()(
      mjz_class_operation_reporter_t &obj) const {
    return println("operator()(obj&)");
  }
  mjz_class_operation_reporter_t &operator()(
      const mjz_class_operation_reporter_t &obj) const {
    return println("operator()(const obj&)");
  }
  mjz_class_operation_reporter_t &operator()(
      mjz_class_operation_reporter_t &&obj) const {
    return println("operator()(obj&&)");
  }

  mjz_class_operation_reporter_t &operator()(int) const {
    return println_oi("()");
  }
  mjz_class_operation_reporter_t &operator++(int) const {
    return println_oi("++");
  }
  mjz_class_operation_reporter_t &operator++() const {
    return println_o("++()");
  }
  mjz_class_operation_reporter_t &operator--(int) const {
    return println_oi("--");
  }
  mjz_class_operation_reporter_t &operator--() const { return println_o("--"); }

  mjz_class_operation_reporter_t &operator+(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "+");
  }
  mjz_class_operation_reporter_t &operator-(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "-");
  }

  mjz_class_operation_reporter_t &operator*(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "*");
  }
  mjz_class_operation_reporter_t &operator/(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "/");
  }
  mjz_class_operation_reporter_t &operator%(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "%");
  }
  mjz_class_operation_reporter_t &operator>>(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, ">>");
  }
  mjz_class_operation_reporter_t &operator<<(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "<<");
  }
  mjz_class_operation_reporter_t &operator&(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "&");
  }
  mjz_class_operation_reporter_t &operator|(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "|");
  }
  mjz_class_operation_reporter_t &operator^(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "^");
  }

  mjz_class_operation_reporter_t &operator+=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "+=");
  }
  mjz_class_operation_reporter_t &operator-=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "-=");
  }
  mjz_class_operation_reporter_t &operator*=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "*=");
  }
  mjz_class_operation_reporter_t &operator/=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "/=");
  }
  mjz_class_operation_reporter_t &operator%=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "%=");
  }
  mjz_class_operation_reporter_t &operator>>=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, ">>=");
  }
  mjz_class_operation_reporter_t &operator<<=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "<<=");
  }
  mjz_class_operation_reporter_t &operator&=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "&=");
  }
  mjz_class_operation_reporter_t &operator|=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "|=");
  }
  mjz_class_operation_reporter_t &operator^=(
      const mjz_class_operation_reporter_t &obj) const {
    return println_obj(obj, "^=");
  }
  mjz_class_operation_reporter_t &operator-() const { return println_o("-"); }
  mjz_class_operation_reporter_t &operator+() const { return println_o("+"); }
  mjz_class_operation_reporter_t *operator&()
      const {  // baaaaaaaaaaaaaaad practic
    println_o("&");
    return (mjz_class_operation_reporter_t *)this;
  }
  mjz_class_operation_reporter_t &operator*() const {
    println_o("*");
    return none();
  }
  mjz_class_operation_reporter_t *operator->() const {
    println_o("->");
    return (mjz_class_operation_reporter_t *)this;
  }
  template <typename T>
  auto operator->*(T p) const {
    println_o("->*");
    return this->*p;
  }
  inline operator bool() const {
    println_o(" bool ");
    return filler;
  }
  inline operator char() const {
    println_o(" char ");
    return filler;
  }

  inline operator int() const {
    println_o(" int ");
    return filler;
  }
  mjz_class_operation_reporter_t &operator!() const { return println_o("!"); }
  mjz_class_operation_reporter_t &operator~() const { return println_o("~"); }

  bool operator&&(  // really bad practic to overload these dont do in any other
                    // senario
      const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "&&");
    return operator char() &&
           ((mjz_class_operation_reporter_t *)(&obj))->operator char();
  }
  bool operator||(  // really bad practic to overload these  dont do in any
                    // other senario
      const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "||");
    return operator char() || obj.operator char();
  }

  inline int64_t compare(const mjz_class_operation_reporter_t &obj) const {
    return (int64_t)((char *)UUID() - (char *)obj.UUID());
  }
  bool operator<=(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "<=");
    return compare(obj) <= 0;
  }
  bool operator>=(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, ">=");

    return compare(obj) >= 0;
  }
  bool operator<(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "<");
    return compare(obj) < 0;
  }
  bool operator>(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, ">");
    return compare(obj) > 0;
  }
  bool operator==(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "==");
    return compare(obj) == 0;
  }
  bool operator!=(const mjz_class_operation_reporter_t &obj) const {
    println_obj(obj, "!=");
    return compare(obj) != 0;
  }
  inline mjz_class_operation_reporter_t &none() const {
    return *((mjz_class_operation_reporter_t *)this);
  }

  inline operator mjz_class_operation_reporter_t &() const {
    return *((mjz_class_operation_reporter_t *)(this));
  }
  inline operator mjz_class_operation_reporter_t &() { return *this; }
  inline operator const mjz_class_operation_reporter_t &() const {
    return none();
  }
  inline operator const mjz_class_operation_reporter_t &() { return none(); }
  template <class the_Stream>
  friend the_Stream &operator<<(
      the_Stream &COUT, const mjz_class_operation_reporter_t &non_const_opr) {
    mjz_class_operation_reporter_t &opr =
        *(mjz_class_operation_reporter_t
              *)(&non_const_opr);  // i promise  that ther will be no changes in
                                   // print functions but they are not const for
                                   // being more usable
    opr.print_c_str(" ostream operator<< (obj) c:\'");
    opr.print_c_str_len_1(&opr.filler, 1);
    opr.print_c_str("\' ID : ");
    opr.print(opr.UUID());
    opr.print_c_str("\n");
    COUT << opr.filler;
    return COUT;
  }
  template <class the_Stream>
  friend the_Stream &operator>>(the_Stream &CIN,
                                mjz_class_operation_reporter_t &opr) {
    opr.print_c_str(" istream operator>> (obj) c:\'");
    opr.print_c_str_len_1(&opr.filler, 1);
    opr.print_c_str("\' to \'");
    CIN >> opr.filler;
    opr.print_c_str_len_1(&opr.filler, 1);
    opr.print_c_str("\' ID : ");
    opr.print(opr.UUID());
    opr.print_c_str("\n");
    return CIN;
  }
};

template <class counter_class, class my_stream>
counter_class mjz_class_operation_reporter_t<counter_class, my_stream>::index{};
using operation_reporter = mjz_class_operation_reporter_t<size_t>;

namespace have_mjz_ard_removed {
using operation_reporter = mjz_class_operation_reporter_t<size_t>;
typedef vr_Scoped_speed_Timer vr_Scoped_speed_Timer;
typedef speed_Timer speed_Timer;
typedef Scoped_speed_Timer Scoped_speed_Timer;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard

namespace mjz_ard {
template <typename T>
const T &print(const T &obj) {
  std::cout << obj;
  return obj;
}
template <typename T>
const T &println(const T &obj) {
  std::cout << obj << '\n';
  return obj;
}
inline void print() {}
inline void println() { std::cout << '\n'; }
template <typename... argT>
void print(argT &&...args) {
  auto list =
      std::initializer_list<std::function<void(void)>>{[&](void) -> void {
        std::cout << std::forward<argT>(args);
        return;
      }...};  // do all tasks in thr rigth order
  for (auto &f : list) f();
}
template <typename... argT>
void print_arr(argT &&...args) {
  auto list =
      std::initializer_list<std::function<void(void)>>{[&](void) -> void {
        std::cout << std::forward<argT>(args);
        return;
      }...};  // do all tasks in thr rigth order
  auto it = list.begin(), end = list.end();
  if (it == end) return;
  std::cout << "{ ";
  for (;;) {
    (*it)();
    if (++it < end) {
      std::cout << " , ";
      continue;
    }
    std::cout << " }";
    break;
  }
}
template <class T>
void print_it(const T begin, T end) {
  auto &it = begin;
  if (it == end) return;
  std::cout << "{ ";
  for (;;) {
    std::cout << (*it);
    if (++it < end) {
      std::cout << " , ";
      continue;
    }
    std::cout << " }";
    break;
  }
}

template <class T, typename FNT>
T for_each(T begin, T end, FNT function) {
  auto it = begin;
  if (it == end) return;
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return begin;
}
template <class T>
void println_it_FE(T begin, T end) {
  auto &it = begin;
  if (it == end) return;
  for (;;) {
    std::cout << (*it) << '\n';
    if (++it < end) {
      continue;
    }
    break;
  }
}
template <class T>
const T &print_it(const T &obj) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;
  std::cout << "{ ";
  for (;;) {
    std::cout << (*it);
    if (++it < end) {
      std::cout << " , ";
      continue;
    }
    std::cout << " }";
    break;
  }
  return obj;
}
template <class T, typename FNT>
const T &for_each(const T &obj, FNT function) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return obj;
}
template <class T, typename FNT>
T &for_each(T &obj, FNT function) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return obj;
}
template <class T>
void println_it_FE(const T &obj) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return;
  for (;;) {
    std::cout << (*it) << '\n';
    if (++it < end) {
      continue;
    }
    break;
  }
}
template <typename... argT>
void println_FE(argT &&...args) {  // println_for_each
  auto list =
      std::initializer_list<std::function<void(void)>>{[&](void) -> void {
        std::cout << std::forward<argT>(args);
        std::cout << '\n';
        return;
      }...};  // do all tasks in thr rigth order
  auto it = list.begin(), end = list.end();
  if (it == end) return;
  for (auto &f : list) f();
}
template <typename... argT>
void println_arr(argT &&...args) {
  print_arr(std::forward<argT>(args)...);
  std::cout << '\n';
}
template <class T>
void println_it(const T &obj) {
  print_it(obj);
  std::cout << '\n';
}
template <typename... argT>
void println(argT &&...args) {
  print(std::forward<argT>(args)...);
  std::cout << '\n';
}
namespace print_c_str_n {
inline void print() {}
template <typename... args_t>
inline void print(const char *c_str, size_t n, args_t &&...args) {
  std::cout.write(c_str, n);
  return print(std::forward<args_t>(args)...);
}
template <typename T, typename... args_t>
inline void print(const T &arg0, args_t &&...args) {
  std::cout << arg0;
  return print(std::forward<args_t>(args)...);
}
template <typename T, typename... args_t>
inline void print(T &arg0, args_t &&...args) {
  std::cout << arg0;
  return print(std::forward<args_t>(args)...);
}

template <typename... args_t>
inline void println(args_t &&...args) {
  print(std::forward<args_t>(args)...);
  std::cout << std::endl;
}
}  // namespace print_c_str_n
}  // namespace mjz_ard

#endif  // !FREAMEWORK_FILE_HEDER
