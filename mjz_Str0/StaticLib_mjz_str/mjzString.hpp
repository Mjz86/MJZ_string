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
#define log_mjz_str(Function, ...) (Function((const char *)__VA_ARGS__))
inline uint32_t usteejtgk_millis() { return millis(); }
class __FlashStringHelper;

namespace mjz_ard {
extern uint32_t num_allocations;

template <class Type>
using mjz_get_value_Type = typename Type::value_type;

template <class std_allocator, typename Type,
          bool DO_std_reallocator_warper_LOG = false>
struct std_reallocator_warper {
  using my_value_Type_t = Type;
  using value_type = my_value_Type_t;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = my_value_Type_t;
  using const_reference = const my_value_Type_t &;
  using size_type = size_t;
  using propagate_on_container_move_assignment = std::true_type;
  std_reallocator_warper() = default;
  ~std_reallocator_warper() = default;

  template <class T>
  constexpr std_reallocator_warper(
      const std_reallocator_warper<std_allocator, T> &) noexcept {}

  [[nodiscard]] my_value_Type_t *allocate(size_t n) {
    return reallocate((my_value_Type_t *)0, n);
  }
  [[nodiscard]] my_value_Type_t *allocate(size_t n, const void *hint) {
    return reallocate((my_value_Type_t *)hint, n);
  }

  void deallocate(my_value_Type_t *p, size_t n) noexcept { free(p); }

 protected:
  [[nodiscard]] void *allocate_raw(size_t number_of_bytes) {
    return reallocate_raw((void *)0, number_of_bytes);
  }
  [[nodiscard]] void *allocate_raw(size_t n, const void *hint) {
    return reallocate_raw((void *)hint, n);
  }
  void deallocate_raw(void *p, size_t number_of_bytes) noexcept { free(p); }

 private:
  [[nodiscard]] my_value_Type_t *reallocate(my_value_Type_t *ptr, size_t n) {
    // if (n > std::numeric_limits<std::size_t>::max() /
    // sizeof(my_value_Type_t)) throw std::bad_array_new_length();

    if (auto p = static_cast<my_value_Type_t *>(
            realloc(ptr, n * sizeof(my_value_Type_t)))) {
      return p;
    }
    // throw std::bad_alloc();
    return 0;
  }
  [[nodiscard]] void *reallocate_raw(void *ptr, size_t number_of_bytes) {
    // if (n > std::numeric_limits<std::size_t>::max() /
    // sizeof(my_value_Type_t)) throw std::bad_array_new_length();
    if (auto p = static_cast<void *>(realloc(ptr, number_of_bytes))) {
      return p;
    }
    // throw std::bad_alloc();
    return 0;
  }
  size_t &get_size_of_mem(void *ptr) {
    static size_t dummy{};
    if (ptr == 0) {
      dummy = 0;
      return dummy;
    }
    return *(size_t *)get_real_mem(ptr);
  }
  void *get_fake_mem(void *ptr) {
    if (ptr == 0) return ptr;
    return (void *)((size_t *)ptr + 1);
  }
  void *get_real_mem(void *ptr) {
    if (ptr == 0) return ptr;
    return (void *)((size_t *)ptr - 1);
  }
  void free_log(void *p, size_t n) {
    if constexpr (DO_std_reallocator_warper_LOG) {
      std::cout << "\nnum_allocations: " << num_allocations << " freed :" << p
                << " with len :" << n << '\n';
    }
  }
  void *alloc_log(void *p, size_t n) {
    if constexpr (DO_std_reallocator_warper_LOG) {
      std::cout << "\nnum_allocations: " << num_allocations
                << " allocated :" << p << " with len :" << n << '\n';
    }
    return p;
  }
  void *realloc_log(void *p, size_t n) {
    if constexpr (DO_std_reallocator_warper_LOG) {
      std::cout << "\nnum_allocations: " << num_allocations
                << " reallocated :" << p << " with len :" << n << '\n';
    }
    return p;
  }
  void *realloc(void *ptr, size_t size) {
    std::allocator_traits<std_allocator> al;
    std_allocator all;
    // num_allocations++;
    realloc_log(get_real_mem(ptr), get_size_of_mem(ptr));
    void *ptr2 = get_fake_mem(
        al.allocate(all, sizeof(size_t) + size, get_real_mem(ptr)));
    get_size_of_mem(ptr2) = size + sizeof(size_t);
    alloc_log(get_real_mem(ptr2), get_size_of_mem(ptr2));
    return ptr2;
  }
  void free(void *ptr) {
    std::allocator_traits<std_allocator> al;
    std_allocator all;
    free_log(get_real_mem(ptr), get_size_of_mem(ptr));
    // num_allocations--;
    al.deallocate(all, (uint8_t *)get_real_mem(ptr), get_size_of_mem(ptr));
  }
};
template <class T1, class T2, class U>
inline constexpr bool operator==(const std_reallocator_warper<U, T1> &,
                                 const std_reallocator_warper<U, T2> &) {
  return true;
}

template <class T1, class T2, class U>
inline constexpr bool operator!=(const std_reallocator_warper<U, T1> &,
                                 const std_reallocator_warper<U, T2> &) {
  return false;
}
template <class T1, class T2, class U1, class U2>
inline constexpr bool operator==(const std_reallocator_warper<U1, T1> &,
                                 const std_reallocator_warper<U2, T2> &) {
  return U1() == U2();
}

template <class T1, class T2, class U1, class U2>
inline constexpr bool operator!=(const std_reallocator_warper<U1, T1> &,
                                 const std_reallocator_warper<U2, T2> &) {
  return U1() != U2();
}

template <class Type>
struct basic_mjz_allocator
    : std_reallocator_warper<std::allocator<uint8_t>, Type> {
  basic_mjz_allocator() = default;
  ~basic_mjz_allocator() = default;
  template <class T>
  constexpr basic_mjz_allocator(const basic_mjz_allocator<T> &) noexcept {}
};

template <class Type, class U>
inline constexpr bool operator==(const basic_mjz_allocator<Type> &,
                                 const basic_mjz_allocator<U> &) {
  return true;
}

template <class Type, class U>
inline constexpr bool operator!=(const basic_mjz_allocator<Type> &,
                                 const basic_mjz_allocator<U> &) {
  return false;
}

template <typename Type>
class mjz_obj_constructor {
 public:
  template <typename... args_t>
  inline Type *construct_at(Type *dest, args_t &&...args) noexcept {
    Type *ptr{};
    try {
      ptr = (new (dest) Type(std::forward(args)...));
    } catch (...) {
    }
    return ptr;
  }
  template <typename... args_t>
  inline [[nodiscard]] Type obj_constructor(args_t &&...args) {
    return Type(std::forward(args)...);
  }
  template <class T, class... Args>
  static inline void construct(mjz_obj_constructor &a, T *p, Args &&...args) {
    a.construct_at(p, std::forward(args)...);
  }
};
template <typename Type>
class mjz_obj_destructor {
 public:
  inline bool destroy_at(Type *ptr) noexcept {
    try {
      ptr->~Type();
    } catch (...) {
      return false;
    }
    return true;
  }
  inline bool destroy_at(Type &ptr) noexcept {
    try {
      ptr.~Type();
    } catch (...) {
      return false;
    }
    return true;
  }
  static inline void destroy(mjz_obj_destructor &a, Type *p) {
    a.destroy_at(p);
  }
  template <class ForwardIt, class Size>
  ForwardIt destroy_n(ForwardIt first, Size n) {
    for (; n > 0; (void)++first, --n) destroy_at(std::addressof(*first));
    return first;
  }
};

template <typename Type, class my_destructor = mjz_obj_destructor<Type>,
          class my_constructor = mjz_obj_constructor<Type>,
          class my_reallocator = basic_mjz_allocator<Type>>

struct mjz_temp_type_allocator_warpper_t : protected my_destructor,
                                           protected my_constructor,
                                           protected my_reallocator {
 public:
  using value_type = Type;
  constexpr mjz_temp_type_allocator_warpper_t(){};
  constexpr ~mjz_temp_type_allocator_warpper_t(){};

  template <class U>
  constexpr mjz_temp_type_allocator_warpper_t(
      const mjz_temp_type_allocator_warpper_t<U> &) noexcept {}

  [[nodiscard]] Type *allocate(size_t n) { return allocate(n, (Type *)0); }
  static inline [[nodiscard]] Type *allocate(
      mjz_temp_type_allocator_warpper_t &a, size_t n, const void *hint) {
    return (Type *)a.allocate(n, hint);
  }
  static inline [[nodiscard]] Type *allocate(
      mjz_temp_type_allocator_warpper_t &a, size_t n) {
    return (Type *)a.allocate(n);
  }
  static inline void deallocate(mjz_temp_type_allocator_warpper_t &a, Type *p,
                                size_t n) noexcept {
    a.deallocate(p, n);
  }
  void deallocate(Type *p, size_t n) noexcept { deallocate_pv(p, n); }
  inline [[nodiscard]] Type *allocate(size_t n, const void *hint) {
    return allocate_pv(n, hint);
  }

  template <typename... args_t>
  inline Type *construct_arr_at(Type *dest, size_t n, bool in_reveres,
                                args_t... args) {
    if (in_reveres) {
      Type *ptr_end = dest - 1;
      Type *ptr = dest + n;
      while ((--ptr) > ptr_end) {
        my_constructor::construct_at(ptr, args...);
      }
      return dest;
    } else {
      Type *ptr = dest - 1;
      Type *ptr_end = dest + n;

      while ((++ptr) < ptr_end) {
        my_constructor::construct_at(ptr, args...);
      }
      return dest;
    }
  }
  inline Type *construct_arr_at(Type *dest, size_t n, bool in_reveres = 0) {
    if (in_reveres) {
      Type *ptr_end = dest - 1;
      Type *ptr = dest + n;
      while ((--ptr) > ptr_end) {
        my_constructor::construct_at(ptr);
      }
      return dest;
    } else {
      Type *ptr = dest - 1;
      Type *ptr_end = dest + n;

      while ((++ptr) < ptr_end) {
        my_constructor::construct_at(ptr);
      }
      return dest;
    }
  }

  template <typename... args_t>
  inline [[nodiscard]] Type *allocate_obj(args_t &&...args) {
    // new Type(std::forward(args)...);
    Type *ptr = allocate_pv(1, 0);

    *get_real_array_ptr<size_t>(ptr) = 1;
    return my_constructor::construct_at(ptr, std::forward(args)...);
  }
  template <typename... args_t>
  inline [[nodiscard]] Type *allocate_obj_array(size_t len, bool in_reveres,
                                                args_t &&...args) {
    // new Type(std::forward(args)...)[len];
    Type *ptr = allocate_pv(len, 0);
    *get_real_array_ptr<size_t>(ptr) = len;
    return construct_arr_at(ptr, len, in_reveres, std::forward(args)...);
  }
  template <typename... args_t>
  inline [[nodiscard]] Type *allocate_obj_array(size_t len,
                                                bool in_reveres = 0) {
    Type *ptr = allocate_pv(len, 0);
    *get_real_array_ptr<size_t>(ptr) = len;
    return construct_arr_at(ptr, len, in_reveres);
  }
  constexpr inline size_t get_number_of_obj_in_array(Type *ptr) {
    return *get_real_array_ptr<size_t>(ptr);
  }
  constexpr inline size_t size_of_array_with(size_t len) {
    return sizeof(size_t) + size_of_type() * len;
  }
  constexpr inline size_t size_of_array_with(Type *ptr) {
    return sizeof(size_t) + size_of_type() * get_number_of_obj_in_array(ptr);
  }
  constexpr inline size_t size_of_type() { return sizeof(Type); }

  inline bool obj_destructor_arr(Type *arr, size_t n, bool in_reveres = 1) {
    bool was_successful{1};
    if (in_reveres) {
      Type *ptr = arr + n;
      Type *ptr_end = arr - 1;
      while ((--ptr) > ptr_end) {
        was_successful &= my_destructor::destroy_at(ptr);
      }
    } else {
      Type *ptr = arr - 1;
      Type *ptr_end = arr + n;
      while ((++ptr) < ptr_end) {
        was_successful &= my_destructor::destroy_at(ptr);
      }
    }
    return was_successful;
  }

  inline bool deallocate_obj(Type *ptr) { return deallocate_obj_array(ptr); }
  inline bool deallocate_obj_array(Type *ptr, bool in_reveres = 1) {
    // delete[] dest;
    bool was_successful =
        obj_destructor_arr(ptr, get_number_of_obj_in_array(ptr), in_reveres);
    deallocate_pv(ptr, size_of_array_with(ptr), 1);
    return was_successful;
  }

 private:
  template <typename T_as, typename T_from>
  constexpr inline T_as *get_real_array_ptr(T_from *fake) {
    return (T_as *)(((size_t *)fake) - 1);
  }
  template <typename T_as, typename T_from>
  constexpr inline T_as *get_fake_array_ptr(T_from *real) {
    return (T_as *)(((size_t *)real) + 1);
  }

  inline [[nodiscard]] Type *allocate_pv(size_t n, const void *hint,
                                         bool raw = 0) {
    return get_fake_array_ptr<Type>(
        my_reallocator::allocate_raw(raw ? n : size_of_array_with(n)));
  }
  void deallocate_pv(Type *p, size_t n, bool raw = 0) noexcept {
    my_reallocator::deallocate_raw(get_real_array_ptr<Type>(p),
                                   raw ? n : size_of_array_with(n));
  }

  template <typename _Type, class _destructor, class _constructor,
            class _reallocator>
  friend bool operator==(
      mjz_temp_type_allocator_warpper_t<Type, my_destructor, my_constructor,
                                        my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _destructor, _constructor,
                                        _reallocator>) {
    return false;
  }
  template <typename _Type, class _destructor, class _constructor,
            class _reallocator>
  friend bool operator!=(
      mjz_temp_type_allocator_warpper_t<Type, my_destructor, my_constructor,
                                        my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _destructor, _constructor,
                                        _reallocator>) {
    return true;
  }
  template <typename _Type, class _destructor, class _constructor>
  friend bool operator==(
      mjz_temp_type_allocator_warpper_t<Type, my_destructor, my_constructor,
                                        my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _destructor, _constructor,
                                        my_reallocator>) {
    return true;
  }
  template <typename _Type, class _destructor, class _constructor>
  friend bool operator!=(
      mjz_temp_type_allocator_warpper_t<Type, my_destructor, my_constructor,
                                        my_reallocator>,
      mjz_temp_type_allocator_warpper_t<_Type, _destructor, _constructor,
                                        my_reallocator>) {
    return false;
  }
};

template <typename Type, class my_reallocator = basic_mjz_allocator<Type>>
using mjz_allocator_warpper_r_t = mjz_temp_type_allocator_warpper_t<
    Type, mjz_obj_destructor<Type>, mjz_obj_constructor<Type>, my_reallocator>;
template <class Type>
struct mjz_allocator_warpper : mjz_allocator_warpper_r_t<Type> {
  mjz_allocator_warpper() = default;
  ~mjz_allocator_warpper() = default;
  template <class T>
  constexpr mjz_allocator_warpper(const mjz_allocator_warpper<T> &) noexcept {}
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

char *utoa(uint32_t value, char *buffer, int radix);
char *lltoa(long long value, char *buffer, int radix);
char *ltoa(long value, char *buffer, int radix);
char *itoa(int value, char *buffer, int radix);
char *dtostrf(double __val, signed char __width, unsigned char __prec,
              char *__s);
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

template <typename T>
class StringSumHelper_t;

template <typename T>
class mjz_str_t;
typedef mjz_str_t<mjz_allocator_warpper<char>> mjz_Str;

typedef StringSumHelper_t<mjz_allocator_warpper<char>> StringSumHelper;

template <typename T = mjz_allocator_warpper<char>>
class extended_mjz_str_t;

typedef extended_mjz_str_t<mjz_allocator_warpper<char>> extended_mjz_Str;

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
    while (*str_i++)
      ;
    return str_i - str;
  }
  constexpr static void *strncpy(void *dest, const char *src, size_t len) {
    return memcpy(dest, src, min(strlen(src) + 1, len));
  }
  constexpr static void *strcpy(void *dest, const char *src) {
    return memcpy(dest, src, strlen(src) + 1);
  }
  constexpr static inline const char *strchr(const char *str, size_t len_,
                                             char ch) {
    const char *str_end = str + len_;
    const char *ptr{str};
    while (ptr < str_end && (*ptr != ch)) ++ptr;
    if (ptr != str) return ptr;
    return NULL;
  }
  constexpr static inline const char *strrchr(const char *str, size_t len_,
                                              char ch) {
    const char *str_end{str - 1};
    const char *ptr = str + len_ - 1;
    while (ptr < str_end && (*ptr != ch)) --ptr;
    if (ptr != str) return ptr;
    return NULL;
  }
  constexpr static inline const char *strstr(const char *const haystack_,
                                             const size_t haystack_len,
                                             const char *const needle_,
                                             const size_t needle_len) {
    const char *a{haystack_}, *b{needle_}, *ptr = haystack_;
    if (*b == 0) {
      return ptr;
    }
    for (; *ptr != 0; ptr += 1) {
      if (*ptr != *b) {
        continue;
      }
      a = ptr;
      while (1) {
        if (*b == 0) {
          return ptr;
        }
        if (*a++ != *b++) {
          break;
        }
      }
      b = needle_;
    }
    return NULL;
  }
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
  constexpr void swap(Type &Left, Type &Right) {
    Type _Tmp = std::move(Left);
    Left = std::move(Right);
    Right = std::move(_Tmp);
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
      char data_c[8];
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
  enum class cpu_endian : uint8_t { little = 0, big = 1 };
  inline constexpr static cpu_endian get_cpu_endian() {
    union {
      int x = 1;
      uint8_t a[4];
    };
    return a[0] ? cpu_endian::little : cpu_endian::big;
  }
  static cpu_endian my_endian;
  inline constexpr static char *get_bit_representation(
      char *buffer, size_t buffer_len, const void *data_ptr, size_t len,
      bool in_reverse = (my_endian == cpu_endian::little)) {
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
    return get_bit_representation(buffer, sizeof(Type) * 8, &data,
                                  sizeof(Type));
  }

 public:
  class stack_str_buf {
    mutable uint8_t STR_is_in_stack{};

   public:
    char stack_buffer[stack_buffer_size + 1]{};  // ptr you're searching for
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
  static inline constexpr Type *do_operation(Type *buffer_,
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
// The ptr class
bool is_blank_characteres_default(char);
char char_to_char_for_reinterpret_fnc_ptr_default(char);
bool is_forbiden_character_default(char);

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

 private:
  // mjz_Str_DATA_storage_cls() = default;
};
enum Dealocation_state : uint8_t {
  dont_deallocate_on_free = MJZ_logic_BIT(0),
  is_moved = MJZ_logic_BIT(1)
};

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
  // : iterator_template((Type *)(list.begin()), (size_t)list.size()) {}
  // this is bad for class types becuse ~list => ~Type => } code... => use
  // after free

  constexpr void throw_if_bad(Type *_iterator) const {
    if (_iterator == (Type *)-1) {
      _iterator = m_iterator;
    }

    if ((m_iterator_begin_ptr <= _iterator) &&
        (_iterator <= m_iterator_end_ptr)) {
      return;
    }

    throw std::exception("bad ptr access : iterator_template::throw_if_bad ");
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
  inline ~iterator_template() { m_iterator = 0; }
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

template <typename Type, bool construct_obj_on_constructor = true,
          class ptr_alloc_warpper = mjz_allocator_warpper<Type>>
struct heap_obj_warper {
 public:
  static constexpr size_t size = sizeof(Type);

 protected:
  uint8_t m_data[size]{};
  bool m_Has_data{};

 private:
  template <typename... args_t>
  static inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                         args_t &&...args) {
    if (plc_has_obj) destroy_at_place(place);
    return ptr_alloc_warpper().construct_at(place, std::forward(args)...);
  }
  static inline void destroy_at_place(Type *place) {
    ptr_alloc_warpper().destroy_at(place);
  }

  template <typename... args_t>
  inline void construct(args_t &&...args) {
    if (pointer_to_unsafe_data() == construct_in_place(pointer_to_unsafe_data(),
                                                       m_Has_data,
                                                       std::forward(args)...)) {
      m_Has_data = 1;
    } else {
      m_Has_data = 0;
    }
  }
  inline void destroy() {
    if (m_Has_data) destroy_at_place(pointer_to_unsafe_data());
    m_Has_data = 0;
  }
  constexpr inline Type *init_with_unsafe_data(bool initialized) {
    m_Has_data = initialized;
    return (Type *)(m_data);
  }
  inline Type *move_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj,
                                std::move(*pointer_to_unsafe_data()));
    m_Has_data = 0;
    return 0;
  }
  inline Type *copy_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj, *pointer_to_unsafe_data());
    return 0;
  }

 public:
  constexpr inline heap_obj_warper() {
    if constexpr (construct_obj_on_constructor) {
      construct();
    }
  };
  inline ~heap_obj_warper() { de_init(); }
  inline heap_obj_warper(heap_obj_warper &&h_obj_w) {
    if (h_obj_w.has_data()) construct(std::move(h_obj_w.operator*()));
  }
  inline heap_obj_warper(const heap_obj_warper &h_obj_w) {
    if (h_obj_w.has_data()) construct(h_obj_w.operator*());
  }
  inline heap_obj_warper(heap_obj_warper &h_obj_w) {
    if (h_obj_w.has_data()) construct(h_obj_w.operator*());
  }
  inline heap_obj_warper(const Type &obj) { construct(obj); }
  inline heap_obj_warper(Type &obj) { construct(obj); }
  inline heap_obj_warper(Type &&obj) { construct(std::move(obj)); }

 public:
  inline heap_obj_warper &operator=(Type &&obj) {
    construct(std::move(obj));
    return *this;
  }
  inline heap_obj_warper &operator=(Type &obj) {
    construct(obj);
    return *this;
  }
  inline heap_obj_warper &operator=(const Type &obj) {
    construct(obj);
    return *this;
  }
  inline heap_obj_warper &operator=(heap_obj_warper &&h_obj_w) {
    if (!!h_obj_w) operator=(std::move(h_obj_w.operator*()));
    return *this;
  }
  inline heap_obj_warper &operator=(const heap_obj_warper &h_obj_w) {
    if (!!h_obj_w) operator=(h_obj_w.operator*());
    return *this;
  }
  inline heap_obj_warper &operator=(heap_obj_warper &h_obj_w) {
    if (!!h_obj_w) operator=(h_obj_w.operator*());
    return *this;
  }

 public:
  inline void init(const heap_obj_warper &obj) { operator=(obj); }
  inline void init(heap_obj_warper &obj) { operator=(obj); }
  inline void init(heap_obj_warper &&obj) { operator=(std::move(obj)); }
  template <typename arg_T, typename Type>
  inline void init(std::initializer_list<arg_T> list) {
    construct(list);
  }
  template <typename arg_T>
  inline void init(iterator_template<arg_T> list) {
    construct(list);
  }

  template <typename... arguments_types>
  inline void init(arguments_types &&...args) {
    construct(std::forward(args)...);
  }

 public:
  inline void de_init() { destroy(); }

  inline void de_init(int fill_VAL) {
    destroy();
    static_str_algo::memset(m_data, fill_VAL, size);
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
  inline Type *pointer_to_unsafe_data_for_unsafe_placement_new(Type *ptr) {
    destroy();
    return pointer_to_unsafe_data();
  }
  constexpr inline Type *init_with_unsafe_placement_new(
      Type *ptr) {  // placement new "new (ptr) Type();"
    return init_with_unsafe_data(ptr == pointer_to_unsafe_data());
  }

 public:
  inline Type &if_no_obj_then_create() {
    if (!m_Has_data) construct();
    return *pointer_to_unsafe_data();
  }

 public:
  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() {
    return (uint8_t *)(m_data);
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const {
    return (uint8_t *)(m_data);
  }
  constexpr inline Type *pointer_to_unsafe_data() { return (Type *)(m_data); }
  constexpr inline const Type *pointer_to_unsafe_data() const {
    return (const Type *const)(m_data);
  }

 public:
  constexpr inline const Type *pointer_to_data() const {
    if (!m_Has_data)
      throw std::exception(
          "mjz_ard::heap_obj_warper::pointer_to_data bad access");
    return pointer_to_unsafe_data();
  }
  constexpr inline Type *pointer_to_data() {
    if (!m_Has_data)
      throw std::exception(
          "mjz_ard::heap_obj_warper::pointer_to_data bad access");
    return pointer_to_unsafe_data();
  }

 public:
  constexpr inline Type *operator->() { return pointer_to_data(); }
  template <typename my_type>
  inline auto operator->*(my_type my_var) {
    return pointer_to_data()->*my_var;
  }
  constexpr inline Type &operator*() { return *operator->(); }
  inline heap_obj_warper *operator&() { return this; }              // &obj
  inline Type *operator&(int) { return pointer_to_data(); }         // obj&0
  inline const heap_obj_warper *operator&() const { return this; }  // &obj
  inline const Type *operator&(int) const {
    return pointer_to_data();
  }  // obj&0
  constexpr inline const Type *operator->() const { return pointer_to_data(); }
  template <typename my_type>
  inline auto operator->*(my_type my_var) const {
    return pointer_to_data()->*my_var;
  }
  constexpr inline const Type &operator*() const { return *operator->(); }

 public:
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

#if 0
 inline bool operator<=>(const heap_obj_warper &other) const {
 return operator() <=> other.operator();
 }
#endif  // ! Arduino
  constexpr inline bool has_data() { return m_Has_data; }
  bool operator!() const { return !m_Has_data; }
  explicit operator bool() const { return m_Has_data; }
  inline operator Type &() { return *pointer_to_data(); }
  inline operator const Type &() const { return *pointer_to_data(); }

 public:
  inline Type *copy_to(Type *dest, bool dest_has_obj) {
    return copy_to_place(dest, dest_has_obj);
  }
  inline Type *move_to(Type *dest, bool dest_has_obj) {
    return move_to_place(dest, dest_has_obj);
  }
  inline Type &copy_to(Type &dest, bool dest_has_obj) {
    return *copy_to(&dest, dest_has_obj);
  }
  inline Type &move_to(Type &dest, bool dest_has_obj) {
    return *move_to(&dest, dest_has_obj);
  }
  inline heap_obj_warper &copy_to(heap_obj_warper &dest) {
    dest.init_with_unsafe_placement_new(
        copy_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  inline heap_obj_warper &move_to(heap_obj_warper &dest) {
    dest.init_with_unsafe_placement_new(
        move_to(dest.pointer_to_unsafe_data(), dest.has_data()));
    return dest;
  }
  inline heap_obj_warper *copy_to(heap_obj_warper *dest) {
    return &copy_to(*dest);
  }
  inline heap_obj_warper *move_to(heap_obj_warper *dest) {
    return &move_to(*dest);
  }
};
template <class my_reallocator>
class mjz_temp_malloc_wrapper_t {
  mjz_temp_malloc_wrapper_t &move(mjz_temp_malloc_wrapper_t &otr) {
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
  mjz_temp_malloc_wrapper_t &move(void *data_ptr, size_t size_of_ptr) {
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
  inline mjz_temp_malloc_wrapper_t(size_t size_of_ptr = 0) {
    m_data_ptr = malloc(size_of_ptr);
  };
  inline mjz_temp_malloc_wrapper_t(size_t size_of_ptr, int VAl_) {
    m_data_ptr = malloc(size_of_ptr);
    memset(VAl_);
  };
  inline mjz_temp_malloc_wrapper_t(void *data_ptr, size_t size_of_ptr) {
    move(data_ptr, size_of_ptr);
  }
  inline mjz_temp_malloc_wrapper_t(void *data_ptr, size_t size_of_ptr,
                                   int VAl_) {
    move(data_ptr, size_of_ptr).memset(VAl_);
  }
  inline mjz_temp_malloc_wrapper_t &change_data_ptr(void *data_ptr,
                                                    size_t size_of_ptr) {
    return move(data_ptr, size_of_ptr);
  }
  inline mjz_temp_malloc_wrapper_t &change_data_ptr(
      mjz_temp_malloc_wrapper_t &&otr) {
    return move(otr);
  }
  inline ~mjz_temp_malloc_wrapper_t() { free(); }
  mjz_temp_malloc_wrapper_t(mjz_temp_malloc_wrapper_t &) = delete;
  inline mjz_temp_malloc_wrapper_t(mjz_temp_malloc_wrapper_t &&otr) noexcept {
    move(otr);
  }
  mjz_temp_malloc_wrapper_t(const mjz_temp_malloc_wrapper_t &) = delete;
  mjz_temp_malloc_wrapper_t &operator=(mjz_temp_malloc_wrapper_t &) = delete;
  inline mjz_temp_malloc_wrapper_t &operator=(
      mjz_temp_malloc_wrapper_t &&otr) noexcept {
    return move(otr);
  };
  mjz_temp_malloc_wrapper_t &operator=(const mjz_temp_malloc_wrapper_t &) =
      delete;
  template <typename Type>
  constexpr inline Type *get_ptr_as() {
    return (Type *)m_data_ptr;
  }
  constexpr inline void *get_ptr() { return m_data_ptr; }
  constexpr inline size_t get_size() { return m_cap_size; }
  void free() {
    if (do_deallocation_on_free_state() && m_cap_size) {
      my_reallocator().deallocate(
          (mjz_get_value_Type<my_reallocator> *)m_data_ptr, m_cap_size);
    }
    m_data_ptr = 0;
    m_cap_size = 0;
  }
  void *malloc(size_t size_of_ptr) {
    free();

    if (size_of_ptr) {
      m_data_ptr = my_reallocator().allocate(size_of_ptr);

      if (m_data_ptr) {
        m_cap_size = size_of_ptr;
      }
    }

    return get_ptr();
  }
  void *realloc(size_t size_of_ptr) {
    // free();
    if (size_of_ptr) {
      m_data_ptr = my_reallocator().allocate(
          size_of_ptr, do_deallocation_on_free_state() ? m_data_ptr : 0);

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

  mjz_temp_malloc_wrapper_t(void *data_ptr, size_t cap_size,
                            uint8_t DO_deallocate)
      : m_data_ptr(data_ptr),
        m_cap_size(cap_size),
        m_Deallocation_state(DO_deallocate) {}
};
using malloc_wrapper = mjz_temp_malloc_wrapper_t<mjz_allocator_warpper<char>>;

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
typedef uint8_t BYTE;       // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

struct SHA256_CTX {
 protected:
  char *to_c_string(char *buf) const;

 public:
  char *copy_to_c_str(char *buf, size_t len) const;
  union {
    BYTE data[64]{};
    BYTE hashed_data[SHA256_BLOCK_SIZE];
  };
  WORD datalen{};
  unsigned long long bitlen{};
  WORD state[8]{};
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> to_string() const;
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
  constexpr basic_mjz_Str_view()
      : m_buffer(const_cast<char *>(empty_STRING_C_STR)), m_length(0) {}

  constexpr basic_mjz_Str_view(const char *buffer, size_t length)
      : basic_mjz_Str_view(const_cast<char *>(buffer), length) {}
  inline ~basic_mjz_Str_view() = default;
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
  inline constexpr basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  inline constexpr basic_mjz_Str_view substr_view_beg_n(
      unsigned int beginIndex, unsigned int number) const {
    return substr_view_beg_n((size_t)beginIndex, (size_t)number);
  }
  inline constexpr basic_mjz_Str_view substr_view(int64_t beginIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex));
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
  constexpr inline size_t max_size() const { return (((size_t)(-1)) >> 1) - 1; }

 public:
  template <typename T = mjz_allocator_warpper<char>>
  std::pair<hash_sha256, mjz_str_t<T>> hash_with_output(uint8_t n = 0) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(size_t beginIndex);
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(size_t beginIndex, size_t endIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring_beg_n(size_t beginIndex, size_t number) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substr(size_t pos = 0, size_t len = npos) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(int64_t beginIndex, int64_t endIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(int64_t beginIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring_beg_n(int64_t beginIndex, size_t number);
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring_beg_n(unsigned int beginIndex,
                               unsigned int number) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(int beginIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(int beginIndex, int endIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring_beg_n(int beginIndex, int number) const;
  template <typename T = mjz_allocator_warpper<char>>
  mjz_str_t<T> substring(size_t beginIndex) const;
  template <typename T = mjz_allocator_warpper<char>>
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

  inline basic_mjz_Str_view sv() const { return *this; }

 public:
  const static char
      the_in_reinterpreted_char_cca[the_reinterpreted_char_cca_size];  // just
  // me
  const static char
      the_out_reinterpreted_char_cca[the_reinterpreted_char_cca_size];  // just
  // me
  const static char forbiden_chars_cnt[forbiden_chars_cnt_size];
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
  size_t println(void) if_override_then_override;  // TODO: V1071
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
  size_t write(const char *buf, size_t size_);
  size_t write(const char *buf);
  size_t write(uint8_t) if_ard_then_override;
  size_t write(char cr);
  bool reserve(size_t size_, bool just_size = 0, bool constructor = 0);
  bool addto_length(size_t addition_tolen, bool just_size = 0);
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
  // void end(); used
  // stream

  // new and delete
  [[nodiscard]] static void *operator new(size_t size_);
  [[nodiscard]] static void *operator new[](size_t size_);
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
    new (&where) mjz_str_t<T>;
    return where;
  }
  inline static mjz_str_t<T> *replace_with_new_str(mjz_str_t<T> *where) {
    where->~mjz_str_t<T>();  // end lifetime
    new (where) mjz_str_t<T>;
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

#ifndef Arduino
  mjz_str_t<T> &operator=(std::string_view &x) {
    return operator=(std::string(x).c_str());
  }
  mjz_str_t<T> &operator=(std::string_view &&x) {
    return operator=(std::string(x).c_str());
  }
  mjz_str_t<T> &assign_range(std::initializer_list<const char> list);
  mjz_str_t<T> &assign_range(iterator_template<const char> list);
  mjz_str_t<T> &operator+=(std::string_view &x) {
    return operator+=(std::string(x).c_str());
  }
  mjz_str_t<T> &operator+=(std::string_view &&x) {
    return operator+=(std::string(x).c_str());
  }
#else
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
  mjz_str_t<T> &operator+=(basic_mjz_String &x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_str_t<T> &operator+=(basic_mjz_String &&x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_str_t(std::string &x) : mjz_str_t<T>(x.c_str()) {}
  mjz_str_t(std::string &&x) : mjz_str_t<T>(x.c_str()) {}
  mjz_str_t(const std::string &x) : mjz_str_t<T>(x.c_str()) {}
  ~mjz_str_t(void);  // make all drived destructors called
  void adjust_cap();
  mjz_str_t<T> &operator-=(const mjz_str_t<T> &othr_);
  mjz_str_t<T> &operator-=(mjz_str_t<T> &&othr_);
  mjz_str_t<T> &operator*=(unsigned int number_of_it);
  mjz_str_t<T> operator*(unsigned int number_of_it);
  mjz_str_t<T> &operator/=(const mjz_str_t<T> &othr_);
  mjz_str_t<T> &operator/=(mjz_str_t<T> &&othr_);
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
    return std__string_view_if_is((const char *)buffer_ref(), length());
  }
  explicit operator const std__string_view_if_is() const {
    return std__string_view_if_is((const char *)buffer_ref(), length());
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
  std__string_view_if_is std_sv() const {
    return std__string_view_if_is((const char *)buffer_ref(), length());
  }
  
  [[nodiscard]] const std::string std_s() const {
    return std::string((const char *)buffer_ref());
  }
  // creates a copy of the assigned value. if the value is null or
  // invalid,or if the memory allocation fails,the string will be
  // marked as invalid ("if (s)" will be false).
  mjz_str_t<T> &operator=(const mjz_str_t<T> &rhs);
  mjz_str_t<T> &operator=(const std::string &x) { return (*this)(x.c_str(),x.length()); }
  mjz_str_t<T> &operator=(const char *cstr);
  mjz_str_t<T> &operator=(const __FlashStringHelper *str);
  mjz_str_t<T> &operator=(mjz_str_t<T> &&rval) noexcept;
  // mjz_str_t<T> &operator=(const mjz_str_t<T> *rval) { return
  // operator=(*rval); } mjz_Str<T> &operator=( const mjz_Str<T> *&&rval){
  // return operator=(std::move(*rval));
  // }// this will give me headaches in the long run so i dont move it
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

  [[nodiscard]] static mjz_str_t<T> create_mjz_Str_char_array(size_t size_,
                                                              char filler = 0,
                                                              bool do_fill = 1);
  [[nodiscard]] static mjz_str_t<T> create_mjz_Str_2D_char_array(
      size_t size_col, size_t size_row, char filler = 0, bool do_fill = 1);

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
  using const_iterator = iterator_template<const char>;
  using iterator = iterator_template<char>;

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
  mjz_str_t<T> &erase(size_t pos_ = 0, size_t len_ = -1);
  iterator erase(iterator p);
  iterator erase(iterator first, iterator last);
  mjz_str_t<T> &erase_from_f_to_l(size_t first, size_t last);
  // search
  // modification
  void find_and_replace(char find, char replace_);
  void find_and_replace(const mjz_str_t<T> &find, const mjz_str_t<T> &replace_);
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

  mjz_str_t<T> &ULL_LL_to_str_add(size_t value, int radix, bool is_signed,
                                  bool force_neg = 0);
  mjz_str_t<T> &ULL_LL_to_str_rep(size_t value, int radix, bool is_signed,
                                  bool force_neg = 0);
  friend void swap(mjz_str_t<T> &lhs, mjz_str_t<T> &rhs) { lhs.swap(rhs); }
  void swap(mjz_str_t<T> &rhs) {
    mjz_str_t<T> &lhs = *this;
    mjz_str_t<T> lhs_buffer(std::move(lhs));  // be careful
    new (&lhs) mjz_str_t<T>;                  // be careful
    lhs = std::move(rhs);
    rhs = std::move(lhs_buffer);
  }
  char &at(int64_t i) { return operator[](i); }
  char &front() { return operator[]((int64_t)-1); }
  char &back() { return buffer_ref()[0]; }

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
  /* template <typename... arguments_types>
  size_t write(arguments_types &...arguments_arr) {
  mjz_Str<T> return_val = std::move(mjz_Str<T>(arguments_arr...));
  return write((const uint8_t *)return_val.c_str(),
  (size_t)return_val.length());
  }
  template <typename... arguments_types>
  size_t write(const arguments_types &...arguments_arr)
  { mjz_Str<T> return_val = std::move(mjz_Str<T>(arguments_arr...)); return
  write((const uint8_t *)return_val.c_str(), (size_t)return_val.length());
  }
  template <typename... arguments_types>
  size_t write(arguments_types &&...arguments_arr) {
  mjz_Str<T> return_val = std::move(mjz_Str<T>(arguments_arr...));
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
  int scanf(const mjz_str_t<T> &format, arguments_types &...arguments_arr) {
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
  int scanf(const mjz_str_t<T> &format,
            const arguments_types &...arguments_arr) {
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
  int scanf_s(const mjz_str_t<T> &format, arguments_types &...arguments_arr) {
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
  int scanf_s(const mjz_str_t<T> &format,
              const arguments_types &...arguments_arr) {
    int ret =
        sscanf_s((char *)buffer_ref(), format.buffer_ref(), arguments_arr...);
    return ret;
  }

  /*
  template <typename... arguments_types>
  mjz_Str<T> &operator-=(
  arguments_types &...arguments_arr) {
  return operator-=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> operator-(arguments_types &...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator/=(
  arguments_types &...arguments_arr) {
  return operator/=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> operator/(arguments_types &...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator-=(
  const arguments_types &...arguments_arr) {
  return operator-=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T>
  operator-(const arguments_types &...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator/=(
  const arguments_types &...arguments_arr) {
  return operator/=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T>
  operator/(const arguments_types &...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator-=(
  arguments_types &&...arguments_arr) {
  return operator-=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T>
  operator-(arguments_types &&...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator-=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator/=(
  arguments_types &&...arguments_arr) {
  return operator/=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T>
  operator/(arguments_types &&...arguments_arr) {
  mjz_Str<T> lhs = mjz_Str<T>(*this);
  return lhs.operator/=(arguments_arr...);
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator<<(
  arguments_types &...arguments_arr) {
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator<<=(
  arguments_types &...arguments_arr) {
  this->operator=(empty_STRING_C_STR);
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator=(
  arguments_types &...arguments_arr) {
  return operator=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator+=(
  arguments_types &...arguments_arr) {
  return operator+=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  */
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
                         arguments_types &...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
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
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str<T>
  // * , ... something)),...something)
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
  mjz_Str<T> &operator<<(
  const arguments_types &...arguments_arr) {
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator<<=(
  const arguments_types &...arguments_arr) {
  this->operator=(empty_STRING_C_STR);
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  */
  template <typename... arguments_types>
  inline mjz_str_t<T> &operator=(const arguments_types &...arguments_arr) {
    operator()();
    concat(arguments_arr...);
    return *this;
  }

  template <typename... arguments_types>
  mjz_str_t<T> &operator+=(const arguments_types &...arguments_arr) {
    concat(arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
                         const arguments_types &...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
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
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str<T>
  // * , ... something)),...something)
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
  mjz_Str<T> &operator<<(
  arguments_types &&...arguments_arr) {
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator<<=(
  arguments_types &&...arguments_arr) {
  this->operator=(empty_STRING_C_STR);
  return operator<<(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator=(
  arguments_types &&...arguments_arr) {
  return operator=(std::move(mjz_Str<T>(arguments_arr...)));
  }
  template <typename... arguments_types>
  mjz_Str<T> &operator+=(
  arguments_types &&...arguments_arr) {
  return operator+=(std::move(mjz_Str<T>(arguments_arr...)));
  }*/
  template <typename your_FUNCTION_Type, typename... arguments_types>
  mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
                         arguments_types &&...arguments_arr) {
    your__function_(*this, arguments_arr...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... arguments_types>
  const mjz_str_t<T> &run_code(your_FUNCTION_Type your__function_,
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
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str<T>
  // * , ... something)),...something)
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
  mjz_str_t(mjz_ard::iterator_template<const char> list) {
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
  mjz_str_t(float value, unsigned char decimalPlaces = 2) {
    static size_t const FLOAT_BUF_SIZE = FLT_MAX_10_EXP +
                                         FLT_MAX_DECIMAL_PLACES + 1 /* '-' */ +
                                         1 /* '.' */ + 1 /* '\0' */;
    init();
    char buf[FLOAT_BUF_SIZE];
    decimalPlaces = min_macro_(decimalPlaces, FLT_MAX_DECIMAL_PLACES);
    *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
  }
  mjz_str_t(double value, unsigned char decimalPlaces = 2) {
    static size_t const DOUBLE_BUF_SIZE = DBL_MAX_10_EXP +
                                          DBL_MAX_DECIMAL_PLACES + 1 /* '-' */ +
                                          1 /* '.' */ + 1 /* '\0' */;
    init();
    char buf[DOUBLE_BUF_SIZE];
    decimalPlaces = min_macro_(decimalPlaces, DBL_MAX_DECIMAL_PLACES);
    *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
  }
  inline mjz_str_t(const uint8_t *cstr, size_t length)
      : mjz_str_t((const char *)cstr, length) {}
  inline explicit mjz_str_t(const char *cstr)
      : mjz_str_t<T>(cstr, (size_t)strlen(cstr)) {}

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
  // }// this will give me headaches in the long run so i dont move it
  // inline explicit mjz_str_t(const mjz_str_t<T> *rval) : mjz_str_t(*rval) {}
  inline mjz_str_t() : mjz_str_t((const char *)empty_STRING_C_STR, 0) {}
  explicit inline mjz_str_t(const basic_mjz_String &otr)
      : mjz_str_t(otr.c_str(), otr.length()) {}
  explicit inline mjz_str_t(const basic_mjz_Str_view &otr)
      : mjz_str_t(otr.c_str(), otr.length()) {}
};
/*
 please dont use mjz_str_view with temporary strings
 if the string that it references goes out of scope (delete , ~obj , },free
 ,...)the string_view will have undefined behavior use this obj like a
 std::string_view not like std::string
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
  using iterator_template_CC = iterator_template<const char>;
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
extern std::shared_ptr<mjz_Str_DATA_storage_cls>
    main_mjz_Str_DATA_storage_Obj_ptr;
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
  bool is_blank() const;
  bool is_forbiden(char) const;
  bool change_reinterpret_char_char(char);
  bool char_to_char_for_reinterpret(char &c_char) const;
  char get_reinterpret_char_char() const;

 protected:
  uint8_t did_drived_mjz_Str_DATA_storage_Obj_ptr_set{0};
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      drived_mjz_Str_DATA_storage_Obj_ptr = main_mjz_Str_DATA_storage_Obj_ptr;
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
template <typename T = mjz_allocator_warpper<char>>
mjz_ard::mjz_str_t<T> ULL_LL_to_str(size_t value, int radix, bool is_signed,
                                    bool force_neg = 0);
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
  Type m_x;
  Type m_y;
  inline ~Vector2() = default;
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
  inline ~Vector3() = default;

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
// mjz_vector.h
// Vector
//
// Created by Emilis Baliukonis on 05/05/2019.
// Copyright � 2019 Emilis Baliukonis. All rights reserved.
//

namespace short_string_names {
typedef mjz_str_t<mjz_allocator_warpper<char>> Str;
typedef mjz_str_t<mjz_allocator_warpper<char>> str;
typedef mjz_str_t<mjz_allocator_warpper<char>> s;
typedef mjz_str_t<mjz_allocator_warpper<char>> S;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> es;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> eS;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> estr;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> eStr;
typedef StringSumHelper_t<mjz_allocator_warpper<char>> StrSH;
typedef mjz_str_view sv;
typedef mjz_str_view strv;
typedef mjz_str_view mstrview;
typedef mjz_str_view mstrv;
typedef Vector3<float> mvf3;
typedef Vector3<float> Vectorf3;
typedef Vector2<float> mvf2;
typedef Vector2<float> Vectorf2;
template <typename T>
using obj_wr = heap_obj_warper<T>;
template <typename T>
using hobj_wr = heap_obj_warper<T>;
template <typename T>
using alctr = mjz_allocator_warpper<T>;
template <typename T>
using alctr_wr = mjz_allocator_warpper<T>;
template <typename T>
using m_alctr = mjz_allocator_warpper<T>;
template <typename T>
using m_alctr_wr = mjz_allocator_warpper<T>;

}  // namespace short_string_names

namespace have_mjz_ard_removed {
typedef mjz_str_t<mjz_allocator_warpper<char>> mjz_Str;
typedef mjz_str_t<mjz_allocator_warpper<char>> mjz_str;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> mjz_estr;
typedef extended_mjz_str_t<mjz_allocator_warpper<char>> mjz_eStr;
typedef malloc_wrapper malloc_wrpr;
template <typename T>
using obj_warper = heap_obj_warper<T>;
template <typename T>
using heap_obj_warper = heap_obj_warper<T>;
template <typename T>
using allocator = mjz_allocator_warpper<T>;
template <typename T>
using allocator_warpper = mjz_allocator_warpper<T>;
template <typename T>
using mjz_allocator = mjz_allocator_warpper<T>;
template <typename T>
using mjz_allocator_warpper = mjz_allocator_warpper<T>;
typedef malloc_wrapper mlc_wrp;
typedef std::string string;
typedef hash_sha256 hash_sha_512;
typedef StringSumHelper_t<mjz_allocator_warpper<char>> mjz_StringSumHelper;
typedef StringSumHelper_t<mjz_allocator_warpper<char>> StringSumHelper;
typedef mjz_str_view mjz_str_view;
typedef mjz_str_view mstrview;
typedef mjz_str_view mstrv;
typedef Vector3<float> mjz_Vectorf3;
typedef Vector2<float> mjz_Vectorf2;
}  // namespace have_mjz_ard_removed
}  // namespace mjz_ard
namespace std {
template <>
struct hash<mjz_ard::basic_mjz_Str_view> {
  size_t operator()(const mjz_ard::basic_mjz_Str_view &k) const {
    return mjz_ard::SHA1_CTX::SHA_1(k.c_str(), k.length()).get_as_64bit_hash();
  }
};  // namespace std::hash

template <typename T>
struct hash<mjz_ard::mjz_str_t<T>> {
  size_t operator()(const mjz_ard::mjz_str_t<T> &k) const {
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

namespace mjz_ard {

template <typename T>
[[nodiscard]] void *mjz_ard::mjz_str_t<T>::operator new(size_t size_) {
  void *p = T().allocate(sizeof(size_t) + size_);
  *((size_t *)p) = sizeof(size_t) + size_;
  return ((size_t *)p) + 1;
}
template <typename T>
[[nodiscard]] void *mjz_ard::mjz_str_t<T>::operator new[](size_t size_) {
  void *p = T().allocate(sizeof(size_t) + size_);
  *((size_t *)p) = sizeof(size_t) + size_;
  return ((size_t *)p) + 1;
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete(void *p) {
  size_t *real = (((size_t *)p) - 1);
  size_t size_ = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, size_);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete[](void *p) {
  size_t *real = (((size_t *)p) - 1);
  size_t size_ = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, size_);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete(void *p, size_t) {
  size_t *real = (((size_t *)p) - 1);
  size_t size_ = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, size_);
}

template <typename T>
void mjz_ard::mjz_str_t<T>::operator delete[](void *p, size_t) {
  size_t *real = (((size_t *)p) - 1);
  size_t size_ = *real;
  T().deallocate((mjz_get_value_Type<T> *)real, size_);
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
    iterator_template<const char> list) {
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
 Base { int *ip; Base(int *ip): ip(ip) {} virtual void useInt() { std::cout <<
 *ip << std::endl; } ~Base() { useInt(); }
 };
 struct Der: Base {
 int theInt;
 Der(): Base(&theInt) {}
 void useIntPointer() override { std::cout << theInt << std::endl; }
 };
 When an object of type Der gets deleted, there is "undefined behavior": First
 the implicit destructor of Der is called, then the explicit destructor of
 Base, Base::~Base. At that point Base::ip is referring to a member of Der that
 has already been destroyed. Share Edit Follow answered Jan 19, 2017 at 6:48
 TheCppZoo's user avatar
 TheCppZoo
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
    goto _end__;
  }

  free_pv(m_buffer, constructor);
  stack_obj_buf.set(0);
  m_buffer = NULL;
_end__:
  m_capacity = m_length = 0;
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::reserve(size_t size_, bool just_size,
                                    bool constructor) {
  int64_t different_of_size_and_cap = (int64_t)size_ - (int64_t)m_capacity;

  if (just_size || different_of_size_and_cap < 0) {
    goto ignored_stack;
  }

  if (size_ < stack_buffer_size) {
    size_ = stack_buffer_size;
  } else {
    int64_t minimumcapadd = min_macro_(m_capacity / 5, stack_buffer_size);
    size_ +=
        (size_t)(static_cast<int64_t>(5) * (different_of_size_and_cap < 5) +
                 minimumcapadd * (different_of_size_and_cap < minimumcapadd));
  }

ignored_stack:

  if (m_buffer && m_capacity >= size_) {
    return 1;
  }

  if (changeBuffer(size_, constructor)) {
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
    m_length = m_length + addition_tolen;
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
      free_pv(stack_obj_buf.stack_buffer, constructor);
      return 0;
    }

    size_t the__length = m_capacity;

    if (the__length) {
      memmove(ptr, stack_obj_buf.stack_buffer,
              min_macro_(the__length, stack_buffer_size));
    }

    free_pv(
        stack_obj_buf.stack_buffer,
        constructor);  // mjz_ard::mjz_str_t<T>::free should care about ptr val
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
  if (reserve(length, 0, 1)) {
    goto _Success_full_;
  }

  invalidate();
  return *this;
_Success_full_:
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
  char *string = dtostrf(num, 4, 2, buf);
  return concat(string);
}
template <typename T>
bool mjz_ard::mjz_str_t<T>::concat(double num) {
  char buf[20];
  char *string = dtostrf(num, 4, 2, buf);
  return concat(string);
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
  if (!lhs.concat(rhs.c_str(), rhs.length())) {
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
mjz_ard::StringSumHelper_t<T> operator+(mjz_ard::mjz_str_t<T> lhs,
                                        const mjz_ard::basic_mjz_String &rhs) {
  return operator_plus((mjz_ard::mjz_str_t<T>)std::move(lhs),
                       (const mjz_ard::basic_mjz_String &)rhs);
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
char &mjz_ard::mjz_str_t<T>::operator[](size_t index) {
  static char dummy_writable_char;

  if (index >= m_length || !m_buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }

  return m_buffer[index];
}
template <typename T>
char &mjz_ard::mjz_str_t<T>::operator[](int64_t index_) {
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
  mjz_ard::mjz_str_t<T> buffer_str_ = substring(0, pos);
  buffer_str_.append(n, c);
  buffer_str_ += substr_view(pos, length());
  return (*this = std::move(buffer_str_));
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
    insert(pos + len, n - len, 0);
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
    memmove(m_buffer + pos, str.c_str(), len);
    insert(pos + len, str.length() - len, 0);
    memmove(m_buffer + pos + len, str.c_str() + len, str.length() - len);
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
    const mjz_ard::mjz_str_t<T> &find, const mjz_ard::mjz_str_t<T> &replace_) {
  find_and_replace(find.c_str(), find.length(), replace_.c_str(),
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
    goto _end__;
  }

  remove((size_t)pos_, (size_t)min(len_, length() - pos_));
_end__:
  return *this;
}
template <typename T>
iterator_template<char> mjz_ard::mjz_str_t<T>::erase(
    iterator_template<char> p) {
  if (p < begin() || end() < p) {
    return begin();
  }

  size_t index_ = p - begin();
  remove(index_, 1);
  return p;
}
template <typename T>
iterator_template<char> mjz_ard::mjz_str_t<T>::erase(
    iterator_template<char> first, iterator_template<char> last) {
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
size_t mjz_ard::mjz_str_t<T>::write(const uint8_t *buf, size_t size_) {
  mjz_ard::mjz_str_t<T>::operator+=(mjz_ard::mjz_str_t<T>(buf, (size_t)size_));
  return size_;
}
template <typename T>
size_t mjz_ard::mjz_str_t<T>::write(const char *buf, size_t size_) {
  mjz_ard::mjz_str_t<T>::operator+=(mjz_ard::mjz_str_t<T>(buf, (size_t)size_));
  return size_;
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
size_t mjz_ard::mjz_str_t<T>::read(uint8_t *buf, size_t size_) {
  if (!available()) {
    return (size_t)-1;
  }

  if (available() < size_) {
    return (size_t)-1;
  }

  if (!buf) {
    return (size_t)-1;
  }

  memmove(buf, m_buffer, size_);
  buf[size_] = 0;
  remove(0, (size_t)size_);
  return size_;
}
template <typename T>
void mjz_ard::mjz_str_t<T>::flush() {}
template <typename T>
void mjz_ard::mjz_str_t<T>::begin(unsigned long) {}
template <typename T>
void mjz_ard::mjz_str_t<T>::begin(unsigned long, uint16_t) {}
template <typename T>
mjz_ard::mjz_str_t<T> ULL_LL_to_str(uint64_t value, int radix, bool is_signed,
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
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::ULL_LL_to_str_add(
    uint64_t value, int radix, bool is_signed, bool force_neg) {
  this->operator+=(ULL_LL_to_str(value, radix, is_signed, force_neg));
  return *this;
}

template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::ULL_LL_to_str_rep(
    uint64_t value, int radix, bool is_signed, bool force_neg) {
  operator=(empty_STRING_C_STR);
  reserve(70, 1);
  char *ptr_ = b_U_lltoa(value, buffer_ref(), radix, is_signed, force_neg);

  if (!ptr_) {
    return *this;
  }

  addto_length((size_t)strlen(ptr_), 1);
  return *this;
}
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
mjz_ard::mjz_str_t<T>::create_mjz_Str_char_array(size_t size_, char filler,
                                                 bool do_fill) {
  mjz_ard::mjz_str_t<T> ret_val;
  ret_val.addto_length(size_, 1);
  char *ret_val_bufer = (char *)ret_val;

  if (do_fill)
    for (size_t i{}; i < size_; i++) {
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
  if (m_length == 0 || find_count == 0) {
    return;
  }

  int64_t diff = replace_count - find_count;
  char *readFrom = C_str();
  char *foundAt{};

  if (diff == 0) {
    while ((foundAt = const_cast<char *>(strstr(
                readFrom, strlen(readFrom), find_cstr, find_count))) != NULL) {
      memmove(foundAt, replace_cstr, replace_count);
      readFrom = foundAt + replace_count;
    }
  } else if (diff < 0) {
    size_t size_ = m_length;  // compute size_ needed for result

    while ((foundAt = const_cast<char *>(strstr(
                readFrom, strlen(readFrom), find_cstr, find_count))) != NULL) {
      readFrom = foundAt + find_count;
      diff = 0 - diff;
      size_ -= diff;
    }

    if (size_ == m_length) {
      return;
    }

    int64_t index = m_length - 1;

    while (index >= 0 &&
           (index = lastIndexOf_cstr(find_cstr, find_count, index)) >= 0) {
      readFrom = m_buffer + index + find_count;
      memmove(readFrom - diff, readFrom, m_length - (readFrom - m_buffer));
      m_length -= diff;
      m_buffer[m_length] = 0;
      memmove(m_buffer + index, replace_cstr, replace_count);
      index--;
    }
  } else {
    size_t size_ = m_length;  // compute size_ needed for result

    while ((foundAt = const_cast<char *>(strstr(
                readFrom, strlen(readFrom), find_cstr, find_count))) != NULL) {
      readFrom = foundAt + find_count;
      size_ += diff;
    }

    if (size_ == m_length) {
      return;
    }

    if (size_ > m_capacity && !changeBuffer(size_, 0)) {
      return;  // XXX: tell user!
    }

    int64_t index = m_length - 1;

    while (index >= 0 &&
           (index = lastIndexOf_cstr(find_cstr, find_count, index)) >= 0) {
      readFrom = m_buffer + index + find_count;
      memmove(readFrom + diff, readFrom, m_length - (readFrom - m_buffer));
      m_length += diff;
      m_buffer[m_length] = 0;
      memmove(m_buffer + index, replace_cstr, replace_count);
      index--;
    }
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
    mjz_ard::mjz_str_t<T> &&othr_) {
  int64_t index_of_remove = lastIndexOf(othr_);

  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }

  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator*=(
    unsigned int number_of_it) {
  mjz_ard::mjz_str_t<T> temperory_str;

  for (size_t i{}; i < number_of_it; i++) {
    temperory_str += *this;
  }

  *this = std::move(temperory_str);
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
  find_and_replace(othr_, othr_.length(), empty_STRING_C_STR, 0);
  return *this;
}
template <typename T>
mjz_ard::mjz_str_t<T> &mjz_ard::mjz_str_t<T>::operator/=(
    mjz_ard::mjz_str_t<T> &&othr_) {
  find_and_replace(othr_, othr_.length(), empty_STRING_C_STR, 0);
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

    output_name_in_output_out.println(" };");
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
  //}
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
  //}
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
  new (this) mjz_ard::mjz_str_t<T>;              // be careful
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

  const char *CIN_c_str = CIN.c_str();
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
mjz_str_t<T> mjz_ard::basic_mjz_Str_view::substring_beg_n(
    unsigned int beginIndex, unsigned int number) const {
  return substring_beg_n((size_t)beginIndex, (size_t)number);
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
  char buffer[1024]{};
  return mjz_str_t<T>(SHA256_CTX::to_c_string(buffer));
}

template <typename T = mjz_allocator_warpper<char>>
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
template <typename T2>
class mv_to_T2 {
 public:
  template <typename T>
  T2 operator()(T &&obj) {
    return obj;
  }
  template <typename T>
  T2 operator()(const T &obj) {
    return T2(obj);
  }
};
template <typename Tc1 = mv_to_T2<mjz_Str>,
          typename Tc2 = mv_to_T2<mjz_str_view>, typename T1, typename T2>
inline StringSumHelper operator+(T1 lhs, T2 rhs) {
  return operator_plus(Tc1()(lhs), Tc2()(rhs));
}
template StringSumHelper operator+(const mjz_Str &, const mjz_str_view &);
template StringSumHelper operator+(const mjz_Str &, const StringSumHelper &);
template StringSumHelper operator+(const mjz_Str &, const basic_mjz_Str_view &);
template StringSumHelper operator+(mjz_Str &&, const mjz_str_view &);
template StringSumHelper operator+(mjz_Str &&, const StringSumHelper &);
template StringSumHelper operator+(mjz_Str &&, const basic_mjz_Str_view &);
template StringSumHelper operator+(StringSumHelper &&, const mjz_str_view &);
template StringSumHelper operator+(StringSumHelper &&, const StringSumHelper &);
template StringSumHelper operator+(StringSumHelper &&,
                                   const basic_mjz_Str_view &);
template StringSumHelper operator+(const StringSumHelper &,
                                   const mjz_str_view &);
template StringSumHelper operator+(const StringSumHelper &,
                                   const StringSumHelper &);
template StringSumHelper operator+(const StringSumHelper &,
                                   const basic_mjz_Str_view &);

template <typename T2>
inline StringSumHelper operator+(StringSumHelper &&lhs, T2 rhs) {
  return operator_plus(mv_to_T2<mjz_Str>(lhs), mv_to_T2<mjz_Str>(rhs));
}
template <typename T2>
inline StringSumHelper operator+(const StringSumHelper &lhs, T2 rhs) {
  return operator_plus(mv_to_T2<mjz_Str>(lhs), mv_to_T2<mjz_Str>(rhs));
}

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

#undef NO_IGNORE_CHAR
#endif  // __mjz_ard_STRINGS__
#endif  // __cplusplus
