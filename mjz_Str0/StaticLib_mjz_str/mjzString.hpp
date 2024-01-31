// removed  the arduino string
/*
this is a simple fast implementation/fork of arduino string and much more...
written by mjz https://github.com/Mjz86
*/

/*
     my take away :
     1.why  is  the standard lib so big?
     because its the boiler plate code .

     2. why is some parts of it compiler  dependent?
   (   std::addressof & std::construct_at & the bit_cast & std::initializer_list
   & std::optional->[with constexpr] ) because of portability( std::addressof is
   implemented by compiler and the c++ ways of going around the [overloaded]
   operator&() are limited ) and mainly ease of use

   3.why i did this monstrosity ?
   because it is one of the best ways of learning about c++ and computer memory
   and cpu design and in some ways it helped with my embedded projects

   4. what did i learn?
       countless advanced c++ stuff and some wired tricks

*/

#ifndef _MJZ_ARD_LIB_FILE_
#define _MJZ_ARD_LIB_FILE_
#define NumberOf(arg) ((size_t)(sizeof(arg) / sizeof(arg[0])))
#ifdef __cplusplus
#ifndef __mjz_ard_STRINGS__
#ifndef MJZ_ARD_LIB_HELPER
#define MJZ_ARD_LIB_HELPER

// #define Arduino

// #define Arduino 1
#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <stack>
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

#define MJZ_EXPRESSION_TO_STR(HTML_TEXT) #HTML_TEXT
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

#define MJZ_logic_BL_bit_to_8_bits_not_bool(B) \
  ((((uint8_t) !(B)) + (uint8_t)(-1)))
#define MJZ_logic_BL_bit_to_8_bits(B) (-((int8_t)(B)))
#define MJZ_logic_bit_to_8_bits(B) ((B) ? (uint8_t)(-1) : 0ULL)
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
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstddef>
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

#endif  // MJZ_ARD_LIB_HELPER
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
#define global_mjz_areana_allocator_on false
#endif  // !global_mjz_areana_allocator_on
#ifndef global_mjz_areana_allocator_log
#define global_mjz_areana_allocator_log true
#endif
#ifndef _MJZ_STD_TERMENATE_NO_THROW
#define _MJZ_STD_TERMENATE_NO_THROW true
#endif  //!_MJZ_STD_TERMENATE_NO_THROW
inline uint32_t usteejtgk_millis() { return millis(); }
class __FlashStringHelper;

namespace mjz_ard {
typedef bool succsess_t;

typedef bool failure_t;

struct mjz_std_termenator {
  mjz_std_termenator() {}
  ~mjz_std_termenator() noexcept(false) {
#if _MJZ_STD_TERMENATE_NO_THROW
    throw "i am dead";
#endif  // _MJZ_STD_TERMENATE_NO_THROW
  }
};

template <typename T, typename... Ts>
[[noreturn]] inline void JThrow(Ts &&...args) noexcept(false) {
  T obj{std::forward<Ts>(args)...};
  throw obj;
}

template <>
[[noreturn]] inline void JThrow<void>() noexcept(false) {
  throw /*the current exception*/;
}

[[noreturn]] inline void JThrow() noexcept(false) { JThrow<void>(); }
template <typename T>
[[noreturn]] inline void JThrow(T &&arg) noexcept(false) {
  throw std::forward<T>(arg);
}

template <typename T, typename... Ts>
[[noreturn]] inline void Throw(Ts &&...args) noexcept(false) {
  mjz_std_termenator terminate_if;
  {
    mjz_std_termenator terminate_if_;
    JThrow<T>(std::forward<Ts>(args)...);
  }
}

template <>
[[noreturn]] inline void Throw<void>() noexcept(false) {
  mjz_std_termenator terminate_if;
  {
    mjz_std_termenator terminate_if_;
    JThrow<void>();
  }
}

[[noreturn]] inline void Throw() noexcept(false) {
  mjz_std_termenator terminate_if;
  {
    mjz_std_termenator terminate_if_;
    JThrow();
  }
}
template <typename T>
[[noreturn]] inline void Throw(T &&arg) noexcept(false) {
  mjz_std_termenator terminate_if;
  {
    mjz_std_termenator terminate_if_;
    JThrow<T>(std::forward<T>(arg));
  }
}
template <class Type>
using mjz_get_value_Type = typename Type::value_type;

template <class Type>
using mjz_get_pointer_Type = typename Type::pointer;
template <size_t N = 0>
class initilizer_in_constructor_helper_class_t {
 protected:
  inline constexpr initilizer_in_constructor_helper_class_t() {}
  inline constexpr ~initilizer_in_constructor_helper_class_t(){};
  inline constexpr initilizer_in_constructor_helper_class_t(
      initilizer_in_constructor_helper_class_t &&){};
  template <typename FN_t, typename... Ts>

  inline constexpr initilizer_in_constructor_helper_class_t(
      FN_t &&initilizer_lambda_in_constructor_member_initilizer_list,
      Ts &&...args) {
    initilizer_lambda_in_constructor_member_initilizer_list(
        std::forward<Ts>(args)...);
  }

 public:
  template <typename FN_t, typename... Ts>
  inline static constexpr decltype(auto) run(FN_t &&function_, Ts &&...args) {
    return std::forward<FN_t>(function_)(std::forward<Ts>(args)...);
  }
};

template <typename T>
struct mjz_resource_acquisition_is_initialization_template {
  T obj;
  std::function<void(T &&)> destructor;
  template <typename... Ts>
  inline mjz_resource_acquisition_is_initialization_template(
      std::function<void(T &&)> destructor_function, Ts &&...args)
      : obj(std::forward<Ts>(args)...),
        destructor(std::move(destructor_function)) {}
  inline ~mjz_resource_acquisition_is_initialization_template() {
    destructor(std::move(obj));
  }
};

template <size_t N>
using initilizer_helper_class_t = initilizer_in_constructor_helper_class_t<N>;
template <size_t N>
using initilizer_class_t = initilizer_in_constructor_helper_class_t<N>;
/*
this argument shall not be used in any non mjz object or function as argument
this is a separator just for the compiler
*/
template <size_t NUM>
struct special_arg {
  using me = special_arg<NUM>;
  static constexpr const size_t N = NUM;
  inline constexpr special_arg() {}
  inline constexpr special_arg(const special_arg &) {}
  inline constexpr special_arg &operator=(special_arg) { return *this; }
};
template <const char *const what_function>
struct special_arg_c {};
template <typename T>
struct special_arg_t {};
template <size_t NUM, size_t init_if_iam_not_zero_the_base_with_me_minus_one_>
struct special_arg_no_init_constructor_t {
  using me = special_arg_no_init_constructor_t<
      NUM, init_if_iam_not_zero_the_base_with_me_minus_one_>;
  static constexpr const size_t N = NUM;
  static constexpr const size_t
      init_if_iam_not_zero_the_base_with_me_minus_one =
          init_if_iam_not_zero_the_base_with_me_minus_one_;
  inline constexpr special_arg_no_init_constructor_t() {}
  inline constexpr special_arg_no_init_constructor_t(
      const special_arg_no_init_constructor_t &) {}
  inline constexpr special_arg_no_init_constructor_t &operator=(
      const special_arg_no_init_constructor_t &) {
    return *this;
  }
};
static const constexpr size_t
    special_arg_no_init_constructor_for_unsafe_warper_value =
        0x0242ac12f0a285;  //   just a random number
template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
using mjz_no_init_uw_special_arg = special_arg_no_init_constructor_t<
    special_arg_no_init_constructor_for_unsafe_warper_value,
    init_if_iam_not_zero_the_base_with_me_minus_one>;

static const constexpr size_t
    special_arg_no_init_constructor_for_mjz_optional_value =
        0x4E58B994E905F5;  //   just a random number

static const constexpr size_t special_arg_init_for_mjz_optional_value =
    (~special_arg_no_init_constructor_for_mjz_optional_value) >> 1;  //   just a
                                                                     //   random
                                                                     //   number
template <size_t init_if_iam_not_zero_the_base_with_me_minus_one = 0>
using mjz_no_init_optional_t = special_arg_no_init_constructor_t<
    special_arg_no_init_constructor_for_mjz_optional_value,
    init_if_iam_not_zero_the_base_with_me_minus_one>;
using mjz_init_optional_t =
    special_arg<special_arg_init_for_mjz_optional_value>;
struct mjz_init_optional_if_t
    : private special_arg<special_arg_init_for_mjz_optional_value + 1> {
 private:
  bool do_init{};

 public:
  inline constexpr mjz_init_optional_if_t(bool do_init_ = 0)
      : do_init(do_init_) {}
  ~mjz_init_optional_if_t() {}
  inline constexpr operator bool() const { return do_init; }
  inline constexpr bool operator!() const { return !do_init; }
};

template <size_t arg_Index, class U_F, class... U>
struct mjz_get_template_argument_class_helper_t {
  using Type = typename mjz_get_template_argument_class_helper_t<arg_Index - 1,
                                                                 U...>::Type;
};
template <size_t arg_Index, class U_F>
struct mjz_get_template_argument_class_helper_t<arg_Index, U_F> {
  using Type = void;
};
template <class U_F>
struct mjz_get_template_argument_class_helper_t<0, U_F> {
  using Type = U_F;
};
template <class U_F, class... U>
struct mjz_get_template_argument_class_helper_t<0, U_F, U...> {
  using Type = U_F;
};
template <size_t arg_Index = 0, typename... U>
using mjz_get_template_argument =
    typename mjz_get_template_argument_class_helper_t<arg_Index, U...>::Type;

template <typename T, size_t arg_Index, T U_F, T... U>
struct mjz_get_template_value_class_helper_t {
  static constexpr const T Val =
      mjz_get_template_value_class_helper_t<T, arg_Index - 1, U...>::Val;
};
template <typename T, size_t arg_Index, T U_F>
struct mjz_get_template_value_class_helper_t<T, arg_Index, U_F> {};
template <typename T, T U_F>
struct mjz_get_template_value_class_helper_t<T, 0, U_F> {
  static constexpr const T Val = U_F;
};
template <typename T, T U_F, T... U>
struct mjz_get_template_value_class_helper_t<T, 0, U_F, U...> {
  static constexpr const T Val = U_F;
};
template <typename T, size_t arg_Index = 0, T... U>
static constexpr const T mjz_get_value_argument =
    mjz_get_template_value_class_helper_t<T, arg_Index, U...>::Val;

template <template <typename... U> class T, typename... U_IN>
struct mjz_initilize_template_arguments_helper_class_t {
  using Type = T<U_IN...>;
};
template <template <typename... U> class T, typename... U_IN>
using mjz_initilize_template_arguments =
    typename mjz_initilize_template_arguments_helper_class_t<T, U_IN...>::Type;

template <typename... args_t>
inline constexpr auto get_arg_count() -> size_t {
  constexpr const std::size_t n = sizeof...(args_t);
  return n;
}

template <class Type>
inline constexpr Type *remove_const(const Type *o) {
  return const_cast<Type *>(o);
}
template <class Type>
inline constexpr Type &remove_const(const Type &o) {
  return const_cast<Type &>(o);
}
template <class Type>
inline constexpr Type &&remove_const(const Type &&o) {
  return const_cast<Type &&>(o);
}
template <class Type>
inline constexpr Type &remove_const(Type &o) {
  return o;
}
template <class Type>
inline constexpr Type *remove_const(Type *o) {
  return o;
}
template <class Type>
inline constexpr Type &&remove_const(Type &&o) {
  return o;
}
template <bool IS_NOT_A_TYPE, bool has_virtual = false>
class not_a_type_if {};
template <>
class not_a_type_if<true, true> {
  not_a_type_if() = delete;
  ~not_a_type_if() = delete;
  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;
  void *operator new(size_t, void *) = delete;
  void *operator new[](size_t, void *) = delete;
  void operator delete(void *, size_t) = delete;
  void operator delete[](void *, size_t) = delete;
  void operator delete(void *, size_t, void *) = delete;
  void operator delete[](void *, size_t, void *) = delete;
  void operator delete(void *) = delete;
  void operator delete[](void *) = delete;
  void operator delete(void *, void *) = delete;
  void operator delete[](void *, void *) = delete;
  virtual not_a_type_if
  a_pure_virtual_function_that_shall_not_be_implemented() = 0;
};
template <>
class not_a_type_if<true, false> {
  not_a_type_if() = delete;
  ~not_a_type_if() = delete;
  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;
  void *operator new(size_t, void *) = delete;
  void *operator new[](size_t, void *) = delete;
  void operator delete(void *, size_t) = delete;
  void operator delete[](void *, size_t) = delete;
  void operator delete(void *, size_t, void *) = delete;
  void operator delete[](void *, size_t, void *) = delete;
  void operator delete(void *) = delete;
  void operator delete[](void *) = delete;
  void operator delete(void *, void *) = delete;
  void operator delete[](void *, void *) = delete;
};
template <bool has_virtual>
class not_a_type_if<false, has_virtual> {};

template <int>
class bit_ref_data {};
template <>
class bit_ref_data<1> {
 protected:
  uint8_t *m_byte{};
  uint8_t m_mask{};

  inline constexpr bit_ref_data(uint8_t mask, uint8_t *byte)
      : m_byte(byte), m_mask(mask) {}

  inline constexpr ~bit_ref_data() {}
  inline constexpr uint8_t &byte() {
    return ((!!this->m_byte) ? (*this->m_byte) : (m_mask));
  }

 public:
  inline constexpr uint8_t byte() const {
    return (this->m_byte) ? *this->m_byte : m_mask;
  }
  inline constexpr uint8_t mask() const {
    return (this->m_byte) ? m_mask : true;
  }
};

template <>
class bit_ref_data<2> {
 protected:
  uint8_t *m_byte{};
  uint8_t m_mask{};

  inline constexpr bit_ref_data(uint8_t mask, uint8_t *byte)
      : m_byte(byte), m_mask(mask) {}

  inline constexpr ~bit_ref_data() {}
  inline constexpr uint8_t &byte() { return *m_byte; }

 public:
  inline constexpr uint8_t byte() const { return *m_byte; }
  inline constexpr uint8_t mask() const { return m_mask; }
};

template <>
class bit_ref_data<3> {
 protected:
  uint8_t *m_byte{};
  uint8_t m_mask{};

  inline constexpr bit_ref_data(uint8_t mask, uint8_t *byte)
      : m_byte(byte), m_mask(mask) {}

  inline constexpr ~bit_ref_data() {}
  inline constexpr uint8_t &byte() { return m_mask; }

 public:
  inline constexpr uint8_t byte() const { return m_mask; }
  inline constexpr uint8_t mask() const { return true; }
};
template <>
class bit_ref_data<0> {
 protected:
  uint8_t *m_byte{};
  uint8_t m_mask{};

  inline constexpr bit_ref_data(uint8_t mask, uint8_t *byte)
      : m_byte(byte), m_mask(mask) {}

  inline constexpr ~bit_ref_data() {}
  inline constexpr uint8_t &byte() {
    if (!this->m_byte) Throw<const char *>("bad bit accsess");
    return *this->m_byte;
  }

 public:
  inline constexpr uint8_t byte() const {
    if (!this->m_byte) Throw<const char *>("bad bit accsess");
    return *this->m_byte;
  }
  inline constexpr uint8_t mask() const {
    if (!this->m_byte) Throw<const char *>("bad bit accsess");
    return m_mask;
  }
};

template <int no_throw_ref = 1>
class bit_reference_or_bit_t : public bit_ref_data<no_throw_ref> {
 public:
  using mask = bit_ref_data<no_throw_ref>::mask;
  using byte = bit_ref_data<no_throw_ref>::byte;
  inline constexpr bool is_ref() const { return !!this->m_byte; }

  inline constexpr bit_reference_or_bit_t() : bit_ref_data(false, nullptr) {}
  explicit inline constexpr bit_reference_or_bit_t(bool b)
      : bit_ref_data(b, nullptr) {}
  static inline constexpr bit_reference_or_bit_t bit_ref_mk(
      bit_reference_or_bit_t &b) {
    return bit_reference_or_bit_t(b.m_mask, b.m_byte);
  }
  static inline constexpr bit_reference_or_bit_t bit_ref_mk(
      const bit_reference_or_bit_t &b) {
    return bit_reference_or_bit_t(b.m_mask, remove_const(b.m_byte));
  }
  explicit inline constexpr bit_reference_or_bit_t(
      const bit_reference_or_bit_t &b)
      : bit_ref_data(b.m_mask, b.m_byte) {}
  explicit inline constexpr bit_reference_or_bit_t(bit_reference_or_bit_t &&b)
      : bit_ref_data(b.m_mask, b.m_byte) {}

  inline constexpr bit_reference_or_bit_t(uint8_t mask, uint8_t *byte_ptr)
      : bit_ref_data(mask, byte_ptr) {}

  inline constexpr bit_reference_or_bit_t(uint8_t *byte_ptr, uint8_t mask)
      : bit_ref_data(mask, byte_ptr) {}
  constexpr inline bit_reference_or_bit_t &set() {
    byte() |= mask();
    return *this;
  }
  constexpr inline bit_reference_or_bit_t &set(bool b) {
    byte() &= MJZ_logic_BL_bit_to_8_bits(b) | ~mask();
    return *this;
  }
  constexpr inline bit_reference_or_bit_t &clear() {
    byte() &= ~mask();
    return *this;
  }
  constexpr inline bit_reference_or_bit_t &flip() {
    byte() ^= mask();
    return *this;
  }

  constexpr inline bool getN() const { return !(byte() & mask()); }
  constexpr inline bool get() const { return !getN(); }
  constexpr inline bool operator!() const { return getN(); }
  constexpr inline operator bool() const { return b(); }

  explicit constexpr inline operator int32_t() const { return b(); }
  explicit constexpr inline operator int64_t() const { return b(); }
  explicit constexpr inline operator uint32_t() const { return b(); }
  explicit constexpr inline operator uint64_t() const { return b(); }

  constexpr inline uint64_t operator+() const { return b(); }
  constexpr inline uint64_t operator-() const {
    return MJZ_logic_BL_bit_to_64_bits(b());
  }
  constexpr inline bool b() const { return get(); }
  constexpr inline bit_reference_or_bit_t operator~() const {
    return {~this->m_mask, this->m_byte};
  }

  bit_reference_or_bit_t &change_reference(bit_reference_or_bit_t &b) {
    this->m_mask = b.m_mask;
    this->m_byte = b.m_byte;
  }
  bit_reference_or_bit_t &operator=(bool b) {
    set(b);
    return *this;
  }

  bit_reference_or_bit_t &operator=(const bit_reference_or_bit_t &b) {
    set(b.b());
    return *this;
  }
  inline constexpr friend bool operator==(bit_reference_or_bit_t a,
                                          bit_reference_or_bit_t b) {
    return b.b() == a.b();
  }
  inline constexpr friend bool operator<=(bit_reference_or_bit_t a,
                                          bit_reference_or_bit_t b) {
    return b.b() <= a.b();
  }
  inline constexpr friend bool operator>=(bit_reference_or_bit_t a,
                                          bit_reference_or_bit_t b) {
    return b.b() >= a.b();
  }
  inline constexpr friend bool operator<(bit_reference_or_bit_t a,
                                         bit_reference_or_bit_t b) {
    return b.b() < a.b();
  }

  inline constexpr friend bool operator>(bit_reference_or_bit_t a,
                                         bit_reference_or_bit_t b) {
    return b.b() > a.b();
  }
  inline constexpr friend bool operator!=(bit_reference_or_bit_t a,
                                          bit_reference_or_bit_t b) {
    return b.b() != a.b();
  }
  inline constexpr friend bool operator&(bit_reference_or_bit_t a,
                                         bit_reference_or_bit_t b) {
    return b.b() && a.b();
  }
  inline constexpr friend bool operator^(bit_reference_or_bit_t a,
                                         bit_reference_or_bit_t b) {
    return b.b() ^ a.b();
  }
  inline constexpr friend bool operator|(bit_reference_or_bit_t a,
                                         bit_reference_or_bit_t b) {
    return b.b() || a.b();
  }

  inline constexpr bit_reference_or_bit_t &operator++() {
    set();
    return *this;
  }
  inline constexpr bit_reference_or_bit_t &operator--() {
    clear();
    return *this;
  }

  inline constexpr bit_reference_or_bit_t operator++(int) {
    bool bl = b();
    set();
    return bl;
  }
  inline constexpr bit_reference_or_bit_t operator--(int) {
    bool bl = b();
    clear();
    return bl;
  }
};
using bit_ref_or_bit = bit_reference_or_bit_t<1>;
using bit_ref = bit_reference_or_bit_t<0>;

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
      : iterator_template_t(nullptr, nullptr, nullptr) {}
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
      Throw<std::out_of_range>(
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
template <typename Type, typename iterator_t, bool is_in_forward_direction>
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
  constexpr inline void find_next() {
    if constexpr (is_in_forward_direction) {
      go_forward();
      return;
    }
    go_backward();
  }
  constexpr inline iterator_template_filter_warper() noexcept : m_iterator() {}
  constexpr inline iterator_template_filter_warper(iterator_t iter) noexcept
      : m_iterator(iter) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(filter_type filter) noexcept
      : m_iterator(), m_filter(filter) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(iterator_t iter,
                                                   filter_type filter) noexcept
      : m_iterator(iter), m_filter(filter) {
    find_next();
  }
  constexpr iterator_template_filter_warper(
      const iterator_template_filter_warper &p) noexcept
      : m_iterator(p.m_iterator), m_filter(p.m_filter) {
    find_next();
  }
  constexpr iterator_template_filter_warper(
      iterator_template_filter_warper &&p) noexcept
      : m_iterator(p.m_iterator), m_filter(p.m_filter) {
    find_next();
  }
  constexpr iterator_template_filter_warper(
      const iterator_template_filter_warper &p, filter_type filter) noexcept
      : m_iterator(p.m_iterator), m_filter(filter) {
    find_next();
  }
  constexpr iterator_template_filter_warper(iterator_template_filter_warper &&p,
                                            filter_type filter) noexcept
      : m_iterator(p.m_iterator), m_filter(filter) {
    find_next();
  }
  constexpr iterator_template_filter_warper &operator=(Type *iter) {
    m_iterator = iter;
    find_next();
    return *this;
  }
  constexpr inline iterator_template_filter_warper(Type *iter, Type *min_end,
                                                   Type *max_end) noexcept
      : m_iterator(iter, min_end, max_end) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(Type *arr,
                                                   size_t len) noexcept
      : m_iterator(arr, len) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(
      const Type *First_arg, const Type *Last_arg) noexcept
      : iterator_template_filter_warper(First_arg, First_arg, Last_arg) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(Type *iter, Type *min_end,
                                                   Type *max_end,
                                                   filter_type filter) noexcept
      : m_iterator(iter, min_end, max_end), m_filter(filter) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(Type *arr, size_t len,
                                                   filter_type filter) noexcept
      : m_iterator(arr, len), m_filter(filter) {
    find_next();
  }
  constexpr inline iterator_template_filter_warper(const Type *First_arg,
                                                   const Type *Last_arg,
                                                   filter_type filter) noexcept
      : m_iterator(First_arg, First_arg, Last_arg), m_filter(filter) {}
  constexpr iterator_template_filter_warper &operator=(iterator_t iter) {
    m_iterator = iter;
    m_filter = (iter.m_filter);
    find_next();
    return *this;
  }
  constexpr iterator_template_filter_warper &operator=(
      const iterator_template_filter_warper &p) {
    m_iterator = (p.m_iterator);
    m_filter = (p.m_filter);
    find_next();
    return *this;
  }
  constexpr iterator_template_filter_warper &operator=(
      iterator_template_filter_warper &&p) noexcept {
    m_iterator = (p.m_iterator);
    m_filter = (p.m_filter);
    find_next();
    return *this;
  }
  inline ~iterator_template_filter_warper() {}
  constexpr bool operator==(
      const iterator_template_filter_warper &other) noexcept {
    find_next();
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(
      const iterator_template_filter_warper &other) noexcept {
    find_next();
    return m_iterator != other.m_iterator;
  }
  constexpr reference operator*() {
    find_next();
    return m_iterator.operator*();
  }
  constexpr pointer operator->() {
    find_next();
    return m_iterator.operator->();
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) {
    find_next();
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
  constexpr reference operator[](std::size_t index) {
    find_next();
    return m_iterator[index];
  }
  constexpr bool operator<(
      const iterator_template_filter_warper &other) noexcept {
    find_next();
    return m_iterator < other.m_iterator;
  }
  constexpr bool operator>(
      const iterator_template_filter_warper &other) noexcept {
    find_next();
    return m_iterator > other.m_iterator;
  }
  constexpr bool operator<=(
      const iterator_template_filter_warper &other) noexcept {
    find_next();
    return m_iterator <= other.m_iterator;
  }
  constexpr bool operator>=(
      const iterator_template_filter_warper &other) noexcept {
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
template <bool is_forward, class iterator_t>
constexpr iterator_template_filter_warper<mjz_get_value_Type<iterator_t>,
                                          iterator_t, is_forward>
to_filter_it(iterator_t it, typename iterator_template_filter_warper<
                                mjz_get_value_Type<iterator_t>, iterator_t,
                                is_forward>::filter_type filter) {
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

template <uint32_t number_of_blocks =
              number_of_global_mjz_areana_allocator_blocks,
          uint16_t block_length = size_of_global_mjz_areana_allocator_blocks,
          bool KEEP_the_heap_clean = false, typename size_type = uint32_t>
class mjz_arena_allocator_t {
  template <typename T, size_type m_size>
  struct data_buffer {
    T m_data[m_size];
    constexpr inline size_type size() { return m_size; }
    constexpr inline T *begin() { return data(); }
    constexpr inline T *end() { return data() + m_size; }
    constexpr inline T *data() { return m_data; }
    constexpr inline const T &operator[](int64_t i) const { return m_data[i]; }
    constexpr inline T &operator[](int64_t i) { return m_data[i]; }
  };

 public:
  constexpr static size_type not_valid = (((size_type)-1 >> 1) - 1);
  constexpr static char EMPTY_char = 'A';
  struct block_data {
    size_type index_of_begin = not_valid;
    size_type index_of_end = not_valid;
  };

 protected:
  data_buffer<block_data, number_of_blocks> m_block_data;
  data_buffer<data_buffer<uint8_t, block_length>, number_of_blocks> m_blocks;
  constexpr inline void clean_all_data() & {
    if constexpr (KEEP_the_heap_clean)
      memset((char *)m_blocks.begin(), EMPTY_char,
             ((char *)m_blocks.end()) - ((char *)m_blocks.begin()));
  }
  constexpr inline size_type get_index_of_pointer(void *ptr) & {
    if (m_blocks.end() <= ptr || ptr < m_blocks.begin()) return not_valid;
    size_type ptr_diff =
        (size_type)((size_type)ptr - (size_type)m_blocks.begin());
    if ((ptr_diff % block_length) && ptr_diff) return not_valid;
    return (ptr_diff / block_length);
  }
  constexpr inline size_type get_index_of_pointer_that_is_not_allocated(
      size_type number_of_blocks_in_a_row = 1,
      size_type begin_search_at_index = 0) & {
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
      size_type size, size_type realloc_index = not_valid) & {
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
    set_range_to(index, {index + size});
    return index;
  }
  constexpr inline void set_range_to(size_type ren_beg, size_type ren_end,
                                     size_type beg, size_type end) & {
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
  constexpr inline void deallocate_block(size_type index) & {
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
  constexpr inline void *get_ptr(size_type index) & {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_blocks[index]).data();
  }
  constexpr inline size_type get_size_in_blocks(size_type index) & {
    if (index == not_valid || m_block_data[index].index_of_begin != index)
      return 0;
    return (m_block_data[index].index_of_end - index);
  }
  constexpr inline size_type get_real_size(size_type index) & {
    return get_size_in_blocks(index) * block_length;
  }
  constexpr inline size_type size_to_number_of_blocks(size_type size) & {
    return (size % block_length) ? ((size / block_length) + 1)
                                 : (size / block_length);
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
  constexpr inline void *unsafe_data() & { return m_blocks.data(); }
  constexpr inline size_type unsafe_size() & {
    return ((char *)m_blocks.end()) - ((char *)m_blocks.begin());
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
    T m_data[(m_size) ? m_size : 1];
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
#if global_mjz_areana_allocator_on

using mjz_global_arena_allocator_type =
    mjz_arena_allocator_t<number_of_global_mjz_areana_allocator_blocks,
                          size_of_global_mjz_areana_allocator_blocks,
                          global_mjz_areana_allocator_on>;
#else
struct mjz_global_arena_allocator_type
    : dynamic_mjz_arena_allocator_t<0, 0, char> {
  mjz_global_arena_allocator_type() : dynamic_mjz_arena_allocator_t(0, 0) {}
};
#endif  //  global_mjz_areana_allocator_on

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
inline void *realloc(void *p, size_t s) {
  void *p2 = ::realloc(p, s);
  return p2;
}
inline void free(void *p) { return ::free(p); }
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
  inline void free(void *ptr, size_t) { my_c_allocator.free(ptr); }
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

template <class T>
using mjz_get_Type = typename T::Type;

template <class T, typename Type, typename... Ts>
concept C_simple_unsafe_init_obj_wrpr_helper =
    requires(T obj, const T cobj, Ts &&...args) {
      T();
      obj.~T();
      T(mjz_no_init_uw_special_arg<0>());
      T(mjz_no_init_uw_special_arg<1>());
      T(mjz_no_init_uw_special_arg<2>());
      T(mjz_no_init_uw_special_arg<3>());
      T(std::forward<Ts>(args)...);
      { *obj } -> std::same_as<Type &>;
      { *cobj } -> std::same_as<const Type &>;
      { obj() } -> std::same_as<Type &>;
      { cobj() } -> std::same_as<const Type &>;
      { obj.operator->() } -> std::same_as<Type *>;
      { cobj.operator->() } -> std::same_as<const Type *>;
      { obj.get() } -> std::same_as<Type &>;
      { cobj.get() } -> std::same_as<const Type &>;
      { obj.ptr() } -> std::same_as<Type *>;
      { cobj.ptr() } -> std::same_as<const Type *>;
      { obj.unsafe_create() } -> std::same_as<Type *>;
      { obj.unsafe_create(std::forward<Ts>(args)...) } -> std::same_as<Type *>;
      { obj.unsafe_destroy() } -> std::same_as<void>;
      { T::has_destroy() } -> std::same_as<bool>;
      requires sizeof(T) == sizeof(Type);
      requires true;
    };

template <class T, typename Type, typename ref_t = Type &,
          typename rv_ref_t = Type &&, typename... args_t>
concept C_mjz_obj_manager_helper =
    requires(ref_t ref, rv_ref_t rvref, Type *ptr, size_t n, T &me_ref,

             args_t &&...args) {
      { T::alignment } -> std::convertible_to<size_t>;
      { T::size_of_type_v } -> std::convertible_to<size_t>;
      typename T::simple_unsafe_init_obj_wrpr_true_false;
      typename T::simple_unsafe_init_obj_wrpr_false_false;
      typename T::simple_unsafe_init_obj_wrpr_true;
      typename T::simple_unsafe_init_obj_wrpr_false;
      typename T::template simple_unsafe_init_obj_wrpr<true, true>;
      typename T::template simple_unsafe_init_obj_wrpr<true, false>;
      typename T::template simple_unsafe_init_obj_wrpr<false, true>;
      typename T::template simple_unsafe_init_obj_wrpr<false, false>;
      typename T::alignment_t;
      typename T::Alignment_t;
      requires !std::is_array_v<Type>;
      requires C_simple_unsafe_init_obj_wrpr_helper<
          typename T::simple_unsafe_init_obj_wrpr_true, Type, args_t...>;
      requires C_simple_unsafe_init_obj_wrpr_helper<
          typename T::simple_unsafe_init_obj_wrpr_false, Type, args_t...>;
      {
        T::construct_at(static_cast<Type *>(ptr), static_cast<args_t>(args)...)
        } -> std::same_as<Type *>;
      {
        T::construct_array_at(static_cast<Type *>(ptr), static_cast<size_t>(n),
                              static_cast<args_t>(args)...)
        } -> std::same_as<Type *>;
      {
        T::destruct_array_at(static_cast<Type *>(ptr), static_cast<size_t>(n))
        } -> std::same_as<bool>;
      {
        T::obj_constructor(static_cast<args_t>(args)...)
        } -> std::same_as<Type>;
      {
        T::obj_constructor_on(static_cast<rv_ref_t>(rvref),
                              static_cast<args_t>(args)...)
        } -> std::same_as<Type &&>;
      {
        T::obj_constructor_on(static_cast<ref_t>(ref),
                              static_cast<args_t>(args)...)
        } -> std::same_as<Type &>;
      {
        T::obj_constructor_on(static_cast<Type *>(ptr),
                              static_cast<args_t>(args)...)
        } -> std::same_as<Type *>;
      {
        T::obj_destructor_on(static_cast<rv_ref_t>(ref))
        } -> std::same_as<void>;
      { T::obj_destructor_on(static_cast<ref_t>(ref)) } -> std::same_as<void>;
      { T::obj_destructor_on(static_cast<Type *>(ptr)) } -> std::same_as<void>;
      {
        T::construct(me_ref, static_cast<Type *>(ptr),
                     static_cast<args_t>(args)...)
        } -> std::same_as<void>;
      {
        T::obj_move_to_obj(static_cast<ref_t>(ref), static_cast<rv_ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_copy_to_obj(static_cast<ref_t>(ref), static_cast<ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_copy_to_obj(static_cast<ref_t>(ref),
                           static_cast<const ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_move_to_obj(static_cast<Type *>(ptr), static_cast<Type *>(ptr))
        } -> std::same_as<Type &>;
      {
        T::obj_copy_to_obj(static_cast<Type *>(ptr), static_cast<Type *>(ptr))
        } -> std::same_as<Type &>;
      {
        T::obj_copy_to_obj(static_cast<Type *>(ptr),
                           static_cast<const Type *>(ptr))
        } -> std::same_as<Type &>;
      {
        T::obj_temp_copy_to_obj(static_cast<ref_t>(ref),
                                static_cast<const rv_ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_go_to_obj(static_cast<ref_t>(ref), static_cast<const ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_go_to_obj(static_cast<ref_t>(ref), static_cast<ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_go_to_obj(static_cast<ref_t>(ref), static_cast<rv_ref_t>(ref))
        } -> std::same_as<Type &>;
      {
        T::obj_equals(static_cast<ref_t>(ref), static_cast<args_t>(args)...)
        } -> std::same_as<Type &>;
      { T::addressof(static_cast<ref_t>(ref)) } -> std::same_as<Type *>;
      {
        T::addressof(static_cast<const Type &>(ref))
        } -> std::same_as<const Type *>;
      { T::to_address(static_cast<Type *>(ptr)) } -> std::same_as<Type *>;
      {
        T::to_address(static_cast<const ref_t>(ref))
        } -> std::convertible_to<const Type *>;
      {
        T::to_address(static_cast<const Type *>(ptr))
        } -> std::convertible_to<const Type *>;
      { T::to_address(static_cast<ref_t>(ref)) } -> std::same_as<Type *>;
      { T::pointer_to(static_cast<ref_t>(ref)) } -> std::same_as<Type *>;
      { T::destroy_at(static_cast<Type *>(ptr)) } -> std::same_as<bool>;
      { T::destroy_at(static_cast<ref_t>(ref)) } -> std::same_as<bool>;
      {
        T::destroy_n(static_cast<Type *>(ptr), static_cast<size_t>(n))
        } -> std::same_as<Type *>;
      {
        T::destroy(static_cast<T &>(me_ref), static_cast<Type *>(ptr))
        } -> std::same_as<void>;
      { T::destroy(static_cast<Type *>(ptr)) } -> std::same_as<void>;
      {
        T::destroy(static_cast<Type *>(ptr), static_cast<Type *>(ptr))
        } -> std::same_as<void>;
    };

template <class T, typename... args_t>
concept C_mjz_obj_manager =
    requires {
      typename T::Type;
      requires C_mjz_obj_manager_helper<T, typename T::Type, typename T::Type &,
                                        typename T::Type &&, args_t...>;
    };

template <typename T, typename Type, typename ref_t = Type &,
          typename rv_ref_t = Type &&, typename... args_t>
concept C_mjz_temp_type_obj_helper =
    requires(ref_t ref, Type *ptr, size_t n, bool b, args_t &&...args) {
      requires C_mjz_obj_manager<T, args_t...>;
      { T::size_of_type() } -> std::same_as<size_t>;
      {
        T::swap(static_cast<ref_t>(ref), static_cast<ref_t>(ref))
        } -> std::same_as<void>;
      {
        T::obj_destructor_arr(static_cast<Type *>(ptr), static_cast<size_t>(n),
                              static_cast<bool>(b))
        } -> std::same_as<bool>;
      {
        T::construct_arr_at(static_cast<Type *>(ptr), static_cast<size_t>(n),
                            static_cast<bool>(b), static_cast<args_t>(args)...)
        } -> std::same_as<Type *>;
      {
        T::construct_arr_at(static_cast<Type *>(ptr), static_cast<size_t>(n),
                            static_cast<bool>(b))
        } -> std::same_as<Type *>;
    };

template <class T, typename... args_t>
concept C_mjz_temp_type_obj =
    requires {
      typename T::Type;
      requires C_mjz_temp_type_obj_helper<T, typename T::Type,
                                          typename T::Type &,
                                          typename T::Type &&, args_t...>;
    };

template <
    class T, typename Type, typename ref_t = Type &,
    typename rv_ref_t = Type &&, class Size = size_t, class InputIt = Type *,
    class OutputIt = Type *, class NoThrowForwardIt = Type *,
    class ForwardIt = Type *, class BidirIt1 = Type *, class BidirIt2 = Type *,
    class BidirIt3 = Type *,
    class UnaryPredicate = std::function<bool(const ref_t)>, typename... args_t>
concept C_mjz_temp_type_obj_algorithims_warpper_helper =
    requires(T manager, Type &ref, Size n, InputIt II_, OutputIt OI_,
             NoThrowForwardIt NTFI_, ForwardIt FI_, BidirIt1 BI1_,
             BidirIt2 BI2_, BidirIt3 BI3_, UnaryPredicate UPFN_) {
      {
        T::uninitialized_copy_n(static_cast<InputIt>(II_), static_cast<Size>(n),
                                static_cast<NoThrowForwardIt>(NTFI_))
        } -> std::same_as<NoThrowForwardIt>;
      {
        T::uninitialized_fill(static_cast<ForwardIt>(FI_),
                              static_cast<ForwardIt>(FI_),
                              static_cast<const ref_t>(ref))
        } -> std::same_as<void>;
      {
        T::uninitialized_fill_n(static_cast<ForwardIt>(FI_),
                                static_cast<Size>(n),
                                static_cast<const ref_t>(ref))
        } -> std::same_as<ForwardIt>;
      {
        T::uninitialized_fill(static_cast<ForwardIt>(FI_),
                              static_cast<ForwardIt>(FI_),
                              static_cast<rv_ref_t>(ref))
        } -> std::same_as<void>;
      {
        T::uninitialized_fill_n(static_cast<ForwardIt>(FI_),
                                static_cast<Size>(n),
                                static_cast<rv_ref_t>(ref))
        } -> std::same_as<ForwardIt>;
      {
        T::uninitialized_move(static_cast<InputIt>(II_),
                              static_cast<InputIt>(II_),
                              static_cast<NoThrowForwardIt>(NTFI_))
        } -> std::same_as<NoThrowForwardIt>;
      {
        T::uninitialized_move_n(static_cast<InputIt>(II_), static_cast<Size>(n),
                                static_cast<NoThrowForwardIt>(NTFI_))
        } -> std::same_as<std::pair<InputIt, NoThrowForwardIt>>;
      {
        T::uninitialized_copy(static_cast<InputIt>(II_),
                              static_cast<InputIt>(II_),
                              static_cast<NoThrowForwardIt>(NTFI_))
        } -> std::same_as<NoThrowForwardIt>;
      {
        T::uninitialized_default_construct(static_cast<ForwardIt>(FI_),
                                           static_cast<ForwardIt>(FI_))
        } -> std::same_as<void>;
      {
        T::uninitialized_value_construct(static_cast<ForwardIt>(FI_),
                                         static_cast<ForwardIt>(FI_))
        } -> std::same_as<void>;
      {
        T::uninitialized_default_construct_n(static_cast<ForwardIt>(FI_),
                                             static_cast<Size>(n))
        } -> std::same_as<ForwardIt>;
      {
        T::uninitialized_value_construct_n(static_cast<ForwardIt>(FI_),
                                           static_cast<Size>(n))
        } -> std::same_as<ForwardIt>;
      {
        T::move_backward(static_cast<BidirIt1>(BI1_),
                         static_cast<BidirIt2>(BI2_),
                         static_cast<BidirIt3>(BI3_))
        } -> std::same_as<BidirIt3>;
      {
        T::copy_backward(static_cast<BidirIt1>(BI1_),
                         static_cast<BidirIt1>(BI1_),
                         static_cast<BidirIt2>(BI2_))
        } -> std::same_as<BidirIt2>;
      {
        T::uninitialized_move_backward(static_cast<BidirIt1>(BI1_),
                                       static_cast<BidirIt2>(BI2_),
                                       static_cast<BidirIt3>(BI3_))
        } -> std::same_as<BidirIt3>;
      {
        T::uninitialized_copy_backward(static_cast<BidirIt1>(BI1_),
                                       static_cast<BidirIt1>(BI1_),
                                       static_cast<BidirIt2>(BI2_))
        } -> std::same_as<BidirIt2>;
      {
        T::copy(static_cast<InputIt>(II_), static_cast<InputIt>(II_),
                static_cast<OutputIt>(OI_))
        } -> std::same_as<OutputIt>;
      {
        T::copy_if(static_cast<InputIt>(II_), static_cast<InputIt>(II_),
                   static_cast<OutputIt>(OI_),
                   static_cast<UnaryPredicate>(UPFN_))
        } -> std::same_as<OutputIt>;
    };

template <
    class T, class Size = size_t, class InputIt = mjz_get_Type<T> *,
    class OutputIt = mjz_get_Type<T> *,
    class NoThrowForwardIt = mjz_get_Type<T> *,
    class ForwardIt = mjz_get_Type<T> *, class BidirIt1 = mjz_get_Type<T> *,
    class BidirIt2 = mjz_get_Type<T> *, class BidirIt3 = mjz_get_Type<T> *,
    class UnaryPredicate = std::function<bool(const mjz_get_Type<T> &)>,
    typename... args_t>
concept C_mjz_temp_type_obj_algorithims_warpper =
    requires {
      typename T::Type;
      requires C_mjz_temp_type_obj_algorithims_warpper_helper<
          T, typename T::Type, typename T::Type &, typename T::Type &&, Size,
          InputIt, OutputIt, NoThrowForwardIt, ForwardIt, BidirIt1, BidirIt2,
          BidirIt3, UnaryPredicate, args_t...>;
    };

template <typename Type>
struct mjz_internal_obj_manager_template_t {
  using me = mjz_internal_obj_manager_template_t;

 public:
  [[nodiscard]] static constexpr inline Type obj_constructor() { return {}; }

  [[nodiscard]] static constexpr inline Type &&obj_constructor_on(
      Type &&uninitilized_data) {
    return std::move(*construct_at(&uninitilized_data));
  }

  [[nodiscard]] static constexpr inline Type &obj_constructor_on(
      Type &uninitilized_data) {
    return *construct_at(&uninitilized_data);
  }

  [[nodiscard]] static constexpr inline Type *obj_constructor_on(
      Type *uninitilized_data) {
    return construct_at(uninitilized_data);
  }

  static constexpr inline Type *construct_at(Type *ptr) noexcept {
    *ptr = Type{};
    return ptr;
  }

  static constexpr inline Type *construct_array_at(Type *dest,
                                                   size_t n) noexcept {
    Type *ptr{dest};
    Type *end{dest + n};
    while (ptr < end) construct_at(ptr++);

    return dest;
  }

  template <typename args_t>
  static constexpr inline Type *construct_at(Type *ptr,
                                             args_t &&args) noexcept {
    *ptr = Type(std::forward<args_t>(args));
    return ptr;
  }
  template <typename args_t>
  static constexpr inline Type *construct_array_at(Type *dest, size_t n,
                                                   args_t &&args) noexcept {
    Type *ptr{dest};
    Type *end{dest + n - 1};
    while (ptr < end) construct_at(ptr++, args);
    end++;
    if (ptr < end) construct_at(ptr, std::forward<args_t>(args));

    return dest;
  }
  static constexpr inline bool destruct_array_at(Type *dest,
                                                 size_t n) noexcept {
    Type *r_end = dest;
    Type *ptr = dest + n;

    while (r_end < ptr) destroy_at(--ptr);

    return true;
  }
  template <typename args_t>
  [[nodiscard]] static constexpr inline Type obj_constructor(args_t &&args) {
    return Type(std::forward<args_t>(args));
  }
  template <typename args_t>
  [[nodiscard]] static constexpr inline Type &&obj_constructor_on(
      Type &&uninitilized_object, args_t &&args) {
    return std::move(
        *construct_at(&uninitilized_object, std::forward<args_t>(args)));
  }
  template <typename args_t>
  [[nodiscard]] static constexpr inline Type &obj_constructor_on(
      Type &uninitilized_object, args_t &&args) {
    return *construct_at(&uninitilized_object, std::forward<args_t>(args));
  }
  template <typename args_t>
  [[nodiscard]] static constexpr inline Type *obj_constructor_on(
      Type *uninitilized_object, args_t &&args) {
    return construct_at(uninitilized_object, std::forward<args_t>(args));
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

  template <bool destroy_on_destruction = true,
            bool create_on_construction = true>
  union simple_unsafe_init_obj_wrpr {
    constexpr inline simple_unsafe_init_obj_wrpr() {
      if constexpr (create_on_construction) unsafe_create();
    }
    template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
    constexpr inline simple_unsafe_init_obj_wrpr(
        mjz_no_init_uw_special_arg<
            init_if_iam_not_zero_the_base_with_me_minus_one>)
      requires(init_if_iam_not_zero_the_base_with_me_minus_one != 0)
    {
      static_assert(
          requires {
            unsafe_create(
                mjz_no_init_uw_special_arg<
                    init_if_iam_not_zero_the_base_with_me_minus_one - 1>());
          });
      unsafe_create(mjz_no_init_uw_special_arg<
                    init_if_iam_not_zero_the_base_with_me_minus_one - 1>());
    }
    constexpr inline simple_unsafe_init_obj_wrpr(
        mjz_no_init_uw_special_arg<0>) {}
    template <class T0>
    constexpr inline simple_unsafe_init_obj_wrpr(T0 &&arg0) {
      unsafe_create(std::forward<T0>(arg0));
    }
    constexpr inline ~simple_unsafe_init_obj_wrpr() {
      if constexpr (destroy_on_destruction) unsafe_destroy();
    }
    constexpr inline Type *ptr() { return me::addressof(obj); }
    constexpr inline Type &get() { return *ptr(); }
    constexpr inline Type &operator()() { return get(); }
    constexpr inline Type &operator*() { return get(); }
    constexpr inline Type *operator->() { return ptr(); }
    constexpr inline const Type *ptr() const { return me::addressof(obj); }
    constexpr inline const Type &get() const { return *ptr(); }
    constexpr inline const Type &operator()() const { return get(); }
    constexpr inline const Type &operator*() const { return get(); }
    constexpr inline const Type *operator->() const { return ptr(); }
    constexpr inline static bool has_destroy() {
      return destroy_on_destruction;
    }

    template <class T>
    constexpr inline Type *unsafe_create(T &&args) {
      return obj_constructor_on(ptr(), std::forward<T>(args));
    }
    constexpr inline Type *unsafe_create() { return obj_constructor_on(ptr()); }

    constexpr inline void unsafe_destroy() { obj_destructor_on(ptr()); }

   private:
    Type obj;
    alignas(Type) volatile char NO_USE_NO_INITILIZATION_[sizeof(Type)];
  };

  // template <  destroy_on_destruction = false,create_on_construction=false>
  using simple_unsafe_init_obj_wrpr_false_false =
      simple_unsafe_init_obj_wrpr<false, false>;
  // template <  destroy_on_destruction = true,create_on_construction=false>
  using simple_unsafe_init_obj_wrpr_true_false =
      simple_unsafe_init_obj_wrpr<true, false>;
  // template <bool destroy_on_destruction = false>
  using simple_unsafe_init_obj_wrpr_false = simple_unsafe_init_obj_wrpr<false>;
  // template <bool destroy_on_destruction = true>
  using simple_unsafe_init_obj_wrpr_true = simple_unsafe_init_obj_wrpr<true>;
  template <class T, class Args>
  static constexpr inline void construct(me &a, T *p, Args &&args) {
    a.construct_at(p, std::forward<Args>(args));
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
  static constexpr inline Type &obj_temp_copy_to_obj(Type &dest,
                                                     const Type &&src) {
    return dest = (std::move(src));
  }
  static constexpr inline Type &obj_go_to_obj(Type &dest, const Type &&src) {
    return obj_temp_copy_to_obj(dest, std::move(src));
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
  template <typename Args>
  static constexpr inline Type &obj_equals(Type &dest, Args &&args) {
    return dest = std::forward<Args>(args);
  }

 public:
  static constexpr inline const Type *addressof(const Type &&obj) = delete;
  static constexpr inline Type *addressof(Type &&obj) = delete;

  static constexpr inline Type *addressof(Type &obj) { return &obj; }
  static constexpr inline const Type *addressof(const Type &obj) {
    return &obj;
  }
  static constexpr inline Type *to_address(Type *p) noexcept { return p; }
  static constexpr inline const Type *to_address(const Type &obj) noexcept {
    return &obj;
  }
  static constexpr inline const Type *to_address(const Type *p) noexcept {
    return p;
  }
  static constexpr inline Type *to_address(Type &obj) noexcept { return &obj; }
  static constexpr inline Type *pointer_to(Type &r) noexcept { return &r; }
  static constexpr inline bool destroy_at(Type *ptr) noexcept {
    *ptr = {};
    return true;
  }
  static constexpr inline bool destroy_at(Type &ptr) noexcept {
    ptr = {};
    return true;
  }
  static constexpr inline void destroy(me &a, Type *p) { a.destroy_at(p); }
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
template <typename Type_>
struct mjz_non_internal_obj_manager_template_t {
  using me = mjz_non_internal_obj_manager_template_t;
  using Type = std::remove_cvref_t<Type_>;

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
  static constexpr inline bool destruct_array_at(Type *dest,
                                                 size_t n) noexcept {
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
  static constexpr inline Type &obj_constructor_on(Type &uninitilized_object,
                                                   args_t &&...args) {
    return *construct_at(addressof(uninitilized_object),
                         std::forward<args_t>(args)...);
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

  template <bool destroy_on_destruction = true,
            bool create_on_construction = false>
  union simple_unsafe_init_obj_wrpr {
    constexpr inline simple_unsafe_init_obj_wrpr() {
      if constexpr (create_on_construction) unsafe_create();
    }
    template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
    constexpr inline simple_unsafe_init_obj_wrpr(
        mjz_no_init_uw_special_arg<
            init_if_iam_not_zero_the_base_with_me_minus_one>)
      requires(init_if_iam_not_zero_the_base_with_me_minus_one != 0)
    {
      unsafe_create(mjz_no_init_uw_special_arg<
                    init_if_iam_not_zero_the_base_with_me_minus_one - 1>());
    }
    constexpr inline simple_unsafe_init_obj_wrpr(
        mjz_no_init_uw_special_arg<0>) {}
    template <class T0, class... Ts>
    constexpr inline simple_unsafe_init_obj_wrpr(T0 &&arg0, Ts &&...args) {
      unsafe_create(std::forward<T0>(arg0), std::forward<Ts>(args)...);
    }
    constexpr inline ~simple_unsafe_init_obj_wrpr() {
      if constexpr (destroy_on_destruction) unsafe_destroy();
    }
    constexpr inline Type *ptr() { return me::addressof(obj); }
    constexpr inline Type &get() { return *ptr(); }
    constexpr inline Type &operator()() { return get(); }
    constexpr inline Type &operator*() { return get(); }
    constexpr inline Type *operator->() { return ptr(); }
    constexpr inline const Type *ptr() const { return me::addressof(obj); }
    constexpr inline const Type &get() const { return *ptr(); }
    constexpr inline const Type &operator()() const { return get(); }
    constexpr inline const Type &operator*() const { return get(); }
    constexpr inline const Type *operator->() const { return ptr(); }
    constexpr inline const static bool has_destroy() {
      return destroy_on_destruction;
    }
    template <class... Ts>
    constexpr inline Type *unsafe_create(Ts &&...args) {
      return obj_constructor_on(ptr(), std::forward<Ts>(args)...);
    }

    constexpr inline void unsafe_destroy() { obj_destructor_on(ptr()); }

   private:
    Type obj;
    alignas(Type) volatile char NO_USE_NO_INITILIZATION_[sizeof(Type)];
  };
  // template <  destroy_on_destruction = false,create_on_construction=false>
  using simple_unsafe_init_obj_wrpr_false_false =
      simple_unsafe_init_obj_wrpr<false, false>;
  // template <  destroy_on_destruction = true,create_on_construction=false>
  using simple_unsafe_init_obj_wrpr_true_false =
      simple_unsafe_init_obj_wrpr<true, false>;
  // template <bool destroy_on_destruction = false>
  using simple_unsafe_init_obj_wrpr_false = simple_unsafe_init_obj_wrpr<false>;
  // template <bool destroy_on_destruction = true>
  using simple_unsafe_init_obj_wrpr_true = simple_unsafe_init_obj_wrpr<true>;
  template <class T, class... Args>
  static constexpr inline void construct(me &a, T *p, Args &&...args) {
    a.construct_at(p, std::forward<Args>(args)...);
  }
  static constexpr inline Type &obj_move_to_obj(Type &dest, Type &&src) {
    return dest = (std::move(src));
  }
  static constexpr inline Type &obj_temp_copy_to_obj(Type &dest,
                                                     const Type &&src) {
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
  static constexpr inline Type &obj_go_to_obj(Type &dest, const Type &&src) {
    return obj_temp_copy_to_obj(dest, std::move(src));
  }
  template <typename... Args>
  static constexpr inline Type &obj_equals(Type &dest, Args &&...args) {
    return dest.operator=(std::forward<Args>(args)...);
  }
  template <typename Args>
  static constexpr inline Type &obj_equals(Type &dest, Args &&args) {
    return dest = (std::forward<Args>(args));
  }

 public:
  static constexpr inline const Type *addressof(const Type &&obj) = delete;
  static constexpr inline Type *addressof(Type &&obj) = delete;

  static constexpr inline Type *addressof(Type &obj) {
    return std::addressof(obj);
  }
  static constexpr inline const Type *addressof(const Type &obj) {
    return std::addressof(obj);
  }
  static constexpr inline Type *to_address(Type *p) noexcept { return p; }
  static constexpr inline const Type *to_address(const Type &obj) noexcept {
    return addressof(obj);
  }
  static constexpr inline const Type *to_address(const Type *p) noexcept {
    return p;
  }
  static constexpr inline Type *to_address(Type &obj) noexcept {
    return addressof(obj);
  }
  static constexpr inline Type *pointer_to(Type &r) noexcept {
    return addressof(r);
  }
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
  static constexpr inline void destroy(me &a, Type *p) { a.destroy_at(p); }
  static constexpr inline void destroy(Type *p) { me::destroy_at(p); }
  template <class ForwardIt, class Size>
  static constexpr inline ForwardIt destroy_n(ForwardIt first, Size n) {
    for (; n > 0; (void)++first, --n) me::destroy_at(addressof(*first));
    return first;
  }
  template <class ForwardIt>
  static constexpr inline void destroy(ForwardIt first, ForwardIt last) {
    for (; first != last; ++first) me::destroy_at(addressof(*first));
  }
};

template <bool B, typename Type>
struct mjz_obj_manager_template_t_helper {
  static_assert(B);
  static_assert(!B);
};
template <typename Type>
struct mjz_obj_manager_template_t_helper<false, Type>
    : public mjz_non_internal_obj_manager_template_t<Type> {};

template <typename Type>
struct mjz_obj_manager_template_t_helper<true, Type>
    : public mjz_internal_obj_manager_template_t<Type> {};

template <typename Type, typename R_T>
struct mjz_obj_manager_template_struct_helper_t
    : public mjz_obj_manager_template_t_helper<
          std::conjunction_v<

              std::disjunction<std::is_fundamental<Type>,
                               std::is_pointer<Type>>,
              std::negation<std::disjunction<
                  std::is_class<Type>, std::has_virtual_destructor<Type>,
                  std::is_union<Type>,
                  std::has_unique_object_representations<Type>,
                  std::is_abstract<Type>, std::is_enum<Type>,
                  std::is_polymorphic<Type>>>>,
          R_T> {
  static_assert(!std::is_array_v<Type>);
};
template <typename T>
struct mjz_obj_manager_template_t
    : public mjz_obj_manager_template_struct_helper_t<std::remove_cvref_t<T>,
                                                      T> {
  using Type = std::remove_cvref_t<T>;
  static_assert(!std::is_array_v<Type>);
  static const constexpr size_t alignment = alignof(Type);
  static const constexpr size_t size_of_type_v = sizeof(Type);
  constexpr static inline size_t size_of_type() { return size_of_type_v; }
  struct alignment_t alignas(alignment) {
    alignas(alignment) uint8_t a[size_of_type_v]{};
  };
  struct Alignment_t alignas(Type) {
    alignas(Type) uint8_t a[sizeof(Type)]{};
  };
};

template <typename Type, bool destroy_on_destruction,
          bool create_on_construction = true>
using mjz_simple_unsafe_init_obj_wrpr_t = typename mjz_obj_manager_template_t<
    Type>::template simple_unsafe_init_obj_wrpr<destroy_on_destruction,
                                                create_on_construction>;

template <typename Type,
          C_mjz_obj_manager my_constructor = mjz_obj_manager_template_t<Type>>
// namespace mjz_ard
struct mjz_temp_type_obj_creator_warpper_t : public my_constructor {
  using me = mjz_temp_type_obj_creator_warpper_t;
  static constexpr inline void swap(Type &a, Type &b) {
    alignas(my_constructor::alignment)
        uint8_t buf[my_constructor::size_of_type_v];
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
          C_mjz_obj_manager my_constructor = mjz_obj_manager_template_t<Type>>
struct mjz_temp_type_obj_algorithims_warpper_t
    : public mjz_temp_type_obj_creator_warpper_t<Type, my_constructor> {
  using me = mjz_temp_type_obj_algorithims_warpper_t;
  template <typename TTT>
  static constexpr inline auto addressof(const TTT &obj) {
    return my_constructor::addressof(obj);
  }
  template <typename TTT>
  static constexpr inline auto addressof(const TTT &&obj) = delete;
  template <typename TTT>
  static constexpr inline auto addressof(TTT &obj) {
    return my_constructor::addressof(obj);
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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
      Throw<void>();
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

  template <class BidirIt1, class BidirIt2, class BidirIt3>
  static constexpr inline BidirIt3 uninitialized_move_backward(
      BidirIt1 first, BidirIt2 last, BidirIt3 d_last) {
    while (first != last) me::construct_at((--d_last), std::move(*(--last)));
    return d_last;
  }
  template <class BidirIt1, class BidirIt2>
  static constexpr inline BidirIt2 uninitialized_copy_backward(
      BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
    while (first != last) construct_at((--d_last), std::forward(*(--last)));
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

template <typename T>
struct mjz_inherited_container : private T {
  inline constexpr T &mjz_inherited_container_get_contained() { return *this; }
  inline constexpr

      const T &
      mjz_inherited_container_get_contained() const {
    return *this;
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
constexpr mjz_ard::C_realloc_free_package_example &&
get_CPP_local_global_allocator() {
  return mjz_ard::C_realloc_free_package_example();
}
[[nodiscard]] void *operator new(size_t m_size) {
  void *p = get_CPP_local_global_allocator().realloc(0, m_size);
  if (p) return p;
  Throw<std::runtime_error>("no memory left ");
}
[[nodiscard]] void *operator new[](size_t m_size) {
  void *p = get_CPP_local_global_allocator().realloc(0, m_size);
  if (p) return p;
  Throw<std::runtime_error>("no memory left ");
}
void operator delete(void *p) { get_CPP_local_global_allocator().free(p); }
void operator delete[](void *p) { get_CPP_local_global_allocator().free(p); }
void operator delete(void *p, size_t) {
  get_CPP_local_global_allocator().free(p);
}
void operator delete[](void *p, size_t) {
  get_CPP_local_global_allocator().free(p);
}
namespace mjz_ard {
#endif  // global_mjz_areana_allocator_on

template <class Type, size_t m_Size>
union M_DATA_U {
 public:
  M_DATA_U() {}   // no object initilization
  ~M_DATA_U() {}  // no object deinitilization
  M_DATA_U(const M_DATA_U &) = delete;
  M_DATA_U &operator=(const M_DATA_U &) = delete;
  M_DATA_U(const M_DATA_U &&) = delete;
  M_DATA_U &operator=(const M_DATA_U &&) = delete;
  M_DATA_U(M_DATA_U &) = delete;
  M_DATA_U &operator=(M_DATA_U &) = delete;
  M_DATA_U(M_DATA_U &&) = delete;
  M_DATA_U &operator=(M_DATA_U &&) = delete;
  char first;
  char first_1;
  char first_2;
  char first_3;
  Type m_elements[m_Size];

  // just  object initilization
  template <typename... args_t>
  constexpr inline M_DATA_U(args_t &&...args)
      : m_elements{std::forward<args_t>(args)...} {}
};

// this will crash the program
[[noreturn]] static inline void trap_crash(void) noexcept(false) {
  *(volatile char *)nullptr = 0;  // address 0 is invalid
  // this will crash the program
}

namespace smart_ptr {
template <typename T>
union data_storage {
  using Type = T;
  char f;
  Type object;
  inline data_storage() {}  // no initilization

  inline ~data_storage() {}  // no deinitilization

  inline operator Type &() & { return *((Type *)&f); }

  inline operator Type &&() && { return (Type &&) * ((Type *)&f); }

  inline operator const Type &() & { return *((const Type *)&f); }

  inline operator const Type &&() && {
    return (const Type &&)*((const Type *)&f);
  }

  inline operator const Type &() const & { return *((const Type *)&f); }

  inline operator const Type &&() const && {
    return (const Type &&)*((const Type *)&f);
  }

  inline operator Type *() & { return ((Type *)&f); }

  inline operator const Type *() & { return ((const Type *)&f); }

  inline operator const Type *() const & { return ((const Type *)&f); }

  inline Type &operator*() & { return operator Type &(); }

  inline const Type &operator*() const & { return *((const Type *)&f); }

  inline Type *operator->() & { return operator Type *(); }

  inline const Type *operator->() const & { return operator const Type *(); }

  inline Type *ptr() & { return operator Type *(); }

  inline Type &get() & { return operator Type &(); }

  inline const Type *ptr() const & { return operator const Type *(); }

  inline const Type &get() const & { return *((const Type *)&f); }

  inline Type &&get() && { return (Type &&) * ((Type *)&f); }

  inline const Type &&get() const && {
    return (const Type &&)*((const Type *)&f);
  }
};
template <typename T>
struct smart_data_structure_base {
  using Type = T;
  using size_lt = uint32_t;
  size_lt shared_ptr_ref_count{};
  size_lt weak_ptr_ref_count{};
  data_storage<T> *data;
  // a char will exist in both derived classes at this spot
  // like this  char first_of_obj_if_allocted_both_in_one_go = pure variable;
  smart_data_structure_base() {}
  ~smart_data_structure_base() {}
};
template <typename T>
struct smart_data_structure_sepereat : public smart_data_structure_base<T> {
  char first_of_obj_if_allocted_both_in_one_go{};
};
template <typename T>
struct smart_data_structure_both : public smart_data_structure_base<T> {
  union data_storage_t {
    data_storage_t() {}   // no initilization of T
    ~data_storage_t() {}  // no deinitilization of T
    T Data_of_var;
    char first_of_obj_if_allocted_both_in_one_go;
  };
  data_storage_t buff;
};
template <typename T>
bool is_sepereat(const smart_data_structure_base<T> &obj) {
  static_assert(offsetof(smart_data_structure_both<T>,
                         buff.first_of_obj_if_allocted_both_in_one_go) ==
                offsetof(smart_data_structure_sepereat<T>,
                         first_of_obj_if_allocted_both_in_one_go));

  return ((void *)(&((smart_data_structure_sepereat<T> *)&obj)
                        ->first_of_obj_if_allocted_both_in_one_go)) !=
         (void *)obj.data;
}

template <typename T>
class smart_ptr_base_template_t : private mjz_allocator_warpper<T> {
 private:
  void deallocate_seperate() {
    if (!m_data) return;
    if (m_data->data) {
      try {
        operator delete(m_data->data);  // no destructor called
      } catch (...) {
      }
    }
    try {
      delete m_data;
    } catch (...) {
    }
    m_data = 0;
  }
  void allocate_seperate(data_storage<T> *p) {
    if (m_data) trap_crash();  // if has object ??
    try {
      m_data = new DB_t;
    } catch (...) {
      m_data = 0;
    }
    if (!m_data) return;
    m_data->data = p;
  }
  void deallocate_both() {
    if (!m_data) return;
    try {
      delete ((smart_data_structure_both<T> *)(m_data));
    } catch (...) {
    }
    m_data = 0;
  }
  void allocate_both() {
    if (m_data) trap_crash();  // if has object ??
    try {
      m_data = new smart_data_structure_both<T>;
    } catch (...) {
      m_data = 0;
    }
    if (!m_data) return;
    try {
      m_data->data =
          (decltype(m_data
                        ->data))(((char *)m_data) +
                                 offsetof(
                                     smart_data_structure_both<T>,
                                     buff.first_of_obj_if_allocted_both_in_one_go));
    } catch (...) {
      m_data->data = 0;
    }

    if (!m_data->data) {
      deallocate_both();
    }
  }

 public:
  void deallocate() {
    if (is_sepereat(*m_data)) return deallocate_seperate();
    return deallocate_both();
  }
  void allocate(data_storage<T> *p = 0) {
    if (p) return allocate_seperate(p);
    return allocate_both();
  }

 public:
  using DB_t = smart_data_structure_base<T>;
  DB_t *m_data{};
  using Type = T;

  void destroy() {
    if (!m_data) return;
    if (!m_data->data) return;
    get_object_allocator().destroy_at(m_data->data->ptr());
  }
  template <typename... args_t>
  bool create(args_t &&...args) {
    return create_with([&, this](Type *p) {
      get_object_allocator().construct_at(p, std::forward<args_t>(args)...);
      return true;
    });
  }
  bool create_with(std::function<bool(Type *)> FN_construct_at) {
    if (!m_data) return false;
    bool sucsses_full{false};
    try {
      sucsses_full = FN_construct_at(m_data->data->ptr());
    } catch (...) {
    }
    return sucsses_full;
  }

  void weak_add(DB_t *p) {
    if (!p) return;
    m_data = p;
    m_data->weak_ptr_ref_count++;
  }
  bool sheared_add(DB_t *p) {
    if (!p) return false;
    m_data = p;
    m_data->shared_ptr_ref_count++;
    return true;
  }

 protected:
  smart_ptr_base_template_t() {}
  ~smart_ptr_base_template_t() {}
  mjz_temp_type_allocator_warpper_t<T> &get_object_allocator() { return *this; }
  const mjz_temp_type_allocator_warpper_t<T> &get_object_allocator() const {
    return *this;
  }
  void update_weak() {
    if (!m_data) return;
    if (!m_data->shared_ptr_ref_count) {
      weak_free();
    }
  }
  void weak_set(DB_t *p) { weak_add(p); }

  void weak_free() {
    if (!m_data) return;
    DB_t &data = *m_data;
    data.weak_ptr_ref_count--;
    if (data.shared_ptr_ref_count) {
    } else if (!data.weak_ptr_ref_count) {
      deallocate();
    }

    m_data = nullptr;
  }
  void sheared_free() {
    if (!m_data) return;
    DB_t &data = *m_data;

    data.shared_ptr_ref_count--;
    if (!data.shared_ptr_ref_count) {
      destroy();
      if (!data.weak_ptr_ref_count) {
        deallocate();
      }
    }
    m_data = nullptr;
  }
};
template <class T>
class weak_ptr_template_t;
template <typename T>
class sheared_ptr_template_t : protected smart_ptr_base_template_t<T> {
  using DB_t = typename smart_ptr_base_template_t<T>::DB_t;
  using Type = T;
  DB_t *&ptr() { return this->m_data; }
  const DB_t *const &ptr() const { return this->m_data; }

 public:
  sheared_ptr_template_t() : smart_ptr_base_template_t<T>() {}
  ~sheared_ptr_template_t() { operator~(); }
  void operator~() { this->sheared_free(); }
  sheared_ptr_template_t(sheared_ptr_template_t &&obj) {
    ptr() = std::exchange(obj.ptr(), {0});
  }
  sheared_ptr_template_t(const sheared_ptr_template_t &&obj) {
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  sheared_ptr_template_t(sheared_ptr_template_t &obj) {
    this->sheared_add(obj.ptr());
  }
  sheared_ptr_template_t(const sheared_ptr_template_t &obj) {
    this->sheared_add(remove_const(obj.ptr()));
  }

  sheared_ptr_template_t &operator=(sheared_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(obj.ptr(), {0});
    return *this;
  }
  sheared_ptr_template_t &operator=(const sheared_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  sheared_ptr_template_t &operator=(sheared_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->sheared_add(obj.ptr());
    return *this;
  }
  sheared_ptr_template_t &operator=(const sheared_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->sheared_add(remove_const(obj.ptr()));
    return *this;
  }

  void reset() noexcept { operator~(); }
  template <class T_der>
  void reset(smart_data_structure_base<T_der> *p)
    requires(std::is_base_of<T, T_der>)
  {
    operator~();
    if (p) {
      ptr() = (smart_data_structure_base<T> *)
          p;  // dont worry its just a {s_lt,s_lt,der ptr}
    }
  }
  T *get() {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  T &operator*() {
    return *get();  // may derefrence null;
  }
  T *operator->() {
    return get();  // may derefrence null;
  }
  long use_count() const noexcept {
    if (!ptr()) return 0;
    return ptr()->shared_ptr_ref_count;
  }

  const T *get() const {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  const T &operator*() const {
    return *get();  // may derefrence null;
  }
  const T *operator->() const {
    return get();  // may derefrence null;
  }

  explicit operator bool() const { return !!ptr(); }
  bool operator!() const { return !ptr(); }

 public:
  template <typename... args_t>
  [[nodiscard]] static sheared_ptr_template_t mjz_make_sheared(
      args_t &&...args) {
    sheared_ptr_template_t ret;
    ret.allocate();
    ret.create(std::forward<args_t>(args)...);
    ret.sheared_add(ret.m_data);
    return ret;
  }

  friend class weak_ptr_template_t<T>;
};

template <typename T, typename... args_t>
sheared_ptr_template_t<T> make_sheared(args_t &&...args) {
  return sheared_ptr_template_t<T>::mjz_make_sheared(
      std::forward<args_t>(args)...);
}

template <typename T>
class weak_ptr_template_t : protected smart_ptr_base_template_t<T> {
  using DB_t = typename smart_ptr_base_template_t<T>::DB_t;
  using Type = T;
  friend class sheared_ptr_template_t<T>;

  DB_t *&ptr() {
    UW();
    return this->m_data;
  }
  const DB_t *const &ptr() const {
    UW();
    return this->m_data;
  }
  void UW() const  // not really
  {
    remove_const(this)->update_weak();
  }

 public:
  weak_ptr_template_t() : smart_ptr_base_template_t<T>() {}
  ~weak_ptr_template_t() { operator~(); }
  void operator~() { this->weak_free(); }
  weak_ptr_template_t(weak_ptr_template_t &&obj) {
    ptr() = std::exchange(obj.ptr(), {0});
  }
  weak_ptr_template_t(const weak_ptr_template_t &&obj) {
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  weak_ptr_template_t(weak_ptr_template_t &obj) { this->weak_add(obj.ptr()); }
  weak_ptr_template_t(const weak_ptr_template_t &obj) {
    this->weak_add(remove_const(obj.ptr()));
  }

  weak_ptr_template_t(const sheared_ptr_template_t<T> &obj) {
    this->weak_add(remove_const(obj.ptr()));
  }

  weak_ptr_template_t &operator=(weak_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(obj.ptr(), {0});
    return *this;
  }
  weak_ptr_template_t &operator=(const weak_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  weak_ptr_template_t &operator=(weak_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(obj.ptr());
    return *this;
  }
  weak_ptr_template_t &operator=(const weak_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(remove_const(obj.ptr()));
    return *this;
  }

  weak_ptr_template_t &operator=(const sheared_ptr_template_t<T> &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(remove_const(obj.ptr()));
    return *this;
  }

  void reset() noexcept { operator~(); }
  template <class T_der>
  void reset(smart_data_structure_base<T_der> *p)
    requires(std::is_base_of<T, T_der>)
  {
    operator~();
    if (p) {
      ptr() = (smart_data_structure_base<T> *)
          p;  // dont worry its just a {s_lt,s_lt,der ptr}
    }
  }
  T *get() {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  T &operator*() {
    return *get();  // may derefrence null;
  }
  T *operator->() {
    return get();  // may derefrence null;
  }

  const T *get() const {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  const T &operator*() const {
    return *get();  // may derefrence null;
  }
  const T *operator->() const {
    return get();  // may derefrence null;
  }

  explicit operator bool() const { return !!ptr(); }
  bool operator!() const { return !ptr(); }
};

};  // namespace smart_ptr

/*
0 for little
1 for big
*/
using endian_type = std::endian;
template <typename T, endian_type normal_class_endianness>
class get_endianed_data {
  static_assert(std::is_fundamental_v<T>);

 private:
  union {
    int8_t m_ca[sizeof(T)];
    uint8_t m_ua[sizeof(T)];
    T m_value;
  };

 public:
  using me = get_endianed_data<T, normal_class_endianness>;
  constexpr static const bool change_from_native_endian =
      (sizeof(T) != 1) && (normal_class_endianness != endian_type::native);
  constexpr static endian_type get_default_endian() {
    return normal_class_endianness;
  }
  inline constexpr static void mem_rev(int8_t *val) {
    int8_t *end_p = val + (sizeof(T) - 1);
    int8_t *beg_p = val;
    while (beg_p < end_p) {
      int8_t buf = *beg_p;
      *beg_p++ = *end_p;
      *end_p-- = buf;
    }
  }
  inline constexpr T mem_rev_cpy() const {
    union U {
      U(T o) : value(o) {}
      ~U() { value.~T(); }
      int8_t ca[sizeof(T)];
      T value;
    } o(m_value);
    mem_rev(o.ca);
    return o.value;
  }

  inline constexpr get_endianed_data(T value = T()) {
    m_value = value;
    if constexpr (!change_from_native_endian) return;
    mem_rev(m_ca);
  }
  inline constexpr get_endianed_data(get_endianed_data &&) = default;
  inline constexpr get_endianed_data(get_endianed_data &) = default;
  inline constexpr get_endianed_data(const get_endianed_data &) = default;
  inline constexpr get_endianed_data &operator=(get_endianed_data &&) = default;
  inline constexpr get_endianed_data &operator=(get_endianed_data &) = default;
  inline constexpr get_endianed_data &operator=(const get_endianed_data &) =
      default;
  inline constexpr T get_native() const {
    if constexpr (change_from_native_endian) return mem_rev_cpy();
    return m_value;
  }
  inline constexpr T get_big() const {
    if constexpr (get_default_endian() == endian_type::big) return m_value;
    return mem_rev_cpy();
  }
  inline constexpr T get_lit() const {
    if constexpr (get_default_endian() == endian_type::little) return m_value;
    return mem_rev_cpy();
  }
  inline constexpr T get_default() const { return m_value; }
  inline constexpr void set_default(T val = T()) const { m_value = val; }
  inline constexpr void set_native(T val = T()) const {
    m_value = val;
    if constexpr (change_from_native_endian) mem_rev(m_ca);
  }

  inline constexpr void set_lit(T val = T()) const {
    m_value = val;
    if constexpr (get_default_endian() != endian_type::little) mem_rev(m_ca);
  }
  inline constexpr void set_big(T val = T()) const {
    m_value = val;
    if constexpr (get_default_endian() != endian_type::big) mem_rev(m_ca);
  }
#define do_work_get_defualt_endian_data(x)      \
  do {                                          \
    if constexpr (!change_from_native_endian) { \
      m_value x;                                \
      return *this;                             \
    }                                           \
    mem_rev_cpy();                              \
    m_value x;                                  \
    mem_rev_cpy();                              \
    return *this;                               \
                                                \
  } while (0)

#define do_work_get_defualt_endian_data_per(x)  \
  do {                                          \
    me per = *this;                             \
                                                \
    if constexpr (!change_from_native_endian) { \
      m_value x;                                \
      return per;                               \
    }                                           \
    mem_rev_cpy();                              \
    m_value x;                                  \
    mem_rev_cpy();                              \
    return per;                                 \
                                                \
  } while (0)
#define do_work_get_defualt_endian_data_from(x) \
  do {                                          \
    if constexpr (!change_from_native_endian) { \
      return {m_value x};                       \
    }                                           \
    return {get_native() x};                    \
                                                \
  } while (0)

#define obj_do_work_get_defualt_endian_data_from(x) \
  do {                                              \
    if constexpr (!change_from_native_endian) {     \
      return {obj.m_value x};                       \
    }                                               \
    return {obj.get_native() x};                    \
                                                    \
  } while (0)

#define obj_do_work_get_defualt_endian_data_from_native_lhs(x) \
  do {                                                         \
    if constexpr (!change_from_native_endian) {                \
      return {obj.m_value x lhs.get_native()};                 \
    }                                                          \
    return {obj.get_native() x lhs.get_native()};              \
                                                               \
  } while (0)

#define obj_do_bitwise_work_from_native_lhs(x) \
  do {                                         \
    me t;                                      \
    t.m_value = {obj.m_value x lhs.m_value};   \
    return t.get_native();                     \
  } while (0)

  inline constexpr get_endianed_data &operator++() {
    do_work_get_defualt_endian_data(++);
  }
  inline constexpr get_endianed_data &operator--() {
    do_work_get_defualt_endian_data(--);
  }
  inline constexpr get_endianed_data &operator+=(T x) {
    do_work_get_defualt_endian_data(+= x);
  }
  inline constexpr get_endianed_data &operator-=(T x) {
    do_work_get_defualt_endian_data(-= x);
  }
  inline constexpr get_endianed_data &operator*=(T x) {
    do_work_get_defualt_endian_data(*= x);
  }
  inline constexpr get_endianed_data &operator/=(T x) {
    do_work_get_defualt_endian_data(/= x);
  }
  inline constexpr get_endianed_data &operator%=(T x) {
    do_work_get_defualt_endian_data(%= x);
  }
  inline constexpr get_endianed_data &operator^=(T x) {
    do_work_get_defualt_endian_data(^= x);
  }
  inline constexpr get_endianed_data &operator&=(T x) {
    do_work_get_defualt_endian_data(&= x);
  }
  inline constexpr get_endianed_data &operator|=(T x) {
    do_work_get_defualt_endian_data(|= x);
  }
  template <typename U>
  inline constexpr get_endianed_data &operator<<=(U x) {
    do_work_get_defualt_endian_data(<<= x);
  }
  template <typename U>
  inline constexpr get_endianed_data &operator>>=(U x) {
    do_work_get_defualt_endian_data(>>= x);
  }
  template <typename U>
  inline constexpr T operator<<(U x) {
    do_work_get_defualt_endian_data_from(<< x);
  }
  template <typename U>
  inline constexpr T operator>>(U x) {
    do_work_get_defualt_endian_data_from(>> x);
  }
  template <typename U>
  inline constexpr T operator+(U x) {
    do_work_get_defualt_endian_data_from(+x);
  }
  template <typename U>
  inline constexpr T operator-(U x) {
    do_work_get_defualt_endian_data_from(-x);
  }
  template <typename U>
  inline constexpr T operator*(U x) {
    do_work_get_defualt_endian_data_from(*x);
  }
  template <typename U>
  inline constexpr T operator/(U x) {
    do_work_get_defualt_endian_data_from(/ x);
  }
  template <typename U>
  inline constexpr T operator|(U x) {
    do_work_get_defualt_endian_data_from(| x);
  }
  template <typename U>
  inline constexpr T operator&(U x) {
    do_work_get_defualt_endian_data_from(&x);
  }
  template <typename U>
  inline constexpr T operator^(U x) {
    do_work_get_defualt_endian_data_from(^x);
  }
  template <typename U>
  inline constexpr T operator%(U x) {
    do_work_get_defualt_endian_data_from(% x);
  }
  template <typename U>
  inline friend constexpr T operator<<(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(<< x);
  }
  template <typename U>
  inline friend constexpr T operator>>(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(>> x);
  }
  template <typename U>
  inline friend constexpr T operator+(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(+x);
  }
  template <typename U>
  inline friend constexpr T operator-(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(-x);
  }
  template <typename U>
  inline friend constexpr T operator*(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(*x);
  }
  template <typename U>
  inline friend constexpr T operator/(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(/ x);
  }
  template <typename U>
  inline friend constexpr T operator|(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(| x);
  }
  template <typename U>
  inline friend constexpr T operator&(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(&x);
  }
  template <typename U>
  inline friend constexpr T operator^(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(^x);
  }
  template <typename U>
  inline friend constexpr T operator%(U x, const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from(% x);
  }

  inline friend constexpr T operator<<(const get_endianed_data &lhs,
                                       const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(<<);
  }

  inline friend constexpr T operator>>(const get_endianed_data &lhs,
                                       const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(>>);
  }

  inline friend constexpr T operator+(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(+);
  }

  inline friend constexpr T operator-(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(-);
  }

  inline friend constexpr T operator*(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(*);
  }

  inline friend constexpr T operator/(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(/);
  }
  inline friend constexpr T operator%(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_work_get_defualt_endian_data_from_native_lhs(%);
  }

  inline friend constexpr T operator|(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_bitwise_work_from_native_lhs(|);
  }

  inline friend constexpr T operator&(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_bitwise_work_from_native_lhs(&);
  }

  inline friend constexpr T operator^(const get_endianed_data &lhs,
                                      const get_endianed_data &obj) {
    obj_do_bitwise_work_from_native_lhs(^);
  }

  inline constexpr get_endianed_data operator++(int) {
    do_work_get_defualt_endian_data_per(++);
  }
  inline constexpr get_endianed_data operator--(int) {
    do_work_get_defualt_endian_data_per(--);
  }
  inline constexpr T operator~() const { return ~get_native(); }
  inline constexpr T operator-() const { return -get_native(); }
  inline constexpr T operator+() const { return +get_native(); }
  inline constexpr T operator*() const { return get_native(); }
  inline constexpr bool operator!() const { return !m_value; }  // no one cares
  inline constexpr explicit operator bool() const {
    return !!m_value;
  }  // no one cares
  template <typename U>
  inline constexpr explicit operator U() const {
    return (U)get_native();
  }  // no one cares

  inline constexpr operator T() const { return get_native(); }

  inline constexpr get_endianed_data &operator()(std::function<T(T)> f) {
    if constexpr (!change_from_native_endian) {
      m_value = f(m_value);
      return *this;
    }
    mem_rev_cpy();
    m_value = f(m_value);
    mem_rev_cpy();
    return *this;
  }

#undef do_work_get_defualt_endian_data
#undef do_work_get_defualt_endian_data_per
#undef do_work_get_defualt_endian_data_from
#undef obj_do_work_get_defualt_endian_data_from
#undef obj_do_work_get_defualt_endian_data_from_native_lhs
#undef obj_do_bitwise_work_from_native_lhs
};

template <typename T>
using get_big_endian_data = get_endianed_data<T, endian_type::big>;
template <typename T>
using get_lit_endian_data = get_endianed_data<T, endian_type::little>;
template <typename T>
using get_little_endian_data = get_endianed_data<T, endian_type::little>;
template <typename T>
using get_native_endian_data = get_endianed_data<T, endian_type::native>;
template <typename T>
using get_non_native_endian_data =
    get_endianed_data<T, (endian_type::native == endian_type::big)
                             ? (endian_type::little)
                             : (endian_type::big)>;

template <class Type, size_t m_Size, bool error_check = 1>
struct mjz_Array : private mjz_temp_type_obj_algorithims_warpper_t<
                       Type> {  // fixed size mjz_Array of values

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

 private:
  using mjz_obj_algorithims = mjz_temp_type_obj_algorithims_warpper_t<Type>;
  M_DATA_U<Type, m_Size> m_data;
  using array_type_t = typename Type (&)[m_Size];

  constexpr inline array_type_t m_elements() { return m_data.m_elements; }

  constexpr inline const array_type_t m_elements() const {
    return m_data.m_elements;
  }

  constexpr inline void init_rest(size_t len) {
    if (m_Size < len) len = m_Size;
    mjz_obj_algorithims::construct_arr_at(m_elements() + len, m_Size - len);
  }

 public:
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
  constexpr [[nodiscard]] inline reference operator[](size_type _Pos) noexcept {
    if constexpr (error_check) {
      if (_Pos >= m_Size) {
        Throw<std::out_of_range>("mjz_Array subscript out of range");
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference operator[](
      size_type _Pos) const noexcept {
    if constexpr (error_check) {
      {
        if (_Pos >= m_Size)
          Throw<std::out_of_range>("mjz_Array subscript out of range");
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference front() noexcept {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline const_reference front() const noexcept {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline reference back() noexcept {
    return m_elements()[m_Size - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept {
    return m_elements()[m_Size - 1];
  }
  constexpr [[nodiscard]] inline Type *data() noexcept { return m_elements(); }
  constexpr [[nodiscard]] inline const Type *data() const noexcept {
    return m_elements();
  }
  [[noreturn]] void invld_throw() const {
    Throw<std::out_of_range>("invalid mjz_Array<T, N> subscript");
  }
  template <size_t N>
  constexpr inline mjz_Array(Type (&&input_c_array)[N]) {
    size_t len = min(N, m_Size);
    mjz_obj_algorithims::uninitialized_move_n(input_c_array, len, m_elements());
    init_rest(len);
  }
  constexpr inline mjz_Array(Type *p, size_t n) {
    size_t len = min(n, m_Size);
    mjz_obj_algorithims::uninitialized_move_n(p, len, m_elements());
    init_rest(len);
  }
  constexpr inline mjz_Array() { init_rest(0); }
  template <typename... args_t>
  constexpr inline mjz_Array(args_t &&...args)
      : m_data(std::forward<args_t>(args)...) {}
  template <typename... args_fn_t>
  static constexpr inline mjz_Array make_emplace(
      args_fn_t &&...args)  // for emplacement
  {
    mjz_Array ret{std::forward<args_fn_t>(args)()...};
    return ret;
  }
  constexpr inline ~mjz_Array() {
    mjz_obj_algorithims::obj_destructor_arr(m_elements(), m_Size);
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array(mjz_Array<Type, O_size, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims::uninitialized_move_n(other.begin(), len,
                                              this->begin());
    init_rest(len);
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array(const mjz_Array<Type, O_size, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims::uninitialized_copy_n(other.begin(), len,
                                              this->begin());
    init_rest(len);
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      mjz_Array<Type, O_size, O_err> &&other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims::move_backward(other.begin(), other.begin() + len,
                                       this->end());
    return *this;
  }
  template <size_t O_size, bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      const mjz_Array<Type, O_size, O_err> &other) {
    size_t len = min(other.size(), this->max_size());
    mjz_obj_algorithims::copy_backward(other.begin(), other.begin() + len,
                                       this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_Array(mjz_Array<Type, m_Size, O_err> &&other) {
    mjz_obj_algorithims::uninitialized_move_n(other.begin(), other.size(),
                                              this->begin());
    init_rest(other.size());
  }
  template <bool O_err>
  constexpr inline mjz_Array(const mjz_Array<Type, m_Size, O_err> &other) {
    mjz_obj_algorithims::uninitialized_copy_n(other.begin(), other.size(),
                                              this->begin());
    init_rest(other.size());
  }
  template <bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      mjz_Array<Type, m_Size, O_err> &&other) {
    mjz_obj_algorithims::move_backward(
        other.begin(), other.begin() + this->size(), this->end());
    return *this;
  }
  template <bool O_err>
  constexpr inline mjz_Array<Type, m_Size, error_check> &operator=(
      const mjz_Array<Type, m_Size, O_err> &other) {
    mjz_obj_algorithims::copy_backward(
        other.begin(), other.begin() + this->size(), this->end());
    return *this;
  }
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
    size_t s = m_size;
    deallocate();
    m_size = s;
    m_data = newData;
    m_capacity = newCapacity;
  }
  constexpr inline void check_range(size_type size) {
    if (m_size < size) Throw<std::out_of_range>(" bad accesses");
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
  constexpr inline bool empty() const noexcept { return !size(); }
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
  constexpr [[nodiscard]] inline reference operator[](size_type _Pos) {
    if constexpr (do_error_check) {
      if (_Pos >= size()) {
        Throw<std::out_of_range>(
            "mjz_static_vector_template_t subscript out of range");
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline const_reference operator[](
      size_type _Pos) const {
    if constexpr (do_error_check) {
      {
        if (_Pos >= size())
          Throw<std::out_of_range>(
              "mjz_static_vector_template_t subscript out of range");
      }
    }
    return m_elements()[_Pos];
  }
  constexpr [[nodiscard]] inline reference front() noexcept {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline const_reference front() const noexcept {
    return m_elements()[0];
  }
  constexpr [[nodiscard]] inline reference back() noexcept {
    return m_elements()[size() - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept {
    return m_elements()[size() - 1];
  }
  constexpr [[nodiscard]] inline Type *data() noexcept { return m_elements(); }
  constexpr [[nodiscard]] inline const Type *data() const noexcept {
    return m_elements();
  }
  [[noreturn]] void invld_throw() const {
    Throw<std::out_of_range>(
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
namespace calucate {

template <typename T, typename U>
inline constexpr uint64_t BMM_algo(T a_, U b_) {
  uint64_t a{}, b{};
  if (a_ < b_) {
    a = b_;
    b = a_;
  } else {
    a = a_;
    b = b_;
  }
  if (!a || !b) return 0;
  if ((a - b) == 1) return 1;
  uint64_t remainder = a % b;
  if (!remainder) return b;
  a = b;
  b = remainder;
  return BMM_algo(a, b);
}
template <typename T0, typename T1, typename... Ts>
inline constexpr uint64_t BMM_algo(T0 arg0, T1 arg1, Ts... args) {
  return BMM_algo(arg0, BMM_algo(arg1, args...));
}
template <typename T, typename U>
inline constexpr uint64_t KMM_algo(T a_, U b_) {
  uint64_t a{(uint64_t)a_}, b{(uint64_t)b_};
  uint64_t bmm = BMM_algo(a, b);
  return a * b / bmm;
}
template <typename T0, typename T1, typename... Ts>
inline constexpr uint64_t KMM_algo(T0 arg0, T1 arg1, Ts... args) {
  return KMM_algo((uint64_t)arg0,
                  KMM_algo((uint64_t)arg1, ((uint64_t)args)...));
}

template <typename T, typename U>
inline constexpr uint64_t min(T a, U b) {
  return a < b ? a : b;
}
template <typename T0, typename T1, typename... Ts>
inline constexpr uint64_t min(T0 arg0, T1 arg1, Ts... args) {
  return min((uint64_t)arg0, min((uint64_t)arg1, ((uint64_t)args)...));
}
template <typename T, typename U>
inline constexpr uint64_t max(T a, U b) {
  return a > b ? a : b;
}
template <typename T0, typename T1, typename... Ts>
inline constexpr uint64_t max(T0 arg0, T1 arg1, Ts... args) {
  return max((uint64_t)arg0, max((uint64_t)arg1, ((uint64_t)args)...));
}
}  // namespace calucate
class static_str_algo {
 public:
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
  static constexpr inline int MJZ__STRnCMP(const char *p1, const char *p2,
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
    const char *ptr = str_end + len_;
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
  constexpr inline MJZ_memcmp_data MJZ__strcmp(const void *ptr_1,
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
    template <size_t N>
    constexpr inline just_str_view_data(char (&a)[N])
        : buffer(a), len(N - (!a[N - 1])) {}
    constexpr inline just_str_view_data() : just_str_view_data(0, 0) {}
    char *buffer;
    size_t len;
  };

 private:
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
 private:
  static constexpr const uint64_t pow_of_2s[] = {1,
                                                 2,
                                                 4,
                                                 8,
                                                 16,
                                                 32,
                                                 64,
                                                 128,
                                                 256,
                                                 512,
                                                 1024,
                                                 2048,
                                                 4096,
                                                 8192,
                                                 16384,
                                                 32768,
                                                 65536,
                                                 131072,
                                                 262144,
                                                 524288,
                                                 1048576,
                                                 2097152,
                                                 4194304,
                                                 8388608,
                                                 16777216,
                                                 33554432,
                                                 67108864,
                                                 134217728,
                                                 268435456,
                                                 536870912,
                                                 1073741824,
                                                 2147483648,
                                                 4294967296,
                                                 8589934592,
                                                 17179869184,
                                                 34359738368,
                                                 68719476736,
                                                 137438953472,
                                                 274877906944,
                                                 549755813888,
                                                 1099511627776,
                                                 2199023255552,
                                                 4398046511104,
                                                 8796093022208,
                                                 17592186044416,
                                                 35184372088832,
                                                 70368744177664,
                                                 140737488355328,
                                                 281474976710656,
                                                 562949953421312,
                                                 1125899906842624,
                                                 2251799813685248,
                                                 4503599627370496,
                                                 9007199254740992,
                                                 18014398509481984,
                                                 36028797018963968,
                                                 72057594037927936,
                                                 144115188075855872,
                                                 288230376151711744,
                                                 576460752303423488,
                                                 1152921504606846976,
                                                 2305843009213693952,
                                                 4611686018427387904,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_3s[] = {1,
                                                 3,
                                                 9,
                                                 27,
                                                 81,
                                                 243,
                                                 729,
                                                 2187,
                                                 6561,
                                                 19683,
                                                 59049,
                                                 177147,
                                                 531441,
                                                 1594323,
                                                 4782969,
                                                 14348907,
                                                 43046721,
                                                 129140163,
                                                 387420489,
                                                 1162261467,
                                                 3486784401,
                                                 10460353203,
                                                 31381059609,
                                                 94143178827,
                                                 282429536481,
                                                 847288609443,
                                                 2541865828329,
                                                 7625597484987,
                                                 22876792454961,
                                                 68630377364883,
                                                 205891132094649,
                                                 617673396283947,
                                                 1853020188851841,
                                                 5559060566555523,
                                                 16677181699666568,
                                                 50031545098999704,
                                                 150094635296999136,
                                                 450283905890997376,
                                                 1350851717672992000,
                                                 4052555153018976256,
                                                 12157665459056928768};
  static constexpr const uint64_t pow_of_4s[] = {1,
                                                 4,
                                                 16,
                                                 64,
                                                 256,
                                                 1024,
                                                 4096,
                                                 16384,
                                                 65536,
                                                 262144,
                                                 1048576,
                                                 4194304,
                                                 16777216,
                                                 67108864,
                                                 268435456,
                                                 1073741824,
                                                 4294967296,
                                                 17179869184,
                                                 68719476736,
                                                 274877906944,
                                                 1099511627776,
                                                 4398046511104,
                                                 17592186044416,
                                                 70368744177664,
                                                 281474976710656,
                                                 1125899906842624,
                                                 4503599627370496,
                                                 18014398509481984,
                                                 72057594037927936,
                                                 288230376151711744,
                                                 1152921504606846976,
                                                 4611686018427387904,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_5s[] = {1,
                                                 5,
                                                 25,
                                                 125,
                                                 625,
                                                 3125,
                                                 15625,
                                                 78125,
                                                 390625,
                                                 1953125,
                                                 9765625,
                                                 48828125,
                                                 244140625,
                                                 1220703125,
                                                 6103515625,
                                                 30517578125,
                                                 152587890625,
                                                 762939453125,
                                                 3814697265625,
                                                 19073486328125,
                                                 95367431640625,
                                                 476837158203125,
                                                 2384185791015625,
                                                 11920928955078124,
                                                 59604644775390624,
                                                 298023223876953152,
                                                 1490116119384765696,
                                                 7450580596923828224,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_6s[] = {1,
                                                 6,
                                                 36,
                                                 216,
                                                 1296,
                                                 7776,
                                                 46656,
                                                 279936,
                                                 1679616,
                                                 10077696,
                                                 60466176,
                                                 362797056,
                                                 2176782336,
                                                 13060694016,
                                                 78364164096,
                                                 470184984576,
                                                 2821109907456,
                                                 16926659444736,
                                                 101559956668416,
                                                 609359740010496,
                                                 3656158440062976,
                                                 21936950640377856,
                                                 131621703842267136,
                                                 789730223053602816,
                                                 4738381338321616896,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_7s[] = {1,
                                                 7,
                                                 49,
                                                 343,
                                                 2401,
                                                 16807,
                                                 117649,
                                                 823543,
                                                 5764801,
                                                 40353607,
                                                 282475249,
                                                 1977326743,
                                                 13841287201,
                                                 96889010407,
                                                 678223072849,
                                                 4747561509943,
                                                 33232930569601,
                                                 232630513987207,
                                                 1628413597910449,
                                                 11398895185373144,
                                                 79792266297612000,
                                                 558545864083284032,
                                                 3909821048582988288,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_8s[] = {1,
                                                 8,
                                                 64,
                                                 512,
                                                 4096,
                                                 32768,
                                                 262144,
                                                 2097152,
                                                 16777216,
                                                 134217728,
                                                 1073741824,
                                                 8589934592,
                                                 68719476736,
                                                 549755813888,
                                                 4398046511104,
                                                 35184372088832,
                                                 281474976710656,
                                                 2251799813685248,
                                                 18014398509481984,
                                                 144115188075855872,
                                                 1152921504606846976,
                                                 9223372036854775808};
  static constexpr const uint64_t pow_of_9s[] = {1,
                                                 9,
                                                 81,
                                                 729,
                                                 6561,
                                                 59049,
                                                 531441,
                                                 4782969,
                                                 43046721,
                                                 387420489,
                                                 3486784401,
                                                 31381059609,
                                                 282429536481,
                                                 2541865828329,
                                                 22876792454961,
                                                 205891132094649,
                                                 1853020188851841,
                                                 16677181699666568,
                                                 150094635296999136,
                                                 1350851717672992000,
                                                 12157665459056928768};
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
  static constexpr const uint64_t pow_of_11s[] = {1,
                                                  11,
                                                  121,
                                                  1331,
                                                  14641,
                                                  161051,
                                                  1771561,
                                                  19487171,
                                                  214358881,
                                                  2357947691,
                                                  25937424601,
                                                  285311670611,
                                                  3138428376721,
                                                  34522712143931,
                                                  379749833583241,
                                                  4177248169415651,
                                                  45949729863572160,
                                                  505447028499293760,
                                                  5559917313492231168,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_12s[] = {1,
                                                  12,
                                                  144,
                                                  1728,
                                                  20736,
                                                  248832,
                                                  2985984,
                                                  35831808,
                                                  429981696,
                                                  5159780352,
                                                  61917364224,
                                                  743008370688,
                                                  8916100448256,
                                                  106993205379072,
                                                  1283918464548864,
                                                  15407021574586368,
                                                  184884258895036416,
                                                  2218611106740436992,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_13s[] = {1,
                                                  13,
                                                  169,
                                                  2197,
                                                  28561,
                                                  371293,
                                                  4826809,
                                                  62748517,
                                                  815730721,
                                                  10604499373,
                                                  137858491849,
                                                  1792160394037,
                                                  23298085122481,
                                                  302875106592253,
                                                  3937376385699289,
                                                  51185893014090760,
                                                  665416609183179904,
                                                  8650415919381338112,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_14s[] = {1,
                                                  14,
                                                  196,
                                                  2744,
                                                  38416,
                                                  537824,
                                                  7529536,
                                                  105413504,
                                                  1475789056,
                                                  20661046784,
                                                  289254654976,
                                                  4049565169664,
                                                  56693912375296,
                                                  793714773254144,
                                                  11112006825558016,
                                                  155568095557812224,
                                                  2177953337809371136,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_15s[] = {1,
                                                  15,
                                                  225,
                                                  3375,
                                                  50625,
                                                  759375,
                                                  11390625,
                                                  170859375,
                                                  2562890625,
                                                  38443359375,
                                                  576650390625,
                                                  8649755859375,
                                                  129746337890625,
                                                  1946195068359375,
                                                  29192926025390624,
                                                  437893890380859392,
                                                  6568408355712890880,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_16s[] = {1,
                                                  16,
                                                  256,
                                                  4096,
                                                  65536,
                                                  1048576,
                                                  16777216,
                                                  268435456,
                                                  4294967296,
                                                  68719476736,
                                                  1099511627776,
                                                  17592186044416,
                                                  281474976710656,
                                                  4503599627370496,
                                                  72057594037927936,
                                                  1152921504606846976,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_17s[] = {1,
                                                  17,
                                                  289,
                                                  4913,
                                                  83521,
                                                  1419857,
                                                  24137569,
                                                  410338673,
                                                  6975757441,
                                                  118587876497,
                                                  2015993900449,
                                                  34271896307633,
                                                  582622237229761,
                                                  9904578032905938,
                                                  168377826559400928,
                                                  2862423051509815808,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_18s[] = {1,
                                                  18,
                                                  324,
                                                  5832,
                                                  104976,
                                                  1889568,
                                                  34012224,
                                                  612220032,
                                                  11019960576,
                                                  198359290368,
                                                  3570467226624,
                                                  64268410079232,
                                                  1156831381426176,
                                                  20822964865671168,
                                                  374813367582081024,
                                                  6746640616477458432,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_19s[] = {1,
                                                  19,
                                                  361,
                                                  6859,
                                                  130321,
                                                  2476099,
                                                  47045881,
                                                  893871739,
                                                  16983563041,
                                                  322687697779,
                                                  6131066257801,
                                                  116490258898219,
                                                  2213314919066161,
                                                  42052983462257056,
                                                  799006685782884096,
                                                  15181127029874798592};
  static constexpr const uint64_t pow_of_20s[] = {1,
                                                  20,
                                                  400,
                                                  8000,
                                                  160000,
                                                  3200000,
                                                  64000000,
                                                  1280000000,
                                                  25600000000,
                                                  512000000000,
                                                  10240000000000,
                                                  204800000000000,
                                                  4096000000000000,
                                                  81920000000000000,
                                                  1638400000000000000,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_21s[] = {1,
                                                  21,
                                                  441,
                                                  9261,
                                                  194481,
                                                  4084101,
                                                  85766121,
                                                  1801088541,
                                                  37822859361,
                                                  794280046581,
                                                  16679880978201,
                                                  350277500542221,
                                                  7355827511386641,
                                                  154472377739119456,
                                                  3243919932521508864,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_22s[] = {1,
                                                  22,
                                                  484,
                                                  10648,
                                                  234256,
                                                  5153632,
                                                  113379904,
                                                  2494357888,
                                                  54875873536,
                                                  1207269217792,
                                                  26559922791424,
                                                  584318301411328,
                                                  12855002631049216,
                                                  282810057883082752,
                                                  6221821273427820544,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_23s[] = {1,
                                                  23,
                                                  529,
                                                  12167,
                                                  279841,
                                                  6436343,
                                                  148035889,
                                                  3404825447,
                                                  78310985281,
                                                  1801152661463,
                                                  41426511213649,
                                                  952809757913927,
                                                  21914624432020320,
                                                  504036361936467392,
                                                  11592836324538748928};
  static constexpr const uint64_t pow_of_24s[] = {1,
                                                  24,
                                                  576,
                                                  13824,
                                                  331776,
                                                  7962624,
                                                  191102976,
                                                  4586471424,
                                                  110075314176,
                                                  2641807540224,
                                                  63403380965376,
                                                  1521681143169024,
                                                  36520347436056576,
                                                  876488338465357824,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_25s[] = {1,
                                                  25,
                                                  625,
                                                  15625,
                                                  390625,
                                                  9765625,
                                                  244140625,
                                                  6103515625,
                                                  152587890625,
                                                  3814697265625,
                                                  95367431640625,
                                                  2384185791015625,
                                                  59604644775390624,
                                                  1490116119384765696,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_26s[] = {1,
                                                  26,
                                                  676,
                                                  17576,
                                                  456976,
                                                  11881376,
                                                  308915776,
                                                  8031810176,
                                                  208827064576,
                                                  5429503678976,
                                                  141167095653376,
                                                  3670344486987776,
                                                  95428956661682176,
                                                  2481152873203736576,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_27s[] = {1,
                                                  27,
                                                  729,
                                                  19683,
                                                  531441,
                                                  14348907,
                                                  387420489,
                                                  10460353203,
                                                  282429536481,
                                                  7625597484987,
                                                  205891132094649,
                                                  5559060566555523,
                                                  150094635296999136,
                                                  4052555153018976256,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_28s[] = {1,
                                                  28,
                                                  784,
                                                  21952,
                                                  614656,
                                                  17210368,
                                                  481890304,
                                                  13492928512,
                                                  377801998336,
                                                  10578455953408,
                                                  296196766695424,
                                                  8293509467471872,
                                                  232218265089212416,
                                                  6502111422497947648,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_29s[] = {1,
                                                  29,
                                                  841,
                                                  24389,
                                                  707281,
                                                  20511149,
                                                  594823321,
                                                  17249876309,
                                                  500246412961,
                                                  14507145975869,
                                                  420707233300201,
                                                  12200509765705828,
                                                  353814783205469056,
                                                  10260628712958601216};
  static constexpr const uint64_t pow_of_30s[] = {1,
                                                  30,
                                                  900,
                                                  27000,
                                                  810000,
                                                  24300000,
                                                  729000000,
                                                  21870000000,
                                                  656100000000,
                                                  19683000000000,
                                                  590490000000000,
                                                  17714700000000000,
                                                  531441000000000000,
                                                  15943230000000000000};
  static constexpr const uint64_t pow_of_31s[] = {1,
                                                  31,
                                                  961,
                                                  29791,
                                                  923521,
                                                  28629151,
                                                  887503681,
                                                  27512614111,
                                                  852891037441,
                                                  26439622160671,
                                                  819628286980801,
                                                  25408476896404832,
                                                  787662783788549760,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_32s[] = {1,
                                                  32,
                                                  1024,
                                                  32768,
                                                  1048576,
                                                  33554432,
                                                  1073741824,
                                                  34359738368,
                                                  1099511627776,
                                                  35184372088832,
                                                  1125899906842624,
                                                  36028797018963968,
                                                  1152921504606846976,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_33s[] = {1,
                                                  33,
                                                  1089,
                                                  35937,
                                                  1185921,
                                                  39135393,
                                                  1291467969,
                                                  42618442977,
                                                  1406408618241,
                                                  46411484401953,
                                                  1531578985264449,
                                                  50542106513726816,
                                                  1667889514952985088,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_34s[] = {1,
                                                  34,
                                                  1156,
                                                  39304,
                                                  1336336,
                                                  45435424,
                                                  1544804416,
                                                  52523350144,
                                                  1785793904896,
                                                  60716992766464,
                                                  2064377754059776,
                                                  70188843638032384,
                                                  2386420683693101056,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_35s[] = {1,
                                                  35,
                                                  1225,
                                                  42875,
                                                  1500625,
                                                  52521875,
                                                  1838265625,
                                                  64339296875,
                                                  2251875390625,
                                                  78815638671875,
                                                  2758547353515625,
                                                  96549157373046880,
                                                  3379220508056640512,
                                                  9223372036854775808};
  static constexpr const uint64_t pow_of_36s[] = {1,
                                                  36,
                                                  1296,
                                                  46656,
                                                  1679616,
                                                  60466176,
                                                  2176782336,
                                                  78364164096,
                                                  2821109907456,
                                                  101559956668416,
                                                  3656158440062976,
                                                  131621703842267136,
                                                  4738381338321616896,
                                                  9223372036854775808};
  static constexpr const uint64_t *const pow_of_rs[] = {
      pow_of_2s,  pow_of_3s,  pow_of_4s,  pow_of_5s,  pow_of_6s,  pow_of_7s,
      pow_of_8s,  pow_of_9s,  pow_of_10s, pow_of_11s, pow_of_12s, pow_of_13s,
      pow_of_14s, pow_of_15s, pow_of_16s, pow_of_17s, pow_of_18s, pow_of_19s,
      pow_of_20s, pow_of_21s, pow_of_22s, pow_of_23s, pow_of_24s, pow_of_25s,
      pow_of_26s, pow_of_27s, pow_of_28s, pow_of_29s, pow_of_30s, pow_of_31s,
      pow_of_32s, pow_of_33s, pow_of_34s, pow_of_35s, pow_of_36s};

  static constexpr const uint64_t num_pow_of_rs[] = {
      NumberOf(pow_of_2s),  NumberOf(pow_of_3s),  NumberOf(pow_of_4s),
      NumberOf(pow_of_5s),  NumberOf(pow_of_6s),  NumberOf(pow_of_7s),
      NumberOf(pow_of_8s),  NumberOf(pow_of_9s),  NumberOf(pow_of_10s),
      NumberOf(pow_of_11s), NumberOf(pow_of_12s), NumberOf(pow_of_13s),
      NumberOf(pow_of_14s), NumberOf(pow_of_15s), NumberOf(pow_of_16s),
      NumberOf(pow_of_17s), NumberOf(pow_of_18s), NumberOf(pow_of_19s),
      NumberOf(pow_of_20s), NumberOf(pow_of_21s), NumberOf(pow_of_22s),
      NumberOf(pow_of_23s), NumberOf(pow_of_24s), NumberOf(pow_of_25s),
      NumberOf(pow_of_26s), NumberOf(pow_of_27s), NumberOf(pow_of_28s),
      NumberOf(pow_of_29s), NumberOf(pow_of_30s), NumberOf(pow_of_31s),
      NumberOf(pow_of_32s), NumberOf(pow_of_33s), NumberOf(pow_of_34s),
      NumberOf(pow_of_35s), NumberOf(pow_of_36s)};
  static constexpr inline uint8_t num_digit_fast_minimal_for_lltoa(
      uint64_t __val, uint8_t radix) {
    if (!__val) return 1;
    uint8_t int_component_log{0};
    uint64_t r_n = radix / 10;
    const uint64_t *pow_of_xs = pow_of_rs[radix - 2];
    uint8_t num_pow = num_pow_of_rs[radix - 2];
    uint8_t i{};
    while (!int_component_log && i < num_pow) {
      int_component_log |=
          MJZ_logic_bit_to_64_bits(__val < pow_of_xs[i]) & (++i);
    }
    int_component_log--;
    // auto const int_component_log_x = ceiling(log((double)__val) /
    // log((double)radix));
    return int_component_log;
  }

 public:
  static constexpr inline just_str_view_data b_U_lltoa_n(
      uint64_t value, char *const BFR_buffer,
      const size_t BFR_len,  // has to be 66 bytes for all values
                             // from 0 to -1ULL  in base2  to base36
      int radix, bool is_signed, bool force_neg = 0, bool is_upper_case = 1) {
    if (!(BFR_buffer && BFR_len && (1 < radix) && (radix < 37))) return {};
    int64_t &signed_var = *((int64_t *)(&value));
    char *buffer = BFR_buffer;
    size_t len = BFR_len;
    if (is_signed && signed_var < 0) {
      signed_var = -signed_var;
      len--;
      *buffer++ = '-';
    }
    size_t neded_len = num_digit_fast_minimal_for_lltoa(value, radix);
    if ((len - 1) < neded_len) {
      *BFR_buffer = '\0';
      return {};
    }

    len = neded_len;
    char *r_ptr_end = buffer - 1;
    char *ptr = buffer + len - 1;
    for (; r_ptr_end < ptr && value; ptr--) {
      uint8_t v = (uint8_t)(value % radix);
      value /= radix;
      *ptr = GET_CHAR_from_int(v, is_upper_case);
    }
    buffer[len] = '\0';
    return {BFR_buffer, len + (buffer != BFR_buffer)};
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
  static constexpr inline int MJZ__STRCMP(const char *p1, const char *p2) {
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

  static constexpr inline just_str_view_data ulltoa(uint64_t value,
                                                    just_str_view_data buffer,
                                                    int radix) {
    return b_U_lltoa_n((uint64_t)value, buffer.buffer, buffer.len, radix, 0);
  }
  static constexpr inline just_str_view_data ultoa(uint32_t value,
                                                   just_str_view_data buffer,
                                                   int radix) {
    return ulltoa((uint64_t)value, buffer, radix);
  }
  static constexpr inline just_str_view_data utoa(uint32_t value,
                                                  just_str_view_data buffer,
                                                  int radix) {
    return ultoa(value, buffer, radix);
  }
  static constexpr inline just_str_view_data lltoa(long long value,
                                                   just_str_view_data buffer,
                                                   int radix) {
    return b_U_lltoa_n((uint64_t)value, buffer.buffer, buffer.len, radix, 1);
  }
  static constexpr inline just_str_view_data ltoa(long value,
                                                  just_str_view_data buffer,
                                                  int radix) {
    return lltoa((long long)value, buffer, radix);
  }
  static constexpr inline just_str_view_data itoa(int value,
                                                  just_str_view_data buffer,
                                                  int radix) {
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
    return MJZ__STRnCMP(rhs, lhs, rhs_l);
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
  static constexpr inline bool is_approximately_equal(double a, double b,
                                                      double accuracy = 0.001) {
    return (abs(a - b) < accuracy);
  }

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
    long flr = (long)floor(number);
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
    long flr = (long)floor(number);
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

/*
NOTE THIS CLASS IS JUST A (REFRENCE / POINTER ) YOU SHALL USE IT LIKE ONE aka
1.DONT RETURN THIS FROM A FUNCTION INTERNAL (STACK) VARIABLE
2.USE THIS WHEN VARIABLE IS REFENECED WITHIN THIS TIME FRAME :
 {THE REFRENED OBJECT SHUOLD  EXSIST  } -  CREATION ----------USE... ----------
LAST USE - {THE REFRENED OBJECT COULD SAFELY DESTROY HEARE }  ... DESTRUCTION
... {THE REFRENED OBJECT COULD SAFELY DESTROY HEARE }
*/
enum class optional_pointer_refrence_has_to_be_valid_on_construction_level {
  NONE = 0,
  not_null_if_is_ref_and_is_const,
  not_null_if_is_ref,
  not_null_if_is_const,
  not_null_if_is_ref_or_is_const,
};

template <
    typename T_ref, bool is_mutable_ptr_,
    optional_pointer_refrence_has_to_be_valid_on_construction_level
        has_to_be_valid_on_construction =
            optional_pointer_refrence_has_to_be_valid_on_construction_level::
                NONE>
class optional_pointer_refrence_class_template_t
    : private mjz_obj_manager_template_t<std::remove_cvref_t<T_ref>> {
 public:
  using Type = std::remove_cvref_t<T_ref>;
  using bare_Type = std::remove_volatile_t<std::remove_reference_t<T_ref>>;
  constexpr static const bool is_mutable_ptr = is_mutable_ptr_;
  constexpr static const bool is_const_type =
      std::is_same_v<bare_Type &, const Type &>;

 private:
  using level_t =
      optional_pointer_refrence_has_to_be_valid_on_construction_level;
  constexpr const static level_t level = has_to_be_valid_on_construction;
  constexpr const static bool Throw_on_null =
      ((level == level_t::not_null_if_is_ref_and_is_const) && is_const_type &&
       is_mutable_ptr) ||
      ((level == level_t::not_null_if_is_ref_or_is_const) &&
       (is_const_type || is_mutable_ptr)) ||
      ((level == level_t::not_null_if_is_const) && is_const_type) ||
      ((level == level_t::not_null_if_is_ref) && is_mutable_ptr);
  inline constexpr void construction_check() const {
    if (m_ptr != 0) return;

    if constexpr (Throw_on_null) Throw(" is null ");
    return;
  }

 public:
  inline constexpr mjz_obj_manager_template_t<std::remove_const_t<Type>>
      &get_obj_creator()
    requires(!is_const_type && is_mutable_ptr)
  {
    return *this;
  }
  inline constexpr const mjz_obj_manager_template_t<std::remove_const_t<Type>>
      &get_obj_creator() const
    requires(!is_const_type)
  {
    return *this;
  }

 private:
  bare_Type *m_ptr{};

 public:
  template <typename T = Type, bool B = true>
  inline constexpr Type *const get_ptr_to_valid_object_or_throw() = delete;
  template <>
  inline constexpr Type *const
  get_ptr_to_valid_object_or_throw<void, !is_const_type && is_mutable_ptr>() {
    return m_ptr;
  }
  template <>
  inline constexpr Type *const
  get_ptr_to_valid_object_or_throw<Type, !is_const_type && is_mutable_ptr>() {
    if (m_ptr) return m_ptr;
    Throw<const char *>("no object ");
  }
  template <typename T = Type, bool B = true>
  inline constexpr Type *const get_ptr_to_valid_object_or_throw() const =
      delete;
  template <>
  inline constexpr Type *const
  get_ptr_to_valid_object_or_throw<void, !is_const_type>() const {
    return m_ptr;
  }
  template <>
  inline constexpr Type *const
  get_ptr_to_valid_object_or_throw<Type, !is_const_type>() const {
    if (m_ptr) return m_ptr;
    Throw<const char *>("no object ");
  }
  inline constexpr optional_pointer_refrence_class_template_t()
    requires(!is_const_type && is_mutable_ptr)
  {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      std::nullptr_t nullp)
    requires(!is_const_type && is_mutable_ptr)
  {
    construction_check();
  }

  inline constexpr optional_pointer_refrence_class_template_t(
      Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(!is_const_type)
      : m_ptr(valid_pointer_to_object_that_meates_the_requirements) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type)
      : optional_pointer_refrence_class_template_t(this->addressof(
            valid_refrence_to_object_that_meates_the_requirements)) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      optional_pointer_refrence_class_template_t
          &valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type)
      : optional_pointer_refrence_class_template_t(
            valid_refrence_to_object_that_meates_the_requirements.m_ptr) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      optional_pointer_refrence_class_template_t
          &&valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type)
      : optional_pointer_refrence_class_template_t(
            valid_refrence_to_object_that_meates_the_requirements.m_ptr) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(Type &&) = delete;
  optional_pointer_refrence_class_template_t &operator=(Type &&) = delete;
  inline constexpr optional_pointer_refrence_class_template_t(const Type &&) =
      delete;
  optional_pointer_refrence_class_template_t &operator=(const Type &&) = delete;
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(!is_const_type && is_mutable_ptr)
  {
    m_ptr = (valid_pointer_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type && is_mutable_ptr)
  {
    m_ptr =
        this->addressof(valid_refrence_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      optional_pointer_refrence_class_template_t
          &valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type && is_mutable_ptr)
  {
    m_ptr = valid_refrence_to_object_that_meates_the_requirements
                .get_ptr_to_valid_object_or_throw();
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      optional_pointer_refrence_class_template_t
          &&valid_refrence_to_object_that_meates_the_requirements)
    requires(!is_const_type && is_mutable_ptr)
  {
    m_ptr = valid_refrence_to_object_that_meates_the_requirements
                .get_ptr_to_valid_object_or_throw();
    construction_check();

    return *this;
  }
  inline constexpr explicit operator bool() const
    requires(!is_const_type)
  {
    return !!get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr bool operator!() const
    requires(!is_const_type)
  {
    return !get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr operator Type *()
    requires(!is_const_type && is_mutable_ptr)
  {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr operator Type *() const
    requires(!is_const_type)
  {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr Type *ptr()
    requires(!is_const_type && is_mutable_ptr)
  {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr Type *ptr() const
    requires(!is_const_type)
  {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr optional_pointer_refrence_class_template_t &set_ptr(
      Type *p = 0)
    requires(!is_const_type && is_mutable_ptr)
  {
    m_ptr = p;
    construction_check();
    return *this;
  }
  inline constexpr Type &get()
    requires(!is_const_type && is_mutable_ptr)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type &get() const
    requires(!is_const_type)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr operator Type &()
    requires(!is_const_type && is_mutable_ptr)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr operator Type &() const
    requires(!is_const_type)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type *operator->()
    requires(!is_const_type && is_mutable_ptr)
  {
    return get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type &operator*()
    requires(!is_const_type && is_mutable_ptr)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type *operator->() const
    requires(!is_const_type)
  {
    return get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type &operator*() const
    requires(!is_const_type)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type &operator()()
    requires(!is_const_type && is_mutable_ptr)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr void operator()(std::function<void(Type &)> f)
    requires(!is_const_type && is_mutable_ptr)
  {
    if (m_ptr) {
      f(*m_ptr);
    }
  }
  inline constexpr Type &operator()() const
    requires(!is_const_type)
  {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr void operator~()
    requires((!Throw_on_null) && is_mutable_ptr)
  {
    m_ptr = 0;
  }
  template <bool B>
  inline constexpr operator optional_pointer_refrence_class_template_t<
      std::add_const_t<Type> &, B>()
    requires(!is_const_type && is_mutable_ptr)
  {
    return {m_ptr};
  }

 public:
  using const_Type = const std::remove_cvref_t<T_ref>;

 public:
  template <typename T = const_Type, bool B = true>
  inline constexpr const_Type *const get_ptr_to_valid_const_object_or_throw() =
      delete;
  template <>
  inline constexpr const_Type *const get_ptr_to_valid_const_object_or_throw<
      void, is_mutable_ptr && is_const_type>() {
    return m_ptr;
  }
  template <>
  inline constexpr const_Type *const get_ptr_to_valid_const_object_or_throw<
      const_Type, is_mutable_ptr && is_const_type>() {
    if (m_ptr) return m_ptr;
    Throw<const char *>("no object ");
  }
  template <typename T = const_Type, bool B = true>
  inline constexpr const_Type *const get_ptr_to_valid_const_object_or_throw()
      const = delete;
  template <>
  inline constexpr const_Type *const
  get_ptr_to_valid_const_object_or_throw<void, is_const_type>() const {
    return m_ptr;
  }
  template <>
  inline constexpr const_Type *const
  get_ptr_to_valid_const_object_or_throw<const_Type, is_const_type>() const {
    if (m_ptr) return m_ptr;
    Throw<const char *>("no object ");
  }
  inline constexpr optional_pointer_refrence_class_template_t()
    requires(is_mutable_ptr && is_const_type)
  {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      std::nullptr_t nullp)
    requires(is_mutable_ptr && is_const_type)
  {
    construction_check();
  }
  using Type = std::remove_const_t<const_Type>;
  inline constexpr optional_pointer_refrence_class_template_t(
      Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : m_ptr(valid_pointer_to_object_that_meates_the_requirements) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : optional_pointer_refrence_class_template_t(this->addressof(
            valid_refrence_to_object_that_meates_the_requirements)) {
    construction_check();
  }

  inline constexpr optional_pointer_refrence_class_template_t(Type &&)
    requires(is_const_type)
  = delete;
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr = (valid_pointer_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr =
        this->addressof(valid_refrence_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  optional_pointer_refrence_class_template_t &operator=(Type &&)
    requires(is_const_type)
  = delete;
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      optional_pointer_refrence_class_template_t
          &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr = valid_refrence_to_object_that_meates_the_requirements
                .get_ptr_to_valid_const_object_or_throw();
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      optional_pointer_refrence_class_template_t
          &&valid_refrence_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr = valid_refrence_to_object_that_meates_the_requirements
                .get_ptr_to_valid_const_object_or_throw();
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      const_Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : m_ptr(valid_pointer_to_object_that_meates_the_requirements) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      const_Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : optional_pointer_refrence_class_template_t(this->addressof(
            valid_refrence_to_object_that_meates_the_requirements)) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      optional_pointer_refrence_class_template_t
          &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : optional_pointer_refrence_class_template_t(
            valid_refrence_to_object_that_meates_the_requirements.m_ptr) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(
      optional_pointer_refrence_class_template_t
          &&valid_refrence_to_object_that_meates_the_requirements)
    requires(is_const_type)
      : optional_pointer_refrence_class_template_t(
            valid_refrence_to_object_that_meates_the_requirements.m_ptr) {
    construction_check();
  }
  inline constexpr optional_pointer_refrence_class_template_t(const_Type &&)
    requires(is_const_type)
  = delete;
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      const_Type *valid_pointer_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr = (valid_pointer_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  inline constexpr optional_pointer_refrence_class_template_t &operator=(
      const_Type &valid_refrence_to_object_that_meates_the_requirements)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr =
        this->addressof(valid_refrence_to_object_that_meates_the_requirements);
    construction_check();

    return *this;
  }
  optional_pointer_refrence_class_template_t &operator=(const_Type &&)
    requires(is_const_type)
  = delete;

  inline constexpr ~optional_pointer_refrence_class_template_t() { m_ptr = 0; }
  inline constexpr explicit operator bool() const
    requires(is_const_type)
  {
    return !!get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr bool operator!() const
    requires(is_const_type)
  {
    return !get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr operator const_Type *()
    requires(is_mutable_ptr && is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr operator const_Type *() const
    requires(is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr const_Type *ptr()
    requires(is_mutable_ptr && is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr const_Type *ptr() const
    requires(is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw<void>();
  }
  inline constexpr optional_pointer_refrence_class_template_t &set_ptr(
      const_Type *p = 0)
    requires(is_mutable_ptr && is_const_type)
  {
    m_ptr = p;
    construction_check();

    return *this;
  }
  inline constexpr const_Type &get()
    requires(is_mutable_ptr && is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type &get() const
    requires(is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr operator const_Type &()
    requires(is_mutable_ptr && is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr operator const_Type &() const
    requires(is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type *operator->()
    requires(is_mutable_ptr && is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type &operator*()
    requires(is_mutable_ptr && is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type *operator->() const
    requires(is_const_type)
  {
    return get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type &operator*() const
    requires(is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr const_Type &operator()()
    requires(is_mutable_ptr && is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }
  inline constexpr void operator()(std::function<void(const_Type &)> f)
    requires(is_mutable_ptr && is_const_type)
  {
    if (m_ptr) {
      f(*m_ptr);
    }
  }
  inline constexpr const_Type &operator()() const
    requires(is_const_type)
  {
    return *get_ptr_to_valid_const_object_or_throw();
  }

  using merf = optional_pointer_refrence_class_template_t &;
  inline constexpr friend bool operator==(merf a, merf b)

  {
    return *a == *b;
  }
  inline constexpr friend bool operator!=(merf a, merf b)

  {
    return *a != *b;
  }
  inline constexpr friend bool operator<=(merf a, merf b)

  {
    return *a <= *b;
  }
  inline constexpr friend bool operator>=(merf a, merf b)

  {
    return *a >= *b;
  }
  inline constexpr friend bool operator<(merf a, merf b)

  {
    return *a < *b;
  }
  inline constexpr friend bool operator>(merf a, merf b)

  {
    return *a > *b;
  }
  inline constexpr friend auto operator&(merf a, merf b)

  {
    return *a & *b;
  }
  inline constexpr friend auto operator|(merf a, merf b)

  {
    return *a | *b;
  }
  inline constexpr friend auto operator^(merf a, merf b)

  {
    return *a ^ *b;
  }
  inline constexpr friend auto operator%(merf a, merf b)

  {
    return *a % *b;
  }
  inline constexpr friend auto operator*(merf a, merf b)

  {
    return *a * *b;
  }
  inline constexpr friend auto operator/(merf a, merf b)

  {
    return *a / *b;
  }
  inline constexpr friend auto operator-(merf a, merf b)

  {
    return *a - *b;
  }
  inline constexpr friend auto operator+(merf a, merf b)

  {
    return *a + *b;
  }
  inline constexpr auto operator&=(merf b)

  {
    return (**this) &= *b;
  }
  inline constexpr auto operator|=(merf b)

  {
    return (**this) |= *b;
  }
  inline constexpr auto operator^=(merf b)

  {
    return (**this) ^= *b;
  }
  inline constexpr auto operator%=(merf b)

  {
    return (**this) %= *b;
  }
  inline constexpr auto operator*=(merf b)

  {
    return (**this) *= *b;
  }
  inline constexpr auto operator/=(merf b)

  {
    return (**this) /= *b;
  }
  inline constexpr auto operator-=(merf b)

  {
    return (**this) -= *b;
  }
  inline constexpr auto operator+=(merf b)

  {
    return (**this) += *b;
  }
};

template <
    typename T_ref, bool mutable_ptr,
    optional_pointer_refrence_has_to_be_valid_on_construction_level level =
        optional_pointer_refrence_has_to_be_valid_on_construction_level::NONE>
using optional_pointer_refrence_template_t =
    optional_pointer_refrence_class_template_t<std::remove_volatile_t<T_ref>,
                                               mutable_ptr, level>;
enum mjz_stack_obj_warper_template_t_data_states : uint8_t {
  in_union,
  in_aligned_chars,
  in_chars
};
template <typename Error_T>
struct mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t {
  using Error_t = Error_T;
  constexpr static const bool class_has_Error = true;
};
template <>
struct mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t<
    void> {
  using Error_t = bool;
  constexpr static const bool class_has_Error = false;
};

template <typename T, C_mjz_obj_manager obj_crtr, C_mjz_obj_manager err_crtr,
          mjz_stack_obj_warper_template_t_data_states m_in_uinion,
          typename in_Error_t = void>
struct mjz_stack_obj_warper_deafualt_data_storage_template_t
    : protected obj_crtr,
      protected err_crtr {
 private:
  template <typename Error_T>
  using select_err_t =
      mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t<
          Error_T>;
  using BS = obj_crtr;

 public:
  using Error_t = typename select_err_t<in_Error_t>::Error_t;
  constexpr static const bool class_has_Error =
      select_err_t<in_Error_t>::class_has_Error;

 private:
  using err_crtr_t = err_crtr;
  using obj_crtr_t = obj_crtr;
  using unsafe_err_t =
      typename err_crtr_t::template simple_unsafe_init_obj_wrpr<false, false>;
  template <mjz_stack_obj_warper_template_t_data_states is_in_uinion>
  union UDB_t {};
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_aligned_chars> {
    const constexpr static size_t alinement_v =
        calucate::KMM_algo(BS::alignment, alignof(unsafe_err_t));
    uint8_t f;
    alignas(alinement_v) /*aliens with T */ mutable uint8_t
        mm_data[BS::size_of_type()];
    alignas(alinement_v) uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return *(T *)m_data; }
    constexpr inline const T &obj_ref() const { return *(const T *)m_data; }
    constexpr inline unsafe_err_t &err_ref() { return *(unsafe_err_t *)m_data; }
    constexpr inline const unsafe_err_t &err_ref() const {
      return *(const unsafe_err_t *)m_data;
    }
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_chars> {
    uint8_t f;
    alignas(1) /*no alinement*/ mutable uint8_t mm_data[BS::size_of_type()];
    alignas(1) uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return *(T *)m_data; }
    constexpr inline const T &obj_ref() const { return *(const T *)m_data; }

    constexpr inline unsafe_err_t &err_ref() { return *(unsafe_err_t *)m_data; }
    constexpr inline const unsafe_err_t &err_ref() const {
      return *(const unsafe_err_t *)m_data;
    }
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_union> {
    uint8_t f;
    mutable uint8_t mm_data[BS::size_of_type()];
    uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return obj; }
    constexpr inline const T &obj_ref() const { return obj; }

    constexpr inline unsafe_err_t &err_ref() { return error; }
    constexpr inline const unsafe_err_t &err_ref() const { return error; }
    T obj;
    unsafe_err_t error;
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  UDB_t<m_in_uinion> m_obj_buf{};

 public:
  enum class data_state : uint8_t { none = 0, value, error };
  using Type = T;
  using BS = obj_crtr;
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());

 protected:
  union {
    data_state m_state{false};
    mutable data_state mm_state;
  };

 public:
  template <typename ret_t>
  constexpr inline ret_t *mm_data() {
    return (ret_t *)&m_obj_buf.f;
  }
  template <typename ret_t>
  constexpr inline const ret_t *m_data() const {
    return (const ret_t *)&m_obj_buf.f;
  }
  template <typename ret_t>
  constexpr inline ret_t *m_data() {
    return (ret_t *)&m_obj_buf.f;
  }
  template <>
  constexpr inline Type *mm_data<Type>() {
    return this->addressof(m_obj_buf.obj_ref());
  }
  template <>
  constexpr inline const Type *m_data<Type>() const {
    return obj_crtr_t::addressof(m_obj_buf.obj_ref());
  }

  template <>
  constexpr inline Type *m_data<Type>() {
    return obj_crtr_t::addressof(m_obj_buf.obj_ref());
  }

  template <>
  constexpr inline Error_t *mm_data<Error_t>() {
    return m_obj_buf.err_ref().ptr();
  }
  template <>
  constexpr inline const Error_t *m_data<Error_t>() const {
    return m_obj_buf.err_ref().ptr();
  }

  template <>
  constexpr inline Error_t *m_data<Error_t>() {
    return m_obj_buf.err_ref().ptr();
  }

  template <>
  constexpr inline uint8_t *mm_data<uint8_t>() {
    return &m_obj_buf.f;
  }
  template <>
  constexpr inline const uint8_t *m_data<uint8_t>() const {
    return &m_obj_buf.f;
  }

  template <>
  constexpr inline uint8_t *m_data<uint8_t>() {
    return &m_obj_buf.f;
  }

 public:
  constexpr inline bool get_has_error() const volatile noexcept {
    return m_state == data_state::error;
  }
  constexpr inline bool get_has_error() const noexcept {
    return m_state == data_state::error;
  }
  constexpr inline bool get_has_data() const volatile noexcept {
    return m_state == data_state::value;
  }
  constexpr inline bool get_has_data() const noexcept {
    return m_state == data_state::value;
  }
  mjz_stack_obj_warper_deafualt_data_storage_template_t() {}
  ~mjz_stack_obj_warper_deafualt_data_storage_template_t() {
    if (get_has_data()) {
      obj_crtr_t::destroy_at(m_data<Type>());
      /* i really dont like this condition*/
      /*Throw<const char*const>(" unexpected leaked object ");*/
    } else if constexpr (!class_has_Error) {
      m_state = data_state::none;
      return;
    }
    if (get_has_error())
      destrouy_err();
    else
      m_state = data_state::none;
  }

 private:
  constexpr inline void destrouy_err() {
    if constexpr (!class_has_Error) {
      data_state::none;
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
      m_state = data_state::none;
    }
  }

 public:
  constexpr inline void base_unsafe_unsafe_set_type(data_state cast_to_type) {
    m_state = cast_to_type;
  }

 public:
  constexpr inline void base_notify_value_unsafe_init_start() {
    if (get_has_data()) {
      obj_crtr_t::destroy_at(m_data<Type>());
    } else if constexpr (!class_has_Error)
      return;
    else if (get_has_error())
      destrouy_err();
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_deinit_start() {
    if constexpr (!class_has_Error) {
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
    }
    /* else if (!get_has_data()) {
    * i really dont like this condition
    *Throw<const char*const>(" unexpected leaked object ");
    }*/
  }
  constexpr inline void base_notify_value_unsafe_init_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_deinit_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_init_end() {
    m_state = data_state::value;
  }
  constexpr inline void base_notify_value_unsafe_deinit_end() {
    m_state = data_state::none;
  }

  constexpr inline void base_notify_error_unsafe_init_start() {
    if (get_has_data()) {
      obj_crtr_t::destroy_at(m_data<Type>());
    } else if constexpr (!class_has_Error)
      return;
    else if (get_has_error())
      destrouy_err();
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_deinit_start() {
    if constexpr (!class_has_Error) {
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
    }
    /* else if (!get_has_data()) {
    * i really dont like this condition
    *Throw<const char*const>(" unexpected leaked object ");
    }*/
  }
  constexpr inline void base_notify_error_unsafe_init_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_deinit_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_init_end() {
    m_state = data_state::error;
  }
  constexpr inline void base_notify_error_unsafe_deinit_end() {
    m_state = data_state::none;
  }
};
template <typename T, bool can_be_void>
concept C_is_valid_optional_type_helper =
    requires() {
      requires std::is_same_v<std::remove_cvref_t<T>, T>;
      requires !std::is_array_v<T>;
    };
template <typename T, bool can_be_void = 0>
concept C_is_valid_optional_type =
    C_is_valid_optional_type_helper<T, can_be_void> ||
    (can_be_void && std::is_same_v<void, T>);

template <
    C_is_valid_optional_type my_iner_Type_,
    bool construct_obj_on_constructor = true,
    C_mjz_temp_type_obj_algorithims_warpper my_obj_creator_t =
        mjz_temp_type_obj_algorithims_warpper_t<my_iner_Type_>,
    bool do_error_check = 1, bool use_object_in_union = false,
    template <typename, class, class,
              mjz_stack_obj_warper_template_t_data_states, class>
    class mjz_stack_obj_warper_data_storage_template_t =
        mjz_stack_obj_warper_deafualt_data_storage_template_t,
    C_is_valid_optional_type<1> data_strorage_in_Error_t = void,
    C_mjz_temp_type_obj_algorithims_warpper my_err_creator_t =
        mjz_temp_type_obj_algorithims_warpper_t<
            mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t<
                data_strorage_in_Error_t>>>
struct mjz_stack_obj_warper_template_class_t
    : private mjz_stack_obj_warper_data_storage_template_t<
          my_iner_Type_, my_obj_creator_t, my_err_creator_t,
          use_object_in_union
              ? mjz_stack_obj_warper_template_t_data_states::in_union
              : mjz_stack_obj_warper_template_t_data_states::in_aligned_chars,
          data_strorage_in_Error_t> {
 public:
  using Type = my_iner_Type_;
  using my_totaly_uniuqe_type_name_of_content_type = Type;
  using my_totaly_uniuqe_type_name_of_content_type_for_mjz_stack_obj_warper_template_class_t =
      Type;
  static constexpr size_t sizeof_Type = my_obj_creator_t::size_of_type();
  constexpr inline my_obj_creator_t get_obj_creator() {
    return my_obj_creator_t(*((const my_obj_creator_t *)this));
  }

 protected:
  using my_obj_helper_t = mjz_stack_obj_warper_data_storage_template_t<
      my_iner_Type_, my_obj_creator_t, my_err_creator_t,
      use_object_in_union
          ? mjz_stack_obj_warper_template_t_data_states::in_union
          : mjz_stack_obj_warper_template_t_data_states::in_aligned_chars,
      data_strorage_in_Error_t>;
  using my_err_helper_t = my_obj_helper_t;

 public:
  using err_crtr_t = my_obj_helper_t::err_crtr_t;
  using Error_t = my_obj_helper_t::Error_t;
  constexpr static const bool class_has_Error =
      my_obj_helper_t::class_has_Error;

 protected:
  constexpr inline const my_obj_helper_t &m_obj_helper() const & {
    return *this;
  }
  constexpr inline my_obj_helper_t &m_obj_helper() & { return *this; }
  constexpr inline const my_obj_helper_t &&m_obj_helper() const && {
    return move_me();
  }
  constexpr inline my_obj_helper_t &&m_obj_helper() && { return move_me(); }

  constexpr inline const my_err_helper_t &m_err_helper() const & {
    return *this;
  }
  constexpr inline my_err_helper_t &m_err_helper() & { return *this; }
  constexpr inline const my_err_helper_t &&m_err_helper() const && {
    return move_me();
  }
  constexpr inline my_err_helper_t &&m_err_helper() && { return move_me(); }

  constexpr inline const my_obj_creator_t &m_obj_creator() const & {
    return *this;
  }
  constexpr inline my_obj_creator_t &m_obj_creator() & { return *this; }
  constexpr inline const my_obj_creator_t &&m_obj_creator() const && {
    return move_me();
  }
  constexpr inline my_obj_creator_t &&m_obj_creator() && { return move_me(); }

  constexpr inline const my_err_creator_t &m_err_creator() const & {
    return *this;
  }
  constexpr inline my_err_creator_t &m_err_creator() & { return *this; }
  constexpr inline const my_err_creator_t &&m_err_creator() const && {
    return move_me();
  }
  constexpr inline my_err_creator_t &&m_err_creator() && { return move_me(); }

 private:
  constexpr inline Type *OP() & { return m_obj_helper().m_data<Type>(); }
  constexpr inline Type &O() & { return *OP(); }
  constexpr inline const Type *OP() const & {
    return m_obj_helper().m_data<Type>();
  }
  constexpr inline const Type &O() const & { return *OP(); }
  constexpr inline Type &&O() && {
    return std::move(*m_obj_helper().m_data<Type>());
  }
  constexpr inline const Type &&O() const && {
    return std::move(*m_obj_helper().m_data<Type>());
  }
  constexpr inline Error_t *EP() & { return m_err_helper().m_data<Error_t>(); }
  constexpr inline Error_t &E() & { return *EP(); }
  constexpr inline const Error_t *EP() const & {
    return m_err_helper().m_data<Error_t>();
  }
  constexpr inline const Error_t &E() const & { return *EP(); }
  constexpr inline Error_t &&E() && {
    return std::move(*m_err_helper().m_data<Error_t>());
  }
  constexpr inline const Error_t &&E() const && {
    return std::move(*m_err_helper().m_data<Error_t>());
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
      alignas(my_obj_creator_t::alignment) uint8_t buf[sizeof_Type];
      Type *that = (Type *)buf;
      m_obj_creator().construct_at(that, std::forward<args_t>(args)...);
      m_obj_creator().obj_go_to_obj(*place, std::move(*that));
      m_obj_creator().destroy_at(that);
      return place;
    }
    return m_obj_creator().construct_at(place, std::forward<args_t>(args)...);
  }
  template <typename U, typename = std::enable_if_t<std::is_fundamental<Type>>>
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            U &&arg) {
    if (plc_has_obj) {
      Type buf = m_obj_creator().constructor(std::forward<U>(arg));
      m_obj_creator().obj_go_to_obj(*place, std::move(buf));
      return place;
    }
    return m_obj_creator().construct_at(place, std::forward<U>(arg));
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
    notify_unsafe_value_init_in([&](const auto &) {
      return pointer_to_unsafe_data() ==
             construct_in_place(pointer_to_unsafe_data(), get_has_data(),
                                std::forward<args_t>(args)...);
    });
  }
  constexpr inline void destroy() {
    notify_unsafe_value_deinit_in_if(get_has_data(), [&](const auto &) {
      destroy_at_place(pointer_to_unsafe_data());
      return true;
    });
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

  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   Error_t &&src) {
    if (plc_has_obj) {
      m_err_creator().obj_go_to_obj(*place, std::move(src));
      return place;
    }
    return m_err_creator().construct_at(place, std::move(src));
  }
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   Error_t &src) {
    if (plc_has_obj) {
      m_err_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_err_creator().construct_at(place, src);
  }
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   const Error_t &src) {
    if (plc_has_obj) {
      m_err_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_err_creator().construct_at(place, src);
  }
  template <typename... args_t>
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   args_t &&...args) {
    if (plc_has_obj) {
      alignas(my_err_creator_t::alignment) uint8_t buf[sizeof_Type];
      Error_t *that = (Error_t *)buf;
      m_err_creator().construct_at(that, std::forward<args_t>(args)...);
      m_err_creator().obj_go_to_obj(*place, std::move(*that));
      m_err_creator().destroy_at(that);
      return place;
    }
    return m_err_creator().construct_at(place, std::forward<args_t>(args)...);
  }
  template <typename U,
            typename = std::enable_if_t<std::is_fundamental<Error_t>>>
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj, U &&arg) {
    if (plc_has_obj) {
      Error_t buf = m_err_creator().constructor(std::forward<U>(arg));
      m_err_creator().obj_go_to_obj(*place, std::move(buf));
      return place;
    }
    return m_err_creator().construct_at(place, std::forward<U>(arg));
  }
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj) {
    if (plc_has_obj) {
      m_err_creator().destroy_at(place);
      m_err_creator().construct_at(place);
      return place;
    }
    return m_err_creator().construct_at(place);
  }
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   Error_t *src) {
    return err_construct_in_place(place, *src);
  }
  constexpr inline Error_t *err_construct_in_place(Error_t *place,
                                                   bool plc_has_obj,
                                                   const Error_t *src) {
    return err_construct_in_place(place, *src);
  }
  constexpr inline void err_destroy_at_place(Error_t *place) {
    m_err_creator().destroy_at(place);
  }
  template <typename... args_t>
  constexpr inline void err_construct(args_t &&...args) {
    notify_unsafe_error_init_in([&](const auto &) {
      return EP() == err_construct_in_place(EP(), get_has_error(),
                                            std::forward<args_t>(args)...);
    });
  }
  constexpr inline void err_destroy() {
    notify_unsafe_error_deinit_in_if(get_has_error(), [&](const auto &) {
      err_destroy_at_place(EP());
      return true;
    });
  }
  constexpr inline Error_t *err_move_to_place(Error_t *dest,
                                              bool dest_has_obj) {
    if (has_error())
      return err_construct_in_place(dest, dest_has_obj, std::move(*EP()));
    err_destroy();
    return 0;
  }
  constexpr inline Error_t *err_copy_to_place(Error_t *dest,
                                              bool dest_has_obj) {
    if (has_error()) return err_construct_in_place(dest, dest_has_obj, *EP());
    return 0;
  }

 public:
  constexpr inline Type &o() & { return **this; }
  constexpr inline Type *op() & { return operator->(); }
  constexpr inline const Type &o() const & { return **this; }
  constexpr inline const Type *op() const & { return operator->(); }
  constexpr inline Type &&o() && { return std::move(*move_me()); }
  constexpr inline Type *op() && = delete;
  constexpr inline const Type &&o() const && { return std::move(*move_me()); }
  constexpr inline const Type *op() const && = delete;
  // unsafe object functions begin

  constexpr inline Type &uo() &noexcept { return *uop(); }
  constexpr inline Type *uop() &noexcept {
    if (get_has_data()) return OP();
    return nullptr;
  }
  constexpr inline const Type &uo() const &noexcept { return *uop(); }
  constexpr inline const Type *uop() const &noexcept {
    if (get_has_data()) return OP();
    return nullptr;
  }
  constexpr inline Type &&uo() &&noexcept { return std::move(uo()); }
  constexpr inline Type *uop() && = delete;
  constexpr inline const Type &&uo() const &&noexcept {
    return std::move(uo());
  }
  constexpr inline const Type *uop() const && = delete;

  // Ultra nunsafe
  using data_state_t = typename my_obj_helper_t::data_state;
  using data_state = data_state_t;
  constexpr const static data_state_t cast_to_none = data_state_t::none;

  constexpr const static data_state_t cast_to_value = data_state_t::value;

  constexpr const static data_state_t cast_to_error = data_state_t::error;
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_unsafe_set_type(data_state cast_to_state) & {
    m_obj_helper().base_unsafe_unsafe_set_type(cast_to_state);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_start() & {
    m_obj_helper().base_notify_value_unsafe_init_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_start() & {
    m_obj_helper().base_notify_value_unsafe_deinit_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_start() && {
    m_obj_helper().base_notify_value_unsafe_init_start();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_start() && {
    m_obj_helper().base_notify_value_unsafe_deinit_start();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_fail() & {
    m_obj_helper().base_notify_value_unsafe_init_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_fail() & {
    m_obj_helper().base_notify_value_unsafe_deinit_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_fail() && {
    m_obj_helper().base_notify_value_unsafe_init_fail();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_fail() && {
    m_obj_helper().base_notify_value_unsafe_deinit_fail();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_end() & {
    m_obj_helper().base_notify_value_unsafe_init_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_end() & {
    m_obj_helper().base_notify_value_unsafe_deinit_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_end() && {
    m_obj_helper().base_notify_value_unsafe_init_end();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_end() && {
    m_obj_helper().base_notify_value_unsafe_deinit_end();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_start() & {
    m_err_helper().base_notify_error_unsafe_init_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_start() & {
    m_err_helper().base_notify_error_unsafe_deinit_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_start() && {
    m_err_helper().base_notify_error_unsafe_init_start();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_start() && {
    m_err_helper().base_notify_error_unsafe_deinit_start();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_fail() & {
    m_err_helper().base_notify_error_unsafe_init_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_fail() & {
    m_err_helper().base_notify_error_unsafe_deinit_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_fail() && {
    m_err_helper().base_notify_error_unsafe_init_fail();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_fail() && {
    m_err_helper().base_notify_error_unsafe_deinit_fail();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_end() & {
    m_err_helper().base_notify_error_unsafe_init_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_end() & {
    m_err_helper().base_notify_error_unsafe_deinit_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_end() && {
    m_err_helper().base_notify_error_unsafe_init_end();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_end() && {
    m_err_helper().base_notify_error_unsafe_deinit_end();
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      me_notify_unsafe_value_init_or_deinit_in(bool is_init,
                                               function_type &&inner_function,
                                               Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(*this, std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              *this, std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            *this, std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      me_notify_unsafe_value_init_or_deinit_in(bool is_init,
                                               function_type &&inner_function,
                                               Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->move_me(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              move_me(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            move_me(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                              function_type &&inner_function,
                                              Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                              function_type &&inner_function,
                                              Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_deinit_in(function_type &&inner_function,
                                   Ts... args) & {
    me_notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_deinit_in(function_type &&inner_function,
                                   Ts... args) && {
    move_me().me_notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_in(function_type &&inner_function,
                                 Ts... args) && {
    move_me().me_notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_deinit_in_if(bool do_notify,
                                   function_type &&inner_function,
                                   Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_deinit_in_if(bool do_notify,
                                   function_type &&inner_function,
                                   Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_deinit_in_if(bool do_notify,
                                      function_type &&inner_function,
                                      Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_deinit_in_if(bool do_notify,
                                      function_type &&inner_function,
                                      Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                                 Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                                 Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_in_if(bool do_notify,
                                    function_type &&inner_function,
                                    Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_in_if(bool do_notify,
                                    function_type &&inner_function,
                                    Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      notify_unsafe_value_init_or_deinit_in(bool is_init,
                                            function_type &&inner_function,
                                            Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      notify_unsafe_value_init_or_deinit_in(bool is_init,
                                            function_type &&inner_function,
                                            Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                           function_type &&inner_function,
                                           Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                           function_type &&inner_function,
                                           Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      me_notify_unsafe_error_init_or_deinit_in(bool is_init,
                                               function_type &&inner_function,
                                               Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(*this, std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              *this, std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            *this, std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      me_notify_unsafe_error_init_or_deinit_in(bool is_init,
                                               function_type &&inner_function,
                                               Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->move_me(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              move_me(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            move_me(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                              function_type &&inner_function,
                                              Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                              function_type &&inner_function,
                                              Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_deinit_in(function_type &&inner_function,
                                   Ts... args) & {
    me_notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_deinit_in(function_type &&inner_function,
                                   Ts... args) && {
    move_me().me_notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_in(function_type &&inner_function,
                                 Ts... args) && {
    move_me().me_notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_deinit_in_if(bool do_notify,
                                   function_type &&inner_function,
                                   Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_deinit_in_if(bool do_notify,
                                   function_type &&inner_function,
                                   Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_deinit_in_if(bool do_notify,
                                      function_type &&inner_function,
                                      Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_deinit_in_if(bool do_notify,
                                      function_type &&inner_function,
                                      Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                                 Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                                 Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_in_if(bool do_notify,
                                    function_type &&inner_function,
                                    Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_in_if(bool do_notify,
                                    function_type &&inner_function,
                                    Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      notify_unsafe_error_init_or_deinit_in(bool is_init,
                                            function_type &&inner_function,
                                            Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      notify_unsafe_error_init_or_deinit_in(bool is_init,
                                            function_type &&inner_function,
                                            Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                           function_type &&inner_function,
                                           Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                           function_type &&inner_function,
                                           Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() & {
    return (uint8_t *)(m_obj_helper().m_data<uint8_t>());
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const & {
    return (uint8_t *)(m_obj_helper().m_data<uint8_t>());
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const && =
      delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() const && = delete;
  constexpr inline Type *pointer_to_unsafe_data() & { return OP(); }
  constexpr inline const Type *pointer_to_unsafe_data() const & { return OP(); }

  constexpr inline mjz_stack_obj_warper_template_class_t &&move_me() {
    return std::move(*this);
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &&move_me()
      const {
    return std::move(*this);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_me() & {
    return *this;
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &copy_me()
      const & {
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_me() && {
    return *this;
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &copy_me()
      const && {
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() & {
    return {*this};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() const & {
    return {*this};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() const && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t temp_me() const {
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t temp_me() {
    return *this;
  }

 public:  // unsafe may cuse undefined behavior
  mjz_stack_obj_warper_template_class_t &remove_const() const & {
    return *mjz_ard::remove_const(this);
  }
  mjz_stack_obj_warper_template_class_t &&remove_const() const && {
    return std::move(*mjz_ard::remove_const(this));
  }
  mjz_stack_obj_warper_template_class_t &remove_const() & { return *this; }
  mjz_stack_obj_warper_template_class_t &&remove_const() && {
    return std::move(*this);
  }
  Type &remove_const_obj() const & { return *m_obj_helper().mm_data<Type>(); }
  Type &&remove_const_obj() const && {
    return std::move(*m_obj_helper().mm_data<Type>());
  }
  Type &remove_const_obj() & { return *m_obj_helper().mm_data<Type>(); }
  Type &&remove_const_obj() && {
    return std::move(*m_obj_helper().mm_data<Type>());
  }
  const Type &&move() const & { return std::move(o()); }
  const Type &&move() const && { return std::move(o()); }
  Type &&move() & { return std::move(o()); }
  Type &&move() && { return std::move(o()); }

  constexpr inline Type &uuo() &noexcept { return *uuop(); }
  constexpr inline Type *uuop() &noexcept { return OP(); }
  constexpr inline const Type &uuo() const &noexcept { return *uuop(); }
  constexpr inline const Type *uuop() const &noexcept { return OP(); }
  constexpr inline Type &&uuo() &&noexcept { return std::move(O()); }
  constexpr inline const Type &&uuo() const &&noexcept {
    return std::move(O());
  }
  constexpr inline const Type *uuop() const && = delete;
  constexpr inline Type *uuop() && = delete;

  // no safe use case
  constexpr inline const Type *invalid_object_pointer() const && {
    return uuop();
  };
  // no safe use case
  constexpr inline Type *invalid_object_pointer() && { return uuop(); }

  // unsafe object functions end

  constexpr inline mjz_stack_obj_warper_template_class_t() {
    if constexpr (construct_obj_on_constructor) {
      construct();
    }
  };
  constexpr inline mjz_stack_obj_warper_template_class_t(mjz_init_optional_t) {
    construct();
  };
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_init_optional_if_t do_init) {
    if (do_init) construct();
  };

  template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_no_init_optional_t<init_if_iam_not_zero_the_base_with_me_minus_one>)
    requires(init_if_iam_not_zero_the_base_with_me_minus_one != 0)
  {
    static_assert(
        requires {
          construct(mjz_no_init_optional_t<
                    init_if_iam_not_zero_the_base_with_me_minus_one - 1>());
        });
    construct(
        mjz_no_init_optional_t<init_if_iam_not_zero_the_base_with_me_minus_one -
                               1>());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_no_init_optional_t<0>) {
    notify_unsafe_value_deinit_in([&](const auto &) {});
  };

  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(T0 &&arg0,
                                                         T_s &&...args) {
    construct(std::move(arg0), std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(const T0 &arg0,
                                                         T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(T0 &arg0,
                                                         T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  constexpr inline ~mjz_stack_obj_warper_template_class_t() { destroy(); }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) {
    if (s_obj_w.has_data()) construct(std::move(s_obj_w.operator*()));
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_stack_obj_warper_template_class_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }

 public:
  template <class Func_t, typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t s_create_op(
      Func_t Func, Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t obj(
        std::forward<Ts>(class_constructor_args)...);
    std::forward<Func_t>(Func)(obj);
    return obj;
  }
  template <class Func_t, typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t
  s_create_op_ignore_args_if_not_fn(Func_t Func,
                                    Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t o;
    mjz_stack_obj_warper_template_class_t *a = &o;
    static_assert(
        std::is_same_v<decltype(!!std::forward<Func_t>(Func)(*a)), bool>);
    if (!!std::forward<Func_t>(Func)(*this)) {
      a->~mjz_stack_obj_warper_template_class_t();
      auto *p = new (a) mjz_stack_obj_warper_template_class_t(
          std::forward<Ts>(class_constructor_args)...);
      if (!p) Throw("failed to construct ");
    }
    return o;
  }
  template <typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t
  s_create_op_ignore_args_if_not(bool b, Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t o;
    if (b) o.replace_me(std::forward<Ts>(class_constructor_args)...);
    return o;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &replace_me(
      Ts &&...args) & {
    this->~mjz_stack_obj_warper_template_class_t();
    auto *p = new (this)
        mjz_stack_obj_warper_template_class_t(std::forward<Ts>(args)...);
    if (!p) Throw("failed to construct ");
    return *this;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &replace_obj(
      Ts &&...args) & {
    emplace(std::forward<Ts>(args)...);
    return *this;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&replace_me(
      Ts &&...args) && {
    return replace_me(std::forward<Ts>(args)...).move_me();
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&replace_obj(
      Ts &&...args) && {
    return replace_obj(std::forward<Ts>(args)...).move_me();
  }
  template <class Func_t, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_with_me(
      Func_t Func, Ts &&...args) & {
    std::forward<Func_t>(Func)(*this, std::forward<Ts>(args)...);
    return *this;
  }
  template <class Func_t, typename... Ts>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_with_me(
      Func_t Func, Ts &&...args) const & {
    std::forward<Func_t>(Func)(*this, std::forward<Ts>(args)...);
    return *this;
  }
  template <class Func_t, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_with_me(
      Func_t Func, Ts &&...args) && {
    std::forward<Func_t>(Func)(move_me(), std::forward<Ts>(args)...);
    return move_me();
  }
  template <class Func_t, typename... Ts>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_with_me(
      Func_t Func, Ts &&...args) const && {
    std::forward<Func_t>(Func)(move_me(), std::forward<Ts>(args)...);
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t(const Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(Type &&obj) {
    construct(std::move(obj));
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      Type &&obj) && {
    construct(std::move(obj));
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      Type &obj) && {
    construct(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      const Type &obj) && {
    construct(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      Type &&obj) & {
    construct(std::move(obj));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      Type &obj) & {
    construct(obj);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      const Type &obj) & {
    construct(obj);
    return *this;
  }
  template <class U>
  inline constexpr
      typename std::enable_if<std::conjunction_v<std::is_constructible<Type, U>,
                                                 std::is_assignable<Type &, U>>,
                              mjz_stack_obj_warper_template_class_t &>::type
      operator=(U &&Obj) & {
    construct(std::forward<U>(Obj));
    return *this;
  }

  template <class U>
  inline constexpr typename std::enable_if<
      std::conjunction_v<
          std::is_constructible<mjz_stack_obj_warper_template_class_t, U>,
          std::negation<std::conjunction<std::is_constructible<Type, U>,
                                         std::is_assignable<Type &, U>>>>,
      mjz_stack_obj_warper_template_class_t &>::type
  operator=(U &&Obj) & {
    operator=(mjz_stack_obj_warper_template_class_t(std::forward<U>(Obj)));
    return *this;
  }

  template <class U>
  inline constexpr
      typename std::enable_if<std::conjunction_v<std::is_constructible<Type, U>,
                                                 std::is_assignable<Type &, U>>,
                              mjz_stack_obj_warper_template_class_t &&>::type
      operator=(U &&Obj) && {
    construct(std::forward<U>(Obj));
    return move_me();
  }

  template <class U>
  inline constexpr typename std::enable_if<
      std::conjunction_v<
          std::is_constructible<mjz_stack_obj_warper_template_class_t, U>,
          std::negation<std::conjunction<std::is_constructible<Type, U>,
                                         std::is_assignable<Type &, U>>>>,
      mjz_stack_obj_warper_template_class_t &&>::type
  operator=(U &&Obj) && {
    operator=(mjz_stack_obj_warper_template_class_t(std::forward<U>(Obj)));
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) && {
    if (!!s_obj_w) operator=(std::move(s_obj_w.operator*()));
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) && {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      mjz_stack_obj_warper_template_class_t &s_obj_w) && {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) & {
    if (!!s_obj_w) operator=(std::move(s_obj_w.operator*()));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) & {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      mjz_stack_obj_warper_template_class_t &s_obj_w) & {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      const mjz_stack_obj_warper_template_class_t &obj) & {
    return operator=(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      mjz_stack_obj_warper_template_class_t &obj) & {
    return operator=(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      mjz_stack_obj_warper_template_class_t &&obj) & {
    return operator=(std::move(obj));
  }
  template <typename arg_T, typename Type>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      std::initializer_list<arg_T> list) & {
    construct(list);
    return *this;
  }
  template <typename arg_T>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      iterator_template_t<arg_T> list) & {
    construct(list);
    return *this;
  }
  template <typename... arguments_types>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      arguments_types &&...args) & {
    construct(std::forward<arguments_types>(args)...);

    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      const mjz_stack_obj_warper_template_class_t &obj) && {
    operator=(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      mjz_stack_obj_warper_template_class_t &obj) && {
    operator=(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      mjz_stack_obj_warper_template_class_t &&obj) && {
    operator=(std::move(obj));
    return move_me();
  }
  template <typename arg_T, typename Type>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      std::initializer_list<arg_T> list) && {
    construct(list);
    return move_me();
  }
  template <typename arg_T>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      iterator_template_t<arg_T> list) && {
    construct(list);
    return move_me();
  }
  template <typename... arguments_types>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      arguments_types &&...args) && {
    construct(std::forward<arguments_types>(args)...);

    return move_me();
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &de_init() & {
    destroy();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &de_init(
      int fill_VAL) & {
    destroy();
    static_str_algo::memset(m_obj_helper().m_data<uint8_t>(), fill_VAL,
                            sizeof_Type);
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&de_init() && {
    destroy();

    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&de_init(
      int fill_VAL) && {
    destroy();
    static_str_algo::memset(m_obj_helper().m_data<uint8_t>(), fill_VAL,
                            sizeof_Type);

    return move_me();
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
 public:
  constexpr inline Type &if_no_obj_then_create() & {
    if (!get_has_data()) construct();
    return *pointer_to_unsafe_data();
  }
  Type &&if_no_obj_then_create() && {
    if (!get_has_data()) construct();
    return std::move(move_me().O());
  }

 public:
 public:
  constexpr inline const Type *throw_if_no_data_or_give_data() && = delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const && =
      delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const & {
    if (!get_has_data()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
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
    if (has_data()) return std::move(move_me().O());
    Throw<std::out_of_range>(
        "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data bad "
        "access");
  }  // overload dosnt give ptr
  constexpr inline const Type &&operator->() const && {
    if (has_data()) return std::move(move_me().O());
    Throw<std::out_of_range>(
        "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data bad "
        "access");
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
    return std::move(move_me().operator->());
  }
  constexpr inline const Type &&operator*() const && {
    return std::move(move_me().operator->());
  }
  constexpr inline Type &operator*() & { return *operator->(); }

  constexpr inline const Type *operator->() const & {
    return pointer_to_data();
  }
  constexpr inline const Type &operator*() const & { return *operator->(); }
  constexpr inline const Type &operator()() const & { return **this; }
  constexpr inline Type &&operator()() && { return std::move(*move_me()); }
  constexpr inline const Type &&operator()() const && {
    return std::move(*move_me());
  }
  constexpr inline Type &operator()() & { return **this; }
  using my_Type_t = mjz_stack_obj_warper_template_class_t;

  constexpr inline my_Type_t &destroy_if_valid() & {
    de_init();
    return *this;
  }
  constexpr inline my_Type_t &create_if_empty() & {
    if (has_data()) {
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &if_valid_destroy_else_create() & {
    if (has_data()) {
      de_init();
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &&destroy_if_valid() && {
    de_init();
    return move_me();
  }
  constexpr inline my_Type_t &&create_if_empty() && {
    if (has_data()) {
      return move_me();
    }
    construct();
    return move_me();
  }
  constexpr inline my_Type_t &&if_valid_destroy_else_create() && {
    if (has_data()) {
      de_init();
      return move_me();
    }
    construct();
    return move_me();
  }

  constexpr inline my_Type_t &operator~() & { return destroy_if_valid(); }
  constexpr inline my_Type_t &operator+() & { return create_if_empty(); }
  constexpr inline my_Type_t &operator-() & {
    return if_valid_destroy_else_create();
  }
  constexpr inline my_Type_t &&operator~() && {
    return move_me().destroy_if_valid();
  }
  constexpr inline my_Type_t &&operator+() && {
    return move_me().create_if_empty();
  }
  constexpr inline my_Type_t &&operator-() && {
    return move_me().if_valid_destroy_else_create();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator--() & {
    de_init();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator--() && {
    de_init();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator--()
      const && = delete;
  constexpr inline mjz_stack_obj_warper_template_class_t operator--(int) {
    mjz_stack_obj_warper_template_class_t temp(move_me());
    de_init();
    return temp;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator++() & {
    // de_init();
    construct();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator++() && {
    // de_init();
    construct();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator++()
      const && = delete;
  constexpr inline mjz_stack_obj_warper_template_class_t operator++(int) {
    mjz_stack_obj_warper_template_class_t temp(move_me());
    // you may want to reuse data soo
    // de_init();
    construct();
    return temp;
  }

  /*
  IMPORTANT NOTE :
  use iter_me to set up an iterator
  (or clone_me because they are the same .
  but use iter_me because its more readable.)
  */

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() & {
    return {*this};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() const & {
    return {*this};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t new_me() && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() const && {
    return {move_me()};
  }
  constexpr inline iterator_template_t<Type> get_iter() & {
    if (!!*this) return {uop(), uop(), uop() + 1};
    return {};
  }
  constexpr inline iterator_template_t<const Type> get_iter() const & {
    if (!!*this) return {uop(), uop(), uop() + 1};
    return {};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *begin() & {
    return this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *end() & {
    return begin() + 1;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin()
      const & {
    return this;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() const & {
    return begin() + 1;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin()
      const & {
    return begin();
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend() const & {
    return end();
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin()
      const && = delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() const && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin()
      const && = delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend()
      const && = delete;
  constexpr inline static size_t size() { return 1; }  // for iterator
  constexpr inline static size_t my_size() { return sizeof_Type; }
  constexpr inline static size_t size_T() { return my_size(); }
  constexpr inline Type &operator()(Type &&moved) & {
    return emplace(std::move(moved));
  }
  constexpr inline Type &operator()(const Type &&moved) & {
    return emplace(std::move(moved));
  }
  template <typename... args_t>
  constexpr inline Type &operator()(args_t &&...args) & {
    return emplace(std::forward<args_t>(args)...);
  }
  constexpr inline Type &operator()(Type &moved) & { return emplace(moved); }
  constexpr inline Type &operator()(const Type &moved) & {
    return emplace(moved);
  }
  constexpr inline Type &operator()(Type &&moved) && {
    emplace(std::move(moved));
    return move_me();
  }
  constexpr inline Type &operator()(const Type &&moved) && {
    emplace(std::move(moved));
    return move_me();
  }
  constexpr inline Type &operator()(Type &moved) && {
    emplace(moved);
    return move_me();
  }
  constexpr inline Type &operator()(const Type &moved) && {
    emplace(moved);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator[](
      std::function<void(Type &)> f) & {
    if (has_data()) f(O());
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator[](
      std::function<void(Type &&)> f) && {
    if (has_data()) f(std::move(move_me().O()));
    return move_me();
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &&operator[](
      std::function<void(const Type &&)> f) const && {
    if (has_data()) f(std::move(move_me().O()));

    return move_me();
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &operator[](
      std::function<void(const Type &)> f) const & {
    if (has_data()) f(O());
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) const & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return *this;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return *this;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) && {
    if (do_throw) {
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return move_me();
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) const && {
    if (do_throw) {
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return move_me();
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
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
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(std::forward<args_t>(args)...);
    return move_me();
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(std::forward<args_t>(args)...);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return no_data_function(*this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return no_data_function(move_me(), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return no_data_function(move_me(), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return no_data_function(*this, std::forward<args_t>(args)...);
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(*this, std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(*this, std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(move_me(), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(move_me(), std::forward<args_t>(args)...);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }

  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const & {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) & {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) && {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const && {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return move_me();
    }
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }

  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (!has_data()) {
      return no_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (!has_data()) {
      return no_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (!has_data()) {
      return no_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (!has_data()) {
      return no_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const & {
    if (!has_data()) {
      no_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) & {
    if (!has_data()) {
      no_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) && {
    if (!has_data()) {
      no_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const && {
    if (!has_data()) {
      no_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    return move_me();
  }

 public:
  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_class_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_class_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_class_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool operator==(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_class_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
 protected:
  constexpr inline bool get_has_data() const volatile noexcept {
    return m_obj_helper().get_has_data();
  }
  constexpr inline bool get_has_data() const noexcept {
    return m_obj_helper().get_has_data();
  }
  constexpr inline bool get_has_error() const volatile noexcept {
    return m_obj_helper().get_has_error();
  }
  constexpr inline bool get_has_error() const noexcept {
    return m_obj_helper().get_has_error();
  }

 public:
  constexpr inline Error_t &uue() &noexcept { return *uuep(); }
  constexpr inline Error_t *uuep() &noexcept { return EP(); }
  constexpr inline const Error_t &uue() const &noexcept { return *uuep(); }
  constexpr inline const Error_t *uuep() const &noexcept { return EP(); }
  constexpr inline Error_t &&uue() &&noexcept { return std::move(E()); }
  constexpr inline const Error_t &&uue() const &&noexcept {
    return std::move(E());
  }
  constexpr inline const Error_t *uuep() const && = delete;
  constexpr inline Error_t *uuep() && = delete;

 public:
  constexpr inline Error_t &e() & { return *ep(); }
  constexpr inline Error_t *ep() & { return throw_if_no_error_or_give_error(); }
  constexpr inline const Error_t &e() const & { return *ep(); }
  constexpr inline const Error_t *ep() const & {
    return throw_if_no_error_or_give_error();
  }
  constexpr inline Error_t &&e() && { return std::move(*move_me()); }
  constexpr inline Error_t *ep() && = delete;
  constexpr inline const Error_t &&e() const && {
    return std::move(*move_me());
  }
  constexpr inline const Error_t *ep() const && = delete;
  // unsafe object functions begin

  constexpr inline Error_t &ue() &noexcept { return *uep(); }
  constexpr inline Error_t *uep() &noexcept {
    if (get_has_error()) return ep();
    return nullptr;
  }
  constexpr inline const Error_t &ue() const &noexcept { return *uep(); }
  constexpr inline const Error_t *uep() const &noexcept {
    if (get_has_error()) return ep();
    return nullptr;
  }
  constexpr inline Error_t &&ue() &&noexcept { return std::move(ue()); }
  constexpr inline Error_t *uep() && = delete;
  constexpr inline const Error_t &&ue() const &&noexcept {
    return std::move(ue());
  }
  constexpr inline const Error_t *uep() const && = delete;

  constexpr inline const Error_t *throw_if_no_error_or_give_error() const & {
    if (!get_has_error()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
            "access");
      } else {
        return (const Error_t *)nullptr;  // :(
      }
    }
    return uuep();
  }

  constexpr inline Error_t *throw_if_no_error_or_give_error() & {
    if (!get_has_error()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
            "access");
      } else {
        return (Error_t *)nullptr;  // :(
      }
    }
    return uuep();
  }
  constexpr inline bool has_error() const noexcept { return get_has_error(); }
  constexpr inline bool has_error() const volatile noexcept {
    return get_has_error();
  }
  constexpr inline bool has_data() const noexcept { return get_has_data(); }
  constexpr inline bool has_data() const volatile noexcept {
    return get_has_data();
  }
  constexpr inline bool operator!() const noexcept { return !get_has_data(); }
  constexpr inline operator Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() const & { return pointer_to_data(); }
  constexpr inline operator Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() const & {
    return *pointer_to_data();
  }
  constexpr inline operator Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() const && {
    return std::move(*move_me());
  }
  constexpr inline operator Type() & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() const & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr inline operator Type() const && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr explicit operator bool() &noexcept { return has_data(); }
  constexpr explicit operator bool() &&noexcept { return has_data(); }
  constexpr explicit operator bool() const &noexcept { return has_data(); }
  constexpr explicit operator bool() const &&noexcept { return has_data(); }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  remove_volatile() volatile &noexcept {
    return *const_cast<mjz_stack_obj_warper_template_class_t *>(this);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  remove_volatile() volatile &&noexcept {
    return *const_cast<mjz_stack_obj_warper_template_class_t *>(this);
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  remove_volatile() volatile const &noexcept {
    return std::move(
        *const_cast<const mjz_stack_obj_warper_template_class_t *>(this));
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  remove_volatile() volatile const &&noexcept {
    return std::move(
        *const_cast<const mjz_stack_obj_warper_template_class_t *>(this));
  }
  constexpr explicit operator bool() volatile &noexcept { return has_data(); }
  constexpr explicit operator bool() volatile &&noexcept { return has_data(); }
  constexpr explicit operator bool() const volatile &noexcept {
    return has_data();
  }
  constexpr explicit operator bool() const volatile &&noexcept {
    return has_data();
  }
  constexpr bool has_value() const noexcept { return has_data(); }

  template <typename... Ts>
  struct deafult_construct_at_fn : private my_obj_creator_t {
    inline constexpr bool operator()(T *dest, Ts &&...args) {
      return !!my_obj_creator_t::construct_at(dest, std::forward<Ts>(args)...);
    }
  };
  template <>
  struct deafult_construct_at_fn<void> : private my_obj_creator_t {
    inline constexpr bool operator()(T *dest) {
      return !!my_obj_creator_t::construct_at(dest);
    }
  };
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &&fn_emplace(
      std::function<bool(T *, Args...)> construct_at_fn, Args &&...args) && {
    ~*this;
    notify_unsafe_value_init_in([&](const auto &) {
      return construct_at_fn(uuop(), std::forward<Args>(args)...);
    });
    return move_me();
  }
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &fn_emplace(
      std::function<bool(T *, Args...)> construct_at_fn, Args &&...args) & {
    ~*this;
    notify_unsafe_value_init_in([&](const auto &) {
      return construct_at_fn(uuop(), std::forward<Args>(args)...);
    });
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&fn_emplace(
      std::function<bool(T *)> construct_at_fn =
          deafult_construct_at_fn<void>()) && {
    ~*this;
    notify_unsafe_value_init_in(
        [&](const auto &) { return (construct_at_fn(uuop())); });
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &fn_emplace(
      std::function<bool(T *)> construct_at_fn =
          deafult_construct_at_fn<void>()) & {
    ~*this;
    notify_unsafe_value_init_in(
        [&](const auto &) { return (construct_at_fn(uuop())); });
    return *this;
  }

  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &&dfn_emplace(
      Args &&...args) && {
    return move_me().fn_emplace<Args...>(deafult_construct_at_fn<Args...>(),
                                         std::forward<Args>(args)...);
  }
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &dfn_emplace(
      Args &&...args) & {
    return fn_emplace<Args...>(deafult_construct_at_fn<Args...>(),
                               std::forward<Args>(args)...);
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&dfn_emplace() && {
    return move_me().fn_emplace(deafult_construct_at_fn<void>());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &dfn_emplace() & {
    return fn_emplace(deafult_construct_at_fn<void>());
  }
  template <class... Args>
  constexpr inline T &emplace(Args &&...args) & {
    ~*this;
    construct(std::forward<Args>(args)...);
    return o();
  }
  template <class U, class... Args>

  constexpr inline T &emplace(std::initializer_list<U> ilist,
                              Args &&...args) & {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return o();
  }
  template <class... Args>
  constexpr inline T &&emplace(Args &&...args) && {
    ~*this;
    construct(std::forward<Args>(args)...);
    return move_me().o();
  }
  template <class U, class... Args>

  constexpr inline T &&emplace(std::initializer_list<U> ilist,
                               Args &&...args) && {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return move_me().o();
  }
  void reset() noexcept {
    try {
      ~(*this);
    } catch (...) {
    }
  }
  template <class F>
  mjz_stack_obj_warper_template_class_t or_else(F &&f) const & {
    return has_data() ? *this : std::forward<F>(f)();
  }
  template <class F>
  mjz_stack_obj_warper_template_class_t or_else(F &&f) && {
    return has_data() ? move_me() : std::forward<F>(f)();
  }
  constexpr Type &&value() && { return *move_me(); }
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
    return has_data() ? std::move(*move_me())
                      : static_cast<T>(std::forward<U>(default_value));
  }

  template <class... Ts>
  inline constexpr Type &value_or_emplace(Ts &&...args) & {
    if (!!*this) return o();
    emplace(std::forward<Ts>(args)...);
    return o();
  }
  template <class... Ts>
  inline constexpr Type &&value_or_emplace(Ts &&...args) && {
    if (!!*this) return std::move(o());
    emplace(std::forward<Ts>(args)...);
    return std::move(o());
  }

  template <class... Ts>
  inline constexpr mjz_stack_obj_warper_template_class_t &optional_or_emplace(
      Ts &&...args) & {
    if (!!*this) return *this;
    emplace(std::forward<Ts>(args)...);
    return *this;
  }
  template <class... Ts>
  inline constexpr mjz_stack_obj_warper_template_class_t &&optional_or_emplace(
      Ts &&...args) && {
    if (!!*this) return move_me();
    emplace(std::forward<Ts>(args)...);
    return move_me();
  }

  template <class F>
  constexpr auto transform(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
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
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_to(
      mjz_stack_obj_warper_template_class_t &dest) & {
    if (this != &dest)
      dest.notify_unsafe_value_init_in([&](const auto &) {
        return copy_to(dest.pointer_to_unsafe_data(), dest.has_data()) ==
               dest.pointer_to_unsafe_data();
      });
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &move_to(
      mjz_stack_obj_warper_template_class_t &dest) {
    if (this != &dest)
      dest.notify_unsafe_value_init_in([&](const auto &) {
        return move_to(dest.pointer_to_unsafe_data(), dest.has_data()) ==
               dest.pointer_to_unsafe_data();
      });
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *copy_to(
      mjz_stack_obj_warper_template_class_t *dest) & {
    return &copy_to(*dest);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *move_to(
      mjz_stack_obj_warper_template_class_t *dest) {
    return &move_to(*dest);
  }
};

template <typename my_iner_Type_, bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = false>
using mjz_stack_obj_warper_template_t = mjz_stack_obj_warper_template_class_t<
    my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
    do_error_check, use_object_in_union>;

template <typename my_iner_Type_, bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = true>
using U_mjz_stack_obj_warper_template_t = mjz_stack_obj_warper_template_class_t<
    my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
    do_error_check, use_object_in_union>;

template <typename my_iner_Type_, bool construct_obj_on_constructor = false,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = true>
using OU_mjz_stack_obj_warper_template_t =
    mjz_stack_obj_warper_template_class_t<
        my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
        do_error_check, use_object_in_union>;

template <typename my_iner_Type_, bool construct_obj_on_constructor = false,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = false>
using O_mjz_stack_obj_warper_template_t = mjz_stack_obj_warper_template_class_t<
    my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
    do_error_check, use_object_in_union>;

template <
    typename OPTIONAL_type, typename OPTIONAL_error_type,
    typename = typename OPTIONAL_type::
        my_totaly_uniuqe_type_name_of_content_type_for_mjz_stack_obj_warper_template_class_t,
    typename = typename OPTIONAL_error_type::
        my_totaly_uniuqe_type_name_of_content_type_for_mjz_stack_obj_warper_template_class_t>
struct mjz_stack_obj_warper_with_error_template_class_t : public OPTIONAL_type {
 public:
  using Base_t = OPTIONAL_type;
  /*
  NOTE:
  if the types are the same type the error will not show if one argument is set
  */
  template <
      typename U = std::enable_if_t<std::negation_v<std::is_same<
          typename OPTIONAL_type::Type, typename OPTIONAL_error_type::Type>>>>
  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_error_type &&er)
      : optional_error(er.move_me()) {}

  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type &&op)
      : Base_t(op.move_me()) {}

  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type &&op, OPTIONAL_error_type &&er)
      : Base_t(Base_t::s_create_op_ignore_args_if_not(!er, std::move(op))),
        optional_error(er.move_me()) {}

  template <
      typename U = std::enable_if_t<std::negation_v<std::is_same<
          typename OPTIONAL_type::Type, typename OPTIONAL_error_type::Type>>>>
  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_error_type const &er)
      : optional_error(er.copy_me()) {}

  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type const &op)
      : Base_t(op.copy_me()) {}

  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type const &op, OPTIONAL_error_type const &er)
      : Base_t(Base_t::s_create_op_ignore_args_if_not(!er, op.copy_me())),
        optional_error(er.copy_me()) {}

  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type const &op, OPTIONAL_error_type &&er)
      : Base_t(Base_t::s_create_op_ignore_args_if_not(!er, op.copy_me())),
        optional_error(er.move_me()) {}
  inline constexpr mjz_stack_obj_warper_with_error_template_class_t(
      OPTIONAL_type &&op, OPTIONAL_error_type const &er)
      : Base_t(Base_t::s_create_op_ignore_args_if_not(!er, op.move_me())),
        optional_error(er.copy_me()) {}
  template <typename... Ts>
  static inline constexpr mjz_stack_obj_warper_with_error_template_class_t
  s_er_create(OPTIONAL_error_type &&er, Ts &&...args) {
    return {
        Base_t::s_create_op_ignore_args_if_not(!er, std::forward<Ts>(args)...),
        er.move_me()};
  }
  template <typename... Ts>
  static inline constexpr mjz_stack_obj_warper_with_error_template_class_t
  s_er_create(const OPTIONAL_error_type &er, Ts &&...args) {
    return {
        Base_t::s_create_op_ignore_args_if_not(!er, std::forward<Ts>(args)...),
        er.copy_me()};
  }
  template <typename... Ts>

  static inline constexpr mjz_stack_obj_warper_with_error_template_class_t
  s_create(Ts &&...args) {
    return {{std::forward<Ts>(args)...}, {}};
  }

  inline constexpr OPTIONAL_error_type &get_error() & { return optional_error; }

  inline constexpr OPTIONAL_error_type &&get_error() && {
    return optional_error.move_me();
  }

  inline constexpr const OPTIONAL_error_type &get_error() const & {
    return optional_error;
  }

  inline constexpr const OPTIONAL_error_type &&get_error() const && {
    return optional_error.move_me();
  }

  inline constexpr bool has_error() const { return !!optional_error; }
  inline constexpr mjz_stack_obj_warper_with_error_template_class_t() {}
  inline constexpr ~mjz_stack_obj_warper_with_error_template_class_t() {}

 public:
  OPTIONAL_error_type optional_error;
};

template <typename my_iner_Type_, typename error_t,
          bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = false>
using mjz_stack_obj_warper_with_error_template_t =
    mjz_stack_obj_warper_with_error_template_class_t<
        mjz_stack_obj_warper_template_class_t<
            my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
            do_error_check, use_object_in_union>,
        OU_mjz_stack_obj_warper_template_t<
            static_str_algo::remove_reference_t<error_t>>>;

template <typename my_iner_Type_, typename error_t,
          bool construct_obj_on_constructor = true,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = true>
using U_mjz_stack_obj_warper_with_error_template_t =
    mjz_stack_obj_warper_with_error_template_class_t<
        mjz_stack_obj_warper_template_class_t<
            my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
            do_error_check, use_object_in_union>,
        OU_mjz_stack_obj_warper_template_t<
            static_str_algo::remove_reference_t<error_t>>>;

template <typename my_iner_Type_, typename error_t,
          bool construct_obj_on_constructor = false,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = true>
using OU_mjz_stack_obj_warper_with_error_template_t =
    mjz_stack_obj_warper_with_error_template_class_t<
        mjz_stack_obj_warper_template_class_t<
            my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
            do_error_check, use_object_in_union>,
        OU_mjz_stack_obj_warper_template_t<
            static_str_algo::remove_reference_t<error_t>>>;

template <typename my_iner_Type_, typename error_t,
          bool construct_obj_on_constructor = false,
          class my_obj_creator_t = mjz_temp_type_obj_algorithims_warpper_t<
              static_str_algo::remove_reference_t<my_iner_Type_>>,
          bool do_error_check = 1, bool use_object_in_union = false>
using O_mjz_stack_obj_warper_with_error_template_t =
    mjz_stack_obj_warper_with_error_template_class_t<
        mjz_stack_obj_warper_template_class_t<
            my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t,
            do_error_check, use_object_in_union>,
        OU_mjz_stack_obj_warper_template_t<
            static_str_algo::remove_reference_t<error_t>>>;

template <size_t my_index, typename T, typename Type0, typename... Types>
struct mjz_get_type_index_helper_class_t {
  constexpr static const size_t Index =
      mjz_get_type_index_helper_class_t<my_index + 1, T, Types...>::Index;
};

template <size_t my_index, typename T, typename... Types>
struct mjz_get_type_index_helper_class_t<my_index, T, T, Types...> {
  constexpr static const size_t Index = my_index;
};
template <size_t my_index, typename T>
struct mjz_get_type_index_helper_class_t<my_index, T, T> {
  constexpr static const size_t Index = my_index;
};
template <size_t my_index, typename T, typename U>
struct mjz_get_type_index_helper_class_t<my_index, T, U> {
  constexpr static const size_t Index = -1;
};

template <typename T, typename... Types>
constexpr static const size_t mjz_get_type_index_v =
    mjz_get_type_index_helper_class_t<0, T, Types...>::Index;

template <size_t my_index, template <typename> class is_valid, typename Type0,
          typename... Types>
struct mjz_get_first_object_with_f_true_index_helper_class_t {
  constexpr static const size_t Index =
      (is_valid<Type0>::value)
          ? (my_index)
          : (mjz_get_first_object_with_f_true_index_helper_class_t<
                my_index + 1, is_valid, Types...>::Index);
};

template <size_t my_index, template <typename> class is_valid, typename Type>
struct mjz_get_first_object_with_f_true_index_helper_class_t<my_index, is_valid,
                                                             Type> {
  constexpr static const size_t Index =
      (is_valid<Type>::value) ? (my_index) : (size_t(-1));
};
template <template <typename> class is_valid, typename... Types>
constexpr static const size_t mjz_get_first_valid_obj_index_v =
    mjz_get_first_object_with_f_true_index_helper_class_t<0, is_valid,
                                                          Types...>::Index;

template <typename T, size_t m_size,
          class obj_cnstructor_t = mjz_temp_type_obj_algorithims_warpper_t<T>>
class safe_array_template_t {
  using value_type = T;
  using Type = T;

  struct DATA_base : private obj_cnstructor_t {
    union DATA_base_UN {
      char f;
      T m_data[m_size];
      template <typename... args_t,
                typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
      constexpr inline DATA_base_UN(args_t &&...args)
          : m_data{std::forward<args_t>(args)...} {}
      constexpr inline DATA_base_UN() {}
      constexpr inline ~DATA_base_UN() {}
    };
    constexpr inline obj_cnstructor_t &creator() { return *this; }
    constexpr inline obj_cnstructor_t &creator() const { return *this; }
    constexpr inline DATA_base() {
      creator().construct_array_at(ptr(), m_size);
    }

    template <typename... args_t,
              typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
    constexpr inline DATA_base(args_t &&...args)
        : db(std::forward<args_t>(args)...) {}
    constexpr inline ~DATA_base() {
      creator().destruct_array_at(ptr(), m_size);
    }

    DATA_base_UN db;
    constexpr inline T *ptr() { return db.m_data; }
    constexpr inline const T *const ptr() const { return db.m_data; }
  };

 public:
  template <typename... args_t,
            typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
  constexpr inline safe_array_template_t(args_t &&...args)
      : db(std::forward<args_t>(args)...) {}

  constexpr inline safe_array_template_t() {}

  constexpr inline ~safe_array_template_t() {}

  using ref_t = optional_pointer_refrence_template_t<Type &, true>;
  using cref_t = optional_pointer_refrence_template_t<const Type &, true>;

  template <size_t I>
      constexpr inline std::enable_if_t < I<m_size, Type &> at() noexcept {
    return ptr()[I];
  }
  template <size_t I>
      constexpr inline std::enable_if_t <
      I<m_size, const Type &> at() const noexcept {
    return ptr()[I];
  }

  constexpr inline ref_t at(size_t i) {
    if (i > m_size) return NULL_type;
    return ptr() + i;
  }
  constexpr inline cref_t at(size_t i) const {
    if (i > m_size) return NULL_type;
    return ptr() + i;
  }

  constexpr inline ref_t operator[](size_t i) { return at(i); }
  constexpr inline cref_t operator[](size_t i) const { return at(i); }
  constexpr inline obj_cnstructor_t &creator() { return db.creator(); }
  constexpr inline obj_cnstructor_t &creator() const { return db.creator(); }
  using array_type = typename Type (&)[m_size];
  using iterator = iterator_template_t<Type, true>;
  using const_iterator = iterator_template_t<const Type, true>;
  using R_iterator = std::reverse_iterator<iterator>;
  using CR_iterator = std::reverse_iterator<const_iterator>;
  constexpr inline const array_type &toa() const { return db.db.m_data; }
  constexpr inline array_type &toa() { return db.db.m_data; }
  constexpr inline iterator begin() { return {ptr(), ptr(), ptr_end()}; }
  constexpr inline const_iterator begin() const {
    return {ptr(), ptr(), ptr_end()};
  }
  constexpr inline const_iterator cbegin() const { return begin(); }
  constexpr inline iterator end() { return {ptr_end(), ptr(), ptr_end()}; }
  constexpr inline const_iterator end() const {
    return {ptr_end(), ptr(), ptr_end()};
  }
  constexpr inline const_iterator cend() const { return end(); }
  constexpr inline CR_iterator crend() const { return cbegin(); }
  constexpr inline R_iterator rend() { return begin(); }
  constexpr inline CR_iterator rend() const { return begin(); }
  constexpr inline CR_iterator crbegin() const { return cend(); }
  constexpr inline R_iterator rbegin() { return end(); }
  constexpr inline CR_iterator rbegin() const { return end(); }
  constexpr inline Type *data() { return ptr(); }
  constexpr inline const Type *data() const { return ptr(); }
  static constexpr inline size_t size() { return m_size; }

  constexpr inline void assign(const Type &obj) {
    for (Type *p = ptr(), *const e = ptr_end(); p < e; p++) {
      creator().obj_copy_to_obj(*p, obj);
    }
  }
  constexpr inline void fill(const Type &obj) { return assign(obj); }
  Type &back() { return *(ptr_end() - 1); }
  const Type &back() const { return *(ptr_end() - 1); }

  Type &front() { return *(ptr()); }
  const Type &front() const { return *(ptr()); }
  bool constexpr inline static empty() noexcept { return false; }

 private:
  static constexpr Type *const NULL_type = nullptr;
  DATA_base db;
  constexpr inline T *ptr() { return db.ptr(); }
  constexpr inline const T *const ptr() const { return db.ptr(); }
  constexpr inline T *ptr_end() { return db.ptr() + m_size; }
  constexpr inline const T *const ptr_end() const { return db.ptr() + m_size; }
};
template <typename T, class O>
class safe_array_template_t<T, 0, O> : not_a_type_if<true> {};
template <size_t n, class O>
class safe_array_template_t<void, n, O> : not_a_type_if<true> {};
template <typename T, size_t n>
class safe_array_template_t<T, n, void> : not_a_type_if<true> {};

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
  constexpr inline bool empty() const noexcept { return !m_Size; }
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
  constexpr inline [[nodiscard]] reference operator[](size_type _Pos) {
    if constexpr (error_check) {
      if (_Pos >= m_Size) {
        Throw<std::out_of_range>("extended_mjz_Array subscript out of range");
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] const_reference operator[](
      size_type _Pos) const noexcept {
    if constexpr (error_check) {
      {
        if (_Pos >= m_Size)
          Throw<std::out_of_range>("extended_mjz_Array subscript out of range");
      }
    }
    return *m_elements[_Pos];
  }
  constexpr inline [[nodiscard]] reference front() noexcept {
    return *m_elements[0];
  }
  constexpr inline [[nodiscard]] const_reference front() const noexcept {
    return *m_elements[0];
  }
  constexpr inline [[nodiscard]] reference back() noexcept {
    return *m_elements[m_Size - 1];
  }
  constexpr [[nodiscard]] inline const_reference back() const noexcept {
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
    Throw<std::out_of_range>("invalid extended_mjz_Array<T, N> subscript");
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
  struct unique_make_er : private mjz_allocator_warpper<container_Type> {
    constexpr inline mjz_allocator_warpper<container_Type> &get_obj_creator() {
      return *this;
    }
  };
  /*
  this class is  unsafe if handled incorrectly
  and may dereference null  but if you want to pass  a  heap allocated
  stack_obj_warper by  reference it is a good option .
  it deallocates the object automatically.

  */
  class my_unique_ptr_of_container_Type : private unique_make_er {
   private:
    mutable container_Type
        *m_obj{};  // sorry for <mutable> but it is what it is
    constexpr inline container_Type *get_then_null() {
      container_Type *t = m_obj;
      m_obj = 0;
      return t;
    }
    my_unique_ptr_of_container_Type &&move_me() { return std::move(*this); }
    my_unique_ptr_of_container_Type &me() { return *this; }

   public:
    constexpr inline my_unique_ptr_of_container_Type(
        my_unique_ptr_of_container_Type &&otr) {
      m_obj = otr.get_then_null();
    }

    constexpr inline my_unique_ptr_of_container_Type(
        const my_unique_ptr_of_container_Type &&otr) {
      m_obj = otr.get_then_null();
    }

    constexpr inline my_unique_ptr_of_container_Type &operator=(
        my_unique_ptr_of_container_Type &&otr) & {
      if (&otr == this) return *this;
      m_obj = otr.get_then_null();
      return *this;
    }

    constexpr inline my_unique_ptr_of_container_Type &operator=(
        const my_unique_ptr_of_container_Type &&otr) & {
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

    constexpr inline ~my_unique_ptr_of_container_Type() { ~*this; }

    constexpr inline my_unique_ptr_of_container_Type &&operator~() && {
      to_null();
      return move_me();
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
    constexpr inline my_unique_ptr_of_container_Type &operator~() & {
      to_null();
      return me();
    }
    constexpr inline void to_null() {
      if (m_obj) {
        this->get_obj_creator().destroy_at(m_obj);
        this->get_obj_creator().deallocate(m_obj, 1);
        m_obj = 0;
      }
    }
    constexpr inline my_unique_ptr_of_container_Type(
        container_Type *valid_heap_object) {
      m_obj = valid_heap_object;
    }
    template <typename... args_t>
    constexpr static inline [[nodiscard]] my_unique_ptr_of_container_Type
    make_unique(args_t &&...args) {
      my_unique_ptr_of_container_Type ret;
      ret.m_obj = (ret.get_obj_creator().allocate(1));
      if (!ret.m_obj) return ret;
      ret.get_obj_creator().construct_at(ret.m_obj,
                                         std::forward<args_t>(args)...);
      return ret;
    }
    constexpr inline my_unique_ptr_of_container_Type() : m_obj(0) {}

    friend class mjz_heap_obj_warper_template_t<
        my_iner_Type_, construct_obj_on_constructor, my_obj_creator_t>;
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
    if (!m_ptr) Throw<const char *>(" NO DATA GIVEN ");
  }

  constexpr inline container_Type_ptr get_ptr_then_iam_invalid() const && {
    return std::move(m_ptr);
  }
  constexpr inline container_Type_ptr uptr() const && {
    return std::move(m_ptr);
  }
  mjz_heap_obj_warper_template_t(const container_Type_ptr &&my_ptr)
      : m_ptr(std::move(my_ptr)) {
    if (!m_ptr) Throw<const char *>(" NO DATA GIVEN ");
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

  // Ultra nunsafe
  constexpr inline Type &uuo() &noexcept { return base().uuo(); }
  constexpr inline Type *uuop() &noexcept { return base().uuuo(); }
  constexpr inline const Type &uuo() const &noexcept { return base().uuo(); }
  constexpr inline const Type *uuop() const &noexcept { return base().uuop(); }
  constexpr inline Type &&uuo() &&noexcept { return base_mv().uuo(); }
  constexpr inline const Type &&uuo() const &&noexcept {
    return base_mv().uuo();
  }
  constexpr inline const Type *uuop() const && = delete;
  constexpr inline Type *uuop() && = delete;  // no safe use case
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
  constexpr inline const Type *operator->() const & {
    return pointer_to_data();
  }
  constexpr inline mjz_heap_obj_warper_template_t &&move_me() {
    return std::move(*this);
  }
  constexpr inline const mjz_heap_obj_warper_template_t &&move_me() const {
    return std::move(*this);
  }
  constexpr inline const Type &operator*() const & { return *operator->(); }
  constexpr inline const Type &operator()() const & { return **this; }
  constexpr inline Type &&operator()() && { return std::move(*move_me()); }
  constexpr inline const Type &&operator()() const && {
    return std::move(*move_me());
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
    return move_me();
  }
  constexpr inline my_Type_t &&operator+() && {
    base_mv().operator+();
    return move_me();
  }
  constexpr inline my_Type_t &&operator-() && {
    base_mv().operator~();
    return move_me();
  }
  constexpr inline mjz_heap_obj_warper_template_t &operator--() & {
    de_init();
    return *this;
  }
  constexpr inline mjz_heap_obj_warper_template_t &&operator--() && {
    de_init();
    return move_me();
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
    return move_me();
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
  constexpr inline static size_t my_size() { return sizeof_Type; }
  constexpr inline static size_t size_T() { return my_size(); }
  constexpr inline Type &operator()(Type &&moved) & {
    return base()(std::move(moved));
  }
  constexpr inline Type &operator()(Type &moved) & { return base()(moved); }
  constexpr inline Type &operator()(const Type &moved) & {
    return base()(moved);
  }
  inline Type &operator()(Type &&moved) const && = delete;
  inline Type &operator()(Type &moved) const && = delete;
  inline Type &operator()(const Type &moved) const && = delete;
  inline Type &operator()(Type &&moved) && = delete;
  inline Type &operator()(Type &moved) && = delete;
  inline Type &operator()(const Type &moved) && = delete;
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  constexpr inline void operator[](std::function<void(Type &)> f) & {
    if (has_data()) f(O());
  }
  constexpr inline void operator[](std::function<void(Type &&)> f) && {
    if (has_data()) f(std::move(move_me().O()));
  }
  constexpr inline void operator[](
      std::function<void(const Type &&)> f) const && {
    if (has_data()) f(std::move(move_me().O()));
  }
  constexpr inline void operator[](
      std::function<void(const Type &)> f) const & {
    if (has_data()) f(O());
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
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class function_type, typename... args_t>
  constexpr inline void i_do_nt_know(bool do_throw, function_type f,
                                     args_t &&...args) const && {
    if (do_throw) {
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return;
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
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
      has_data_function(std::move(move_me().O()),
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
      has_data_function(std::move(move_me().O()),
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
  constexpr inline operator Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() const && {
    return std::move(*move_me());
  }
  constexpr inline operator Type() & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() const & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr inline operator Type() const && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr explicit operator bool() const noexcept { return has_data(); }
  constexpr bool has_value() const noexcept { return has_data(); }
  template <class... Args>
  constexpr inline T &emplace(Args &&...args) & {
    ~*this;
    construct(std::forward<Args>(args)...);
    return o();
  }
  template <class... Args>
  constexpr inline T &&emplace(Args &&...args) && {
    ~*this;
    construct(std::forward<Args>(args)...);
    return move_me().o();
  }

  template <class U, class... Args>
  constexpr inline T &emplace(std::initializer_list<U> ilist,
                              Args &&...args) & {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return o();
  }
  template <class U, class... Args>
  constexpr inline T &&emplace(std::initializer_list<U> ilist,
                               Args &&...args) && {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return move_me().o();
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
    return has_data() ? move_me() : std::forward<F>(f)();
  }
  constexpr Type &&value() && { return *move_me(); }
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
    return has_data() ? std::move(*move_me())
                      : static_cast<T>(std::forward<U>(default_value));
  }

  template <class... Ts>
  constexpr const Type &value_or_emplace(Ts &&...args) const & {
    if (!!*this) return o();
    emplace(std::forward<Ts>(args)...);
    return o();
  }
  template <class... Ts>
  constexpr Type &value_or_emplace(Ts &&...args) & {
    if (!!*this) return o();
    emplace(std::forward<Ts>(args)...);
    return o();
  }
  template <class... Ts>
  constexpr Type &&value_or_emplace(Ts &&...args) && {
    if (!!*this) return std::move(o());
    emplace(std::forward<Ts>(args)...);
    return std::move(o());
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
          std::move(move_me().base())))>(
          std::forward<F>(f)(std::move(move_me().base())));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(move_me().base())))>(
          std::forward<F>(f)(std::move(move_me().base())));
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
          std::move(move_me().base())))>(
          std::forward<F>(f)(std::move(move_me().base())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    if (has_data()) {
      return mjz_heap_obj_warper_template_t<decltype(std::forward<F>(f)(
          std::move(move_me().base())))>(
          std::forward<F>(f)(std::move(move_me().base())));
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
    ptr += l;
  }

 protected:
  constexpr inline char *to_c_string(char *const buf_) const {
    char *buf = buf_;
    paste_str("const  char ", buf);
    paste_str(" hash ", buf);
    paste_str(" [] = {  ", buf);

    buf += static_str_algo::itoa((int64_t)hashed_data[0], {buf, 5}, 10).len;

    for (int64_t i = 1; i < NumberOf(hashed_data); i++) {
      paste_str(",", buf);

      buf += static_str_algo::itoa((int64_t)hashed_data[i], {buf, 5}, 10).len;
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

template <uint64_t V>
constexpr uint64_t make_binary() {
  return V;
}
template <uint64_t V, char C, char... Cs>
constexpr uint64_t make_binary() {
  static_assert(C == '0' || C == '1', "invalid binary");
  return make_binary<2 * V + C - '0', Cs...>();
}
template <char... Cs>
uint64_t operator"" _b() {
  static_assert(sizeof...(Cs) <= 64, "overflow");
  return make_binary<0, Cs...>();
}
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
  Type x, y;
  inline ~Vector2(){};
  constexpr inline Vector2 &operator=(const Vector2 &v) {
    x = (v.x);
    y = (v.y);
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
    x = (x);
    y = (y);
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
  constexpr inline Vector2() : x{}, y{} {}
  constexpr inline Vector2(const Type &s) : x(s), y(s) {}
  constexpr inline Vector2(const Type &x, const Type &y) : x(x), y(y) {}
  constexpr inline Vector2(const Vector2<Type> &v) : x(v.x), y(v.y) {}
  constexpr inline Vector2(Vector2 &v) : Vector2((const Vector2<Type> &)v){};
  constexpr inline Vector2(Vector2 &&v) : Vector2(v){};
  constexpr inline bool operator==(const Vector2<Type> &v) const {
    return abs(x - v.x) < 0.01 && abs(y - v.y) < 0.01;
  }
  constexpr inline bool operator!=(const Vector2<Type> &v) const {
    return !(operator==(v));
  }
  /**********************************************
  Indexing operator
   **********************************************/
  constexpr inline Type &operator[](int i) { return *(&x + i); }
  constexpr inline const Type operator[](int i) const { return *(&x + i); }
  /*********************************************
  Non modifying math operators
   *********************************************/
  constexpr inline Vector2<Type> operator-() const {
    return Vector2<Type>(-x, -y);
  }
  constexpr inline Vector2<Type> operator+(const Vector2<Type> &v) const {
    return Vector2<Type>(x + v.x, y + v.y);
  }
  constexpr inline Vector2<Type> operator-(const Vector2<Type> &v) const {
    return Vector2<Type>(x - v.x, y - v.y);
  }
  constexpr inline Vector2<Type> operator*(const Type &s) const {
    return Vector2<Type>(x * s, y * s);
  }
  constexpr inline Vector2<Type> operator*(const Vector2<Type> &v) const {
    return Vector2<Type>(x * v.x, y * v.y);
  }
  constexpr inline Vector2<Type> operator/(const Type &s) const {
    return Vector2<Type>(x / s, y / s);
  }
  /*******************************************
  Modifying Math Operators
   *******************************************/
  constexpr inline Vector2<Type> &operator+=(const Vector2<Type> &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator-=(const Vector2<Type> &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator*=(const Type &s) {
    x *= s;
    y *= s;
    return *this;
  }
  constexpr inline Vector2<Type> &operator*=(const Vector2<Type> &v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  constexpr inline Vector2<Type> &operator/=(const Type &s) {
    x /= s;
    y /= s;
    return *this;
  }
  /*******************************************
  Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  constexpr inline operator const Type *() const {
    return static_cast<Type *>(&x);
  }
  constexpr inline operator Type *() { return static_cast<Type *>(&x); }
  /********************************************
  Useful Vector Operations
   ********************************************/
  constexpr inline Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(x * x + y * y);
  }
  constexpr inline Type lengthSq() const { return x * x + y * y; }
  constexpr inline Vector2<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Vector2<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Type dot(const Vector2<Type> &v) const {
    return x * v.x + y * v.y;
  }
  constexpr inline Type cross(
      const Vector2<Type> &v) const {  // 3-D cross product with z assumed 0
    return x * v.y + v.x * y;  // return magnitude of resulting mjz_vector
  }
  friend std::ostream &operator<<(std::ostream &outs, const Vector2<Type> &v) {
    outs << "<" << v.x << ", " << v.y << ">";
    return outs;
  }
  friend std::istream &operator>>(std::istream &ins, Vector2<Type> &v) {
    ins >> v.x >> v.y;
    return ins;
  }
  friend constexpr inline Vector2<Type> operator*(Type s,
                                                  const Vector2<Type> &v) {
    return Vector2<Type>(s * v.x, s * v.y);
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
  constexpr inline Type *begin() { return &x; }
  constexpr inline Type *end() { return begin() + size(); }
  constexpr inline const Type *begin() const { return &x; }
  constexpr inline const Type *end() const { return begin() + size(); }
  constexpr inline const Type *cbegin() const { return begin(); }
  constexpr inline const Type *cend() const { return end(); }
  constexpr inline static const size_t size() { return 2; }
  constexpr inline bool operator!() const { return !x && !y; }
  constexpr inline operator bool() const { return !!*this; }
};
/*********************************************************************************
Vector3 -- 3D mjz_vector
*********************************************************************************/

template <class Type>
class Vector3 {
 public:
  Type x, y, z;
  constexpr inline ~Vector3(){};
  constexpr inline Vector3 &operator()(Vector3 &obj) { return *this = obj; };
  constexpr inline Vector3 &operator()(Vector3 &&obj) { return *this = obj; };
  constexpr inline Vector3 &operator()(const Vector3 &obj) {
    return *this = obj;
  };
  constexpr inline Vector3 &operator()(const Type &x, const Type &y,
                                       const Type &z) {
    x = (x);
    x = (y);
    x = (z);
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
  constexpr inline Vector3() : x{}, y{}, z{} {}
  constexpr inline Vector3(const Type &s) : x(s), y(s), z(s) {}
  constexpr inline Vector3(const Type &x, const Type &y, const Type &z)
      : x(x), y(y), z(z) {}
  constexpr inline Vector3(const Vector2<Type> &v, const Type &s = Type())
      : x(v.x), y(v.y), z(s) {}
  constexpr inline Vector3(const Vector3<Type> &v) : x(v.x), y(v.y), z(v.z) {}
  constexpr inline Vector3(Vector3 &v) : Vector3((const Vector3<Type> &)v){};
  constexpr inline Vector3(Vector3 &&v) : Vector3(v){};
  constexpr inline Vector3 &operator=(const Vector3 &v) {
    x = (v.x);
    y = (v.y);
    z = (v.z);
    return *this;
  }
  constexpr inline Vector3 &operator=(Vector3 &v) {
    return operator=((const Vector3<Type> &)v);
  }
  constexpr inline Vector3 &operator=(Vector3 &&v) { return operator=(v); }
  constexpr inline bool operator==(const Vector3<Type> &v) const {
    return abs(x - v.x) < 0.01 && abs(y - v.y) < 0.01 && abs(z - v.z) < 0.01;
  }
  constexpr inline bool operator!=(const Vector3<Type> &v) const {
    return !operator==(v);
  }
  /**********************************************
  Indexing operator
   **********************************************/
  constexpr inline Type &operator[](int i) { return *(&x + i); }
  const Type operator[](int i) const { return *(&x + i); }
  /**********************************************
  Itrator
   **********************************************/
  constexpr inline Type *begin() { return &x; }
  constexpr inline Type *end() { return begin() + size(); }
  constexpr inline const Type *begin() const { return &x; }
  constexpr inline const Type *end() const { return begin() + size(); }
  constexpr inline const Type *cbegin() const { return begin(); }
  constexpr inline const Type *cend() const { return end(); }
  constexpr inline static const size_t size() { return 3; }
  /*********************************************
  Non modifying math operators
   *********************************************/
  constexpr inline Vector3<Type> operator-() const {
    return Vector3<Type>(-x, -y, -z);
  }
  constexpr inline Vector3<Type> operator+(const Vector3<Type> &v) const {
    return Vector3<Type>(x + v.x, y + v.y, z + v.z);
  }
  constexpr inline Vector3<Type> operator-(const Vector3<Type> &v) const {
    return Vector3<Type>(x - v.x, y - v.y, z - v.z);
  }
  constexpr inline Vector3<Type> operator*(const Type &s) const {
    return Vector3<Type>(x * s, y * s, z * s);
  }
  constexpr inline Vector3<Type> operator*(const Vector3<Type> &v) const {
    return Vector3<Type>(x * v.x, y * v.y, z * v.z);
  }
  constexpr inline Vector3<Type> operator/(const Type &s) const {
    return Vector3<Type>(x / s, y / s, z / s);
  }
  constexpr inline Vector3<Type> operator/(const Vector3<Type> &v) const {
    return Vector3<Type>(x / v.x, y / v.y, z / v.z);
  }
  /*******************************************
  Modifying Math Operators
   *******************************************/
  constexpr inline Vector3<Type> &operator+=(const Vector3<Type> &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator-=(const Vector3<Type> &v) {
    x -= v.x;
    y -= v.y;
    y -= v.z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator*=(const Type &s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  constexpr inline Vector3<Type> &operator*=(const Vector3<Type> &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator/=(const Vector3<Type> &v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }
  constexpr inline Vector3<Type> &operator/=(const Type &s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }
  /*******************************************
  Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  constexpr inline operator const Type *() const {
    return static_cast<Type *>(&x);
  }
  constexpr inline bool operator!() const { return !x && !y && !z; }
  constexpr inline operator bool() const { return !!*this; }
  constexpr inline operator Type *() { return static_cast<Type *>(&x); }
  /********************************************
  Useful Vector Operations
   ********************************************/
  constexpr inline Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(lengthSq());
  }
  constexpr inline Type lengthSq() const { return x * x + y * y + z * z; }
  constexpr inline Vector3<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Vector3<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  constexpr inline Type dot(const Vector3<Type> &v) const {
    return x * v.x + y * v.y + z * v.z;
  }
  constexpr inline Vector3<Type> cross_with_this(
      const Vector3<Type> &v) { /* NOTE this function modifies the
mjz_vector unlike 2D and non-member versions */
    Type x_(x), y_(y), z_(z);
    x = y_ * v.z - z_ * v.y;
    y = z_ * v.x - x_ * v.z;
    z = x_ * v.y - y_ * v.x;
    return *this;
  }
  friend std::ostream &operator<<(std::ostream &outs, const Vector3<Type> &v) {
    outs << "<" << v.x << ", " << v.y << ", " << v.z << ">";
    return outs;
  }
  friend std::istream &operator>>(std::istream &ins, Vector3<Type> &v) {
    ins >> v.x >> v.y >> v.z;
    return ins;
  }
  constexpr inline friend Vector3<Type> operator*(Type s,
                                                  const Vector3<Type> &v) {
    return Vector3<Type>(s * v.x, s * v.y, s * v.z);
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
    return Vector3<Type>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                         v1.x * v2.y - v1.y * v2.x);
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

template <uint8_t min_of_stack_string>
class minimal_mjz_string_data : protected static_str_algo {
  static_assert(sizeof(void *) == sizeof(uintptr_t),
                " for this class this is not good");
  typedef uintptr_t Size_t;
  static_assert(requires(Size_t s, size_t s2) {
                  { s } -> std::convertible_to<size_t>;
                  { s2 } -> std::convertible_to<Size_t>;
                });
  static_assert(min_of_stack_string < 255);
  struct dynamic_DB_t {
    alignas(Size_t) volatile Size_t
        dummy_no_init_with_internal_array_length_inside;
    alignas(Size_t) char *buffer{};
    alignas(Size_t) Size_t length{};
    alignas(Size_t) Size_t capacity{};
  };

 private:
  constexpr static const size_t min_size_of_minimal_mjz_string_data =
      sizeof(dynamic_DB_t) - 2 * sizeof(char);
  constexpr const static size_t delta_size_ =
      (min_of_stack_string - min_size_of_minimal_mjz_string_data);
  constexpr static const size_t size_of_minimal_mjz_string_data =
      min_of_stack_string < min_size_of_minimal_mjz_string_data
          ? min_size_of_minimal_mjz_string_data
          : (min_size_of_minimal_mjz_string_data +
             ((delta_size_ / sizeof(Size_t)) +
              !!(delta_size_ % sizeof(Size_t))) *
                 sizeof(Size_t));

  static_assert(size_of_minimal_mjz_string_data < 255);
  constexpr static const uint8_t static_storage_cap =
      size_of_minimal_mjz_string_data;
  constexpr static const uint8_t is_external_array_v{255};

  struct static_DB_t {
    uint8_t internal_array_length{};  // alings with dynamic_DB_t::dummy in
                                      // below union
    char internal_array[static_storage_cap + 1]{};
    void reset() {
      mjz_obj_manager_template_t<static_DB_t>::obj_destructor_on(this);
      mjz_obj_manager_template_t<static_DB_t>::obj_constructor_on(this);
    }
  };
  struct DB_t {
    union {
      dynamic_DB_t db_d;
      static_DB_t db_s{};
      static_assert(sizeof(dynamic_DB_t) <= sizeof(static_DB_t));
    };
    DB_t() {}

    ~DB_t() {}
    DB_t(DB_t &&other) : db_s(other.db_s) {
      // deafult copy
      other.db_s.reset();
    }
    DB_t &operator=(DB_t &&other) {
      db_s = other.db_s;
      other.db_s.reset();
    }
    DB_t &operator=(DB_t const &) = delete;

    DB_t(DB_t const &) = delete;
  };
  // no need for "__attribute__((packed));"
  // i have done it maually  :)
  constexpr static const uint8_t real_static_size = sizeof(static_DB_t);
  constexpr static const uint8_t real_size = sizeof(DB_t);
  static_assert(real_static_size == real_size);
  DB_t m_db;

  static_assert(sizeof(DB_t) == real_size,
                "what happened  to you that you got a padding?!");
  static_assert(!(sizeof(DB_t) % sizeof(Size_t)),
                "what happened  to you that you got a padding?!");

 public:
  minimal_mjz_string_data(const char *s, size_t n) {
    if (!s || !can_have_len(n)) return;
    memcpy(get_str(), s, n);
    add_len(n);
  }
  minimal_mjz_string_data() {}
  inline bool is_dynamic() const {
    return m_db.db_s.internal_array_length == is_external_array_v;
  }
  inline dynamic_DB_t str_set_dynamic(char *buffer, size_t cap) {
    if (!buffer || !cap) return {};
    if (is_dynamic()) {
      dynamic_DB_t per = m_db.db_d;
      if (cap < per.length) return {};
      if (!per.buffer || !per.length || !per.capacity || per.buffer == buffer)
        return per;
      memmove(buffer, per.buffer, per.length);
      m_db.db_d.buffer = buffer;
      m_db.db_d.capacity = cap;
      return per;
    }
    if (m_db.db_s.internal_array_length)
      memmove(buffer, m_db.db_s.internal_array,
              m_db.db_s.internal_array_length);
    dynamic_DB_t ret;
    ret.buffer = 0;
    ret.capacity = static_storage_cap;
    ret.length = m_db.db_s.internal_array_length;
    m_db.db_s.internal_array_length = is_external_array_v;
    m_db.db_d.buffer = buffer;
    m_db.db_d.capacity = cap;
    m_db.db_d.length = ret.length;
    return ret;
  }
  inline size_t get_cap() const {
    if (is_dynamic()) return m_db.db_d.capacity;
    return static_storage_cap;
  }
  inline const char *get_str() const {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline char *get_str() {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline size_t get_len() const {
    if (is_dynamic()) return m_db.db_d.length;
    return m_db.db_s.internal_array_length;
  }
  inline bool set_len(size_t length) {
    if (!can_have_len(length)) return false;
    if (is_dynamic()) {
      m_db.db_d.length = length;
      return true;
    }
    m_db.db_s.internal_array_length = length;
    return true;
  }
  inline bool add_len(size_t add_length) {
    return set_len(get_len() + add_length);
  }
  inline bool can_have_len(size_t length) const { return length <= get_cap(); }
  inline bool can_add_len(size_t add_length) const {
    return can_have_len(get_len() + add_length);
  }
  inline minimal_mjz_string_data &move_to_me(minimal_mjz_string_data &&d) {
    m_db = std::move(d.m_db);
    return *this;
  }

  char *get_buffer() { return get_str(); }
  const char *get_buffer() const { return get_str(); }
  size_t get_length() const { return get_len(); }
};

template <typename T>
concept no_decay_c_str =
    requires {
      requires(std::is_same_v<T, const char *> || std::is_same_v<T, char *>);
    };
template <typename T>
concept mjz_Str_data_strorage =
    requires(T obj, const T cobj, char *a, const char *ca, size_t n) {
      obj.get_buffer();
      cobj.get_buffer();
      obj.get_length();
      cobj.get_length();
      T(ca, n);
      T(a, n);
    };
struct deafult_mjz_Str_data_strorage {
  const char *str_view{};
  size_t len_view{};
  inline constexpr const char *get_buffer() const { return str_view; }
  inline constexpr size_t get_length() const { return len_view; }
  inline constexpr deafult_mjz_Str_data_strorage(const char *s, size_t n)
      : str_view(s), len_view(n) {}
};

template <mjz_Str_data_strorage Base_t>
class basic_mjz_Str_view : protected Base_t, protected static_str_algo {
 protected:
  using Base_t::get_buffer;
  using Base_t::get_length;  // the ptr length (not counting the '\0')

 public:
  constexpr static size_t npos = -1;
  constexpr inline const char *begining_of_str_ptr() const {
    return get_buffer();
  }
  constexpr inline const char *ending_of_str_ptr() const {
    return get_buffer() + get_length();
  }

 public:
  constexpr basic_mjz_Str_view(const char *const buffer, size_t length)
      : Base_t(buffer, length) {}
  template <size_t N>
  constexpr basic_mjz_Str_view(const char (&const a)[N])
      : basic_mjz_Str_view(a, N - (!a[N - 1])) {}
  constexpr basic_mjz_Str_view(const just_str_view_data &s)
      : basic_mjz_Str_view(s.buffer, s.len) {}
  constexpr explicit inline basic_mjz_Str_view(no_decay_c_str auto c_string)
      : basic_mjz_Str_view(c_string, strlen(c_string)) {}
  constexpr basic_mjz_Str_view()
      : basic_mjz_Str_view(static_str_algo::empty_STRING_C_STR, 0) {}
  constexpr inline ~basic_mjz_Str_view() = default;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view &operator=(const basic_mjz_Str_view &) = default;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view(const basic_mjz_Str_view &) = default;

 public:
  constexpr bool is_blank() const {
    size_t i{};
    while (i < get_length()) {
      if (!is_blank_characteres_default(get_buffer()[i])) {
        return 0;
      }
      i++;
    }
    return 1;
  }
  constexpr bool empty() const {
    return (!get_length() ||
            get_buffer() == static_str_algo::empty_STRING_C_STR ||
            get_buffer() == nullptr);
  }
  static constexpr const bool is_const_view =
      requires(Base_t o) {
        { o.get_buffer() } -> std::convertible_to<char *>;
      };
  constexpr inline size_t length(void) const { return get_length(); }
  constexpr inline char *data()
    requires(is_const_view)
  {
    return get_buffer();
  }
  constexpr inline const char *data() const { return get_buffer(); }

 public:
  constexpr inline const char *c_str() const & { return get_buffer(); }
  constexpr inline const char *c_str() & { return get_buffer(); }
  constexpr inline char *C_str() & { return get_buffer(); }

 public:
  constexpr explicit operator const bool() const { return !is_blank(); }
  constexpr char operator[](int64_t index_) const {
    size_t index = signed_index_to_unsigned(index_);
    if ((size_t)index >= get_length() || !get_buffer()) {
      return 0;
    }
    return get_buffer()[index];
  }
  constexpr char operator[](size_t index) const {
    if (index >= get_length() || !get_buffer()) {
      return 0;
    }
    return get_buffer()[index];
  }
  constexpr bool operator!() const { return is_blank(); }
  // parsing/conversion
  constexpr long long toLL(void) const {
    if (get_buffer()) {
      return to_LL();
    }
    return 0;
  }
  constexpr long long to_LL(int radix = 10, bool *had_error = 0,
                            uint8_t error_level = 0) const {
    if (get_buffer()) {
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
    if (get_buffer()) {
      char *ptr{};
      return strtod(get_buffer(), &ptr);
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
    return compare_two_str(get_buffer(), get_length(), s.get_buffer(),
                           s.get_length());
  }
  constexpr int64_t compareTo(const char *cstr) const {
    return compare_two_str(get_buffer(), get_length(), cstr, strlen(cstr));
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
    return compare_two_str(get_buffer(), get_length(), s, n);
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
    return lastIndexOf(s2, get_length() - s2.get_length());
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
    return static_str_algo::find_first_of_in_str(get_buffer(), get_length(), s,
                                                 n);
  }
  constexpr inline size_t find_last_of_in_str(const char *s, size_t n) const {
    return static_str_algo::find_last_of_in_str(get_buffer(), get_length(), s,
                                                n);
  }
  constexpr inline size_t find_first_of_in_str(
      const basic_mjz_Str_view &str) const {
    return find_first_of_in_str(str.get_buffer(), str.get_length());
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
    return static_str_algo::find_last_of_not_in_str(get_buffer(), get_length(),
                                                    s, n);
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
    return static_str_algo::find_first_of_not_in_str(get_buffer(), get_length(),
                                                     s, n);
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
    return are_two_str_equale(get_buffer(), get_length(), cstr, cstr_len);
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
    return UN_ORDERED_compare(s.get_buffer(), s.get_length());
  }
  constexpr size_t UN_ORDERED_compare(const char *s, size_t s_len) const {
    const unsigned char *ucs_ = (const unsigned char *)s;
    const unsigned char *ucbuffer_ = (const unsigned char *)this->get_buffer();
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
    if (get_length() != s2.get_length()) {
      //
      return false;
    }
    if (get_length() == 0) {
      //
      return true;
    }
    const char *p1 = get_buffer();
    const char *p2 = s2.get_buffer();
    const char *end_p1 = get_buffer() + length();
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
    if (get_length() < s2.get_length()) {
      //
      return false;
    }
    //
    return startsWith(s2, 0);
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2, size_t offset) const {
    if (offset > get_length() - s2.get_length() || !get_buffer() ||
        !s2.get_buffer()) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&get_buffer()[offset], s2.get_buffer(),
                       s2.get_length()) == 0;
  }
  constexpr bool endsWith(const basic_mjz_Str_view &s2) const {
    if (get_length() < s2.get_length() || !get_buffer() || !s2.get_buffer()) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&get_buffer()[get_length() - s2.get_length()],
                       s2.get_buffer(), s2.get_length()) == 0;
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
    if (index >= get_length()) {
      buf[0] = 0;
      //
      return;
    }
    size_t n = bufsize - 1;
    if (n > get_length() - index) {
      n = get_length() - index;
    }
    memmove((char *)buf, get_buffer() + index, min_macro_(n, get_length()));
    buf[n] = 0;
    //
  }
  constexpr inline void toCharArray(char *buf, size_t bufsize,
                                    size_t index = 0) const {
    getBytes((unsigned char *)buf, bufsize, index);
  }
  constexpr int64_t indexOf(char c) const { return indexOf(c, 0); }
  constexpr int64_t indexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *temp =
        strchr(get_buffer() + fromIndex, length() + fromIndex, ch);
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - get_buffer());
  }
  constexpr int64_t indexOf_cstr(const char *c_str_, size_t len_str,
                                 size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *found =
        strstr(get_buffer() + fromIndex, length() - fromIndex, c_str_, len_str);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - get_buffer());
  }
  constexpr int64_t lastIndexOf_cstr(const char *cstr__, size_t length__,
                                     size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *found = r_strstr(get_buffer() + fromIndex, length() - fromIndex,
                                 cstr__, length__);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - get_buffer());
  }
  constexpr inline int64_t lastIndexOf_cstr(const char *cstr__,
                                            size_t length__) const {
    return lastIndexOf_cstr(cstr__, length__, 0);
  }
  constexpr int64_t lastIndexOf(char theChar) const {
    return lastIndexOf(theChar, 0);
  }
  constexpr int64_t lastIndexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *temp = strrchr(get_buffer() + fromIndex, length(), ch);
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - get_buffer());
  };
  constexpr const char *substring_give_ptrULL(size_t left, size_t right,
                                              const char *&c_str_out,
                                              size_t &len_out) const {
    if (left > right) {
      size_t temp = right;
      right = left;
      left = temp;
    }
    if (left >= get_length()) {
      //
      len_out = 0;
      return (c_str_out = 0);
    }
    if (right > get_length()) {
      right = get_length();
    }
    c_str_out = get_buffer() + left;
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
  constexpr const char at(int64_t i) { return operator[](i); }
  constexpr const char back() { return get_buffer()[0]; }
  constexpr const char front() { return operator[]((int64_t)-1); }
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
};

//
// mjz_vector.h
// Vector
//
// Created by Emilis Baliukonis on 05/05/2019.
// Copyright   2019 Emilis Baliukonis. All rights reserved.
//
template <class T>
struct functions_return_value_helper_class {
  using type = OU_mjz_stack_obj_warper_template_t<T>;
};
template <class T>
struct functions_return_value_helper_class<const T> {
  using type = OU_mjz_stack_obj_warper_template_t<T>;
};
template <class T>
using functions_return_value_t =
    typename functions_return_value_helper_class<T>::type;
template <class T>
struct mjz_ref_return_helper_class {
  using type = optional_pointer_refrence_template_t<
      functions_return_value_t<
          std::remove_reference_t<std::remove_volatile_t<T>>> &,
      false>;
};
template <class T>
struct mjz_ref_return_helper_class<const T> {
  using type = optional_pointer_refrence_template_t<
      const functions_return_value_t<
          std::remove_reference_t<std::remove_volatile_t<T>>> &,
      false>;
};
template <class T>
using mjz_ref_return_helper_class_t =
    typename mjz_ref_return_helper_class<T>::type;

namespace have_mjz_ard_removed {
template <class T>
using mjz_RAII_t = mjz_resource_acquisition_is_initialization_template<T>;
template <class T>
using RAII_t = mjz_RAII_t<T>;

using malloc_wrpr = malloc_wrapper;
using algorithm = static_str_algo;
template <typename T, size_t size>
using ex_Array = extended_mjz_Array<T, size>;
template <typename T, size_t size>
using ex_array = extended_mjz_Array<T, size>;
template <typename T, size_t size>
using Array = mjz_Array<T, size>;
template <size_t N>
using mjz_initilizer_t = initilizer_in_constructor_helper_class_t<N>;
template <size_t N>
using initilizer_t = initilizer_in_constructor_helper_class_t<N>;
template <typename T>
using Vector = mjz_Vector<T>;
template <typename T, size_t size>
using array = mjz_Array<T, size>;
using initilizer_in_constructor_helper_class =
    initilizer_in_constructor_helper_class_t<0>;
template <size_t N>
using initilizer_in_constructor_helper_class_t =
    initilizer_in_constructor_helper_class_t<N>;
template <typename Type, bool destroy_on_destruction = true,
          bool create_on_construction = true>
using simple_unsafe_init_obj_wrpr_t =
    mjz_simple_unsafe_init_obj_wrpr_t<Type, destroy_on_destruction,
                                      create_on_construction>;
template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
using no_init_uw_special_arg_t =
    mjz_no_init_uw_special_arg<init_if_iam_not_zero_the_base_with_me_minus_one>;
using no_init_uw_special_arg = no_init_uw_special_arg_t<0>;
template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
using no_init_optional_t = typename mjz_no_init_optional_t<
    init_if_iam_not_zero_the_base_with_me_minus_one>::me;
using no_init_optional = no_init_optional_t<0>;
using init_optional = typename mjz_init_optional_t::me;
using init_optional_if = mjz_init_optional_if_t;

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
template <typename T, bool is_forward>
using filter_iterator =
    iterator_template_filter_warper<T, iterator_template_t<T>, is_forward>;
template <typename T, bool is_forward>
using filter_iterator_template =
    iterator_template_filter_warper<T, iterator_template_t<T>, is_forward>;
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
using optional = OU_mjz_stack_obj_warper_template_t<T>;
template <typename T>
using mjz_optional = OU_mjz_stack_obj_warper_template_t<T>;

template <typename T, typename E>
using optional_err = OU_mjz_stack_obj_warper_with_error_template_t<T, E>;
template <typename T, typename E>
using mjz_optional_err = OU_mjz_stack_obj_warper_with_error_template_t<T, E>;
template <typename T>
using functions_return_value = functions_return_value_t<T>;
template <typename T>
using mjz_functions_return_value = functions_return_value<T>;
template <typename T>
using mjz_function_return_value = functions_return_value<T>;
template <typename T>
using function_return_value = functions_return_value<T>;
template <typename T>
using function_return = functions_return_value<T>;
template <typename T>
using func_return = functions_return_value<T>;
template <typename T>
using func_ret = functions_return_value<T>;
template <typename T>
using mjz_ref_return = mjz_ref_return_helper_class_t<T>;
template <typename T>
using ref_return = mjz_ref_return<T>;
template <typename T>
using mjz_optional_return = mjz_ref_return<T>;
template <typename T>
using optional_return = mjz_optional_return<T>;
template <typename T>
using mjz_nullable_return = mjz_optional_return<T>;
template <typename T>
using nullable_return = mjz_optional_return<T>;

/*
NOTE: if you return a callee_ret :
1. if you didnt store the  caller_ret you will use a non valid object that is
not in your stack frame (aka using a temporary variable outside the lifetime by
a pointer to r value)

2.if the caller_ret object is alive  you may use the returned callee_ret

3. if you want to use it but not have it as an lvalue in caller functions stack
frame you may use the callee_ret::operator()(std::function<(callee_ret)>) on the
function like this : foo(some args...)([](callee_ret ret)->void{some code....});



*/
template <typename T>
using mjz_caler_ret = func_ret<T>;
template <typename T>
using mjz_calee_ret = ref_return<T>;
template <typename T>
using caler_ret = func_ret<T>;
template <typename T>
using calee_ret = ref_return<T>;

template <typename T>
using mjz_caller_ret = func_ret<T>;
template <typename T>
using mjz_callee_ret = ref_return<T>;
template <typename T>
using caller_ret = func_ret<T>;
template <typename T>
using callee_ret = ref_return<T>;
#ifndef _MJZ_NO_CALEE_CALER_HELPER_MACRO_

template <class T>
using get_caler_return_type_t = typename T::Type;
template <class T>
using get_calee_return_type_t = typename T::Type::Type;

#define CR_NO_RETURN(RET) ((RET).copy_me())
// NOTE: this function only works if calee_ret<T> is checked in the called
// function Undefined Behaviour otherwise
#define CR_CALL_IF(_CONDITION, _RET)                                \
  ((_CONDITION)                                                     \
       ? (::mjz_ard::have_mjz_ard_removed::calee_ret<               \
             typename decltype(_RET)::                              \
                 my_totaly_uniuqe_type_name_of_content_type>(_RET)) \
       : (::mjz_ard::have_mjz_ard_removed::calee_ret<               \
             typename decltype(_RET)::                              \
                 my_totaly_uniuqe_type_name_of_content_type>(nullptr)))

// NOTE: this function only works if calee_ret<T> is checked in the called
// function Undefined Behaviour otherwise
#define CR_NR_CALL_IF(_CONDITION, _RET)                       \
  ((_CONDITION)                                               \
       ? (::mjz_ard::have_mjz_ard_removed::calee_ret<         \
             typename decltype(_RET)::                        \
                 my_totaly_uniuqe_type_name_of_content_type>( \
             (_RET).copy_me()))                               \
       : (::mjz_ard::have_mjz_ard_removed::calee_ret<         \
             typename decltype(_RET)::                        \
                 my_totaly_uniuqe_type_name_of_content_type>(nullptr)))

// use  these macros in functions that are like calee_ret(calee_ret)
#define CE_RETURN_IF0(RET) \
  do {                     \
    if (!RET) return {};   \
  } while (0)
#define CE_RETURN_EMPLACE(RET, RET_val) \
  do {                                  \
    auto &RET_ = (RET);                 \
    if (!RET_) return {};               \
    RET_->emplace(RET_val);             \
    if (!*(RET_)) return {};            \
    return RET_;                        \
  } while (0)
#define CE_RETURN_WITH(RET)  \
  do {                       \
    auto &RET_ = (RET);      \
    if (!RET_) return {};    \
    if (!*(RET_)) return {}; \
    return RET_;             \
  } while (0)

#define CE_NE_RETURN_IF0(RET) \
  do {                        \
    try {                     \
      CE_RETURN_IF0(RET);     \
    } catch (...) {           \
      return {};              \
    }                         \
  } while (0)
#define CE_NE_RETURN_EMPLACE(RET, RET_val) \
  do {                                     \
    try {                                  \
      CE_RETURN_EMPLACE(RET, RET_val);     \
    } catch (...) {                        \
      return {};                           \
    }                                      \
                                           \
  } while (0)
#define CE_NE_RETURN_WITH(RET) \
  do {                         \
    try {                      \
      CE_RETURN_WITH(RET);     \
    } catch (...) {            \
      return {};               \
    }                          \
  } while (0)

#endif  // ! _MJZ_NO_CALEE_CALER_HELPER_MACRO_

template <typename T>
using mjz_optional = OU_mjz_stack_obj_warper_template_t<T>;

template <typename T, size_t n>
using safe_array =
    safe_array_template_t<T, n, mjz_temp_type_obj_algorithims_warpper_t<T>>;

template <typename T_ref>
using mjz_optional_ref = optional_pointer_refrence_template_t<T_ref, false>;
template <typename T_ref>
using optional_ref = mjz_optional_ref<T_ref>;
template <typename T_ref>
using mjz_var_ref = optional_pointer_refrence_template_t<
    T_ref, false,
    optional_pointer_refrence_has_to_be_valid_on_construction_level::
        not_null_if_is_ref>;
template <typename T_ref>
using var_ref = mjz_var_ref<T_ref>;
template <typename T_ptr>
using mjz_optional_ptr = optional_pointer_refrence_template_t<
    std::enable_if_t<
        std::is_same_v<std::remove_pointer_t<std::remove_cvref_t<T_ptr>> *,
                       std::remove_cvref_t<T_ptr>>,
        std::remove_pointer_t<T_ptr> &>,
    true>;
template <typename T_ptr>
using optional_ptr = mjz_optional_ptr<T_ptr>;

template <typename T>
using mjz_optional_refrence_of_type = mjz_optional_ptr<T *>;
template <typename T>
using mjz_optional_ref_of_type = mjz_optional_refrence_of_type<T>;
template <typename T>
using mjz_optional_ref_of_t = mjz_optional_refrence_of_type<T>;
template <typename T>
using mjz_op_ref_of_t = mjz_optional_refrence_of_type<T>;

template <typename T>
using optional_refrence_of_type = mjz_optional_refrence_of_type<T>;
template <typename T>
using optional_ref_of_type = mjz_optional_refrence_of_type<T>;
template <typename T>
using optional_ref_of_t = mjz_optional_refrence_of_type<T>;
template <typename T>
using op_ref_of_t = mjz_optional_refrence_of_type<T>;

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
using mjz_Vectorf3 = Vector3<float>;
using mjz_Vectorf2 = Vector2<float>;

typedef bool succsess_t;

typedef bool failure_t;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard
namespace std {

template <typename Type>
struct hash<mjz_ard::Vector3<Type>> {
  size_t operator()(const mjz_ard::Vector3<Type> &k) const {
    return hash<std::string_view>()(
        std::string_view((const char *)&k, sizeof(mjz_ard::Vector3<Type>)));
  }
};
template <typename Type>
struct hash<mjz_ard::Vector2<Type>> {
  size_t operator()(const mjz_ard::Vector2<Type> &k) const {
    return hash<std::string_view>()(
        std::string_view((const char *)&k, sizeof(mjz_ard::Vector2<Type>)));
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

#ifndef _NOT_USING_MJZ_ARD_
namespace mjz_ard_types = mjz_ard::have_mjz_ard_removed;
namespace mjz = mjz_ard;
namespace mjzt = mjz_ard_types;
#define USE_MJZ_NS()                             \
  using namespace mjz_ard;                       \
  using namespace mjz_ard::have_mjz_ard_removed; \
  using namespace mjz_ard::iostream;             \
  using namespace mjz_ard::util

//  put semicolon

#endif  // ! _NOT_USING_MJZ_ARD_

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
  template <bool Y = 0>
  Scoped_speed_Timer &operator()(const std::string &name,
                                 const bool non_matter = Y) {
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

struct defult_UUID_geter_class {
  using UUID_t = const void *const;
  const static constexpr bool is_visible = false;
  template <typename T>
  inline UUID_t operator()(const T *p) const {
    return p;
  }
};
template <class counter_class>
struct mjz_class_operation_reporter_count_t {
  static counter_class index;
};
template <class counter_class>
counter_class mjz_class_operation_reporter_count_t<counter_class>::index{};
template <class counter_class, class filler_type = char,
          class my_stream = std_stream_class_warper,
          class UUID_geter_class = defult_UUID_geter_class>
class mjz_class_operation_reporter_t
    : public mjz_class_operation_reporter_count_t<counter_class>,
      protected UUID_geter_class {
  using mjz_class_operation_reporter_count_t<counter_class>::index;
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

 private:
  void constructor(int i) {
    print_c_str(" created with int :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(float i) {
    print_c_str(" created with float :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(double i) {
    print_c_str(" created with double :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(size_t i) {
    print_c_str(" created with size_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(int64_t i) {
    print_c_str(" created with int64_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(void *i) {
    print_c_str(" created with size_t :(");
    print(i);
    println_wi(index++, ")  ");
  }
  void constructor(char c) {
    print_c_str(" created with char :(");
    print(c);
    println_wi(index++, ")  ");
  }
  template <
      typename c_t,
      typename = std::enable_if_t<std::disjunction_v<
          std::is_same<c_t, const char *>, std::is_same<c_t, const uint8_t *>>>>
  void constructor(c_t s) {
    print_c_str(" created with str : \"");
    println_wi(index++, s, "\"   ");
  }

  void constructor(std::initializer_list<const char *> args) {
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
  void constructor(std::initializer_list<std::string_view> args) {
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
  template <size_t N>
  void constructor(const char (&a)[N]) {
    print_c_str(" created with const str , len  : \"");
    print_c_str_len_1(a, N);
    println_wi(index++, "\"   ");
  }
  template <size_t N>
  void constructor(char (&a)[N]) {
    print_c_str(" created with str , len  : \"");
    print_c_str_len_1(a, N);
    println_wi(index++, "\"   ");
  }

  void constructor(mjz_class_operation_reporter_t &&obj) {
    index++;
    println_wf(obj, " move constructed ");
  }
  void constructor(const mjz_class_operation_reporter_t &&obj) {
    index++;
    println_wf(obj, " const move constructed ");
  }
  void constructor(const std::nullptr_t &) {
    println_wi(index++, " created : ");
  }
  void constructor(const mjz_class_operation_reporter_t &obj) {
    index++;
    println_wf(obj, " const  copy constructed ");
  }
  void constructor(mjz_class_operation_reporter_t &obj) {
    index++;
    println_wf(obj, " copy constructed ");
  }

 public:
  mutable filler_type filler = '|';
  using UUID_t = typename UUID_geter_class::UUID_t;
  inline UUID_t UUID() const {
    return (*((const UUID_geter_class *)this))((const void *)this);
  };
  inline UUID_t UUID() { return (*((UUID_geter_class *)this))((void *)this); };
  template <int non_matter = 0, typename = std::enable_if_t<UUID_t::is_visible>>
  inline UUID_geter_class &base() & {
    return *this;
  }
  template <int non_matter = 0, typename = std::enable_if_t<UUID_t::is_visible>>
  inline UUID_geter_class &&base() && {
    return std::move(*this);
  }
  template <int non_matter = 0, typename = std::enable_if_t<UUID_t::is_visible>>
  inline const UUID_geter_class &&base() const & {
    return *this;
  }
  template <int non_matter = 0, typename = std::enable_if_t<UUID_t::is_visible>>
  inline const UUID_geter_class &&base() const && {
    return std::move(*this);
  }
  mjz_class_operation_reporter_t() { println_wi(index++, " created : "); }
  mjz_class_operation_reporter_t(const mjz_class_operation_reporter_t &o) {
    constructor(o);
  }
  mjz_class_operation_reporter_t(mjz_class_operation_reporter_t &&o) {
    constructor(std::move(o));
  }
  mjz_class_operation_reporter_t(mjz_class_operation_reporter_t &o) {
    constructor(o);
  }
  mjz_class_operation_reporter_t(const mjz_class_operation_reporter_t &&o) {
    constructor(std::move(o));
  }
  template <typename T0, typename... Ts>
  inline mjz_class_operation_reporter_t(T0 &&arg0, Ts &&...args)
      : UUID_geter_class(std::forward<Ts>(args)...) {
    constructor(std::forward<T0>(arg0));
    ;
  }

  ~mjz_class_operation_reporter_t() {
    println_wi(--index, " destroyed : ");
    filler = '_';
  }
  mjz_class_operation_reporter_t &operator=(
      mjz_class_operation_reporter_t &&obj) {
    println_wf(obj, " moved to me  ");
    return none();
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
    return (int64_t)((char *)this - (char *)this);
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
  inline const mjz_class_operation_reporter_t *This() const & { return this; }
  inline mjz_class_operation_reporter_t *This() & { return this; }

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
        remove_const(non_const_opr);  // i promise  that ther will be no
                                      // changes in
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

using operation_reporter = mjz_class_operation_reporter_t<size_t, char>;

using operation_reporter_32 = mjz_class_operation_reporter_t<size_t, char32_t>;

template <class T>
struct mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t {
  mutable T obj{};
  template <typename... Ts>
  inline constexpr mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t(
      Ts &&...args)
      : obj(std::forward<Ts>(args)...) {}
  inline constexpr mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t() {
  }
  inline constexpr ~mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t() {
  }
  using UUID_t = T &;

  const static constexpr bool is_visible = true;
  template <typename U>
  UUID_t operator()(U *) const {
    return obj;
  }
};
template <class T>
struct mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t<
    const T> {
  const T obj{};
  template <typename... Ts>
  inline constexpr mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t(
      Ts &&...args)
      : obj(std::forward<Ts>(args)...) {}
  inline constexpr mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t() {
  }
  mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t(
      const mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t
          &other)
      : obj(other.obj) {}
  mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t(
      mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t
          &&other)
      : obj(std::move(other.obj)) {}
  inline constexpr ~mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t() {
  }
  using UUID_t = const T &;

  const static constexpr bool is_visible = true;
  template <typename U>
  UUID_t operator()(U *) const {
    return obj;
  }
};
template <typename generate_at_first_use = void>
struct named_operation_reporter_class_t
    : public mjz_class_operation_reporter_t<
          size_t, char, std_stream_class_warper,
          mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t<
              std::string>> {
  using BASE = mjz_class_operation_reporter_t<
      size_t, char, std_stream_class_warper,
      mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t<
          std::string>>;
  named_operation_reporter_class_t() : BASE(std::nullptr_t(), ((void *)this)) {}
  template <
      typename c_t,
      typename = std::enable_if_t<std::disjunction_v<
          std::is_same<c_t, const char *>, std::is_same<c_t, const uint8_t *>>>>
  explicit named_operation_reporter_class_t(c_t cstr)
      : BASE(std::nullptr_t(), cstr) {}
  template <size_t N>
  named_operation_reporter_class_t(const char (&a)[N])
      : BASE(std::nullptr_t(), (decltype(a))a) {}
  template <size_t N>
  named_operation_reporter_class_t(char (&a)[N])
      : BASE(std::nullptr_t(), {a, N}) {}
  template <typename T>
  named_operation_reporter_class_t(T &&arg)
      : BASE(std::nullptr_t(), std::forward<T>(arg)) {}
  template <typename T>
  named_operation_reporter_class_t &operator=(T &&arg) {
    BASE::operator=(std::forward<T>(arg));
    return *this;
  }
  ~named_operation_reporter_class_t() {}
};
using named_operation_reporter = named_operation_reporter_class_t<void>;
template <class T>
using operation_reporter_ID_with = mjz_class_operation_reporter_t<
    size_t, char, std_stream_class_warper,
    mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t<T>>;
template <class T>
using operation_reporter_32_ID_with = mjz_class_operation_reporter_t<
    size_t, char32_t, std_stream_class_warper,
    mjz_class_operation_reporter_with_T_with_uuid_defult_UUID_geter_class_t<T>>;
#ifndef __MJZ_operation_reporter_ON_LINE_DEACTIVATE_
#define MJZ_LINE_AS_STR__(X) (#X)
#define MJZ_LINE_AS_STR_(X) (MJZ_LINE_AS_STR__(X))

#define MJZ_LINE_AS_STR__NP(X) #X
#define MJZ_LINE_AS_STR_NP(X) MJZ_LINE_AS_STR__NP(X)
#define GET_FLINE_C_STR() (MJZ_LINE_AS_STR_(__FILE__ : __LINE__))
#define CREATE_ONFLINE_operation_reporter(NAME) \
  auto NAME = named_operation_reporter(GET_FLINE_C_STR())
// for  example this may be ("(""line" "7"")") and that will be "(line7)"
#define GET_LINE_C_STR() \
  ("(" MJZ_LINE_AS_STR_NP(line) MJZ_LINE_AS_STR_NP(__LINE__) ")")
#define CREATE_ONLINE_operation_reporter(NAME) \
  auto NAME = named_operation_reporter(GET_LINE_C_STR())
#endif  // !__MJZ_operation_reporter_ON_LINE_DEACTIVATE_

namespace have_mjz_ard_removed {
using operation_reporter = mjz_class_operation_reporter_t<size_t>;
typedef vr_Scoped_speed_Timer vr_Scoped_speed_Timer;
typedef speed_Timer speed_Timer;
typedef Scoped_speed_Timer Scoped_speed_Timer;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard

namespace mjz_ard {
namespace util {
constexpr inline bool f_EQ(double a, double b, double accuracy = 0.001) {
  return ::mjz_ard::static_str_algo::is_approximately_equal(a, b, accuracy);
}

}  // namespace util
namespace iostream {
template <typename T>
const T &scan(T &obj) {
  std::cin >> obj;
  return obj;
}
template <typename T>
const T &scanln(T &obj) {
  getline(std::cin, obj);
  return obj;
}

inline void scan() { std::cin.get(); }
inline void ignore() { std::cin.ignore(); }
inline void scanln() {
  char buf[1001]{};
  std::cin.getline(buf, 1000, '\n');
}

template <typename... argT>
void scan(argT &...args) {
  (void)std::initializer_list<char>{(scan(args), 0)...};
}

template <typename... argT>
void scanln(argT &...args) {
  (void)std::initializer_list<char>{
      (scan(args), 0)...};  // do all tasks in thr rigth order
  scanln();
}
template <typename... argT>
void scanln_FE(argT &...args) {
  (void)std::initializer_list<char>{
      (scanln(args), 0)...};  // do all tasks in thr rigth order
}

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
  (void)std::initializer_list<char>{
      ((std::cout << std::forward<argT>(args)), '0')...};
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
}  // namespace iostream
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

#endif  // !_MJZ_ARD_LIB_FILE_