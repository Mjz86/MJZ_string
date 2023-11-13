/*
  String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#pragma once

#define NumberOf(arg) ((size_t)(sizeof(arg) / sizeof(arg[0])))
#ifdef __cplusplus
#ifndef __mjz_ard_STRINGS__
#define __mjz_ard_STRINGS__
#include "pch.h"
// #include "mjzString_helper.hpp"
#ifdef Arduino
#define std__string_view_if_is String
#else
#define std__string_view_if_is std::string_view
#include "framework.h"
#endif
// #define  virtual
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
#define log_mjz_str(Function, ...) (Function((const char *)__VA_ARGS__))
inline uint32_t usteejtgk_millis() { return millis(); }
class __FlashStringHelper;

namespace mjz_ard {
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
void Set_nth_bit_andret32(void *data, uint64_t nthbt, bool set_to);
bool Get_nth_bit_andret32(const void *data, uint64_t nthbt);
void tgl_nth_bit_andret32(void *data, uint64_t nthbt);
void Set_nth_bit_andret8(void *data, uint64_t nthbt, bool set_to);
void tgl_nth_bit_andret8(void *data, uint64_t nthbt);
bool Get_nth_bit_andret8(const void *data, uint64_t nthbt);

char *ultoa(uint32_t value, char *buffer, int radix);
char *ulltoa(uint64_t value, char *buffer, int radix);
char *b_U_lltoa(uint64_t value, char *BFR_buffer, int radix, bool is_signed,
                bool force_neg = 0, bool is_upper_case = 1);
uint8_t get_num_from_char(uint8_t in_CHAR_, bool *to_neg = 0);
enum uint8_t_error_level : uint8_t {
  on = 0,
  stack_ovf_prtt_off = MJZ_logic_BIT(0),
  LL_ovf_prtt_off = MJZ_logic_BIT(1),
  len_ovf_prtt_off = MJZ_logic_BIT(2)
};
struct memcmp_data {
  int64_t first_delta_index{-1};
  int first_delta{};
  inline constexpr memcmp_data(int64_t i, int in)
      : first_delta_index(i), first_delta(in) {}
  inline constexpr memcmp_data() : memcmp_data(-1, 0) {}
};
struct MJZ_memcmp_data : public memcmp_data {
  size_t num_delta{};
  inline constexpr MJZ_memcmp_data(int64_t i, int in, size_t n)
      : memcmp_data(i, in), num_delta(n) {}
  inline constexpr MJZ_memcmp_data() : memcmp_data(-1, 0), num_delta(0) {}
};
template <typename cmpr_type>
MJZ_memcmp_data MJZ_memcmp(const void *ptr_1_, const void *ptr_2_,
                           size_t size_of_data_in_byte) {
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
          MJZ_logic_BL_bit_to_64_bits(is_different && !(first_delta_index_in_8_t)) &
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
MJZ_memcmp_data MJZ_strcmp(const void *ptr_1, const void *ptr_2) {
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
char GET_CHAR_from_int(uint8_t intager_in, bool is_upper_case);
int MJZ_STRCMP(const char *p1, const char *p2);
int MJZ_STRnCMP(const char *p1, const char *p2, size_t lenght);
int64_t C_STR_to_LL(const char *buffer, uint8_t buffer_len, int radix,
                    bool *had_error = 0, uint8_t error_level = 0);
// When compiling programs with this class,the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency,typically
// with little or no increase in code size.
// -felide-constructors
// -std=c++0x
// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper
// *>(PSTR(string_literal)))
// An inherited class for holding the result of a concatenation. These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;
class mjz_Str;
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
/*
  in a bad case
  class A {
  virtual void foo() = 0;
  };
  class B {
  virtual void foo() = 0;
  };
  class C : public A, public B {
  virtual void foo();
  };
  void C::foo(){ }
  void C::A::foo(){ }
  void C::B::foo(){ }
*/

class static_str_algo {
 public:
  static constexpr int64_t the_reinterpreted_char_cca_size = 17;
  static constexpr int64_t forbiden_chars_cnt_size = 3;
  static size_t constexpr FLT_MAX_DECIMAL_PLACES = 10;
  static size_t constexpr DBL_MAX_DECIMAL_PLACES = FLT_MAX_DECIMAL_PLACES;
  static constexpr const char *empty_STRING_C_STR = "";

 public:
  template <class Type>
  static inline constexpr Type BL_min(Type a, Type b) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(b < a);
    return (BL & b) + ((~BL) & a);
  }

  template <class Type>
  static inline constexpr Type BL_max(Type a, Type b) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(a < b);
    return (BL & b) + ((~BL) & a);
  }
  template <class Type>
  static inline constexpr Type BL_abs(Type x) {
    uint64_t BL = MJZ_logic_BL_bit_to_64_bits(0 < x);
    return (BL & x) + ((~BL) & (-x));
  }
 

 public:
  static constexpr int MJZ_STRnCMP(const char *p1, const char *p2,
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

  constexpr static int memcmp(const void *str1, const void *str2,
                              size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;

    while ((0 < count--) && *s1++ != *s2++)
      ;
    if (*--s1 != *--s2) return *s1 < *s2 ? -1 : 1;
    return 0;
  }
  constexpr static memcmp_data memcmp_d(const void *str1, const void *str2,
                                        size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s1beg = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;

    while ((0 < count--) && *s1++ != *s2++)
      ;
    if (*--s1 != *--s2) return {s1 - s1beg, *s1 < *s2 ? -1 : 1};

    return {};
  }

  constexpr static int memcmp(const void *str1, size_t count1, const void *str2,
                              size_t count2) {
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
  constexpr static memcmp_data memcmp_d(const void *str1, size_t count1,
                                        const void *str2, size_t count2) {
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
  constexpr static void *memset(void *_Dst, int _Val, size_t _Size) {
    if (!(_Dst && _Size)) return _Dst;
    uint8_t *Dst{static_cast<uint8_t *>(_Dst)};
    uint8_t *Dst_end{Dst + _Size};
    while (Dst < Dst_end) {
      *Dst++ = static_cast<uint8_t>(_Val);
    }
    return _Dst;
  }
  constexpr static void *memmove(void *dest, const void *src, size_t len) {
    char *d = (char *)dest;
    const char *s = (const char *)src;

    if (d < s)
      while (len--) {
        *d++ = *s++;
      }
    else {
      const char *lasts = s + (len - 1);
      char *lastd = d + (len - 1);

      while (len--) {
        *lastd-- = *lasts--;
      }
    }

    return dest;
  }
  constexpr static void *memcpy(void *dest, const void *src, size_t len) {
    return memmove(dest, src, len);
  }
  constexpr static inline size_t strlen(const char *str) {
    if (!(str && *str)) {
      return 0;
    }

    const char *str_i = str;

    while (1) {
      if (!*++str_i) {
        return str_i - str;
      }
    }
  }
  constexpr static void *strncpy(void *dest, const char *src, size_t len) {
    return memcpy(dest, src, min(strlen(src) + 1, len));
  }
  constexpr static void *strcpy(void *dest, const char *src) {
    return memcpy(dest, src, strlen(src) + 1);
  }
  constexpr static inline const char *strchr(const char *str, size_t len_,
                                             char ch) {
    const char *haystack_end = str + len_;

    for (const char *i{str}; i < haystack_end; ++i) {
      if (*(i) == ch) {
        return i;
      }
    }

    return NULL;
  }
  constexpr static inline const char *strrchr(const char *str, size_t len_,
                                              char ch) {
    const char *haystack_end = str + len_;
    const char *i{haystack_end - 1};

    for (;;) {
      if (*(i) == ch) {
        return i;
      }

      if (i <= str) {
        break;
      }

      --i;
    }

    return NULL;
  }
  constexpr static inline const char *strstr(const char *const haystack_,
                                             const size_t haystack_len,
                                             const char *const needle_,
                                             const size_t needle_len) {
    const char *haystack = haystack_;
    const char *needle = needle_;

    /* Length of NEEDLE. */
    /* Known minimum length of HAYSTACK. */
    /* Handle empty NEEDLE special case. */
    if (needle[0] == '\0') {
      return (char *)haystack;
    }

    if (haystack_len < needle_len) {
      return NULL;
    }

    if (haystack_len == 0) {
      return NULL;
    }

    /* Skip until we find the first matching char from NEEDLE. */
    const char *haystack_end = haystack + haystack_len;
    haystack = strchr(haystack, haystack_len, needle[0]);

    if (haystack_end == haystack || needle[1] == '\0' || !haystack) {
      return (char *)haystack;
    }

    /* Ensure HAYSTACK length is at least as long as NEEDLE length.
      Since a match may occur early on in a huge HAYSTACK, use strnlen
      and read ahead a few cachelines for improved performance. */
    /* Check whether we have a match. This improves performance since we avoid
      the initialization overhead of the two-way algorithm. */
    if (static_str_algo::memcmp(haystack, needle, needle_len) == 0) {
      return (char *)haystack;
    }

    /* Perform the search. Abstract memory is considered to be an array
      of 'unsigned char' values, not an array of 'char' values. See
      ISO C 99 section 6.2.6.1. */
    return NULL;
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

    return !memcmp(rhs, lhs, rhs_l);
    // return compare_two_str(rhs, rhs_l, lhs, lhs_l) == 0;
  }
  struct _char_8_a {
    union {
      bool data_b[8]{};
      char8_t data_c[8];
      uint8_t data_u[8];
    };

    inline constexpr _char_8_a() = default;
    inline constexpr bool &operator[](uint8_t i) { return data_b[i]; }
  };

  inline constexpr static _char_8_a char_get_bits(uint8_t val, bool to_char) {
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

  inline constexpr static char *get_bit_representation(
      char *buffer, size_t buffer_len, const void *data_ptr, size_t len,
      bool in_reverse = (std::endian::little == std::endian::native)) {
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
  inline constexpr static char *get_bit_representation(char *buffer,
                                                       const Type &data) {
    return get_bit_representation(buffer, sizeof(Type) * 8, &data, sizeof(Type),
                                  std::endian::little == std::endian::native);
  }

 public:
  static constexpr int64_t stack_buffer_size = sizeof(std::string);
  class stack_str_buf {
    mutable bool STR_is_in_stack{};

   public:
    char stack_buffer[stack_buffer_size + 1]{};  // string you're searching for
    stack_str_buf() : STR_is_in_stack(0) {
      stack_buffer[stack_buffer_size] = 0;
    }
    stack_str_buf(const stack_str_buf &) = default;
    stack_str_buf(stack_str_buf &&) = default;
    stack_str_buf &operator=(stack_str_buf &&) = default;
    stack_str_buf &operator=(const stack_str_buf &) = default;
    bool set(bool STR_is_in_stack_) {
      STR_is_in_stack = STR_is_in_stack_;

      if (STR_is_in_stack_) {
        goto _return___;
      }

      static_str_algo::memset(stack_buffer, 0, stack_buffer_size);
      stack_buffer[stack_buffer_size] = 0;
    _return___:
      return STR_is_in_stack;
    }
    bool get() { return STR_is_in_stack; }
    // virtual //i dont need it
    ~stack_str_buf() { STR_is_in_stack = 0; }
  };
  template <typename T2, typename T1>
  inline static constexpr T2 bit_cast(const T1 &data) {
    T2 data2{};
    data2.~T2();
    memset(&data2, 0, sizeof(T2));
    memcpy(&data2, &data, min(sizeof(T2), sizeof(T1)));
    return data2;
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
 static inline constexpr Type *do_operation(Type *buffer_, function_type function,
                                      const Type *a_arr, const Type *b_arr,
                                      size_t len) {
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
    //   long &i = *(long *)&y;                 // log base 2
    // long i = *(long *)&y;
    long i = std::bit_cast<long>(y);
    i = magic_constant - divide_by_2(i);  // some magic
                                          //  y = *(float *)&i;
    y = std::bit_cast<float>(i);

    y = y * (three_halfs - (x2 * y * y));  // 1st iteration of newtons method\

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

  constexpr static inline uint64_t round(double x) {
    uint64_t fx = floor(x);
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
        term *= x / j;
      }
      retval += term;
    }
    return retval;
  }

  template <typename Type>
  constexpr static inline Type expt(int32_t number) {
    return ((0 < number) ? expULt(number) : (Type)1 / expULt(-(number)));
  }
  template <typename Type>
  constexpr static inline Type expt(Type number) {
    return ((0 < number) ? expUt(number) : (Type)1 / expUt(-(number)));
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

  constexpr static inline double cos_rad_until_pi_over_2(double x) {
    if (abs(x - HALF_PI) < 0.001) return 0;
    constexpr double neg_1ovr_fact2 = -1.0 / factorial(2);
    constexpr double _1ovr_fact4 = 1.0 / factorial(4);
    constexpr double neg_1ovr_fact6 = -1.0 / factorial(6);
    constexpr double _1ovr_fact8 = 1.0 / factorial(8);
    constexpr double neg_1ovr_fact10 = -1.0 / factorial(10);

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

  constexpr static inline double sin_rad_until_pi_over_2(double x) {
    if (abs(x - HALF_PI) < 0.001) return 1;
    constexpr double _1ovr_fact1 = 1.0 / factorial(1);
    constexpr double neg_1ovr_fact3 = -1.0 / factorial(3);
    constexpr double _1ovr_fact5 = 1.0 / factorial(5);
    constexpr double neg_1ovr_fact7 = -1.0 / factorial(7);
    constexpr double _1ovr_fact9 = 1.0 / factorial(9);
    constexpr double neg_1ovr_fact11 = -1.0 / factorial(11);
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
  constexpr static inline auto abs(const Type &x) {
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
// The string class
bool is_blank_characteres_default(char);
char char_to_char_for_reinterpret_fnc_ptr_default(char);
bool is_forbiden_character_default(char);
std::istream &helper__op_shift_input_(const mjz_Str &rhs, std::istream &CIN,
                                      mjz_Str &get_shift_op_s);
const mjz_Str &helper__op_shift_input_(const mjz_Str &rhs, const mjz_Str &CIN,
                                       mjz_Str &get_shift_op_s);
class mjz_Str_DATA_storage_cls;
class mjz_Str_DATA_storage_cls
    : public std::enable_shared_from_this<mjz_Str_DATA_storage_cls> {
 protected:
  std::function<void(void *)> free_fnctn = [](void *ptr) -> void {
    ::free(ptr);
  };
  std::function<void *(void *, size_t)> realloc_fnctn =
      [](void *ptr, size_t size_) -> void * {
    void *ptr_ = ::realloc(ptr, size_);
    return ptr_;
  };
  ;
  std::function<bool(char)> is_blank_character = is_blank_characteres_default;
  std::function<char(char)> char_to_char_for_reinterpret_fnc_ptr =
      char_to_char_for_reinterpret_fnc_ptr_default;
  std::function<bool(char)> is_forbiden_character =
      is_forbiden_character_default;  // like bool
  // (*is_forbiden_character)(char)
  // but better
  unsigned long _timeout = 1000;  // number of milliseconds to wait for the next
  // char before aborting timed read
  int write_error = 0;
  char reinterpret_char_char = '\\';
  friend class mjz_Str;
  friend void str_helper__op_shift_input_(mjz_Str &rhs, mjz_Str &CIN);
  friend std::istream &helper__op_shift_input_(const mjz_Str &rhs,
                                               std::istream &CIN,
                                               mjz_Str &get_shift_op_s);
  friend const mjz_Str &helper__op_shift_input_(const mjz_Str &rhs,
                                                const mjz_Str &CIN,
                                                mjz_Str &get_shift_op_s);

 public:
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
  void change_free_fnctn_functions(std::function<void(void *)> free_) {
    free_fnctn = free_;
  }
  void change_realloc_fnctn_functions(
      std::function<void *(void *, size_t)> realloc_) {
    realloc_fnctn = realloc_;
  }

 private:
  // mjz_Str_DATA_storage_cls() = default;
};
enum Dealocation_state : uint8_t {
  dont_deallocate_on_free = MJZ_logic_BIT(0),
  is_moved = MJZ_logic_BIT(1)
};
template <class Type>
struct Mallocator {
  typedef Type value_type;

  Mallocator() = default;

  template <class U>
  constexpr Mallocator(const Mallocator<U> &) noexcept {}

  [[nodiscard]] Type *allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(Type))
      throw std::bad_array_new_length();

    if (auto p = static_cast<Type *>(std::malloc(n * sizeof(Type)))) {
      report(p, n);
      return p;
    }

    throw std::bad_alloc();
  }

  void deallocate(Type *p, std::size_t n) noexcept {
    report(p, n, 0);
    std::free(p);
  }

 private:
  void report(Type *p, std::size_t n, bool alloc = true) const {
    std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(Type) * n
              << " bytes at " << std::hex << std::showbase
              << reinterpret_cast<void *>(p) << std::dec << '\n';
  }
};

template <class Type, class U>
inline constexpr bool operator==(const Mallocator<Type> &,
                                 const Mallocator<U> &) {
  return true;
}

template <class Type, class U>
inline constexpr bool operator!=(const Mallocator<Type> &,
                                 const Mallocator<U> &) {
  return false;
}
// iterator_template Class
template <typename Type>
class iterator_template {
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
  using value_type = Type;
  using const_reference = const Type &;
  using size_type = size_t;

  // using iterator_concept = std::contiguous_iterator_tag;
  inline constexpr iterator_template() noexcept
      : iterator_template(nullptr, nullptr, (Type *)-1) {}
  // iterator_template(Type *iter ) noexcept: m_iterator{iter} {}

  inline constexpr iterator_template(Type *iter, Type *min_end,
                                     Type *max_end) noexcept
      : m_iterator{iter},
        m_iterator_begin_ptr{min_end},
        m_iterator_end_ptr{max_end} {}

  inline constexpr iterator_template(Type *arr, size_t len) noexcept
      : m_iterator(arr),
        m_iterator_begin_ptr(arr),
        m_iterator_end_ptr(arr + len) {}

  inline constexpr iterator_template(const Type *First_arg,
                                     const Type *Last_arg) noexcept
      : iterator_template(First_arg, First_arg, Last_arg) {}

  // inline constexpr iterator_template(std::initializer_list<Type>list)
  // noexcept
  //   : iterator_template((Type *)(list.begin()), (size_t)list.size()) {}
  // this is bad for class types  becuse  ~list => ~Type => }   code... => use
  // after free

  constexpr void throw_if_bad(Type *_iterator) const {
    if (_iterator == (Type *)-1) {
      _iterator = m_iterator;
    }

    if ((m_iterator_begin_ptr <= _iterator) &&
        (_iterator <= m_iterator_end_ptr)) {
      return;
    }

    throw std::exception(
        "bad ptr access : mjz_ard::iterator_template::throw_if_bad ");
  }
  constexpr void throw_if_bad() const { throw_if_bad((Type *)-1); }

  constexpr iterator_template(const iterator_template &p) noexcept
      : m_iterator(p.m_iterator),
        m_iterator_begin_ptr(p.m_iterator_begin_ptr),
        m_iterator_end_ptr(p.m_iterator_end_ptr) {}
  constexpr iterator_template(iterator_template &&p) noexcept
      : m_iterator(p.m_iterator),
        m_iterator_begin_ptr(p.m_iterator_begin_ptr),
        m_iterator_end_ptr(p.m_iterator_end_ptr) {}
  constexpr iterator_template &operator=(Type *iter) {
    m_iterator = iter;
    throw_if_bad();
    return *this;
  }
  constexpr iterator_template &operator=(const iterator_template &p) {
    m_iterator = (p.m_iterator);
    m_iterator_begin_ptr = p.m_iterator_begin_ptr;
    m_iterator_end_ptr = p.m_iterator_end_ptr;
    throw_if_bad();
    return *this;
  }
  constexpr iterator_template &operator=(iterator_template &&p) noexcept {
    m_iterator = (p.m_iterator);
    m_iterator_begin_ptr = p.m_iterator_begin_ptr;
    m_iterator_end_ptr = p.m_iterator_end_ptr;
    return *this;
  }
  constexpr ~iterator_template() { m_iterator = 0; }
  constexpr bool operator==(const iterator_template &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(const iterator_template &other) const noexcept {
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
  template <typename my_type, typename Type = value_type>
  inline auto operator->*(my_type my_var) {
    return operator->()->*my_var;
  }
  inline constexpr const Type *begin() const { return m_iterator_begin_ptr; }
  inline constexpr const Type *end() const { return m_iterator_end_ptr; }
  inline constexpr Type *begin() { return m_iterator_begin_ptr; }
  inline constexpr Type *end() { return m_iterator_end_ptr; }
  inline constexpr iterator_template base() {
    return iterator_template(m_iterator_begin_ptr, m_iterator_begin_ptr,
                             m_iterator_end_ptr);
  }

  constexpr size_t size() const noexcept {
    return static_cast<size_t>(m_iterator_end_ptr - m_iterator_begin_ptr);
  }
  constexpr iterator_template &operator++() noexcept {
    ++m_iterator;
    return *this;
  }
  constexpr iterator_template operator++(int) noexcept {
    iterator_template tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr iterator_template &operator--() noexcept {
    --m_iterator;
    return *this;
  }
  constexpr iterator_template operator--(int) noexcept {
    iterator_template tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr iterator_template &operator+=(
      const difference_type other) noexcept {
    m_iterator += other;
    return *this;
  }
  constexpr iterator_template &operator-=(
      const difference_type other) noexcept {
    m_iterator -= other;
    return *this;
  }
  constexpr iterator_template &operator+=(
      const iterator_template &other) noexcept {
    m_iterator += other.m_iterator;
    return *this;
  }
  constexpr iterator_template &operator-=(
      const iterator_template &other) noexcept {
    m_iterator -= other.m_iterator;
    return *this;
  }
  constexpr reference operator[](std::size_t index) const {
    throw_if_bad(m_iterator + index);
    return m_iterator[index];
  }
  constexpr bool operator<(const iterator_template &other) const noexcept {
    return m_iterator < other.m_iterator;
  }
  constexpr bool operator>(const iterator_template &other) const noexcept {
    return m_iterator > other.m_iterator;
  }
  constexpr bool operator<=(const iterator_template &other) const noexcept {
    return m_iterator <= other.m_iterator;
  }
  constexpr bool operator>=(const iterator_template &other) const noexcept {
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
  constexpr friend iterator_template operator+(
      const iterator_template &me, const difference_type other) noexcept {
    return iterator_template(me.m_iterator + other, me.m_iterator_begin_ptr,
                             me.m_iterator_end_ptr);
  }
  constexpr friend iterator_template operator-(
      const iterator_template &me, const difference_type other) noexcept {
    return iterator_template(me.m_iterator - other, me.m_iterator_begin_ptr,
                             me.m_iterator_end_ptr);
  }
  constexpr friend iterator_template operator+(
      const difference_type other, const iterator_template &me) noexcept {
    return iterator_template(
        other + me.m_iterator,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  // friend iterator_template operator-(const difference_type other, const
  // iterator_template& me) noexcept { // bad function dont use
  // return iterator_template(me.m_iterator - (pointer)other);
  // }
  constexpr friend iterator_template operator+(
      const iterator_template &other, const iterator_template &me) noexcept {
    return iterator_template(
        other.m_iterator + me,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  constexpr friend difference_type operator-(
      const iterator_template &other, const iterator_template &me) noexcept {
    return std::distance(other.m_iterator, me.m_iterator);
  }
  constexpr friend void swap(iterator_template &lhs,
                             iterator_template &rhs) noexcept {
    iterator_template lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
  constexpr friend void swap(reference lhs, reference rhs) noexcept {
    value_type lhsm_ = lhs;
    lhs = rhs;
    rhs = lhsm_;
  }
};

template <class T>
inline constexpr const T *begin(iterator_template<T> it) noexcept {
  return it.begin();
}

template <class T>
inline constexpr const T *end(iterator_template<T> it) noexcept {
  return it.end();
}
template <typename Type>
class heap_obj_warper {
 public:
  // note this static is NOT  THE SAME   for int , bool ,... becuse this is a
  // template
  static constexpr size_t size = sizeof(Type);

 protected:
  uint8_t m_data[size]{};
  bool m_Has_data{};
  constexpr inline Type *init_with_unsafe_data(bool initialized) {
    m_Has_data = initialized;
    return (Type *)(m_data);
  }

 public:
  using T = Type;
  using type = Type;
  constexpr inline heap_obj_warper() = default;
  inline ~heap_obj_warper() { data_de_init(); }
  inline heap_obj_warper &operator=(heap_obj_warper &&h_obj_w) {
    if (!!h_obj_w) operator*() = std::move(h_obj_w.operator*());
    return *this;
  }
  inline heap_obj_warper &operator=(const heap_obj_warper &h_obj_w) {
    if (!!h_obj_w) operator*() = h_obj_w.operator*();
    return *this;
  }
  inline heap_obj_warper &operator=(heap_obj_warper &h_obj_w) {
    if (!!h_obj_w) operator*() = h_obj_w.operator*();
    return *this;
  }
  inline heap_obj_warper(heap_obj_warper &&h_obj_w) {
    if (new (pointer_to_unsafe_data()) Type(std::move(h_obj_w.operator*())))
      m_Has_data = 1;
  }
  inline heap_obj_warper(const heap_obj_warper &h_obj_w) {
    if (!!h_obj_w)
      if (new (pointer_to_unsafe_data()) Type(h_obj_w.operator*()))
        m_Has_data = 1;
  }
  inline heap_obj_warper(heap_obj_warper &h_obj_w) {
    if (!!h_obj_w)
      if (new (pointer_to_unsafe_data()) Type(h_obj_w.operator*()))
        m_Has_data = 1;
  }
  inline heap_obj_warper(const Type &obj) {
    if (new (pointer_to_unsafe_data()) Type(obj)) m_Has_data = 1;
  }
  inline heap_obj_warper(Type &obj) {
    if (new (pointer_to_unsafe_data()) Type(obj)) m_Has_data = 1;
  }
  inline heap_obj_warper(Type &&obj) {
    if (new (pointer_to_unsafe_data()) Type(std::move(obj))) m_Has_data = 1;
  }
  inline heap_obj_warper &operator=(Type &&obj) {
    operator*() = std::move(obj);
    return *this;
  }
  inline heap_obj_warper &operator=(Type &obj) {
    operator*() = obj;
    return *this;
  }
  inline heap_obj_warper &operator=(const Type &obj) {
    operator*() = obj;
    return *this;
  }

  template <typename arg_T, typename Type>
  inline void data_init(std::initializer_list<arg_T> list) {
    data_de_init();
    if (new (m_data) Type(list)) m_Has_data = 1;
  }
  template <typename arg_T>
  inline void data_init(iterator_template<arg_T> list) {
    data_de_init();
    if (new (m_data) Type(list)) m_Has_data = 1;
  }

  template <typename... arguments_types>
  inline void data_init(arguments_types... args) {
    data_de_init();
    if (new (m_data) Type(args...)) m_Has_data = 1;
  }
  template <typename... arguments_types>
  inline void data_init_mv(arguments_types &&...args) {
    data_de_init();
    if (new (m_data) Type(std::move(args)...)) m_Has_data = 1;
  }
  template <typename... arguments_types>
  inline void data_init_r(const arguments_types &...args) {
    data_de_init();
    if (new (m_data) Type(args...)) m_Has_data = 1;
  }
  template <typename... arguments_types>
  inline void data_init_ct(const arguments_types &...args) {
    data_de_init();
    if (new (m_data) Type(args...)) m_Has_data = 1;
  }

  inline void data_de_init() {
    if (m_Has_data) pointer_to_data()->~Type();
    m_Has_data = 0;
  }

  inline void data_de_init(int fill_VAL) {
    if (m_Has_data) pointer_to_data()->~Type();
    m_Has_data = 0;
    static_str_algo::memset(m_data, fill_VAL, size);
  }

  constexpr inline bool has_data() { return m_Has_data; }

  // this may be uninitialized initialized...
  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() {
    return (uint8_t *)(m_data);
  }

  // a small inline pointer_to_unsafe_data_buffer
  constexpr inline uint8_t *PTUDB() { return pointer_to_unsafe_data_buffer(); }

  // this may be uninitialized initialized...
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const {
    return (uint8_t *)(m_data);
  }

  // a small inline pointer_to_unsafe_data_buffer
  constexpr inline const uint8_t *PTUDB() const {
    return pointer_to_unsafe_data_buffer();
  }

  // this may be uninitialized initialized...
  constexpr inline Type *pointer_to_unsafe_data() { return (Type *)(m_data); }

  // a small inline pointer_to_unsafe_data
  constexpr inline Type *PTUD() { return pointer_to_unsafe_data(); }
  // this may be uninitialized initialized...
  constexpr inline const Type *pointer_to_unsafe_data() const {
    return (const Type *const)(m_data);
  }

  // a small inline pointer_to_unsafe_data
  constexpr inline const Type *PTUD() const { return pointer_to_unsafe_data(); }

  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  // use like init_with_unsafe_new(new (init_with_unsafe_data()) Type());

  constexpr inline Type *init_with_unsafe_new(
      Type *ptr) {  // placement new  "new (ptr) Type();"
    return init_with_unsafe_data(ptr == pointer_to_unsafe_data());
  }

  // a small inline init_with_unsafe_new
  constexpr inline Type *IWUN(Type *ptr) { return init_with_unsafe_new(ptr); }
  constexpr inline const Type *pointer_to_data() const {
    if (!m_Has_data) throw std::exception(" bad access");
    return pointer_to_unsafe_data();
  }
  constexpr inline Type *pointer_to_data() {
    if (!m_Has_data) throw std::exception(" bad access");
    return pointer_to_unsafe_data();
  }

  constexpr inline Type *operator->() { return pointer_to_data(); }
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return pointer_to_data()->*my_var;
  }

  constexpr inline Type &operator*() { return *operator->(); }

  constexpr inline const Type *operator->() const { return pointer_to_data(); }
  template <typename my_type>
  inline auto operator->*(my_type my_var) const {
    return pointer_to_data()->*my_var;
  }
  constexpr inline const Type &operator*() const { return *operator->(); }

  inline bool operator==(const heap_obj_warper &other) const {
    return operator() == other.operator();
  }

  inline bool operator!=(const heap_obj_warper &other) const {
    return operator() != other.operator();
  }

  inline bool operator<(const heap_obj_warper &other) const {
    return (operator()) < (other.operator());
  }

  inline bool operator<=(const heap_obj_warper &other) const {
    return (operator()) <= (other.operator());
  }

  inline bool operator>(const heap_obj_warper &other) const {
    return (operator()) > (other.operator());
  }

  inline bool operator>=(const heap_obj_warper &other) const {
    return (operator()) >= (other.operator());
  }

#ifndef Arduino

  inline bool operator<=>(const heap_obj_warper &other) const {
    return operator() <=> other.operator();
  }
#endif  // ! Arduino

  bool operator!() const { return !m_Has_data; }
  explicit operator bool() const { return m_Has_data; }
  inline operator Type &() { return *pointer_to_data(); }
  inline operator const Type &() const { return *pointer_to_data(); }
};

class malloc_wrapper {
  malloc_wrapper &move(malloc_wrapper &otr) {
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
  malloc_wrapper &move(void *data_ptr, size_t size_of_ptr) {
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
  inline malloc_wrapper(size_t size_of_ptr = 0) {
    m_data_ptr = malloc(size_of_ptr);
  };
  inline malloc_wrapper(size_t size_of_ptr, int VAl_) {
    m_data_ptr = malloc(size_of_ptr);
    memset(VAl_);
  };
  inline malloc_wrapper(void *data_ptr, size_t size_of_ptr) {
    move(data_ptr, size_of_ptr);
  }
  inline malloc_wrapper(void *data_ptr, size_t size_of_ptr, int VAl_) {
    move(data_ptr, size_of_ptr).memset(VAl_);
  }
  inline malloc_wrapper &change_data_ptr(void *data_ptr, size_t size_of_ptr) {
    return move(data_ptr, size_of_ptr);
  }
  inline malloc_wrapper &change_data_ptr(malloc_wrapper &&otr) {
    return move(otr);
  }
  inline ~malloc_wrapper() { free(); }
  malloc_wrapper(malloc_wrapper &) = delete;
  inline malloc_wrapper(malloc_wrapper &&otr) noexcept { move(otr); }
  malloc_wrapper(const malloc_wrapper &) = delete;
  malloc_wrapper &operator=(malloc_wrapper &) = delete;
  inline malloc_wrapper &operator=(malloc_wrapper &&otr) noexcept {
    return move(otr);
  };
  malloc_wrapper &operator=(const malloc_wrapper &) = delete;
  template <typename Type>
  constexpr inline Type *get_ptr_as() {
    return (Type *)m_data_ptr;
  }
  constexpr inline void *get_ptr() { return m_data_ptr; }
  constexpr inline size_t get_size() { return m_cap_size; }
  void free() {
    if (do_deallocation_on_free_state() && m_cap_size) {
      ::free(m_data_ptr);
    }
    m_data_ptr = 0;
    m_cap_size = 0;
  }
  void *malloc(size_t size_of_ptr) {
    free();

    if (size_of_ptr) {
      m_data_ptr = ::malloc(size_of_ptr);

      if (m_data_ptr) {
        m_cap_size = size_of_ptr;
      }
    }

    return get_ptr();
  }
  void *realloc(size_t size_of_ptr) {
    // free();
    if (size_of_ptr) {
      m_data_ptr = ::realloc(do_deallocation_on_free_state() ? m_data_ptr : 0,
                             size_of_ptr);

      if (m_data_ptr) {
        m_cap_size = size_of_ptr;
      }
    }

    return get_ptr();
  }
  inline void *operator()(size_t size_of_ptr) { return malloc(size_of_ptr); }
  constexpr inline void *operator()() { return get_ptr(); }
  constexpr inline void memset(int _Val) {
    if (m_cap_size) {
      static_str_algo::memset(get_ptr(), _Val, m_cap_size);
    }
  }

  malloc_wrapper(void *data_ptr, size_t cap_size, uint8_t DO_deallocate)
      : m_data_ptr(data_ptr),
        m_cap_size(cap_size),
        m_Deallocation_state(DO_deallocate) {}
};

/*********************************************************************
   Filename:   sha256.h
   Author:     Brad Conte (brad AT bradconte.com)
   Copyright:
   Disclaimer: This code is presented "as is" without any guarantees.
   Details:    Defines the API for the corresponding SHA1 implementation.
 *********************************************************************/

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32  // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef uint8_t BYTE;       // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

struct SHA256_CTX {
 protected:
  char *to_string(char *buf) const;

 public:
  char *copy_to_c_string(char *buf, size_t len) const {
    if (len < 1024) return 0;
    static_str_algo::memset(buf, 0, len);
    return to_string(buf);
  }
  union {
    BYTE data[64]{};
    BYTE hashed_data[SHA256_BLOCK_SIZE];
  };
  WORD datalen{};
  unsigned long long bitlen{};
  WORD state[8]{};
  mjz_Str to_string() const;
  friend std::ostream &operator<<(std::ostream &CIN, const SHA256_CTX &obj);
  static inline int compare_hash(const void *rhs, const SHA256_CTX &lhs) {
    return SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  static inline int compare_hash(const SHA256_CTX &rhs, const void *lhs) {
    return SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend inline int operator!=(const void *rhs, const SHA256_CTX &lhs) {
    return SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  friend inline bool operator!=(const SHA256_CTX &rhs, const void *lhs) {
    if (lhs == 0) return 1;
    return SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend inline bool operator==(const void *rhs, const SHA256_CTX &lhs) {
    return !SHA256_CTX::compare_hash(rhs, lhs.hashed_data);
  }
  friend inline bool operator==(const SHA256_CTX &rhs, const void *lhs) {
    return !SHA256_CTX::compare_hash(rhs.hashed_data, lhs);
  }
  friend inline bool operator!=(const SHA256_CTX &rhs, const SHA256_CTX &lhs) {
    return !!SHA256_CTX::compare_hash(rhs.hashed_data, lhs.hashed_data);
  }
  friend inline bool operator==(const SHA256_CTX &rhs, const SHA256_CTX &lhs) {
    return !SHA256_CTX::compare_hash(rhs.hashed_data, lhs.hashed_data);
  }
  inline static int real_compare_hash(const void *rhs, const void *lhs) {
    return static_str_algo::memcmp(rhs, lhs, SHA256_BLOCK_SIZE);
  }
  static inline int compare_hash(const void *rhs, const void *lhs) {
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
    WORD i;

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
    WORD i;
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
    size_t i;

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
    WORD i;
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
  // the string length (not counting the '\0')
  size_t m_length;

 public:
  constexpr basic_mjz_Str_view(char *buffer, size_t length)
      : m_buffer(buffer), m_length(length) {}
  constexpr basic_mjz_Str_view()
      : m_buffer(const_cast<char *>(empty_STRING_C_STR)), m_length(0) {}

  constexpr basic_mjz_Str_view(const char *buffer, size_t length)
      : basic_mjz_Str_view(const_cast<char *>(buffer), length) {}
  constexpr ~basic_mjz_Str_view() = default;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &&) = delete;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view &operator=(const basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view(const basic_mjz_Str_view &) = delete;

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
  constexpr inline const char *c_str() const { return buffer_ref(); }
  constexpr inline const char *c_str() { return buffer_ref(); }
  constexpr inline char *C_str() { return m_buffer; }
  constexpr inline size_t length(void) const { return m_length; }
  constexpr inline char *data() { return m_buffer; }
  constexpr inline const char *data() const { return m_buffer; }

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

  inline constexpr int64_t indexOf(const basic_mjz_Str_view &s2) const {
    return indexOf(s2, 0);
  }
  inline constexpr int64_t indexOf(const basic_mjz_Str_view &s2,
                                   size_t fromIndex) const {
    return indexOf_cstr(s2.data(), s2.length(), fromIndex);
  }

  inline constexpr int64_t lastIndexOf(const basic_mjz_Str_view &s2) const {
    return lastIndexOf(s2, m_length - s2.m_length);
  }

  inline constexpr int64_t lastIndexOf(const basic_mjz_Str_view &s2,
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

  inline constexpr size_t find_last_not_of_in_str(const char *s,
                                                  size_t n) const {
    return static_str_algo::find_last_of_not_in_str(m_buffer, m_length, s, n);
  }

  inline constexpr size_t find_last_not_of(const basic_mjz_Str_view &str,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(str.c_str(), str.length());
  }

  inline constexpr size_t find_last_not_of(const char *s,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(s, strlen(s));
  }

  inline constexpr size_t find_last_not_of(const char *s, size_t pos,
                                           size_t n) const {
    return substr_view(pos).find_last_not_of_in_str(s, n);
  }

  inline constexpr size_t find_last_not_of(char c, size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(&c, 1);
  }

  inline constexpr size_t find_first_not_of_in_str(const char *s,
                                                   size_t n) const {
    return static_str_algo::find_first_of_not_in_str(m_buffer, m_length, s, n);
  }

  inline constexpr size_t find_first_not_of(const basic_mjz_Str_view &str,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(str.c_str(), str.length());
  }

  inline constexpr size_t find_first_not_of(const char *s,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(s, strlen(s));
  }
  inline constexpr size_t find_first_not_of(const char *s, size_t pos,
                                            size_t n) const {
    return substr_view(pos).find_first_not_of_in_str(s, n);
  }
  inline constexpr size_t find_first_not_of(char c, size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(&c, 1);
  }

  constexpr bool equals(const char *cstr, size_t cstr_len) const {
    return are_two_str_equale(m_buffer, m_length, cstr, cstr_len);
  }
  constexpr bool equals(const basic_mjz_Str_view &s2) const {
    //
    return (m_length == s2.m_length && compareTo(s2) == 0);
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
  inline constexpr void toCharArray(char *buf, size_t bufsize,
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
    if (length__ == 0 || m_length == 0 || length__ > m_length) {
      return -1;
    }

    if (fromIndex >= m_length) {
      fromIndex = m_length - 1;
    }

    int64_t found = -1;

    for (const char *p = m_buffer; p <= m_buffer + fromIndex; p++) {
      p = strstr(p, m_length, cstr__, length__);

      if (!p) {
        break;
      }

      if ((size_t)(p - m_buffer) <= fromIndex) {
        found = (int64_t)(p - m_buffer);
      }
    }

    return found;
  }
  inline constexpr int64_t lastIndexOf_cstr(const char *cstr__,
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

  constexpr const char &at(int64_t i) const { return operator[](i); }
  constexpr const char &back() const { return buffer_ref()[0]; }
  constexpr const char &front() const { return operator[]((int64_t)-1); }

  inline constexpr basic_mjz_Str_view substr_view(size_t beginIndex) {
    return substr_view(beginIndex, length() - beginIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view(size_t beginIndex,
                                                  size_t endIndex) const {
    const char *out_ptr{};
    size_t out_len{};
    substring_give_ptrULL(beginIndex, endIndex, out_ptr, out_len);
    return out_len ? basic_mjz_Str_view(out_ptr, out_len)
                   : basic_mjz_Str_view();
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(size_t beginIndex,
                                                        size_t number) {
    return substr_view(beginIndex, number + beginIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view(int64_t beginIndex,
                                                  int64_t endIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(endIndex));
  }
  inline constexpr basic_mjz_Str_view substr_view(int64_t beginIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex));
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(
      unsigned int beginIndex, unsigned int number) const {
    return substr_view_beg_n((size_t)beginIndex, (size_t)number);
  }
  inline constexpr basic_mjz_Str_view substr_view(int beginIndex) const {
    return substr_view((int64_t)beginIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view(int beginIndex,
                                                  int endIndex) const {
    return substr_view((int64_t)beginIndex, (int64_t)endIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(int beginIndex,
                                                        int number) const {
    return substr_view_beg_n((int64_t)beginIndex, (size_t)number);
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(size_t beginIndex,
                                                        size_t number) const {
    return substr_view(beginIndex, number + beginIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view(size_t beginIndex) const {
    return substr_view(beginIndex, length() - beginIndex);
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }

 public:
  std::pair<hash_sha256, mjz_Str> hash_with_output(uint8_t n = 0) const;
  mjz_Str substring(size_t beginIndex);
  mjz_Str substring(size_t beginIndex, size_t endIndex) const;
  mjz_Str substring_beg_n(size_t beginIndex, size_t number) const;
  mjz_Str substr(size_t pos = 0, size_t len = npos) const;
  mjz_Str substring(int64_t beginIndex, int64_t endIndex) const;
  mjz_Str substring(int64_t beginIndex) const;
  mjz_Str substring_beg_n(int64_t beginIndex, size_t number);
  mjz_Str substring_beg_n(unsigned int beginIndex, unsigned int number) const;
  mjz_Str substring(int beginIndex) const;
  mjz_Str substring(int beginIndex, int endIndex) const;
  mjz_Str substring_beg_n(int beginIndex, int number) const;
  mjz_Str substring(size_t beginIndex) const;
  mjz_Str substring_beg_n(int64_t beginIndex, size_t number) const;

  constexpr inline size_t max_size() const { return (((size_t)(-1)) >> 1) - 1; }
};

class mjz_Str;
class mjz_str_view;
class basic_mjz_String : public basic_mjz_Str_view {
 protected:
  friend class mjz_Str;
  // the array length minus one (for the '\0')
  size_t m_capacity;

 public:
  inline basic_mjz_String(char *bfr, size_t cap, size_t len)
      : basic_mjz_Str_view(bfr, len), m_capacity(cap){};
  inline basic_mjz_String(const char *bfr, size_t cap, size_t len)
      : basic_mjz_String(const_cast<char *>(bfr), cap, len){};
  inline basic_mjz_String() : basic_mjz_String(empty_STRING_C_STR, 0, 0) {}
  inline ~basic_mjz_String() = default;
  ;
  basic_mjz_String(basic_mjz_String &&) = delete;
  basic_mjz_String(const basic_mjz_String &) = delete;
  basic_mjz_String(basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &&) = delete;
  basic_mjz_String &operator=(const basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &) = delete;

 public:
};
#ifndef Arduino
class mjz_Str : public basic_mjz_String {
#if 0

  }
#endif
#else
class mjz_Str : public basic_mjz_String,
                public Stream {  //
#endif  // Arduino
 protected:
  static std::shared_ptr<mjz_Str_DATA_storage_cls>
      main_mjz_Str_DATA_storage_Obj_ptr;
  bool did_drived_mjz_Str_DATA_storage_Obj_ptr_set{0};
  // void (mjz_Str::*update_event_F_p)( void); //departed
  // (object_ptr->*pointer_name)(arguments)//like (this->*update_event_F_p)();
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      drived_mjz_Str_DATA_storage_Obj_ptr = main_mjz_Str_DATA_storage_Obj_ptr;
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      &drived_mjz_Str_DATA_storage_Obj_ptr_set();
  friend class StringSumHelper;
  // use a function pointer to allow for "if (s)" without the
  // complications of an operator bool(). for more information,see:
  // http://www.artima.com/cppsource/safebool.html
  typedef void (mjz_Str::*StringIfHelperType)() const;
  void StringIfHelper() const {}
  inline bool realloc_helper_is_in_stack(void *ptr);

 private:
  // copy and move
  mjz_Str &copy(const char *cstr, size_t length) {
    return copy(cstr, length, 0);
  }
  mjz_Str &copy(const __FlashStringHelper *pstr, size_t length);
  void move(mjz_Str &rhs);
  [[nodiscard]] void *realloc_pv(void *ptr, size_t new_size, bool constructor);
  void free_pv(void *&ptr, bool constructor);
  void free_pv(void *const &ptr, bool constructor);
  mjz_Str &copy(const char *cstr, size_t length, bool constructor);

 protected:
  [[nodiscard]]  // virtual
  void *
  realloc(void *ptr, size_t new_size) {
    return get_realloc_alloc_function()(ptr, new_size);
  }
  // virtual
  void free(void *ptr) { return get_free_alloc_function()(ptr); }

  stack_str_buf stack_obj_buf;

 public:
  const static char
      the_in_reinterpreted_char_cca[the_reinterpreted_char_cca_size];  // just
  // me
  const static char
      the_out_reinterpreted_char_cca[the_reinterpreted_char_cca_size];  // just
  // me
  const static char forbiden_chars_cnt[forbiden_chars_cnt_size];
  // stream
 protected:
  friend void str_helper__op_shift_input_(mjz_Str &rhs, mjz_Str &CIN);
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
  // mjz_Str and used parseFloat/Int with a custom ignore character. To keep
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
  // the stream or -1 if timeout
  void setWriteError(int err = 1) if_override_then_override {
    drived_mjz_Str_DATA_storage_Obj_ptr_set()->write_error = err;
  }

 public:
  int getWriteError() if_override_then_override {
    return drived_mjz_Str_DATA_storage_Obj_ptr->write_error;
  }
  void clearWriteError() if_override_then_override { setWriteError(0); }
  // default to zero, meaning "a single write may block"
  // should be overridden by subclasses with buffering
  virtual int availableForWrite() if_ard_then_override {
    return (int)availableForWriteLL();
  }
  virtual int64_t availableForWriteLL() if_ard_then_override { return 1; }
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
  size_t print(const mjz_Str &) if_override_then_override;
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
  size_t println(const mjz_Str &) if_override_then_override;
  size_t println(void) if_override_then_override;  // TODO: V1071
  // https://pvs-studio.com/en/docs/warnings/V1071/
  // Consider inspecting the 'println' function.
  // The return value is not always used. Total
  // calls: 13, discarded results: 1.
  // parsing methods
  void setTimeout(
      unsigned long timeout);  // sets maximum milliseconds to wait for
  // stream data, default is 1 second
  unsigned long getTimeout(void) {
    return drived_mjz_Str_DATA_storage_Obj_ptr->_timeout;
  }
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
  // Arduino mjz_Str functions to be added here
  mjz_Str read_mjz_Str();
  mjz_Str read_mjz_Str_Until(char terminator);
  size_t write(const char *buf, size_t size_);
  size_t write(const char *buf);
  size_t write(uint8_t) if_ard_then_override;
  inline size_t write(char cr) { return concat(cr); }
  size_t write(const uint8_t *buf, size_t size_) if_ard_then_override;
  int64_t availableLL() if_ard_then_override;
  int available() if_ard_then_override { return (int)availableLL(); }
  int read() if_ard_then_override;
  int peek() if_ard_then_override;
  void flush() if_ard_then_override;
  size_t read(uint8_t *buf, size_t size_);
  size_t readBytes(char *buffer_, size_t length) {
    return read((uint8_t *)buffer_, length);
  }
  size_t readBytes(uint8_t *buffer_, size_t length) {
    return readBytes((char *)buffer_, length);
  }
  void begin(unsigned long);
  void begin(unsigned long, uint16_t);
  //  void end(); used
  // stream

  // new and delete
  [[nodiscard]] static void *operator new(size_t size_);
  [[nodiscard]] static void *operator new[](size_t size_);
  static void operator delete(void *p);
  static void operator delete[](void *ptr);
  inline static void *operator new(size_t, void *where) { return where; }
  inline static void operator delete(void *ret_piont, void *) {
    delete (mjz_Str *)ret_piont;
  }
  inline static void operator delete(void *ret_piont, size_t, void *) {
    delete (mjz_Str *)ret_piont;
  }
  inline static mjz_Str &replace_with_new_str(mjz_Str &where) {
    // obj.mjz_Str::~mjz_Str();//bad not calling virtually
    // obj->~mjz_Str(); // good calls the most derived constructor
    where.~mjz_Str();  // end lifetime
    new (&where) mjz_Str;
    return where;
  }
  inline static mjz_Str *replace_with_new_str(mjz_Str *where) {
    where->~mjz_Str();  // end lifetime
    new (where) mjz_Str;
    return where;
  }
  // this class is a namespace in this part
  class realloc_new_ns {
   public:
    [[nodiscard]] static void *operator new(size_t size_);
    [[nodiscard]] static void *operator new[](size_t size_);
    static void operator delete(void *p);
    static void operator delete[](void *ptr);
    static void *operator new(size_t, void *where);
    inline static mjz_Str &replace_heap_str_with_new_str(mjz_Str &where) {
      where.~mjz_Str();  // end lifetime
      new (&where) mjz_Str;
      return where;
    }
    inline static mjz_Str *replace_heap_str_with_new_str(mjz_Str *where) {
      where->~mjz_Str();  // end lifetime
      new (where) mjz_Str;
      return where;
    }
  };
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return this->*my_var;
  }
  template <typename my_type>
  inline auto operator->*(my_type my_var) const {
    return this->*my_var;
  }
  // constructors
  // creates a copy of the initial value.
  // if the initial value is null or invalid,or if memory allocation
  // fails,the string will be marked as invalid (i.e. "if (s)" will
  // be false).
  inline mjz_Str(const char *cstr, size_t length);
  inline mjz_Str(const uint8_t *cstr, size_t length)
      : mjz_Str((const char *)cstr, length) {}
  inline mjz_Str(const mjz_Str &str);
  inline mjz_Str(const __FlashStringHelper *str);
  inline explicit mjz_Str(const char *cstr)
      : mjz_Str(cstr, (size_t)strlen(cstr)) {}
  mjz_Str(std::initializer_list<const char>);
  mjz_Str(iterator_template<const char>);

  template <class InputIterator>
  mjz_Str(InputIterator first, InputIterator last) {
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

  explicit mjz_Str(mjz_Str &&rval) noexcept;
  // explicit mjz_Str(const mjz_Str *&&rval) : mjz_Str(std::move(*rval)) {
  // }// this will give me headaches in the long run so i dont move it
  inline explicit mjz_Str(const mjz_Str *rval) : mjz_Str(*rval) {}
  explicit mjz_Str(char c);
  explicit mjz_Str(unsigned char, unsigned char base = 10);
  explicit mjz_Str(int, unsigned char base = 10);
  explicit mjz_Str(unsigned int, unsigned char base = 10);
  explicit mjz_Str(long, unsigned char base = 10);
  explicit mjz_Str(unsigned long, unsigned char base = 10);
  explicit mjz_Str(long long int, unsigned char base = 10);
  explicit mjz_Str(unsigned long long int, unsigned char base = 10);
  explicit mjz_Str(float, unsigned char decimalPlaces = 2);
  explicit mjz_Str(double, unsigned char decimalPlaces = 2);
  inline mjz_Str() : mjz_Str((const char *)empty_STRING_C_STR, 0) {}
  explicit inline mjz_Str(const basic_mjz_String &otr)
      : mjz_Str(otr.c_str(), otr.length()) {}
  explicit inline mjz_Str(const basic_mjz_Str_view &otr)
      : mjz_Str(otr.c_str(), otr.length()) {}
#ifndef Arduino
  mjz_Str &operator=(std::string_view &x) {
    return operator=(std::string(x).c_str());
  }
  mjz_Str &operator=(std::string_view &&x) {
    return operator=(std::string(x).c_str());
  }
  mjz_Str &assign_range(std::initializer_list<const char> list);
  mjz_Str &assign_range(iterator_template<const char> list);
  mjz_Str &operator+=(std::string_view &x) {
    return operator+=(std::string(x).c_str());
  }
  mjz_Str &operator+=(std::string_view &&x) {
    return operator+=(std::string(x).c_str());
  }
#else
  mjz_Str(String &x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str(String &&x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str(const String &x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str &operator=(String &x) { return operator=(x.c_str()); }
  mjz_Str &operator=(const String &x) { return operator=(x.c_str()); }
  mjz_Str &operator=(String &&x) { return operator=(x.c_str()); }
  mjz_Str &operator+=(String &x) { return operator+=(x.c_str()); }
  mjz_Str &operator+=(const String &x) { return operator+=(x.c_str()); }
  mjz_Str &operator+=(String &&x) { return operator+=(x.c_str()); }
#endif
  mjz_Str &operator=(basic_mjz_String &x) {
    return (*this)(x.c_str(), x.length());
  }
  mjz_Str &operator=(basic_mjz_String &&x) {
    return (*this)(x.c_str(), x.length());
  }
  mjz_Str &operator+=(basic_mjz_String &x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_Str &operator+=(basic_mjz_String &&x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_Str(std::string &x) : mjz_Str(x.c_str()) {}
  mjz_Str(std::string &&x) : mjz_Str(x.c_str()) {}
  mjz_Str(const std::string &x) : mjz_Str(x.c_str()) {}
  virtual ~mjz_Str(void);  // make all drived destructors called
  void adjust_cap();
  mjz_Str &operator-=(const mjz_Str &othr_);
  mjz_Str &operator-=(mjz_Str &&othr_);
  mjz_Str &operator*=(unsigned int number_of_it);
  mjz_Str operator*(unsigned int number_of_it);
  mjz_Str &operator/=(const mjz_Str &othr_);
  mjz_Str &operator/=(mjz_Str &&othr_);
  mjz_Str &operator++();  // print empty line
  mjz_Str operator++(int);
  mjz_Str &operator--();  // read one character
  inline char operator-() { return (char)read(); }
  inline mjz_Str &operator~() { return operator()(); }
  inline mjz_Str &operator+() { return ++(*this); }

  mjz_Str operator--(int);
  inline mjz_Str &operator()() {
    operator()(empty_STRING_C_STR, (size_t)0);
    return *this;
  };
  inline mjz_Str &operator()(const mjz_Str &other) {
    this->operator=(other);
    return *this;
  };
  inline mjz_Str &operator()(const char *other) {
    operator()(other, (size_t)strlen(other));
    return *this;
  };
  inline mjz_Str &operator()(const char *other, size_t size_len);
  inline mjz_Str &operator()(mjz_Str &&other) {
    this->operator=(std::move(other));
    return *this;
  };
  // memory management
  // return true on success,false on failure (in which case,the string
  // is left unchanged). reserve(0),if successful,will validate an
  // invalid string (i.e.,"if (s)" will be true afterwards)
  bool reserve(size_t size_, bool just_size = 0, bool constructor = 0);
  bool addto_length(size_t addition_tolen, bool just_size = 0);
  explicit operator char *() { return buffer_ref(); }
  explicit operator const uint8_t *() const {
    return (const uint8_t *)buffer_ref();
  }
  explicit operator uint8_t *() { return (uint8_t *)buffer_ref(); }
  explicit operator void *() { return (void *)buffer_ref(); }
  explicit operator uint16_t *() { return (uint16_t *)buffer_ref(); }
  explicit operator uint32_t *() { return (uint32_t *)buffer_ref(); }
  explicit operator uint64_t *() { return (uint64_t *)buffer_ref(); }
  explicit operator const void *() const { return (void *)buffer_ref(); }
  explicit operator const uint16_t *() const {
    return (uint16_t *)buffer_ref();
  }
  explicit operator const uint32_t *() const {
    return (uint32_t *)buffer_ref();
  }
  explicit operator const uint64_t *() const {
    return (uint64_t *)buffer_ref();
  }
  explicit operator const size_t() const { return m_length; }
  explicit operator size_t() const { return m_length; }
  explicit operator size_t &() { return (size_t &)m_length; }
  explicit operator const long() const { return toInt(); }
  explicit operator const long long() const { return toLL(); }
  explicit operator const float() const { return toFloat(); }
  explicit operator const double() const { return toDouble(); }
  explicit operator std__string_view_if_is() const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  explicit operator const std__string_view_if_is() const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  [[nodiscard]] explicit operator std::string() const {
    return std::string((const char *)buffer_ref(), length());
  }
  [[nodiscard]] explicit operator const std::string() const {
    return std::string((const char *)buffer_ref(), length());
  }
  operator const char *() const { return buffer_ref(); }
  char &operator[](size_t index);
  char &operator[](int64_t index);
  std__string_view_if_is std_new_sv() = delete;
  std__string_view_if_is std_sv() const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  std__string_view_if_is &&std_sv_temp() const {
    return std::move(std__string_view_if_is((const char *)buffer_ref()));
  }
  [[nodiscard]] const std::string std_s() const {
    return std::string((const char *)buffer_ref());
  }
  [[nodiscard]] const std::string &&std_st() const {
    return std::move(std::string((const char *)buffer_ref()));
  }
  // creates a copy of the assigned value. if the value is null or
  // invalid,or if the memory allocation fails,the string will be
  // marked as invalid ("if (s)" will be false).
  mjz_Str &operator=(const mjz_Str &rhs);
  mjz_Str &operator=(std::string &x) { return operator=(x.c_str()); }
  mjz_Str &operator=(std::string &&x) { return operator=(x.c_str()); }
  mjz_Str &operator=(const std::string &x) { return operator=(x.c_str()); }
  mjz_Str &operator=(const char *cstr);
  mjz_Str &operator=(const __FlashStringHelper *str);
  mjz_Str &operator=(mjz_Str &&rval) noexcept;
  mjz_Str &operator=(const mjz_Str *rval) { return operator=(*rval); }
  //  mjz_Str &operator=( const mjz_Str *&&rval){
  // return operator=(std::move(*rval));
  // }// this will give me headaches in the long run so i dont move it
  // concatenate (works w/ built-in types)
  // returns true on success,false on failure (in which case,the string
  // is left unchanged). if the argument is null or invalid,the
  // concatenation is considered unsuccessful.
  bool concat(const mjz_Str &str);
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

  mjz_Str &append(const mjz_Str &str) {
    concat(str);
    return *this;
  }
  mjz_Str &append(const mjz_Str &str, size_t subpos, size_t sublen);
  mjz_Str &append(const char *s) {
    concat(s);
    return *this;
  }
  mjz_Str &append(const char *s, size_t n) {
    concat(s, n);
    return *this;
  }
  mjz_Str &append(size_t n, char c) {
    if (!addto_length(n)) return *this;
    memset(m_buffer + length() - n, c, n);
    return *this;
  }
  template <class InputIterator>
  mjz_Str &append(InputIterator first, InputIterator last) {
    size_t newlen = last - first;
    if (!addto_length(newlen)) return *this;

    char *ptr_{m_buffer + length() - newlen};
    for (; first != last; first++) {  // i don't like != but :(
      *ptr_++ = *first;
    }
    return *this;
  }

  const mjz_Str &operator>>(mjz_Str &typing) const;
  const mjz_Str &operator>>(mjz_Str *typing) const;
  mjz_Str &operator>>(mjz_Str &typing);
  mjz_Str &operator>>(mjz_Str *typing);
  const mjz_Str &operator>>=(mjz_Str &typing) const {
    typing.operator=(empty_STRING_C_STR);
    return operator>>(typing);
  }
  const mjz_Str &operator>>=(mjz_Str *typing) const {
    typing->operator=(empty_STRING_C_STR);
    return operator>>(typing);
  }
  mjz_Str &operator>>=(mjz_Str &typing) {
    typing.operator=(empty_STRING_C_STR);
    return operator>>(typing);
  }
  mjz_Str &operator>>=(mjz_Str *typing) {
    typing->operator=(empty_STRING_C_STR);
    return operator>>(typing);
  }
  mjz_Str &operator<<(mjz_Str &typing);
  mjz_Str &operator<<(mjz_Str *typing);
  mjz_Str &operator<<(const mjz_Str &typing);
  mjz_Str &operator<<(mjz_Str &&typing);
  mjz_Str &operator<<=(mjz_Str &typing) {
    if (&typing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typing);
    }

    mjz_Str new_temp = typing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  mjz_Str &operator<<=(const mjz_Str &typing) {
    if (&typing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typing);
    }

    mjz_Str new_temp = typing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  mjz_Str &operator<<=(mjz_Str &&typing) {
    if (&typing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typing);
    }

    mjz_Str new_temp = typing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  mjz_Str &operator>>(char &var) {
    get_s_shift_op_r().scanf_s("%c", &var, 1);
    return get_s_shift_op_r();
  }
  mjz_Str &operator>>(int &var) {
    get_s_shift_op_r().scanf_s("%d", &var);
    return get_s_shift_op_r();
  }
  mjz_Str &operator>>(double &var) {
    var = (double)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  mjz_Str &operator>>(float &var) {
    var = (float)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  // if there's not enough memory for the concatenated value,the string
  // will be left unchanged (but this isn't signalled in any way)
  mjz_Str &operator+=(const mjz_Str &rhs) {
    concat(rhs);
    return (*this);
  }
  mjz_Str &operator+=(mjz_Str &&rhs) {
    concat(rhs);
    return (*this);
  }
  mjz_Str &operator+=(const char *cstr) {
    concat(cstr);
    return (*this);
  }
  mjz_Str &operator+=(char c) {
    concat(c);
    return (*this);
  }
  mjz_Str &operator+=(unsigned char num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(int num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(unsigned int num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(long num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(unsigned long num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(long long num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(unsigned long long num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(float num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(double num) {
    concat(num);
    return (*this);
  }
  mjz_Str &operator+=(const __FlashStringHelper *str) {
    concat(str);
    return (*this);
  }
  mjz_Str &operator+=(const basic_mjz_Str_view &str) {
    concat(str);
    return (*this);
  }
  friend mjz_Str &&operator_plus(mjz_Str &&lhs, const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(mjz_Str &&lhs,
                                   const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(const basic_mjz_Str_view &rhs,
                                   const basic_mjz_Str_view &lhs);
  friend StringSumHelper operator+(mjz_Str lhs, const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(const basic_mjz_Str_view &lhs,
                                   const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(mjz_Str &&lhs, const StringSumHelper &rhs);
  friend StringSumHelper operator+(mjz_Str lhs, const StringSumHelper &rhs);
  friend StringSumHelper operator+(const basic_mjz_Str_view &lhs,
                                   const StringSumHelper &rhs);
  friend StringSumHelper operator+(const StringSumHelper &lhs,
                                   const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(StringSumHelper &&lhs,
                                   const basic_mjz_Str_view &rhs);
  friend StringSumHelper operator+(StringSumHelper &&lhs, const char *cstr);
  friend StringSumHelper operator+(StringSumHelper &&lhs, char c);
  friend StringSumHelper operator+(StringSumHelper &&lhs, unsigned char num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, int num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, unsigned int num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, long num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, unsigned long num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, long long num);
  friend StringSumHelper operator+(StringSumHelper &&lhs,
                                   unsigned long long num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, float num);
  friend StringSumHelper operator+(StringSumHelper &&lhs, double num);
  friend StringSumHelper operator+(StringSumHelper &&lhs,
                                   const __FlashStringHelper *rhs);

  typedef void *(*function_ptr)(const mjz_Str &, void *);
  operator StringIfHelperType() const {
    return buffer_ref() ? &mjz_Str::StringIfHelper : 0;
  }
  void *do_this_for_me(function_ptr, void *x = 0);
  // comparison (only works w/ Strings and "strings")
  friend bool is_blank_characteres_default(char);
  friend char char_to_char_for_reinterpret_fnc_ptr_default(char);
  friend bool is_forbiden_character_default(char);
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
  inline const std::function<bool(char)> &get_is_blank_character_function()
      const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->is_blank_character;
  }
  inline const std::function<void(void *)> &get_free_alloc_function() const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->free_fnctn;
  }
  inline const std::function<void *(void *, size_t)>
      &get_realloc_alloc_function() const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->realloc_fnctn;
  }
  inline const std::function<char(char)>
      &get_char_to_char_for_reinterpret_fnc_ptr_function() const {
    return drived_mjz_Str_DATA_storage_Obj_ptr
        ->char_to_char_for_reinterpret_fnc_ptr;
  }
  inline const std::function<bool(char)> &get_is_forbiden_character_function()
      const {
    return drived_mjz_Str_DATA_storage_Obj_ptr->is_forbiden_character;
  }
  bool is_blank() const;
  bool is_forbiden(char) const;
  bool change_reinterpret_char_char(char);
  bool char_to_char_for_reinterpret(char &c_char) const;
  static int8_t char_to_int_for_string(char c_char);
  friend std::istream &helper__op_shift_input_(const mjz_Str &rhs,
                                               std::istream &CIN,
                                               mjz_Str &get_shift_op_s);
  [[nodiscard]] static mjz_Str create_mjz_Str_char_array(size_t size_,
                                                         char filler = 0,
                                                         bool do_fill = 1);
  [[nodiscard]] static mjz_Str create_mjz_Str_2D_char_array(size_t size_col,
                                                            size_t size_row,
                                                            char filler = 0,
                                                            bool do_fill = 1);
  friend const mjz_Str &helper__op_shift_input_(const mjz_Str &rhs,
                                                const mjz_Str &CIN,
                                                mjz_Str &get_shift_op_s);
  friend std::istream &operator<<(mjz_Str &rhs, std::istream &CIN) {
    return helper__op_shift_input_(rhs, CIN, rhs.get_s_shift_op_l_s());
  }
  friend std::istream &operator>>(std::istream &CIN, mjz_Str &rhs) {
    return helper__op_shift_input_(rhs, CIN, rhs.get_shift_op_r_s());
  }
  friend std::ostream &operator<<(std::ostream &COUT, const mjz_Str &rhs) {
    COUT.write(rhs.get_shift_op_l_sc().c_str(),
               rhs.get_shift_op_l_sc().length());
    return COUT;
  }
  friend std::ostream &operator>>(const mjz_Str &rhs, std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_sc().c_str(),
               rhs.get_s_shift_op_r_sc().length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT, mjz_Str &rhs) {
    COUT.write(rhs.get_shift_op_l_s().c_str(), rhs.get_shift_op_l_s().length());
    return COUT;
  }
  friend std::ostream &operator>>(mjz_Str &rhs, std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_s().c_str(),
               rhs.get_s_shift_op_r_s().length());
    return COUT;
  }
  friend std::ostream &operator>>(mjz_Str &&rhs, std::ostream &COUT) {
    COUT.write(rhs.get_s_shift_op_r_sc().c_str(),
               rhs.get_s_shift_op_r_sc().length());
    return COUT;
  }
  friend std::ostream &operator<<(std::ostream &COUT, mjz_Str &&rhs) {
    COUT.write(rhs.get_shift_op_l_sc().c_str(),
               rhs.get_shift_op_l_sc().length());
    return COUT;
  }
  friend class STRINGSerial;
  // character access
  void setCharAt(size_t index, char c);
  // character access
  void setCharAt(int64_t index, char c);
  inline char *begin_c_str() { return m_buffer; }
  char *end_c_str() { return m_buffer + length(); }
  char *endAST_c_str() { return end_c_str(); }
  const char *begin_c_str() const { return m_buffer; }
  const char *endAST_c_str() const { return end_c_str(); }
  const char *end_c_str() const { return m_buffer + length(); }

  // Iterator Class
  using const_iterator = mjz_ard::iterator_template<const char>;
  using iterator = mjz_ard::iterator_template<char>;

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
  mjz_Str &erase(size_t pos_ = 0, size_t len_ = -1);
  iterator erase(iterator p);
  iterator erase(iterator first, iterator last);
  mjz_Str &erase_from_f_to_l(size_t first, size_t last);
  // search
  // modification
  void find_and_replace(char find, char replace_);
  void find_and_replace(const mjz_Str &find, const mjz_Str &replace_);
  void find_and_replace(const char *find, size_t find_count,
                        const char *replace_, size_t replace_count);

  mjz_Str &replace(size_t pos, size_t len, const basic_mjz_Str_view &str);
  mjz_Str &replace_ll(int64_t pos, size_t len, const basic_mjz_Str_view &str);
  mjz_Str &replace(iterator i1, iterator i2, const basic_mjz_Str_view &str);

  mjz_Str &replace(size_t pos, size_t len, const basic_mjz_Str_view &str,
                   size_t subpos, size_t sublen);
  mjz_Str &replace(size_t pos, size_t len, const char *s);
  mjz_Str &replace(size_t pos, size_t len, const char *s, size_t n);

  mjz_Str &replace(iterator i1, iterator i2, const char *s, size_t n);
  mjz_Str &replace(iterator i1, iterator i2, const char *s) {
    return replace(i1, i2, s, strlen(s));
  }
  mjz_Str &replace(size_t pos, size_t len, size_t n, char c);

  mjz_Str &replace(iterator i1, iterator i2, size_t n, char c);

  template <class InputIterator>
  mjz_Str &replace(iterator i1, iterator i2, InputIterator first,
                   InputIterator last) {
    mjz_ard::mjz_Str str(first, last);
    replace(i1, i2, str.c_str(), str.length());
    return *this;
  }

  void remove(size_t index);
  void remove(size_t index, size_t count);
  void toLowerCase(void);
  void toUpperCase(void);
  void trim(void);
  mjz_Str string_do_interpret();
  void string_do_interpret(mjz_Str &instr);
  mjz_Str &ULL_LL_to_str_add(size_t value, int radix, bool is_signed,
                             bool force_neg = 0);
  mjz_Str &ULL_LL_to_str_rep(size_t value, int radix, bool is_signed,
                             bool force_neg = 0);
  friend void swap(mjz_Str &lhs, mjz_Str &rhs) { lhs.swap(rhs); }
  void swap(mjz_Str &rhs) {
    mjz_Str &lhs = *this;
    mjz_Str lhs_buffer(std::move(lhs));  // be careful
    new (&lhs) mjz_Str;                  // be careful
    lhs = std::move(rhs);
    rhs = std::move(lhs_buffer);
  }
  char &at(int64_t i) { return operator[](i); }
  char &front() { return operator[]((int64_t)-1); }
  char &back() { return buffer_ref()[0]; }

  mjz_Str &push_back(char cr_) {
    write(cr_);
    return *this;
  }
  mjz_Str &insert(size_t pos, const basic_mjz_Str_view &other);
  mjz_Str &insert(size_t pos, const basic_mjz_Str_view &other, size_t subpos,
                  size_t sublen = -1);
  mjz_Str &insert(size_t pos, const char *s, size_t n);
  mjz_Str &insert(size_t pos, const char *other) {
    return insert(pos, other, strlen(other));
  }
  mjz_Str &pop_back() {
    if (m_length) {
      buffer_ref()[--m_length] = 0;
    }

    return *this;
  }
  mjz_Str &insert(size_t pos, size_t n, char c);
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
    mjz_ard::mjz_Str str(first, last);
    insert(p, str.length(), str.c_str());
  }

  friend class basic_mjz_Str_view;

  constexpr size_t capacity() const { return m_capacity; }
  void clear() { operator()(); }
  void shrink_to_fit() {
    mjz_Str str = create_mjz_Str_char_array(length(), 0, 1);
    memmove(str.C_str(), c_str(), str.length());
    *this = std::move(str);
  }
  inline bool empty() const { return is_blank(); }

 protected:
  //  void update_event();
  //  void update_event_ard_string();
  const mjz_Str &get_shift_op_rc() const;
  mjz_Str &get_shift_op_r();
  const mjz_Str &get_shift_op_lc() const;
  mjz_Str &get_shift_op_l();
  const mjz_Str &get_shift_op_r_sc() const;
  mjz_Str &get_shift_op_r_s();
  const mjz_Str &get_shift_op_l_sc() const;
  mjz_Str &get_shift_op_l_s();
  const mjz_Str &get_s_shift_op_rc() const;
  mjz_Str &get_s_shift_op_r();
  const mjz_Str &get_s_shift_op_lc() const;
  mjz_Str &get_s_shift_op_l();
  const mjz_Str &get_s_shift_op_r_sc() const;
  mjz_Str &get_s_shift_op_r_s();
  const mjz_Str &get_s_shift_op_l_sc() const;
  mjz_Str &get_s_shift_op_l_s();
  void init(bool constructor = 1);
  void invalidate(bool constructor = 1);
  bool changeBuffer(size_t maxStrLen, bool constructor);

 public:
  // easy quality of life
  inline mjz_Str *operator->() { return this; }
  inline const mjz_Str *operator->() const { return this; }
  inline mjz_Str &operator*() { return *this; }
  inline const mjz_Str &operator*() const { return *this; }
  // templated
  template <typename TYPE_>
  mjz_Str substring(TYPE_ beginIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex);
  };
  template <typename TYPE_, typename TYPE_2>
  mjz_Str substring(TYPE_ beginIndex, TYPE_2 endIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex, (int64_t)endIndex);
  }
  template <typename TYPE_, typename TYPE_2>
  mjz_Str substring_beg_n(TYPE_ beginIndex, TYPE_2 number) const {
    return basic_mjz_String::substring_beg_n((int64_t)beginIndex,
                                             (int64_t)number);
  }
  /* template <typename... arguments_types>
   size_t write(arguments_types &...arguments_arr) {
    mjz_Str return_val = std::move(mjz_Str(arguments_arr...));
    return write((const uint8_t *)return_val.c_str(),
                 (size_t)return_val.length());
  }
  template <typename... arguments_types>
   size_t write(const arguments_types &...arguments_arr)
  { mjz_Str return_val = std::move(mjz_Str(arguments_arr...)); return
  write((const uint8_t *)return_val.c_str(), (size_t)return_val.length());
  }
  template <typename... arguments_types>
   size_t write(arguments_types &&...arguments_arr) {
    mjz_Str return_val = std::move(mjz_Str(arguments_arr...));
    return write((const uint8_t *)return_val.c_str(),
                 (size_t)return_val.length());
  }
  */

  template <typename... arguments_types>
  int scanf(const char *format, arguments_types &...arguments_arr) {
    int ret = sscanf((char *)buffer_ref(), format, arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf(const mjz_Str &format, arguments_types &...arguments_arr) {
    int ret =
        sscanf((char *)buffer_ref(), format.buffer_ref(), arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf(const char *format, const arguments_types &...arguments_arr) {
    int ret = sscanf((char *)buffer_ref(), format, arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf(const mjz_Str &format, const arguments_types &...arguments_arr) {
    int ret =
        sscanf((char *)buffer_ref(), format.buffer_ref(), arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const char *format, arguments_types &...arguments_arr) {
    int ret = sscanf_s((char *)buffer_ref(), format, arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const mjz_Str &format, arguments_types &...arguments_arr) {
    int ret =
        sscanf_s((char *)buffer_ref(), format.buffer_ref(), arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const char *format, const arguments_types &...arguments_arr) {
    int ret = sscanf_s((char *)buffer_ref(), format, arguments_arr...);
    return ret;
  }
  template <typename... arguments_types>
  int scanf_s(const mjz_Str &format, const arguments_types &...arguments_arr) {
    int ret =
        sscanf_s((char *)buffer_ref(), format.buffer_ref(), arguments_arr...);
    return ret;
  }

  /*
  template <typename... arguments_types>
   mjz_Str &operator-=(
      arguments_types &...arguments_arr) {
    return operator-=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str operator-(arguments_types &...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator/=(
      arguments_types &...arguments_arr) {
    return operator/=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str operator/(arguments_types &...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator-=(
      const arguments_types &...arguments_arr) {
    return operator-=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str
  operator-(const arguments_types &...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator/=(
      const arguments_types &...arguments_arr) {
    return operator/=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str
  operator/(const arguments_types &...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator-=(
      arguments_types &&...arguments_arr) {
    return operator-=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str
  operator-(arguments_types &&...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator/=(
      arguments_types &&...arguments_arr) {
    return operator/=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str
  operator/(arguments_types &&...arguments_arr) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
   mjz_Str &operator<<(
      arguments_types &...arguments_arr) {
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator<<=(
      arguments_types &...arguments_arr) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator=(
      arguments_types &...arguments_arr) {
    return operator=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator+=(
      arguments_types &...arguments_arr) {
    return operator+=(std::move(mjz_Str(arguments_arr...)));
  }
   */
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_Str &run_code(your_FUNCTION_Type your__function_,
                    arguments_types &...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_Str &run_code(your_FUNCTION_Type your__function_,
                          arguments_types &...arguments_arr) const {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type>
  auto operator()(your_FUNCTION_Type your__function_) {
    return your__function_(*this);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
  /*
  template <typename... arguments_types>
   mjz_Str &operator<<(
      const arguments_types &...arguments_arr) {
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator<<=(
      const arguments_types &...arguments_arr) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  */
  template <typename... arguments_types>
  inline mjz_Str &operator=(const arguments_types &...arguments_arr) {
    operator()();
    concat(arguments_arr...);
    return *this;
  }

  template <typename... arguments_types>
  mjz_Str &operator+=(const arguments_types &...arguments_arr) {
    concat(arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_Str &run_code(your_FUNCTION_Type your__function_,
                    const arguments_types &...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_Str &run_code(your_FUNCTION_Type your__function_,
                          const arguments_types &...arguments_arr) const {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           const arguments_types &...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           const arguments_types &...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  const arguments_types &...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  const arguments_types &...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
  /*
  template <typename... arguments_types>
   mjz_Str &operator<<(
      arguments_types &&...arguments_arr) {
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator<<=(
      arguments_types &&...arguments_arr) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator=(
      arguments_types &&...arguments_arr) {
    return operator=(std::move(mjz_Str(arguments_arr...)));
  }
  template <typename... arguments_types>
   mjz_Str &operator+=(
      arguments_types &&...arguments_arr) {
    return operator+=(std::move(mjz_Str(arguments_arr...)));
  }*/
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_Str &run_code(your_FUNCTION_Type your__function_,
                    arguments_types &&...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_Str &run_code(your_FUNCTION_Type your__function_,
                          arguments_types &&...arguments_arr) const {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &&...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto run_code_and_return(your_FUNCTION_Type your__function_,
                           arguments_types &&...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &&...arguments_arr) {
    return your__function_(*this, arguments_arr...);
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  auto operator()(your_FUNCTION_Type your__function_,
                  arguments_types &&...arguments_arr) const {
    return your__function_(*this, arguments_arr...);
  }
};
/*
  please dont use mjz_str_view with temporary strings
  if the string that it references goes out of scope (delete , ~obj , },free
  ,...)the string_view will have undefined behavior use this obj like a
  std::string_view  not like std::string
*/
class mjz_str_view : public basic_mjz_Str_view {
 protected:
  constexpr char *&buffer_ref(void) { return m_buffer; }
  using basic_mjz_Str_view::buffer_ref;
  using basic_mjz_Str_view::C_str;
  using basic_mjz_Str_view::c_str;
  constexpr mjz_str_view &copy(const basic_mjz_Str_view &s) {
    m_buffer = const_cast<char *>(s.c_str());
    m_length = s.length();
    return *this;
  };
  constexpr inline const char *begin_c_str() const { return c_str(); }
  constexpr inline const char *end_c_str() const { return c_str() + length(); }

 public:
  constexpr inline const char *data() const { return m_buffer; }
  mjz_str_view(const mjz_Str &s) : mjz_str_view(s.c_str(), s.length()) {}
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

  constexpr mjz_str_view &operator=(mjz_Str &&s) { return copy(s); }
  mjz_str_view(mjz_Str &&s) : mjz_str_view(s) {}

  constexpr mjz_str_view(const mjz_str_view &s)
      : mjz_str_view(s.c_str(), s.length()) {}
  constexpr mjz_str_view(mjz_str_view &s)
      : mjz_str_view(s.c_str(), s.length()) {}
  constexpr mjz_str_view &operator=(const mjz_str_view &s) { return copy(s); };
  constexpr mjz_str_view &operator=(mjz_str_view &s) { return copy(s); }
  constexpr inline ~mjz_str_view() = default;
  using iterator_template_CC = iterator_template<const char>;
  using const_iterator = iterator_template_CC;

  constexpr const_iterator begin() const {
    return const_iterator(begin_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  constexpr const_iterator end() const {
    return const_iterator(end_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  typedef std::reverse_iterator<const_iterator> const_rev_iterator;
  constexpr const_rev_iterator rbegin() const {
    return const_rev_iterator(end());
  };
  constexpr const_rev_iterator rend() const {
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
    std::swap(m_length, rhs.m_length);
    std::swap(m_buffer, rhs.m_buffer);
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
  friend StringSumHelper operator+(const mjz_str_view &rhs,
                                   const mjz_str_view &lhs);
  friend StringSumHelper operator+(const basic_mjz_Str_view &rhs,
                                   const basic_mjz_Str_view &lhs);
  friend StringSumHelper operator+(const StringSumHelper &lhs,
                                   const mjz_str_view &rhs);
  friend StringSumHelper operator+(StringSumHelper &&lhs,
                                   const mjz_str_view &rhs);
  friend StringSumHelper operator+(const mjz_str_view &lhs,
                                   const basic_mjz_String &rhs);
  friend StringSumHelper operator+(const mjz_str_view &lhs,
                                   const StringSumHelper &rhs);

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
  mjz_virtual_string_view(const mjz_Str &s)
      : mjz_virtual_string_view(s.c_str(), s.length()) {}
  mjz_virtual_string_view(const char *cstr_, size_t len)
      : mjz_str_view(cstr_, len) {}
  mjz_virtual_string_view(const char *cstr_)
      : mjz_virtual_string_view(cstr_, strlen(cstr_)) {}
  mjz_virtual_string_view() : mjz_virtual_string_view(empty_STRING_C_STR, 0) {}
  mjz_virtual_string_view(mjz_virtual_string_view &&s) =
      default;  // we are string views and not strings
  mjz_virtual_string_view &operator=(mjz_virtual_string_view &&) = default;

  mjz_virtual_string_view &operator=(mjz_Str &&s) {
    copy(s);
    return *this;
  }
  mjz_virtual_string_view(mjz_Str &&s) : mjz_str_view(s) {}
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
class StringSumHelper : public mjz_Str {
 public:
  StringSumHelper(const basic_mjz_String &s)
      : mjz_Str(mjz_Str(s.c_str(), s.length())([](mjz_Str &obj) {
          std::cout << obj << "\n";
          obj.reserve(1);
          return obj;
        })) {}
  StringSumHelper(basic_mjz_String &&s) : StringSumHelper(s) {}
  StringSumHelper(const basic_mjz_Str_view &otr) : mjz_Str(otr) {}
  StringSumHelper(const mjz_str_view &s) : mjz_Str(s) {}
  StringSumHelper(mjz_str_view &&s) : StringSumHelper(s) {}
  StringSumHelper(const mjz_Str &s) : mjz_Str(s) {}
  StringSumHelper(const char *p) : mjz_Str(p) {}
  StringSumHelper(char c) : mjz_Str(c) {}
  StringSumHelper(unsigned char num) : mjz_Str(num) {}
  StringSumHelper(int num) : mjz_Str(num) {}
  StringSumHelper(unsigned int num) : mjz_Str(num) {}
  StringSumHelper(long num) : mjz_Str(num) {}
  StringSumHelper(unsigned long num) : mjz_Str(num) {}
  StringSumHelper(long long num) : mjz_Str(num) {}
  StringSumHelper(unsigned long long num) : mjz_Str(num) {}
  StringSumHelper(float num) : mjz_Str(num) {}
  StringSumHelper(double num) : mjz_Str(num) {}
  // non virtual
  ~StringSumHelper(){};
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
mjz_Str ULL_LL_to_str(size_t value, int radix, bool is_signed,
                      bool force_neg = 0);

mjz_Str &getline(mjz_Str &is, mjz_Str &str, char delim);
mjz_Str &getline(mjz_Str &is, mjz_Str &str);
template <size_t buffer_len = 2048>
std::istream &getline(std::istream &is, mjz_Str &str, char delim) {
  char buffer[buffer_len]{};
  is.getline(buffer, buffer_len, delim);
  uint32_t available_len = mjz_ard::static_str_algo::strlen(buffer);
  str.append(buffer, available_len);
  return is;
}
template <size_t buffer_len = 2048>
std::istream &getline(std::istream &is, mjz_Str &str) {
  return getline<buffer_len>(is, str, '\n');
}
template <typename Type>
inline mjz_Str get_bit_representation(const Type &data) {
  mjz_Str buffer;
  buffer.addto_length(sizeof(Type) * 8);
  static_str_algo::get_bit_representation(
      buffer.C_str(), buffer.length(), &data, sizeof(Type),
      std::endian::little == std::endian::native);
  return buffer;
}

inline mjz_Str operator"" _m_str(const char *p) { return mjz_Str(p); }
inline mjz_Str operator"" _m_str(char c) { return mjz_Str(c); }
inline mjz_Str operator"" _m_str(unsigned long long num) {
  return mjz_Str(num);
}
inline mjz_Str operator"" _m_str(long double num) {
  return mjz_Str((double)num);
}
inline mjz_Str operator"" _m_pstr(char c) {
  return mjz_Str(c).string_do_interpret();
}
inline mjz_Str operator"" _m_pstr(unsigned long long num) {
  return mjz_Str(num).string_do_interpret();
}
inline mjz_Str operator"" _m_pstr(long double num) {
  return mjz_Str((double)num).string_do_interpret();
}
inline mjz_Str operator"" _m_pstr(const char *initilizer) {
  return mjz_Str(initilizer).string_do_interpret();
}
inline mjz_Str operator""_m_str(const char *initilizer, size_t length_) {
  return mjz_Str(initilizer, length_);
}
inline mjz_Str operator"" _m_pstr(const char *initilizer, size_t length_) {
  return mjz_Str(initilizer, length_).string_do_interpret();
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
inline mjz_Str operator"" _s(const char *p) { return mjz_Str(p); }
inline mjz_Str operator"" _s(char c) { return mjz_Str(c); }
inline mjz_Str operator"" _s(unsigned long long num) { return mjz_Str(num); }
inline mjz_Str operator"" _s(long double num) { return mjz_Str((double)num); }
inline mjz_Str operator"" _ps(char c) {
  return mjz_Str(c).string_do_interpret();
}
inline mjz_Str operator"" _ps(unsigned long long num) {
  return mjz_Str(num).string_do_interpret();
}
inline mjz_Str operator"" _ps(long double num) {
  return mjz_Str((double)num).string_do_interpret();
}
inline mjz_Str operator"" _ps(const char *initilizer) {
  return mjz_Str(initilizer).string_do_interpret();
}
inline mjz_Str operator""_s(const char *initilizer, size_t length_) {
  return mjz_Str(initilizer, length_);
}
inline mjz_Str operator"" _ps(const char *initilizer, size_t length_) {
  return mjz_Str(initilizer, length_).string_do_interpret();
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
    Email:  tm507211@ohio.edu

    Date:   July 20, 2015

    Description: Implementation of mjz_vector classes
************************************************************/

}  // namespace mjz_ard

namespace mjz_ard {

/***************************************************************************************
  Vector2  -- 2-D mjz_vector class
***************************************************************************************/
template <class Type>
class Vector2 {
 public:
  Type m_x;
  Type m_y;
  constexpr inline ~Vector2() = default;
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

  inline constexpr Type &x() { return m_x; }
  inline constexpr Type &y() { return m_y; }
  inline constexpr const Type &x() const { return m_x; }
  inline constexpr const Type &y() const { return m_y; }

  inline constexpr Vector2(const Type &s = Type()) : m_x(s), m_y(s) {}
  inline constexpr Vector2(const Type &x, const Type &y) : m_x(x), m_y(y) {}
  inline constexpr Vector2(const Vector2<Type> &v) : m_x(v.m_x), m_y(v.m_y) {}

  inline constexpr Vector2(Vector2 &v) : Vector2((const Vector2<Type> &)v){};

  inline constexpr Vector2(Vector2 &&v) : Vector2(v){};
  inline constexpr bool operator==(const Vector2<Type> &v) const {
    return abs(m_x - v.m_x) < 0.01 && abs(m_y - v.m_y) < 0.01;
  }
  inline constexpr bool operator!=(const Vector2<Type> &v) const {
    return !(operator==(v));
  }

  /**********************************************
    Indexing operator
  **********************************************/
  inline constexpr Type &operator[](int i) { return *(&m_x + i); }
  inline constexpr const Type operator[](int i) const { return *(&m_x + i); }

  /*********************************************
    Non modifying math operators
  *********************************************/
  inline constexpr Vector2<Type> operator-() const {
    return Vector2<Type>(-m_x, -m_y);
  }
  inline constexpr Vector2<Type> operator+(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x + v.m_x, m_y + v.m_y);
  }
  inline constexpr Vector2<Type> operator-(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x - v.m_x, m_y - v.m_y);
  }
  inline constexpr Vector2<Type> operator*(const Type &s) const {
    return Vector2<Type>(m_x * s, m_y * s);
  }
  inline constexpr Vector2<Type> operator*(const Vector2<Type> &v) const {
    return Vector2<Type>(m_x * v.m_x, m_y * v.m_y);
  }
  inline constexpr Vector2<Type> operator/(const Type &s) const {
    return Vector2<Type>(m_x / s, m_y / s);
  }

  /*******************************************
    Modifying Math Operators
  *******************************************/
  inline constexpr Vector2<Type> &operator+=(const Vector2<Type> &v) {
    m_x += v.m_x;
    m_y += v.m_y;
    return *this;
  }
  inline constexpr Vector2<Type> &operator-=(const Vector2<Type> &v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    return *this;
  }
  inline constexpr Vector2<Type> &operator*=(const Type &s) {
    m_x *= s;
    m_y *= s;
    return *this;
  }
  inline constexpr Vector2<Type> &operator*=(const Vector2<Type> &v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    return *this;
  }
  inline constexpr Vector2<Type> &operator/=(const Type &s) {
    m_x /= s;
    m_y /= s;
    return *this;
  }

  /*******************************************
    Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  inline constexpr operator const Type *() const {
    return static_cast<Type *>(&m_x);
  }
  inline constexpr operator Type *() { return static_cast<Type *>(&m_x); }

  /********************************************
    Useful Vector Operations
  ********************************************/
  inline constexpr Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(m_x * m_x + m_y * m_y);
  }
  inline constexpr Type lengthSq() const { return m_x * m_x + m_y * m_y; }
  inline constexpr Vector2<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  inline constexpr Vector2<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  inline constexpr Type dot(const Vector2<Type> &v) const {
    return m_x * v.m_x + m_y * v.m_y;
  }
  inline constexpr Type cross(
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

  friend inline constexpr Vector2<Type> operator*(Type s,
                                                  const Vector2<Type> &v) {
    return Vector2<Type>(s * v.m_x, s * v.m_y);
  }

  /********************************************************
   Basic Trig functions of angle between vectors
  ********************************************************/

  friend inline constexpr Type cos(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return dot(v1, v2) / v1.length() / v2.length();
  }

  friend inline constexpr Type sin(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return cross(v1, v2) / v1.length() / v2.length();
  }

  friend inline constexpr Type tan(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return sin(v1, v2) / cos(v1, v2);
  }

  friend inline constexpr Type angle(const Vector2<Type> &v1,
                                     const Vector2<Type> &v2) {
    return std::acos(cos(v1, v2));
  }

  friend inline constexpr Type dot(const Vector2<Type> &v1,
                                   const Vector2<Type> &v2) {
    return v1.dot(v2);
  }

  friend inline constexpr Type cross(const Vector2<Type> &v1,
                                     const Vector2<Type> &v2) {
    return v1.cross(v2);
  }
};

/*********************************************************************************
 Vector3 -- 3D mjz_vector
*********************************************************************************/

template <class Type>
class Vector3 {
 public:
  Type m_x;
  Type m_y;
  Type m_z;
  constexpr inline ~Vector3() = default;

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

  inline constexpr Type &x() { return m_x; }
  inline constexpr Type &y() { return m_y; }
  inline constexpr Type &z() { return m_z; }
  inline constexpr const Type &x() const { return m_x; }
  inline constexpr const Type &y() const { return m_y; }
  inline constexpr const Type &z() const { return m_z; }

  inline constexpr Vector3(const Type &s = Type()) : m_x(s), m_y(s), m_z(s) {}
  inline constexpr Vector3(const Type &x, const Type &y, const Type &z)
      : m_x(x), m_y(y), m_z(z) {}
  inline constexpr Vector3(const Vector2<Type> &v, const Type &s = Type())
      : m_x(v.m_x), m_y(v.m_y), m_z(s) {}
  inline constexpr Vector3(const Vector3<Type> &v)
      : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) {}

  inline constexpr Vector3(Vector3 &v) : Vector3((const Vector3<Type> &)v){};

  inline constexpr Vector3(Vector3 &&v) : Vector3(v){};

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

  inline constexpr bool operator==(const Vector3<Type> &v) const {
    return abs(m_x - v.m_x) < 0.01 && abs(m_y - v.m_y) < 0.01 &&
           abs(m_z - v.m_z) < 0.01;
  }
  inline constexpr bool operator!=(const Vector3<Type> &v) const {
    return !operator==(v);
  }

  /**********************************************
    Indexing operator
  **********************************************/
  inline constexpr Type &operator[](int i) { return *(&m_x + i); }
  const Type operator[](int i) const { return *(&m_x + i); }

  /*********************************************
    Non modifying math operators
  *********************************************/
  inline constexpr Vector3<Type> operator-() const {
    return Vector3<Type>(-m_x, -m_y, -m_z);
  }
  inline constexpr Vector3<Type> operator+(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
  }
  inline constexpr Vector3<Type> operator-(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
  }
  inline constexpr Vector3<Type> operator*(const Type &s) const {
    return Vector3<Type>(m_x * s, m_y * s, m_z * s);
  }
  inline constexpr Vector3<Type> operator*(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
  }
  inline constexpr Vector3<Type> operator/(const Type &s) const {
    return Vector3<Type>(m_x / s, m_y / s, m_z / s);
  }
  inline constexpr Vector3<Type> operator/(const Vector3<Type> &v) const {
    return Vector3<Type>(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
  }

  /*******************************************
    Modifying Math Operators
  *******************************************/
  inline constexpr Vector3<Type> &operator+=(const Vector3<Type> &v) {
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    return *this;
  }
  inline constexpr Vector3<Type> &operator-=(const Vector3<Type> &v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_y -= v.m_z;
    return *this;
  }
  inline constexpr Vector3<Type> &operator*=(const Type &s) {
    m_x *= s;
    m_y *= s;
    m_z *= s;
    return *this;
  }
  inline constexpr Vector3<Type> &operator*=(const Vector3<Type> &v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
    return *this;
  }
  inline constexpr Vector3<Type> &operator/=(const Vector3<Type> &v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    m_z /= v.m_z;
    return *this;
  }
  inline constexpr Vector3<Type> &operator/=(const Type &s) {
    m_x /= s;
    m_y /= s;
    m_z /= s;
    return *this;
  }

  /*******************************************
    Cast to Type* (lets you use vec2 as Type array)
  *******************************************/
  inline constexpr operator const Type *() const {
    return static_cast<Type *>(&m_x);
  }
  inline constexpr operator Type *() { return static_cast<Type *>(&m_x); }

  /********************************************
    Useful Vector Operations
  ********************************************/
  inline constexpr Type length() const {
    return 1 / static_str_algo::Q_rsqrt_unsafe(lengthSq());
  }
  inline constexpr Type lengthSq() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
  }
  inline constexpr Vector3<Type> &normalize() {
    return operator*=((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  inline constexpr Vector3<Type> unit() const {
    return operator*((Type)static_str_algo::Q_rsqrt_unsafe(lengthSq()));
  }
  inline constexpr Type dot(const Vector3<Type> &v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
  }

  inline constexpr Vector3<Type> cross_with_this(
      const Vector3<Type> &v) { /* NOTE this function modifies the mjz_vector
                                unlike 2D and non-member versions */
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
  inline constexpr friend Vector3<Type> operator*(Type s,
                                                  const Vector3<Type> &v) {
    return Vector3<Type>(s * v.m_x, s * v.m_y, s * v.m_z);
  }

  /********************************************************
   Basic Trig functions of angle between vectors
  ********************************************************/
  inline constexpr friend Type cos(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return dot(v1, v2) / v1.length() / v2.length();
  }
  inline constexpr friend Type sin(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return cross(v1, v2).length() / v1.length() / v2.length();
  }

  inline constexpr friend Type tan(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return sin(v1, v2) / cos(v1, v2);
  }

  inline constexpr friend Type angle(const Vector3<Type> &v1,
                                     const Vector3<Type> &v2) {
    return std::acos(cos(v1, v2));
  }

  inline constexpr friend Type dot(const Vector3<Type> &v1,
                                   const Vector3<Type> &v2) {
    return v1.dot(v2);
  }

  inline constexpr friend Vector3<Type> cross(const Vector3<Type> &v1,
                                              const Vector3<Type> &v2) {
    return Vector3<Type>(v1.m_y * v2.m_z - v1.m_z * v2.m_y,
                         v1.m_z * v2.m_x - v1.m_x * v2.m_z,
                         v1.m_x * v2.m_y - v1.m_y * v2.m_x);
  }
};

/******************************************************************************************
    Author: Charlie Murphy
    Email:  tm507211@ohio.edu

    Date:   Augost 7, 2015

    Description: Defines point class.
******************************************************************************************/

template <class Type>
class Point3D {
 public:
  Type m_x;
  Type m_y;
  Type m_z;
  Type m_w;

  inline constexpr Point3D(const Type &s = Type())
      : m_x(s), m_y(s), m_z(s), m_w(s) {}
  inline constexpr Point3D(const Type &x, const Type &y, const Type &z,
                           const Type &w)
      : m_x(x), m_y(y), m_z(z), m_w(w) {}
  inline constexpr Point3D(const Point3D<Type> &p)
      : m_x(p.m_x), m_y(p.m_y), m_z(p.m_z), m_w(p.m_w) {}
  inline constexpr Point3D(const Point3D<Type> &p, const Vector3<Type> &v)
      : m_x(p.m_x + v.m_x),
        m_y(p.m_y + v.m_y),
        m_z(p.m_z + v.m_z),
        m_w(p.m_w) {}

  inline constexpr Point3D(Point3D &p) : Point3D((const Point3D<Type> &)p){};

  inline constexpr Point3D(Point3D &&p) : Point3D(p){};

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
//  mjz_vector.h
//  Vector
//
//  Created by Emilis Baliukonis on 05/05/2019.
//  Copyright © 2019 Emilis Baliukonis. All rights reserved.
//

namespace short_string_names {
typedef mjz_ard::mjz_Str Str;
typedef mjz_ard::mjz_Str str;
typedef mjz_ard::mjz_Str s;
typedef mjz_ard::mjz_Str S;
typedef mjz_ard::StringSumHelper StrSH;
typedef mjz_ard::mjz_str_view sv;
typedef mjz_ard::mjz_str_view strv;
typedef mjz_ard::mjz_str_view mstrview;
typedef mjz_ard::mjz_str_view mstrv;
typedef mjz_ard::Vector3<float> mvf3;
typedef mjz_ard::Vector3<float> Vectorf3;

typedef mjz_ard::Vector2<float> mvf2;
typedef mjz_ard::Vector2<float> Vectorf2;

}  // namespace short_string_names

}  // namespace mjz_ard
namespace have_mjz_ard_removed {
typedef mjz_ard::mjz_Str mjz_Str;
typedef mjz_ard::mjz_Str mjz_str;
typedef mjz_ard::malloc_wrapper malloc_wrpr;
typedef mjz_ard::malloc_wrapper mlc_wrp;
typedef std::string string;
typedef mjz_ard::hash_sha256 hash_sha_512;
typedef mjz_ard::StringSumHelper mjz_StringSumHelper;
typedef mjz_ard::mjz_str_view mjz_str_view;
typedef mjz_ard::mjz_str_view mstrview;
typedef mjz_ard::mjz_str_view mstrv;
typedef mjz_ard::Vector3<float> mjz_Vectorf3;
typedef mjz_ard::Vector2<float> mjz_Vectorf2;
}  // namespace have_mjz_ard_removed
namespace std {
template <>
struct hash<mjz_ard::basic_mjz_Str_view> {
  size_t operator()(const mjz_ard::basic_mjz_Str_view &k) const {
    return mjz_ard::SHA1_CTX::SHA_1(k.c_str(), k.length()).get_as_64bit_hash();
  }
};  // namespace std::hash

template <>
struct hash<mjz_ard::mjz_Str> {
  size_t operator()(const mjz_ard::mjz_Str &k) const {
    return mjz_ard::SHA1_CTX::SHA_1(k.c_str(), k.length()).get_as_64bit_hash();
  }
};  // namespace std::hash

template <>
struct hash<mjz_ard::mjz_str_view> {
  size_t operator()(const mjz_ard::mjz_str_view &k) const {
    return mjz_ard::SHA1_CTX::SHA_1(k.data(), k.length()).get_as_64bit_hash();
  }
};  // namespace std::hash
template <>
struct hash<mjz_ard::mjz_virtual_string_view> {
  size_t operator()(const mjz_ard::mjz_virtual_string_view &k) const {
    return mjz_ard::SHA1_CTX::SHA_1(k.data(), k.length()).get_as_64bit_hash();
  }
};  // namespace std::hash

template <typename Type>
inline mjz_ard::Vector3<Type> sqrt(mjz_ard::Vector3<Type> v) {
  return mjz_ard::Vector3<Type>(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
}
}  // namespace std
#undef NO_IGNORE_CHAR
#endif  // __mjz_ard_STRINGS__
#endif  // __cplusplus
