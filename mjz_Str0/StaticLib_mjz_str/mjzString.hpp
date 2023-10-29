/*
 String library for Wiring & arduino
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan. All right reserved.
 Copyright 2011,Paul Stoffregen,paul@pjrc.com
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License,or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details. //TODO: V1042
 https://pvs-studio.com/en/docs/warnings/V1042/ This file is marked with
 copyleft license, which requires you to open the derived source code. You
 should have received a copy of the GNU Lesser General Public License along with
 this library; if not,write to the Free Software Foundation,Inc.,51 Franklin
 St,Fifth Floor,Boston,MA 02110-1301 USA
*/
#pragma once
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
// #define if_virtual_then_virtual virtual
// use final if want to not be virtualized again
// virtual
#ifndef if_virtual_then_virtual
#define if_virtual_then_virtual
#endif  // if_virtual_then_virtual
// virtual
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
  stack_ovf_prtt_off = MJZ_frm_stack_ovf_BIT(0),
  LL_ovf_prtt_off = MJZ_frm_stack_ovf_BIT(1),
  len_ovf_prtt_off = MJZ_frm_stack_ovf_BIT(2)
};
template <typename cmpr_type>
size_t MJZ_memcmp(const void *ptr_1_, const void *ptr_2_,
                  size_t size_of_data_in_byte) {
  const uint8_t *ptr_1 = (const uint8_t *)ptr_1_;
  const uint8_t *ptr_2 = (const uint8_t *)ptr_2_;
  size_t size_of_data = size_of_data_in_byte / sizeof(cmpr_type);
  uint8_t last_char_cmpr_size = size_of_data_in_byte % sizeof(cmpr_type);
  size_t diff_of_data{};
  const cmpr_type *ptr_1_rf = (const cmpr_type *)ptr_1;
  const cmpr_type *ptr_2_rf = (const cmpr_type *)ptr_2;
  for (size_t i{}; i < size_of_data; i++) {
    diff_of_data += !(ptr_1_rf[i] == ptr_2_rf[i]);
  }
  size_t first_index_of_last = size_of_data_in_byte - last_char_cmpr_size;
  bool last_diff_of_data{};
  for (size_t i{}; i < last_char_cmpr_size; i++) {
    last_diff_of_data |=
        !(ptr_1[i + first_index_of_last] == ptr_2[i + first_index_of_last]);
  }
  diff_of_data += last_diff_of_data;
  return diff_of_data;
}
template <typename cmpr_type>
size_t MJZ_strcmp(const void *ptr_1, const void *ptr_2) {
  size_t len_1_ = strlen((const char *)ptr_1);
  size_t len_2_ = strlen((const char *)ptr_2);
  if (len_2_ != len_1_) {
    return -1;
  }
  return MJZ_memcmp<cmpr_type>(ptr_1, ptr_2, len_1_);
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

class malloc_wrapper {
  malloc_wrapper &move(malloc_wrapper &otr) {
    otr.dont_deallocation_on_free();
    free();
    m_data_ptr = otr.m_data_ptr;
    m_cap_size = otr.m_cap_size;
    return *this;
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
  bool m_DO_deallocate{1};

 public:
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
  ~malloc_wrapper() { free(); }
  malloc_wrapper(malloc_wrapper &) = delete;
  inline malloc_wrapper(malloc_wrapper &&otr) noexcept { move(otr); }
  malloc_wrapper(const malloc_wrapper &) = delete;
  malloc_wrapper &operator=(malloc_wrapper &) = delete;
  inline malloc_wrapper &operator=(malloc_wrapper &&otr) noexcept {
    return move(otr);
  };
  malloc_wrapper &operator=(const malloc_wrapper &) = delete;
  template <typename T>
  inline T *get_ptr_as() {
    return (T *)m_data_ptr;
  }
  inline void *get_ptr() { return m_data_ptr; }
  inline size_t get_size() { return m_cap_size; }
  void free() {
    if (!m_DO_deallocate) goto __return_;
    if (m_cap_size) ::free(m_data_ptr);
  __return_:
    m_data_ptr = 0;
    m_cap_size = 0;
  }
  void *malloc(size_t size_of_ptr) {
    free();
    if (size_of_ptr) {
      m_data_ptr = ::malloc(size_of_ptr);
      if (m_data_ptr) m_cap_size = size_of_ptr;
    }
    return get_ptr();
  }
  void *realloc(size_t size_of_ptr) {
    // free();
    if (size_of_ptr) {
      m_data_ptr = ::realloc(m_DO_deallocate ? m_data_ptr : 0, size_of_ptr);
      if (m_data_ptr) m_cap_size = size_of_ptr;
    }
    return get_ptr();
  }
  inline void *operator()(size_t size_of_ptr) { return malloc(size_of_ptr); }
  inline void *operator()() { return get_ptr(); }
  inline void memset(int _Val) {
    if (m_cap_size) ::memset(get_ptr(), _Val, m_cap_size);
  }
  inline void dont_deallocation_on_free() { m_DO_deallocate = 0; }
  inline void do_deallocation_on_free() { m_DO_deallocate = 1; }

  malloc_wrapper(void *data_ptr, const size_t &cap_size, bool DO_deallocate)
      : m_data_ptr(data_ptr),
        m_cap_size(cap_size),
        m_DO_deallocate(DO_deallocate) {}
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
  // using iterator_concept = std::contiguous_iterator_tag;
  constexpr iterator_template() noexcept
      : iterator_template(nullptr, nullptr, (Type *)-1) {}
  // iterator_template(Type *iter ) noexcept: m_iterator{iter} {}
  constexpr iterator_template(Type *iter, Type *min_end, Type *max_end)noexcept
      : m_iterator{iter},
        m_iterator_begin_ptr{min_end},
        m_iterator_end_ptr{max_end} {
  }
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

  constexpr iterator_template(const iterator_template &p)noexcept
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
  constexpr ~iterator_template() {
    throw_if_bad();
    m_iterator = 0;
  }
  constexpr bool operator==(const iterator_template &other) const noexcept {
    return m_iterator == other.m_iterator;
  }
  constexpr bool operator!=(const iterator_template &other) const noexcept {
    return m_iterator != other.m_iterator;
  }
  constexpr reference operator*() const  {
    throw_if_bad();
    return *m_iterator;
  }
  constexpr pointer operator->() const  {
    throw_if_bad();
    return m_iterator;
  }
  constexpr iterator_template &operator++()noexcept {
    ++m_iterator;
    return *this;
  }
  constexpr iterator_template operator++(int)noexcept {
    iterator_template tmp(*this);
    ++(*this);
    return tmp;
  }
  constexpr iterator_template &operator--()noexcept {
    --m_iterator;
    return *this;
  }
  constexpr iterator_template operator--(int) noexcept{
    iterator_template tmp(*this);
    --(*this);
    return tmp;
  }
  constexpr iterator_template &operator+=(const difference_type other) noexcept{
    m_iterator += other;
    return *this;
  }
  constexpr iterator_template &operator-=(const difference_type other) noexcept{
    m_iterator -= other;
    return *this;
  }
  constexpr iterator_template &operator+=(const iterator_template &other)noexcept {
    m_iterator += other.m_iterator;
    return *this;
  }
  constexpr iterator_template &operator-=(const iterator_template &other) noexcept{
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
  constexpr friend iterator_template operator+(const iterator_template &me,
                                               const difference_type other) noexcept{
    return iterator_template(
        me.m_iterator + other,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  constexpr friend iterator_template operator-(const iterator_template &me,
                                               const difference_type other) noexcept{
    return iterator_template(
        me.m_iterator - other,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  constexpr friend iterator_template operator+(const difference_type other,
                                               const iterator_template &me) noexcept{
    return iterator_template(
        other + me.m_iterator,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  // friend iterator_template operator-(const difference_type other, const
  // iterator_template& me) noexcept { // bad function dont use
  // return iterator_template(me.m_iterator - (pointer)other);
  // }
  constexpr friend iterator_template operator+(const iterator_template &other,
                                               const iterator_template &me)noexcept {
    return iterator_template(
        other.m_iterator + me,
        min(other.m_iterator_begin_ptr, me.m_iterator_begin_ptr),
        max(other.m_iterator_end_ptr, me.m_iterator_end_ptr));
  }
  constexpr friend difference_type operator-(const iterator_template &other,
                                             const iterator_template &me) noexcept{
    return std::distance(other.m_iterator, me.m_iterator);
  }
  constexpr friend void swap(iterator_template &lhs, iterator_template &rhs) noexcept{
    iterator_template lhsm_iterator = lhs;
    lhs = rhs;
    rhs = lhsm_iterator;
  }
  constexpr friend void swap(reference lhs, reference rhs) noexcept{
    value_type lhsm_ = lhs;
    lhs = rhs;
    rhs = lhsm_;
  }
};
class static_str_algo {
 public:
  static constexpr int64_t the_reinterpreted_char_cca_size = 17;
  static constexpr int64_t forbiden_chars_cnt_size = 3;
  static size_t constexpr FLT_MAX_DECIMAL_PLACES = 10;
  static size_t constexpr DBL_MAX_DECIMAL_PLACES = FLT_MAX_DECIMAL_PLACES;
  static constexpr const char *empty_STRING_C_STR = "";

 public:
 public:
  static constexpr int MJZ_STRnCMP(const char *p1, const char *p2,
                                   size_t lenght) {
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;
    const unsigned char *END_OF_char = s1 + lenght;
    unsigned char c1, c2;
    do {
      c1 = (unsigned char)*s1;
      s1++;
      c2 = (unsigned char)*s2;
      s2++;
      if (END_OF_char < s1) {
        return c1 - c2;
      }
    } while (c1 == c2);
    return c1 - c2;
  }
  constexpr static int memcmp(const void *str1, const void *str2,
                              size_t count) {
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    while (0 < count--) {
      if (*s1++ != *s2++) return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
  }
  constexpr static int memcmp(const void *str1, size_t count1, const void *str2,
                              size_t count2) {
    if (count1 != count2) {
      return count1 < count2 ? -1 : 1;
    }
    if (str1 == str2) {
      return 0;
    }
    return memcmp(str1, str2, count2);
  }

  constexpr static void *memmove(void *dest, const void *src, size_t len) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    if (d < s)
      while (len--) *d++ = *s++;
    else {
      const char *lasts = s + (len - 1);
      char *lastd = d + (len - 1);
      while (len--) *lastd-- = *lasts--;
    }
    return dest;
  }
  constexpr static void *memcpy(void *dest, const void *src, size_t len) {
    return memmove(dest, src, len);
  }
  static inline size_t strlen(const char *str) {
    if (!(str && *str)) return 0;
    const char *str_i = str;
    while (1) {
      if (!*++str_i) return str_i - str;
    }
  }
  constexpr static inline const char *strchr(const char *str, size_t len_,
                                             char ch) {
    const char *haystack_end = str + len_;
    for (const char *i{str}; i < haystack_end; ++i) {
      if (*(i) == ch) return i;
    }
    return NULL;
  }
  constexpr static inline const char *strrchr(const char *str, size_t len_,
                                              char ch) {
    const char *haystack_end = str + len_;
    const char *i{haystack_end - 1};
    for (;;) {
      if (*(i) == ch) return i;
      if (i <= str) break;
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
    if (needle[0] == '\0') return (char *)haystack;
    if (haystack_len < needle_len) return NULL;
    if (haystack_len == 0) return NULL;
    /* Skip until we find the first matching char from NEEDLE. */
    const char *haystack_end = haystack + haystack_len;
    haystack = strchr(haystack, haystack_len, needle[0]);
    if (haystack_end == haystack || needle[1] == '\0' || !haystack)
      return (char *)haystack;
    /* Ensure HAYSTACK length is at least as long as NEEDLE length.
    Since a match may occur early on in a huge HAYSTACK, use strnlen
    and read ahead a few cachelines for improved performance. */
    /* Check whether we have a match. This improves performance since we avoid
    the initialization overhead of the two-way algorithm. */
    if (static_str_algo::memcmp(haystack, needle, needle_len) == 0)
      return (char *)haystack;
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
      // // (this->*update_event_F_p)(); //departed
      return (lhs == NULL || *lhs == 0);
    }
    if (lhs == NULL) {
      // // (this->*update_event_F_p)(); //departed
      return rhs[0] == 0;
    }
    if (lhs_l != rhs_l) return 0;
    // // (this->*update_event_F_p)(); //departed
    return compare_two_str(rhs, rhs_l, lhs, lhs_l) == 0;
  }

 public:
  static constexpr int64_t stack_buffer_size = 15;
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
      if (STR_is_in_stack_) goto _return___;
      memset(stack_buffer, 0, stack_buffer_size);
      stack_buffer[stack_buffer_size] = 0;
    _return___:
      return STR_is_in_stack;
    }
    bool get() { return STR_is_in_stack; }
    // virtual //i dont need it
    ~stack_str_buf() { STR_is_in_stack = 0; }
  };
};
/*********************************************************************
 * Filename:   sha256.h
 * Author:     Brad Conte (brad AT bradconte.com)
 * Copyright:
 * Disclaimer: This code is presented "as is" without any guarantees.
 * Details:    Defines the API for the corresponding SHA1 implementation.
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
  union {
    BYTE data[64]{};
    BYTE hashed_data[SHA256_BLOCK_SIZE];
  };
  WORD datalen{};
  unsigned long long bitlen{};
  WORD state[8]{};
  mjz_Str to_string() const;
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
      m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) |
             (data[j + 3]);
    for (; i < 64; ++i)
      m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

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
      while (i < 56) ctx->data[i++] = 0x00;
    } else {
      ctx->data[i++] = 0x80;
      while (i < 64) ctx->data[i++] = 0x00;
      sha256_transform(ctx, ctx->data);
      memset(ctx->data, 0, 56);
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

class basic_mjz_Str_view : public static_str_algo {
 public:
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
  constexpr basic_mjz_Str_view(const char *buffer, size_t length)
      : basic_mjz_Str_view(const_cast<char *>(buffer), length) {}
  constexpr ~basic_mjz_Str_view() = default;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &&) = delete;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view &operator=(const basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &&) = delete;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &) = delete;
  constexpr basic_mjz_Str_view(const basic_mjz_Str_view &) = delete;

 public:
  constexpr inline char *&buffer_ref() { return m_buffer; }
  constexpr inline const char *buffer_ref() const { return m_buffer; }
  constexpr bool is_blank() const {
    size_t i{};
    while (i < m_length) {
      if (!is_blank_characteres_default(m_buffer[i])) return 0;
      i++;
    }
    return 1;
  }
  constexpr bool empty() const {
    return (!m_length || m_buffer == empty_STRING_C_STR || m_buffer == nullptr);
  }
  constexpr inline const char *c_str() const { return buffer_ref(); }
  constexpr inline const char *c_str() { return buffer_ref(); }
  constexpr inline const char *C_str() const { return buffer_ref(); }
  constexpr inline char *C_str() { return m_buffer; }
  constexpr inline size_t length(void) const { return m_length; }

 public:
  constexpr explicit operator const bool() const { return !is_blank(); }
  constexpr char operator[](int64_t index) const {
    index = signed_index_to_unsigned(index);
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
    // // (this->*update_event_F_p)(); //departed
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

  // hash function
  constexpr hash_sha256 mjz_hash(uint8_t n = 0) const {
    return SHA256_CTX::hash_msg_to_sha_512_n(c_str(), length(), n);
  }
  constexpr hash_sha256 hash() const { return mjz_hash(0); }
  constexpr int64_t compareTo(const basic_mjz_Str_view &s) const {
    return compare_two_str(m_buffer, m_length, s.m_buffer, m_length);
  }

  constexpr int64_t compareTo(const char *cstr) const {
    return compare_two_str(m_buffer, m_length, cstr, strlen(cstr));
  }
  constexpr bool equals(const char *cstr, size_t cstr_len) const {
    return are_two_str_equale(m_buffer, m_length, cstr, cstr_len);
  }
  constexpr bool equals(const basic_mjz_Str_view &s2) const {
    // // (this->*update_event_F_p)(); //departed
    return (m_length == s2.m_length && compareTo(s2) == 0);
  }
  constexpr inline bool equals(const char *cstr) const {
    return equals(cstr, strlen(cstr));
  }
  constexpr inline size_t signed_index_to_unsigned(int64_t input) const {
    if (input < 0) {
      input += length();
    }
    if (length() < (size_t)(input)) {
      input = length();
    }
    return (size_t)(input);
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
      // // (this->*update_event_F_p)(); //departed
      return true;
    }
    if (m_length != s2.m_length) {
      // // (this->*update_event_F_p)(); //departed
      return false;
    }
    if (m_length == 0) {
      //// (this->*update_event_F_p)(); //departed
      return true;
    }
    const char *p1 = m_buffer;
    const char *p2 = s2.m_buffer;
    const char *end_p1 = m_buffer + length();
    while (p1 < end_p1) {
      if (tolower(*p1++) != tolower(*p2++)) {
        // // (this->*update_event_F_p)(); //departed
        return false;
      }
    }
    // // (this->*update_event_F_p)(); //departed
    return true;
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2) const {
    if (m_length < s2.m_length) {
      // // (this->*update_event_F_p)(); //departed
      return false;
    }
    // // (this->*update_event_F_p)(); //departed
    return startsWith(s2, 0);
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2, size_t offset) const {
    if (offset > m_length - s2.m_length || !m_buffer || !s2.m_buffer) {
      // // (this->*update_event_F_p)(); //departed
      return false;
    }
    // // (this->*update_event_F_p)(); //departed
    return MJZ_STRnCMP(&m_buffer[offset], s2.m_buffer, s2.m_length) == 0;
  }
  constexpr bool endsWith(const basic_mjz_Str_view &s2) const {
    if (m_length < s2.m_length || !m_buffer || !s2.m_buffer) {
      // // (this->*update_event_F_p)(); //departed
      return false;
    }
    // // (this->*update_event_F_p)(); //departed
    return MJZ_STRnCMP(&m_buffer[m_length - s2.m_length], s2.m_buffer,
                       s2.m_length) == 0;
  }
  // Function that return the length
  constexpr size_t size() const { return length(); }
  constexpr if_virtual_then_virtual char charAt(int64_t loc) const {
    // // (this->*update_event_F_p)(); //departed
    loc = signed_index_to_unsigned(loc);
    return operator[](loc);
  }
  constexpr char charAt(size_t loc) const {
    // // (this->*update_event_F_p)(); //departed
    return operator[](loc);
  }
  constexpr void getBytes(unsigned char *buf, size_t bufsize,
                          size_t index = 0) const {
    if (!bufsize || !buf) {
      // // (this->*update_event_F_p)(); //departed
      return;
    }
    if (index >= m_length) {
      buf[0] = 0;
      // // (this->*update_event_F_p)(); //departed
      return;
    }
    size_t n = bufsize - 1;
    if (n > m_length - index) {
      n = m_length - index;
    }
    memmove((char *)buf, m_buffer + index, min_macro_(n, m_length));
    buf[n] = 0;
    // // (this->*update_event_F_p)(); //departed
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
      // // (this->*update_event_F_p)(); //departed
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

 public:  // non constexpr
  mjz_str_view substr_view(size_t beginIndex);
  mjz_str_view substr_view(size_t beginIndex, size_t endIndex) const;
  mjz_str_view substr_view_beg_n(size_t beginIndex, size_t number);
  mjz_str_view substr_view(int64_t beginIndex, int64_t endIndex) const;
  mjz_str_view substr_view(int64_t beginIndex) const;
  mjz_str_view substr_view_beg_n(int64_t beginIndex, size_t number);
  mjz_str_view substr_view_beg_n(unsigned int beginIndex,
                                 unsigned int number) const;
  mjz_str_view substr_view(int beginIndex) const;
  mjz_str_view substr_view(int beginIndex, int endIndex) const;
  mjz_str_view substr_view_beg_n(int beginIndex, int number) const;
  mjz_str_view substr_view_beg_n(size_t beginIndex, size_t number) const;
  mjz_str_view substr_view(size_t beginIndex) const;
  mjz_str_view substr_view_beg_n(int64_t beginIndex, size_t number) const;

 public:
  std::pair<hash_sha256, mjz_Str> hash_with_output(uint8_t n = 0) const;

  if_virtual_then_virtual int64_t indexOf(const mjz_Str &str) const;
  if_virtual_then_virtual int64_t indexOf(const mjz_Str &str,
                                          size_t fromIndex) const;
  if_virtual_then_virtual mjz_Str substring(size_t beginIndex);
  if_virtual_then_virtual mjz_Str substring(size_t beginIndex,
                                            size_t endIndex) const;
  if_virtual_then_virtual mjz_Str substring_beg_n(size_t beginIndex,
                                                  size_t number);
  if_virtual_then_virtual mjz_Str substring(int64_t beginIndex,
                                            int64_t endIndex) const;
  if_virtual_then_virtual mjz_Str substring(int64_t beginIndex) const;
  if_virtual_then_virtual mjz_Str substring_beg_n(int64_t beginIndex,
                                                  size_t number);
  if_virtual_then_virtual mjz_Str substring_beg_n(unsigned int beginIndex,
                                                  unsigned int number) const;
  if_virtual_then_virtual mjz_Str substring(int beginIndex) const;
  if_virtual_then_virtual mjz_Str substring(int beginIndex, int endIndex) const;
  if_virtual_then_virtual mjz_Str substring_beg_n(int beginIndex,
                                                  int number) const;
  mjz_Str substring_beg_n(size_t beginIndex, size_t number) const;
  mjz_Str substring(size_t beginIndex) const;
  mjz_Str substring_beg_n(int64_t beginIndex, size_t number) const;
  if_virtual_then_virtual int64_t lastIndexOf(const mjz_Str &str) const;
  if_virtual_then_virtual int64_t lastIndexOf(const mjz_Str &str,
                                              size_t fromIndex) const;
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
  inline ~basic_mjz_String(){};
  basic_mjz_String(basic_mjz_String &&) = delete;
  basic_mjz_String(const basic_mjz_String &) = delete;
  basic_mjz_String(basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &&) = delete;
  basic_mjz_String &operator=(const basic_mjz_String &) = delete;
  basic_mjz_String &operator=(basic_mjz_String &) = delete;

 public:
  inline size_t max_size() const { return m_capacity; }
};
#ifndef Arduino
class mjz_Str : public basic_mjz_String {
#if 0
	 
}
#endif
#else
class mjz_Str : public basic_mjz_String,
                public if_virtual_then_virtual Stream {  //
#endif  // Arduino
 protected:
  static std::shared_ptr<mjz_Str_DATA_storage_cls>
      main_mjz_Str_DATA_storage_Obj_ptr;
  bool did_drived_mjz_Str_DATA_storage_Obj_ptr_set{0};
  // void (mjz_Str::*update_event_F_p)( void); //departed // to call
  // (object_ptr->*pointer_name)(arguments)//
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      drived_mjz_Str_DATA_storage_Obj_ptr = main_mjz_Str_DATA_storage_Obj_ptr;
  std::shared_ptr<mjz_Str_DATA_storage_cls>
      &drived_mjz_Str_DATA_storage_Obj_ptr_set();
  friend class StringSumHelper;
  // use a function pointer to allow for "if (s)" without the
  // complications of an operator bool(). for more information,see:
  // http://www.artima.com/cppsource/safebool.html
  typedef void (mjz_Str::*StringIfHelperType)() const;
  if_virtual_then_virtual void StringIfHelper() const {}
  inline bool realloc_helper_is_in_stack(void *ptr);
  // private:
  void *realloc_pv(void *ptr, size_t new_size, bool constructor);
  void free_pv(void *&ptr, bool constructor);
  void free_pv(void *const &ptr, bool constructor);
  mjz_Str &copy(const char *cstr, size_t length, bool constructor);

 protected:
  virtual void *realloc(void *ptr, size_t new_size) {
    return get_realloc_alloc_function()(ptr, new_size);
  }
  virtual void free(void *ptr) { return get_free_alloc_function()(ptr); }

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
  size_t println(void)
      if_override_then_override;  // TODO: V1071
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
  bool find(const char *target);  // reads data from the stream until the target
  // string is found
  bool find(const uint8_t *target) { return find((const char *)target); }
  // returns true if target string is found, false if timed out (see setTimeout)
  bool find(const char *target,
            size_t length);  // reads data from the stream until the target
  // string of given length is found
  bool find(const uint8_t *target, size_t length) {
    return find((const char *)target, length);
  }
  // returns true if target string is found, false if timed out
  bool find(char target) { return find(&target, 1); }
  bool findUntil(const char *target,
                 const char *terminator);  // as find but search ends if the
  // terminator string is found
  bool findUntil(const uint8_t *target, const char *terminator) {
    return findUntil((const char *)target, terminator);
  }
  bool findUntil(const char *target, size_t targetLen, const char *terminate,
                 size_t termLen);  // as above but search ends if the terminate
  // string is found
  bool findUntil(const uint8_t *target, size_t targetLen, const char *terminate,
                 size_t termLen) {
    return findUntil((const char *)target, targetLen, terminate, termLen);
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
  mjz_Str readmjz_Str();
  mjz_Str readmjz_StrUntil(char terminator);
  if_virtual_then_virtual size_t write(const char *buf, size_t size_);
  if_virtual_then_virtual size_t write(const char *buf);
  if_virtual_then_virtual size_t write(uint8_t) if_ard_then_override;
  if_virtual_then_virtual size_t write(const uint8_t *buf,
                                       size_t size_) if_ard_then_override;
  if_virtual_then_virtual int64_t availableLL() if_ard_then_override;
  if_virtual_then_virtual int available() if_ard_then_override {
    return (int)availableLL();
  }
  if_virtual_then_virtual int read() if_ard_then_override;
  if_virtual_then_virtual int peek() if_ard_then_override;
  if_virtual_then_virtual void flush() if_ard_then_override;
  if_virtual_then_virtual size_t read(uint8_t *buf, size_t size_);
  if_virtual_then_virtual size_t readBytes(char *buffer_, size_t length) {
    return read((uint8_t *)buffer_, length);
  }
  size_t readBytes(uint8_t *buffer_, size_t length) {
    return readBytes((char *)buffer_, length);
  }
  if_virtual_then_virtual void begin(unsigned long);
  if_virtual_then_virtual void begin(unsigned long, uint16_t);
  // if_virtual_then_virtual void end(); used
  // stream

  // new and delete
  static void *operator new(size_t size_);
  static void *operator new[](size_t size_);
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
    // obj.mjz_Str::~mjz_Str();//bad not calling virtualy
    // obj->~mjz_Str(); // good calls the most drived constructor
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
    static void *operator new(size_t size_);
    static void *operator new[](size_t size_);
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
  if_virtual_then_virtual mjz_Str &operator=(std__string_view_if_is &x) {
    return operator=(std::string(x).c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(std__string_view_if_is &&x) {
    return operator=(std::string(x).c_str());
  }
  if_virtual_then_virtual mjz_Str &operator+=(std__string_view_if_is &x) {
    return operator+=(std::string(x).c_str());
  }
  if_virtual_then_virtual mjz_Str &operator+=(std__string_view_if_is &&x) {
    return operator+=(std::string(x).c_str());
  }
#else
  mjz_Str(std__string_view_if_is &x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str(std__string_view_if_is &&x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  mjz_Str(const std__string_view_if_is &x)
      : mjz_Str(((x.c_str() != 0) ? (x.c_str())
                                  : ((const char *)empty_STRING_C_STR))) {}
  if_virtual_then_virtual mjz_Str &operator=(std__string_view_if_is &x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(const std__string_view_if_is &x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(std__string_view_if_is &&x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator+=(std__string_view_if_is &x) {
    return operator+=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator+=(const std__string_view_if_is &x) {
    return operator+=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator+=(std__string_view_if_is &&x) {
    return operator+=(x.c_str());
  }
#endif
  if_virtual_then_virtual mjz_Str &operator=(basic_mjz_String &x) {
    return (*this)(x.c_str(), x.length());
  }
  if_virtual_then_virtual mjz_Str &operator=(basic_mjz_String &&x) {
    return (*this)(x.c_str(), x.length());
  }
  if_virtual_then_virtual mjz_Str &operator+=(basic_mjz_String &x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  if_virtual_then_virtual mjz_Str &operator+=(basic_mjz_String &&x) {
    concat(x.c_str(), x.length());
    return *this;
  }
  mjz_Str(std::string &x) : mjz_Str(x.c_str()) {}
  mjz_Str(std::string &&x) : mjz_Str(x.c_str()) {}
  mjz_Str(const std::string &x) : mjz_Str(x.c_str()) {}
  virtual ~mjz_Str(void);  // make all drived destructors called
  void adjust_cap();
  if_virtual_then_virtual mjz_Str &operator-=(const mjz_Str &othr_);
  if_virtual_then_virtual mjz_Str &operator-=(mjz_Str &&othr_);
  if_virtual_then_virtual mjz_Str &operator*=(unsigned int number_of_it);
  if_virtual_then_virtual mjz_Str operator*(unsigned int number_of_it);
  if_virtual_then_virtual mjz_Str &operator/=(const mjz_Str &othr_);
  if_virtual_then_virtual mjz_Str &operator/=(mjz_Str &&othr_);
  if_virtual_then_virtual mjz_Str &operator++();  // print empty line
  if_virtual_then_virtual mjz_Str operator++(int);
  if_virtual_then_virtual mjz_Str &operator--();  // read one character
  if_virtual_then_virtual mjz_Str operator--(int);
  inline if_virtual_then_virtual mjz_Str &operator()() {
    this->operator=(empty_STRING_C_STR);
    return *this;
  };
  inline if_virtual_then_virtual mjz_Str &operator()(const mjz_Str &other) {
    this->operator=(other);
    return *this;
  };
  inline if_virtual_then_virtual mjz_Str &operator()(const char *other) {
    operator()(other, (size_t)strlen(other));
    return *this;
  };
  inline if_virtual_then_virtual mjz_Str &operator()(const char *other,
                                                     size_t size_len);
  inline if_virtual_then_virtual mjz_Str &operator()(mjz_Str &&other) {
    this->operator=(std::move(other));
    return *this;
  };
  // memory management
  // return true on success,false on failure (in which case,the string
  // is left unchanged). reserve(0),if successful,will validate an
  // invalid string (i.e.,"if (s)" will be true afterwards)
  if_virtual_then_virtual bool reserve(size_t size_, bool just_size = 0,
                                       bool constructor = 0);
  bool addto_length(size_t addition_tolen, bool just_size = 0);
  if_virtual_then_virtual explicit operator char *() { return buffer_ref(); }
  if_virtual_then_virtual explicit operator const uint8_t *() const {
    return (const uint8_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator uint8_t *() {
    return (uint8_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator void *() {
    return (void *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator uint16_t *() {
    return (uint16_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator uint32_t *() {
    return (uint32_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator uint64_t *() {
    return (uint64_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator const void *() const {
    return (void *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator const uint16_t *() const {
    return (uint16_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator const uint32_t *() const {
    return (uint32_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator const uint64_t *() const {
    return (uint64_t *)buffer_ref();
  }
  if_virtual_then_virtual explicit operator const size_t() const {
    return m_length;
  }
  if_virtual_then_virtual explicit operator size_t() const { return m_length; }
  if_virtual_then_virtual explicit operator size_t &() {
    return (size_t &)m_length;
  }
  if_virtual_then_virtual explicit operator const long() const {
    return toInt();
  }
  if_virtual_then_virtual explicit operator const long long() const {
    return toLL();
  }
  if_virtual_then_virtual explicit operator const float() const {
    return toFloat();
  }
  if_virtual_then_virtual explicit operator const double() const {
    return toDouble();
  }
  if_virtual_then_virtual explicit operator std__string_view_if_is() const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  if_virtual_then_virtual explicit operator const std__string_view_if_is()
      const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  if_virtual_then_virtual explicit operator std::string() const {
    return std::string((const char *)buffer_ref(), length());
  }
  if_virtual_then_virtual explicit operator const std::string() const {
    return std::string((const char *)buffer_ref(), length());
  }
  if_virtual_then_virtual operator const char *() const { return buffer_ref(); }
  if_virtual_then_virtual char &operator[](size_t index);
  if_virtual_then_virtual char &operator[](int64_t index);
  if_virtual_then_virtual std__string_view_if_is std_new_sv() = delete;
  if_virtual_then_virtual std__string_view_if_is stdsv() const {
    return std__string_view_if_is((const char *)buffer_ref());
  }
  if_virtual_then_virtual std__string_view_if_is &&stdsvt() const {
    return std::move(std__string_view_if_is((const char *)buffer_ref()));
  }
  if_virtual_then_virtual const std::string stds() const {
    return std::string((const char *)buffer_ref());
  }
  if_virtual_then_virtual const std::string &&stdst() const {
    return std::move(std::string((const char *)buffer_ref()));
  }
  // creates a copy of the assigned value. if the value is null or
  // invalid,or if the memory allocation fails,the string will be
  // marked as invalid ("if (s)" will be false).
  if_virtual_then_virtual mjz_Str &operator=(const mjz_Str &rhs);
  if_virtual_then_virtual mjz_Str &operator=(std::string &x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(std::string &&x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(const std::string &x) {
    return operator=(x.c_str());
  }
  if_virtual_then_virtual mjz_Str &operator=(const char *cstr);
  if_virtual_then_virtual mjz_Str &operator=(const __FlashStringHelper *str);
  if_virtual_then_virtual mjz_Str &operator=(mjz_Str &&rval) noexcept;
  if_virtual_then_virtual mjz_Str &operator=(const mjz_Str *rval) {
    return operator=(*rval);
  }
  // if_virtual_then_virtual mjz_Str &operator=( const mjz_Str *&&rval){
  // return operator=(std::move(*rval));
  // }// this will give me headaches in the long run so i dont move it
  // concatenate (works w/ built-in types)
  // returns true on success,false on failure (in which case,the string
  // is left unchanged). if the argument is null or invalid,the
  // concatenation is considered unsuccessful.
  if_virtual_then_virtual bool concat(const mjz_Str &str);
  if_virtual_then_virtual bool concat(
      const char *
          cstr);  // TODO: V1071 https://pvs-studio.com/en/docs/warnings/V1071/
                  // Consider inspecting the 'concat' function. The return value
                  // is not always used. Total calls: 11, discarded results: 1.
  if_virtual_then_virtual bool concat(const char *cstr, size_t length);
  if_virtual_then_virtual bool concat(const uint8_t *cstr, size_t length) {
    return concat((const char *)cstr, length);
  }
  if_virtual_then_virtual bool concat(char c);
  if_virtual_then_virtual bool concat(unsigned char num);
  if_virtual_then_virtual bool concat(int num);
  if_virtual_then_virtual bool concat(unsigned int num);
  if_virtual_then_virtual bool concat(long num);
  if_virtual_then_virtual bool concat(unsigned long num);
  if_virtual_then_virtual bool concat(long long num);
  if_virtual_then_virtual bool concat(unsigned long long num);
  if_virtual_then_virtual bool concat(float num);
  if_virtual_then_virtual bool concat(double num);
  if_virtual_then_virtual bool concat(const __FlashStringHelper *str);
  if_virtual_then_virtual const mjz_Str &operator>>(mjz_Str &typeing) const;
  if_virtual_then_virtual const mjz_Str &operator>>(mjz_Str *typeing) const;
  if_virtual_then_virtual mjz_Str &operator>>(mjz_Str &typeing);
  if_virtual_then_virtual mjz_Str &operator>>(mjz_Str *typeing);
  if_virtual_then_virtual const mjz_Str &operator>>=(mjz_Str &typeing) const {
    typeing.operator=(empty_STRING_C_STR);
    return operator>>(typeing);
  }
  if_virtual_then_virtual const mjz_Str &operator>>=(mjz_Str *typeing) const {
    typeing->operator=(empty_STRING_C_STR);
    return operator>>(typeing);
  }
  if_virtual_then_virtual mjz_Str &operator>>=(mjz_Str &typeing) {
    typeing.operator=(empty_STRING_C_STR);
    return operator>>(typeing);
  }
  if_virtual_then_virtual mjz_Str &operator>>=(mjz_Str *typeing) {
    typeing->operator=(empty_STRING_C_STR);
    return operator>>(typeing);
  }
  if_virtual_then_virtual mjz_Str &operator<<(mjz_Str &typeing);
  if_virtual_then_virtual mjz_Str &operator<<(mjz_Str *typeing);
  if_virtual_then_virtual mjz_Str &operator<<(const mjz_Str &typeing);
  if_virtual_then_virtual mjz_Str &operator<<(mjz_Str &&typeing);
  if_virtual_then_virtual mjz_Str &operator<<=(mjz_Str &typeing) {
    if (&typeing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typeing);
    }
    mjz_Str new_temp = typeing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  if_virtual_then_virtual mjz_Str &operator<<=(const mjz_Str &typeing) {
    if (&typeing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typeing);
    }
    mjz_Str new_temp = typeing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  if_virtual_then_virtual mjz_Str &operator<<=(mjz_Str &&typeing) {
    if (&typeing != this) {
      this->operator=(empty_STRING_C_STR);
      return operator<<(typeing);
    }
    mjz_Str new_temp = typeing;
    this->operator=(empty_STRING_C_STR);
    return operator<<(new_temp);
  }
  if_virtual_then_virtual mjz_Str &operator>>(char &var) {
    get_s_shift_op_r().scanf("%c", &var);
    return get_s_shift_op_r();
  }
  if_virtual_then_virtual mjz_Str &operator>>(int &var) {
    get_s_shift_op_r().scanf("%d", &var);
    return get_s_shift_op_r();
  }
  if_virtual_then_virtual mjz_Str &operator>>(double &var) {
    var = (double)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  if_virtual_then_virtual mjz_Str &operator>>(float &var) {
    var = (float)(get_s_shift_op_r());
    return get_s_shift_op_r();
  }
  // if there's not enough memory for the concatenated value,the string
  // will be left unchanged (but this isn't signalled in any way)
  if_virtual_then_virtual mjz_Str &operator+=(const mjz_Str &rhs) {
    concat(rhs);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(mjz_Str &&rhs) {
    concat(rhs);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(const char *cstr) {
    concat(cstr);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(char c) {
    concat(c);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(unsigned char num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(int num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(unsigned int num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(long num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(unsigned long num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(long long num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(unsigned long long num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(float num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(double num) {
    concat(num);
    return (*this);
  }
  if_virtual_then_virtual mjz_Str &operator+=(const __FlashStringHelper *str) {
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
  if_virtual_then_virtual void *do_this_for_me(function_ptr, void *x = 0);
  // comparison (only works w/ Strings and "strings")
  friend bool is_blank_characteres_default(char);
  friend char char_to_char_for_reinterpret_fnc_ptr_default(char);
  friend bool is_forbiden_character_default(char);
  friend class basic_mjz_String;
  if_virtual_then_virtual void set_realloc_free_functions(
      std::function<void(void *)> free_,
      std::function<void *(void *, size_t)> realloc_) {
    if (free_ && realloc_) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->realloc_fnctn = realloc_;
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->free_fnctn = free_;
    }
  }
  if_virtual_then_virtual void change_is_blank_character_function(
      std::function<bool(char)> fnction) {
    if (fnction) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()->is_blank_character = fnction;
    }
  }
  if_virtual_then_virtual void
  change_char_to_char_for_reinterpret_fnc_ptr_function(
      std::function<char(char)> fnction) {
    if (fnction) {
      drived_mjz_Str_DATA_storage_Obj_ptr_set()
          ->char_to_char_for_reinterpret_fnc_ptr = fnction;
    }
  }
  if_virtual_then_virtual void change_is_forbiden_character_function(
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
  if_virtual_then_virtual bool is_forbiden(char) const;
  if_virtual_then_virtual bool change_reinterpret_char_char(char);
  if_virtual_then_virtual bool char_to_char_for_reinterpret(char &c_char) const;
  static int8_t char_to_int_for_string(char c_char);
  friend std::istream &helper__op_shift_input_(const mjz_Str &rhs,
                                               std::istream &CIN,
                                               mjz_Str &get_shift_op_s);
  static mjz_Str create_mjz_Str_char_array(size_t size_, char filler = 0,
                                           bool do_fill = 1);
  static mjz_Str create_mjz_Str_2D_char_array(size_t size_col, size_t size_row,
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
  if_virtual_then_virtual void setCharAt(size_t index, char c);
  // character access
  if_virtual_then_virtual void setCharAt(int64_t index, char c);
  inline if_virtual_then_virtual char *begin_c_str() { return C_str(); }
  if_virtual_then_virtual char *end_c_str() { return buffer_ref() + length(); }
  if_virtual_then_virtual char *endAST_c_str() { return end_c_str(); }
  if_virtual_then_virtual const char *begin_c_str() const { return c_str(); }
  if_virtual_then_virtual const char *endAST_c_str() const {
    return end_c_str();
  }
  if_virtual_then_virtual const char *end_c_str() const {
    return c_str() + length();
  }

  // Iterator Class
  using iterator_template_CC = iterator_template<const char>;
  using iterator_template_C = iterator_template<char>;
  using const_iterator = iterator_template_CC;
  using iterator = iterator_template_C;

  const_iterator begin() const {
    return const_iterator(begin_c_str(), begining_of_str_ptr(),
                          ending_of_str_ptr());
  }
  const_iterator end() const {
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
    return rev_iterator({end(), begining_of_str_ptr(), ending_of_str_ptr()});
  }
  const_rev_iterator rbegin() const {
    return const_rev_iterator(
        {end(), begining_of_str_ptr(), ending_of_str_ptr()});
  };
  rev_iterator rend() {
    return rev_iterator({begin(), begining_of_str_ptr(), ending_of_str_ptr()});
  }
  const_rev_iterator rend() const {
    return const_rev_iterator(
        {begin(), begining_of_str_ptr(), ending_of_str_ptr()});
  };
  // erase
  mjz_Str &erase(size_t pos_ = 0, size_t len_ = -1);
  iterator erase(iterator p);
  iterator erase(iterator first, iterator last);
  mjz_Str &erase_from_f_to_l(size_t first, size_t last);
  // search
  // modification
  if_virtual_then_virtual void replace(char find, char replace_);
  if_virtual_then_virtual void replace(const mjz_Str &find,
                                       const mjz_Str &replace_);
  if_virtual_then_virtual void replace(const char *find, size_t find_count,
                                       const char *replace_,
                                       size_t replace_count);
  if_virtual_then_virtual void remove(size_t index);
  if_virtual_then_virtual void remove(size_t index, size_t count);
  if_virtual_then_virtual void toLowerCase(void);
  if_virtual_then_virtual void toUpperCase(void);
  if_virtual_then_virtual void trim(void);
  if_virtual_then_virtual mjz_Str string_do_interpret();
  if_virtual_then_virtual void string_do_interpret(mjz_Str &instr);
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
  mjz_Str &insert(size_t pos, const mjz_Str &other);
  mjz_Str &insert(size_t pos, const mjz_Str &other, size_t subpos,
                  size_t sublen = -1);
  mjz_Str &insert(size_t pos, const char *s, size_t n);
  mjz_Str &insert(size_t pos, const char *other) {
    return insert(pos, other, strlen(other));
  }
  mjz_Str &pop_back() {
    if (m_length) buffer_ref()[--m_length] = 0;
    return *this;
  }
  friend class basic_mjz_Str_view;

 protected:
  // if_virtual_then_virtual void update_event();
  // if_virtual_then_virtual void update_event_ard_string();
  if_virtual_then_virtual const mjz_Str &get_shift_op_rc() const;
  if_virtual_then_virtual mjz_Str &get_shift_op_r();
  if_virtual_then_virtual const mjz_Str &get_shift_op_lc() const;
  if_virtual_then_virtual mjz_Str &get_shift_op_l();
  if_virtual_then_virtual const mjz_Str &get_shift_op_r_sc() const;
  if_virtual_then_virtual mjz_Str &get_shift_op_r_s();
  if_virtual_then_virtual const mjz_Str &get_shift_op_l_sc() const;
  if_virtual_then_virtual mjz_Str &get_shift_op_l_s();
  if_virtual_then_virtual const mjz_Str &get_s_shift_op_rc() const;
  if_virtual_then_virtual mjz_Str &get_s_shift_op_r();
  if_virtual_then_virtual const mjz_Str &get_s_shift_op_lc() const;
  if_virtual_then_virtual mjz_Str &get_s_shift_op_l();
  if_virtual_then_virtual const mjz_Str &get_s_shift_op_r_sc() const;
  if_virtual_then_virtual mjz_Str &get_s_shift_op_r_s();
  if_virtual_then_virtual const mjz_Str &get_s_shift_op_l_sc() const;
  if_virtual_then_virtual mjz_Str &get_s_shift_op_l_s();
  if_virtual_then_virtual void init(bool constructor = 1);
  if_virtual_then_virtual void invalidate(bool constructor = 1);
  if_virtual_then_virtual bool changeBuffer(size_t maxStrLen, bool constructor);
  // copy and move
  if_virtual_then_virtual mjz_Str &copy(const char *cstr, size_t length) {
    return copy(cstr, length, 0);
  }
  if_virtual_then_virtual mjz_Str &copy(const __FlashStringHelper *pstr,
                                        size_t length);
  if_virtual_then_virtual void move(mjz_Str &rhs);

 public:
  // easy quality of life
  inline mjz_Str *operator->() { return this; }
  inline const mjz_Str *operator->() const { return this; }
  inline mjz_Str &operator*() { return *this; }
  inline const mjz_Str &operator*() const { return *this; }
  // templated
  template <typename TYPE_>
  if_virtual_then_virtual mjz_Str substring(TYPE_ beginIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex);
  };
  template <typename TYPE_, typename TYPE_2>
  if_virtual_then_virtual mjz_Str substring(TYPE_ beginIndex,
                                            TYPE_2 endIndex) const {
    return basic_mjz_String::substring((int64_t)beginIndex, (int64_t)endIndex);
  }
  template <typename TYPE_, typename TYPE_2>
  if_virtual_then_virtual mjz_Str substring_beg_n(TYPE_ beginIndex,
                                                  TYPE_2 number) const {
    return basic_mjz_String::substring_beg_n((int64_t)beginIndex,
                                             (int64_t)number);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual size_t write(Args_frScnf &...args_frScnf) {
    mjz_Str return_val = std::move(mjz_Str(args_frScnf...));
    return write((const uint8_t *)return_val.c_str(),
                 (size_t)return_val.length());
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual size_t write(const Args_frScnf &...args_frScnf) {
    mjz_Str return_val = std::move(mjz_Str(args_frScnf...));
    return write((const uint8_t *)return_val.c_str(),
                 (size_t)return_val.length());
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual size_t write(Args_frScnf &&...args_frScnf) {
    mjz_Str return_val = std::move(mjz_Str(args_frScnf...));
    return write((const uint8_t *)return_val.c_str(),
                 (size_t)return_val.length());
  }
  template <typename... Args_frScnf>
  int scanf(const char *format, Args_frScnf &...args_frScnf) {
    int ret = sprintf_alt_((char *)buffer_ref(), (size_t)length(), format,
                           args_frScnf...);
    return ret;
  }
  template <typename... Args_frScnf>
  int scanf(const mjz_Str &format, Args_frScnf &...args_frScnf) {
    int ret = sprintf_alt_((char *)buffer_ref(), length(), format.buffer_ref(),
                           args_frScnf...);
    return ret;
  }
  template <typename... Args_frScnf>
  int scanf(const char *format, const Args_frScnf &...args_frScnf) {
    int ret = sprintf_alt_((char *)buffer_ref(), (size_t)length(), format,
                           args_frScnf...);
    return ret;
  }
  template <typename... Args_frScnf>
  int scanf(const mjz_Str &format, const Args_frScnf &...args_frScnf) {
    int ret = sprintf_alt_((char *)buffer_ref(), length(), format.buffer_ref(),
                           args_frScnf...);
    return ret;
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator-=(Args_frScnf &...args_frScnf) {
    return operator-=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator-(Args_frScnf &...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator/=(Args_frScnf &...args_frScnf) {
    return operator/=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator/(Args_frScnf &...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator-=(
      const Args_frScnf &...args_frScnf) {
    return operator-=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator-(const Args_frScnf &...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator/=(
      const Args_frScnf &...args_frScnf) {
    return operator/=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator/(const Args_frScnf &...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator-=(Args_frScnf &&...args_frScnf) {
    return operator-=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator-(Args_frScnf &&...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator-=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator/=(Args_frScnf &&...args_frScnf) {
    return operator/=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str operator/(Args_frScnf &&...args_frScnf) {
    mjz_Str lhs = mjz_Str(*this);
    return lhs.operator/=(args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<(Args_frScnf &...args_frScnf) {
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<=(Args_frScnf &...args_frScnf) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator=(Args_frScnf &...args_frScnf) {
    return operator=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator+=(Args_frScnf &...args_frScnf) {
    return operator+=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &run_code(your_FUNCTION_Type your__function_,
                                            Args_frScnf &...args_frScnf) {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual const mjz_Str &run_code(
      your_FUNCTION_Type your__function_, Args_frScnf &...args_frScnf) const {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_, Args_frScnf &...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_, Args_frScnf &...args_frScnf) const {
    return your__function_(this, args_frScnf...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_) {
    return your__function_(this);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_,
                                          Args_frScnf &...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_,
                                          Args_frScnf &...args_frScnf) const {
    return your__function_(this, args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<(
      const Args_frScnf &...args_frScnf) {
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<=(
      const Args_frScnf &...args_frScnf) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator=(
      const Args_frScnf &...args_frScnf) {
    return operator=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator+=(
      const Args_frScnf &...args_frScnf) {
    return operator+=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &run_code(your_FUNCTION_Type your__function_,
                                            const Args_frScnf &...args_frScnf) {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual const mjz_Str &run_code(
      your_FUNCTION_Type your__function_,
      const Args_frScnf &...args_frScnf) const {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_, const Args_frScnf &...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_,
      const Args_frScnf &...args_frScnf) const {
    return your__function_(this, args_frScnf...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_,
                                          const Args_frScnf &...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(
      your_FUNCTION_Type your__function_,
      const Args_frScnf &...args_frScnf) const {
    return your__function_(this, args_frScnf...);
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<(Args_frScnf &&...args_frScnf) {
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator<<=(Args_frScnf &&...args_frScnf) {
    this->operator=(empty_STRING_C_STR);
    return operator<<(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator=(Args_frScnf &&...args_frScnf) {
    return operator=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &operator+=(Args_frScnf &&...args_frScnf) {
    return operator+=(std::move(mjz_Str(args_frScnf...)));
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual mjz_Str &run_code(your_FUNCTION_Type your__function_,
                                            Args_frScnf &&...args_frScnf) {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual const mjz_Str &run_code(
      your_FUNCTION_Type your__function_, Args_frScnf &&...args_frScnf) const {
    your__function_(this, args_frScnf...);
    return *this;
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_, Args_frScnf &&...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto run_code_and_return(
      your_FUNCTION_Type your__function_, Args_frScnf &&...args_frScnf) const {
    return your__function_(this, args_frScnf...);
  }
  // ret( (gets a lambda / function pointer / std::function with ret(mjz_Str *
  // , ... something)),...something)
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_,
                                          Args_frScnf &&...args_frScnf) {
    return your__function_(this, args_frScnf...);
  }
  template <typename your_FUNCTION_Type, typename... Args_frScnf>
  if_virtual_then_virtual auto operator()(your_FUNCTION_Type your__function_,
                                          Args_frScnf &&...args_frScnf) const {
    return your__function_(this, args_frScnf...);
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
      default;  // we are string viewes and not strings
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
    if (length() <= pos) return 0;
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
      default;  // we are string viewes and not strings
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
      : mjz_Str(mjz_Str(s.c_str(), s.length())([](mjz_Str *this_) {
          std::cout << *this_ << "\n";
          this_->reserve(1);
          return *this_;
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
mjz_Str ULL_LL_to_str(size_t value, int radix, bool is_signed,
                      bool force_neg = 0);

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
  return operator""_m_strv(p, mjz_Str::strlen(p));
}

namespace short_string_convestion_operators {
inline mjz_str_view operator""_msv(const char *initilizer, size_t length_) {
  return operator""_m_strv(initilizer, length_);
}
inline mjz_str_view operator"" _msv(const char *p) {
  return operator""_m_strv(p, mjz_Str::strlen(p));
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
  return operator""_v(p, mjz_Str::strlen(p));
}
inline mjz_str_view operator"" _v(const char *p) { return operator""_mv(p); }

}  // namespace short_string_convestion_operators
namespace short_string_names {
typedef mjz_ard::mjz_Str Str;
typedef mjz_ard::mjz_Str str;
typedef mjz_ard::mjz_Str s;
typedef mjz_ard::mjz_Str S;
typedef std::string sstr;
typedef mjz_ard::StringSumHelper StrSH;
typedef mjz_ard::mjz_str_view sv;
typedef mjz_ard::mjz_str_view strv;
typedef mjz_ard::mjz_str_view mstrview;
typedef mjz_ard::mjz_str_view mstrv;
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
}  // namespace have_mjz_ard_removed
namespace std {
template <>
struct hash<mjz_ard::basic_mjz_Str_view> {
  size_t operator()(const mjz_ard::basic_mjz_Str_view &k) const {
    // Compute individual hash values for first, second and third
    // http://stackoverflow.com/a/1646913/126995
    return hash<string_view>()({k.c_str(), k.length()});
  }
};  // namespace std::hash
}  // namespace std
#undef NO_IGNORE_CHAR
#endif  // __mjz_ard_STRINGS__
#endif  // __cplusplus
