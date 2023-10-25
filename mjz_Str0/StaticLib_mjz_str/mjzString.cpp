
#include "pch.h"
//
#include "mjzString.hpp"
my_data_randomizer_union my_data_randomizer_uni;
#ifndef Arduino
namespace mjz_ard {
// Public Methods //////////////////////////////////////////////////////////////
/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
long long map(long long x, long long in_min, long long in_max,
              long long out_min, long long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }
}  // namespace mjz_ard
#endif
/*
 mjz_Str library for Wiring & mjz_ardralloc
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan. All rights reserved.
 Copyright 2011,Paul Stoffregen,paul@pjrc.com
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
#ifndef asdfghjklkjhgfdsasdfghjkjhgfdfghj
#define asdfghjklkjhgfdsasdfghjkjhgfdfghj
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#ifdef Arduino
namespace std::chrono {
namespace mjz_ard {
unsigned long long millisR() {
  milliseconds ms =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  return ms.count();
}
}  // namespace mjz_ard
}  // namespace std::chrono
#endif
#ifndef Arduino
unsigned long static time_MIlis_{};
unsigned long millis() {
  time_MIlis_ += 10;
  return time_MIlis_;
}
#endif
#ifdef Arduino
unsigned long long millisR() { return std::chrono::mjz_ard::millisR(); }
#endif
namespace mjz_ard {
/*********************************************/
/* Static Member Initialisation */
/*********************************************/
// size_t const basic_mjz_String::FLT_MAX_DECIMAL_PLACES;
// size_t const basic_mjz_String::DBL_MAX_DECIMAL_PLACES;
/*********************************************/
/* Constructors */
/*********************************************/
void randomSeed(unsigned long seed) {
  if (seed != 0) {
    std::srand(seed);
  }
}
long random(long howbig) {
  if (howbig == 0) {
    return 0;
  }
  return std::rand() % howbig;
}
long random(long howsmall, long howbig) {
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}
char *dtostrf(double __val, signed char __width, unsigned char __prec,
              char *__s) {
  std::string frmt = "%";
  frmt += std::to_string(__width) + '.' + std::to_string(__prec) + "lf";
  sprintf_alt_(__s, static_cast<size_t>(__width) + 1, frmt.c_str(), __val);
  return __s;
}
#define end_of_transmission_char 4
#define null_char 0
char GET_CHAR_from_int(uint8_t intager_in, bool is_upper_case) {
  const char volatile static *const volatile alphabett_table_lower =
      "0123456789abcdefghijklmnopqrstuvwxyz\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0";
  const char volatile static *const volatile alphabett_table_upper =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
      "\0\0\0\0\0\0\0\0\0";
  const char volatile *const volatile alphabett_table =
      (is_upper_case ? alphabett_table_upper : alphabett_table_lower);
  if (intager_in == 255) {
    return '-';
  } else if (36 < intager_in) {
    return ' ';
  }
  return (char)alphabett_table[intager_in];
}
char *b_U_lltoa(uint64_t value, char *BFR_buffer, int radix, bool is_signed,
                bool force_neg, bool is_upper_case) {
  bool IS_NEGITIVE__ =
      (is_signed && (*((int64_t *)&value) < 0) && ((radix == 10) || force_neg));
  if (IS_NEGITIVE__) {
    *((int64_t *)&value) =
        ((-1) * (*((int64_t *)&value)));  // use a positive insted of the - sign
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
          buffer[k] = buffer_[j];  // when its done we revese the string
          k++;
        }
      }
      break;
    }
  }
  buffer[number_of_numbers] = 0;
  if (IS_NEGITIVE__) {
    BFR_buffer[0] = '-';
    memmove(BFR_buffer + 1, buffer, number_of_numbers + 1);  //+null
  } else {
    memmove(BFR_buffer, buffer, number_of_numbers + 1);  //+null
  }
  return BFR_buffer;
}
mjz_Str ULL_LL_to_str(uint64_t value, int radix, bool is_signed,
                      bool force_neg) {
  mjz_Str ret_var;
  ret_var.reserve(70, 1);
  auto ptr_ = b_U_lltoa(value, (char *)ret_var, radix, is_signed, force_neg);
  if (!ptr_) return ret_var;
  ret_var.addto_length((uint64_t)strlen(ptr_), 1);
  return ret_var;
}
uint8_t get_num_from_char(uint8_t in_CHAR_, bool *to_neg) {
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
int64_t mjz_pow_UINT(uint32_t base, uint32_t power_Of_base) {
  int64_t ret_val = 1;
  for (uint32_t i{}; i < power_Of_base; i++) {
    ret_val *= base;
  }
  return ret_val;
}
static bool dummy_bool = 0;
long long C_STR_to_LL(const char *buffer, uint8_t buffer_len, int radix,
                      bool *had_error, uint8_t error_level) {
  if (!had_error) {
    had_error = &dummy_bool;
  }
  const double MAX_LL_IN_D = 9.223372036854775807e+18;
  uint8_t buffer_for_number[70]{};
  for (int64_t i{}; i < 70; i++) {
    buffer_for_number[i] = 127;
  }
  bool is_neg_bool{};
  bool stack_ovf_prtt = !MJZ_frm_stack_ovf_BIT_CHECK(error_level, 0);
  bool LL_ovf_prtt = !MJZ_frm_stack_ovf_BIT_CHECK(error_level, 1);
  bool len_ovf_prtt =
      (!MJZ_frm_stack_ovf_BIT_CHECK(error_level, 2) && stack_ovf_prtt);
  if ((len_ovf_prtt &&
       (std::pow((double)buffer_len - 2, (double)radix) > MAX_LL_IN_D ||
        (buffer_len - 1) > 63)) ||
      radix > 36 || radix < 2 || buffer_len > 65 || !buffer || !buffer_len) {
    *had_error = 1;
    if (stack_ovf_prtt) {
      return 0;
    }
  }
  if (!buffer) return 0;
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
char *ulltoa(uint64_t value, char *buffer, int radix) {
  return b_U_lltoa((uint64_t)value, buffer, radix, 0);
}
char *ultoa(uint32_t value, char *buffer, int radix) {
  return ulltoa((uint64_t)value, buffer, radix);
}
char *utoa(uint32_t value, char *buffer, int radix) {
  return ultoa(value, buffer, radix);
}
char *lltoa(long long value, char *buffer, int radix) {
  return b_U_lltoa((uint64_t)value, buffer, radix, 1);
}
char *ltoa(long value, char *buffer, int radix) {
  return lltoa((long long)value, buffer, radix);
}
char *itoa(int value, char *buffer, int radix) {
  return lltoa((long long)value, buffer, radix);
}
int MJZ_STRCMP(const char *p1, const char *p2) {
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
int MJZ_STRnCMP(const char *p1, const char *p2, size_t lenght) {
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
void *mjz_Str::operator new(size_t size_) {
  void *p = ::operator new(size_);
  // void * p = malloc(size_); will also work fine
  return p;
}
void *mjz_Str::operator new[](size_t size_) {
  void *p = ::operator new(size_);
  // void * p = malloc(size_); will also work fine
  return p;
}
void mjz_Str::operator delete(void *p) { ::free(p); }
void mjz_Str::operator delete[](void *ptr) { ::free(ptr); }
void *mjz_Str::realloc_new_ns::operator new(size_t size_) {
  void *p = ::realloc(0, size_);
  return p;
}
void *mjz_Str::realloc_new_ns::operator new[](size_t size_) {
  void *p = ::realloc(0, size_);
  return p;
}
void mjz_Str::realloc_new_ns::operator delete(void *p) { ::free(p); }
void mjz_Str::realloc_new_ns::operator delete[](void *ptr) { ::free(ptr); }
void *mjz_Str::realloc_new_ns::operator new(size_t size_, void *where) {
  return ::realloc(where, size_);
}
mjz_Str::mjz_Str(const char *cstr, size_t length) {
  init();
  if (cstr) {
    copy(cstr, length, 1);
  }
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(const mjz_Str &value) {
  init();
  *this = value;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(const __FlashStringHelper *pstr) {
  init();
  *this = pstr;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(mjz_Str &&rval) noexcept
    : basic_mjz_String((rval.stack_obj_buf.get() ? stack_obj_buf.stack_buffer
                                                 : rval.m_buffer),
                       (rval.m_capacity), (rval.m_length)),
      stack_obj_buf(rval.stack_obj_buf) {
  // update_event_F_p = &mjz_Str::update_event;//departed
  if (rval.stack_obj_buf.get()) {
    rval.free_pv(rval.m_buffer, 1);
  }
  rval.m_buffer = NULL;
  rval.m_capacity = 0;
  rval.m_length = 0;
  rval.stack_obj_buf.set(0);
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(char c) {
  init();
  char buf[2] = {c, 0};
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(unsigned char value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned char)];
  utoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(int value, unsigned char base) {
  init();
  char buf[2 + 8 * sizeof(int)];
  itoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(unsigned int value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned int)];
  utoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(long value, unsigned char base) {
  init();
  char buf[2 + 8 * sizeof(long)];
  ltoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(unsigned long value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned long)];
  ultoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(long long int value, unsigned char base) {
  init();
  char buf[2 + 8 * sizeof(long long)];
  lltoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(long long unsigned int value, unsigned char base) {
  init();
  char buf[1 + 8 * sizeof(unsigned long long)];
  ulltoa(value, buf, base);
  *this = buf;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(float value, unsigned char decimalPlaces) {
  static size_t const FLOAT_BUF_SIZE = FLT_MAX_10_EXP + FLT_MAX_DECIMAL_PLACES +
                                       1 /* '-' */ + 1 /* '.' */ + 1 /* '\0' */;
  init();
  char buf[FLOAT_BUF_SIZE];
  decimalPlaces = min_macro_(decimalPlaces, FLT_MAX_DECIMAL_PLACES);
  *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::mjz_Str(double value, unsigned char decimalPlaces) {
  static size_t const DOUBLE_BUF_SIZE = DBL_MAX_10_EXP +
                                        DBL_MAX_DECIMAL_PLACES + 1 /* '-' */ +
                                        1 /* '.' */ + 1 /* '\0' */;
  init();
  char buf[DOUBLE_BUF_SIZE];
  decimalPlaces = min_macro_(decimalPlaces, DBL_MAX_DECIMAL_PLACES);
  *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
  // (this->*update_event_F_p)(); //departed
}
mjz_Str::~mjz_Str(void) {
  invalidate(1);
}  // i don't need to do this but this is explained in stackoverfllow . the
   // vtable of the drived free override gets destroyed when ~mjz_Str() gets
   // called so mjz_Str::free shoud be called and i do it explicitly its not
   // necessary but i do it see
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
subobjects but not x or one of its base class subobjects, the behavior
is undefined.
You can find this recommendation in many sources, including Scott Meyers'
Effective C++: 55 Specific Ways to Improve Your Programs and Designs (Item 9:
Never call virtual functions during construction or destruction.)
or Herb Sutter's
C++ Coding Standards: 101 Rules, Guidelines, and Best Practices (49. Avoid
calling virtual functions in constructors and destructors).
Yes, the object will have a pointer to its vtable when you call the destructor.
The standard explicitly says it is possible to call virtual functions in the
destructor, and says what happens. There is agreement that even though it is
allowed it is a bad practice, because it is inherently brittle code that leads
to surprises with apparently innocent changes.
If you have that DerDer inherits from Der, which inherits from Base, all of them
override member function void member(), and you are in the destructor of Der,
and call member(), you are calling Der::member(), not DerDer::member(), because
the DerDer part of your object is GONE, DESTROYED already. A base class can
refer inadvertently to data in a derived class, for example:
struct Base {
int *ip;
Base(int *ip): ip(ip) {}
virtual void useInt() { std::cout << *ip << std::endl; }
~Base() { useInt(); }
};
struct Der: Base {
int theInt;
Der(): Base(&theInt) {}
void useIntPointer() override { std::cout << theInt << std::endl; }
};
When an object of type Der gets deleted, there is "undefined behavior": First
the implicit destructor of Der is called, then the explicit destructor of Base,
Base::~Base. At that point Base::ip is referring to a member of Der that has
already been destroyed.
Share
Edit
Follow
answered Jan 19, 2017 at 6:48
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
inline void mjz_Str::init(bool) {
  m_buffer = NULL;
  // update_event_F_p = &mjz_Str::update_event;//departed
  ;
  m_capacity = 0;
  stack_obj_buf.set(0);
  m_length = 0;
}
void mjz_Str::invalidate(bool constructor) {
  if (!m_buffer) goto _end__;
  free_pv(m_buffer, constructor);
  stack_obj_buf.set(0);
  m_buffer = NULL;
_end__:
  m_capacity = m_length = 0;
  // (this->*update_event_F_p)(); //departed
}
bool mjz_Str::reserve(size_t size_, bool just_size, bool constructor) {
  int64_t different_of_size_and_cap = (int64_t)size_ - (int64_t)m_capacity;
  if (just_size || different_of_size_and_cap < 0) goto ignored_stack;
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
    // (this->*update_event_F_p)(); //departed
    return 1;
  }
  if (changeBuffer(size_, constructor)) {
    if (m_length == 0) {
      m_buffer[0] = 0;
    }
    // (this->*update_event_F_p)(); //departed
    return true;
  }
  // (this->*update_event_F_p)(); //departed
  return false;
}
bool mjz_Str::addto_length(size_t addition_tolen, bool just_size) {
  bool ret = 1;
  if ((addition_tolen + m_length) > m_capacity) {
    ret = reserve(addition_tolen + m_length + 1, just_size);
  }
  if (ret) {
    m_length = m_length + addition_tolen;
  }
  return ret;
}
bool mjz_Str::realloc_helper_is_in_stack(void *ptr) {
  return (stack_obj_buf.get() || stack_obj_buf.stack_buffer == (char *)ptr);
}
void *mjz_Str::realloc_pv(void *ptr, size_t new_size, bool constructor) {
  bool ptr_is_in_stack = realloc_helper_is_in_stack(ptr);
  bool ptr_is_buffer = !(ptr_is_in_stack || (0 == ptr));  //(buffer == ptr);
  bool ptr_Can_set_to_stack = (new_size <= (stack_buffer_size + 1));
  volatile char *&my_buffer = (volatile char *&)m_buffer;
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
    ptr = (constructor ? mjz_Str::realloc(0, new_size)
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
    free_pv(stack_obj_buf.stack_buffer,
            constructor);  // mjz_Str::free shoud care about ptr val
    return ptr;
  }
  return (constructor ? mjz_Str::realloc(ptr, new_size)
                      : this->realloc(ptr, new_size));
}
void mjz_Str::free_pv(void *&ptr, bool constructor) {
  if (stack_obj_buf.get() || stack_obj_buf.stack_buffer == ptr) {
    ptr = 0;
    stack_obj_buf.set(0);
    return;
  }
  if (ptr) {
    if (constructor)
      mjz_Str::free(ptr);
    else
      this->free(ptr);
  }
  ptr = 0;
}
void mjz_Str::free_pv(void *const &ptr, bool constructor) {
  void *ptr_ = ptr;
  free_pv(ptr_, constructor);
}
bool mjz_Str::changeBuffer(size_t maxStrLen, bool constructor) {
  char *newbuffer{};
  newbuffer = (char *)realloc_pv(m_buffer, maxStrLen + 1, constructor);
  if (newbuffer) {
    m_buffer = newbuffer;
    m_capacity = maxStrLen;
    // (this->*update_event_F_p)(); //departed
    return true;
  }
  // (this->*update_event_F_p)(); //departed
  return false;
}
/*********************************************/
/* Copy and Move */
/*********************************************/
mjz_Str &mjz_Str::copy(const char *cstr, size_t length, bool constructor) {
  if (reserve(length, 0, 1)) goto _Success_full_;
  invalidate();
  // (this->*update_event_F_p)(); //departed
  return *this;
_Success_full_:
  m_length = length;
  memmove(m_buffer, cstr, length);
  m_buffer[m_length] = '\0';
  // (this->*update_event_F_p)(); //departed
  return *this;
}
#define PGM_P const char *
mjz_Str &mjz_Str::copy(const __FlashStringHelper *pstr, size_t length) {
  if (!reserve(length)) {
    invalidate();
    // (this->*update_event_F_p)(); //departed
    return *this;
  }
  m_length = length;
  memmove(m_buffer, (PGM_P)pstr, length);
  // (this->*update_event_F_p)(); //departed
  return *this;
}
void mjz_Str::move(mjz_Str &rhs) {
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
  // (this->*update_event_F_p)(); //departed
}
mjz_Str &mjz_Str::operator=(const mjz_Str &rhs) {
  if (this == &rhs) {
    // (this->*update_event_F_p)(); //departed
    return *this;
  }
  if (rhs.m_buffer) {
    copy(rhs.m_buffer, rhs.m_length);
  } else {
    invalidate();
  }
  // (this->*update_event_F_p)(); //departed
  return *this;
}
mjz_Str &mjz_Str::operator=(mjz_Str &&rval) noexcept {
  move(rval);
  // (this->*update_event_F_p)(); //departed
  return *this;
}
mjz_Str &mjz_Str::operator=(const char *cstr) {
  operator()(cstr);
  // (this->*update_event_F_p)(); //departed
  return *this;
}
mjz_Str &mjz_Str::operator()(const char *other, size_t size_len) {
  if (other) {
    copy(other, size_len);
  } else {
    invalidate();
  }
  // (this->*update_event_F_p)(); //departed
  return *this;
}
mjz_Str &mjz_Str::operator=(const __FlashStringHelper *pstr) {
  if (pstr) {
    copy(pstr, (size_t)strlen((PGM_P)pstr));
  } else {
    invalidate();
  }
  // (this->*update_event_F_p)(); //departed
  return *this;
}
/*********************************************/
/* concat */
/*********************************************/
bool mjz_Str::concat(const mjz_Str &s) {
  // (this->*update_event_F_p)(); //departed
  return concat(s.m_buffer, s.m_length);
}
bool mjz_Str::concat(const char *cstr, size_t length) {
  size_t newlen = m_length + length;
  if (!cstr) {
    // (this->*update_event_F_p)(); //departed
    return false;
  }
  if (length == 0) {
    // (this->*update_event_F_p)(); //departed
    return true;
  }
  if (!reserve(newlen)) {
    // (this->*update_event_F_p)(); //departed
    return false;
  }
  memmove(m_buffer + m_length, cstr, length);
  m_length = newlen;
  m_buffer[m_length] = '\0';
  // (this->*update_event_F_p)(); //departed
  return true;
}
bool mjz_Str::concat(const char *cstr) {
  if (!cstr) {
    // (this->*update_event_F_p)(); //departed
    return false;
  }
  // (this->*update_event_F_p)(); //departed
  return concat(cstr, (size_t)strlen(cstr));
}
bool mjz_Str::concat(char c) {
  // (this->*update_event_F_p)(); //departed
  return concat(&c, 1);
}
bool mjz_Str::concat(unsigned char num) {
  char buf[1 + 3 * sizeof(unsigned char)];
  itoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(int num) {
  char buf[2 + 3 * sizeof(int)];
  itoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(unsigned int num) {
  char buf[1 + 3 * sizeof(unsigned int)];
  utoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(long num) {
  char buf[2 + 3 * sizeof(long)];
  ltoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(unsigned long num) {
  char buf[1 + 3 * sizeof(unsigned long)];
  ultoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(long long num) {
  char buf[2 + 3 * sizeof(long long)];
  lltoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(unsigned long long num) {
  char buf[1 + 3 * sizeof(unsigned long long)];
  ulltoa(num, buf, 10);
  // (this->*update_event_F_p)(); //departed
  return concat(buf);
}
bool mjz_Str::concat(float num) {
  char buf[20];
  char *string = dtostrf(num, 4, 2, buf);
  // (this->*update_event_F_p)(); //departed
  return concat(string);
}
bool mjz_Str::concat(double num) {
  char buf[20];
  char *string = dtostrf(num, 4, 2, buf);
  // (this->*update_event_F_p)(); //departed
  return concat(string);
}
bool mjz_Str::concat(const __FlashStringHelper *str) {
  if (!str) {
    // (this->*update_event_F_p)(); //departed
    return false;
  }
  size_t length = (size_t)strlen((const char *)str);
  if (length == 0) {
    // (this->*update_event_F_p)(); //departed
    return true;
  }
  size_t newlen = m_length + length;
  if (!reserve(newlen)) {
    // (this->*update_event_F_p)(); //departed
    return false;
  }
  memmove(m_buffer + m_length, (const char *)str, newlen);
  m_length = newlen;
  // (this->*update_event_F_p)(); //departed
  return true;
}
/*********************************************/
/* Concatenate */
/*********************************************/
mjz_Str &&operator_plus(mjz_Str &&lhs, const basic_mjz_String &rhs) {
  if (!lhs.concat(rhs.c_str(), rhs.length())) {
    lhs.invalidate();
  }
  return std::move(lhs);
}
StringSumHelper operator+(mjz_Str &&lhs, const basic_mjz_String &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(mjz_Str lhs, const basic_mjz_String &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const basic_mjz_String &lhs,
                          const basic_mjz_String &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(mjz_Str &&lhs, const StringSumHelper &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(mjz_Str lhs, const StringSumHelper &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const basic_mjz_String &lhs,
                          const StringSumHelper &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const StringSumHelper &lhs,
                          const basic_mjz_String &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(StringSumHelper &&lhs, const basic_mjz_String &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const StringSumHelper &lhs, const mjz_str_view &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(StringSumHelper &&lhs, const mjz_str_view &rhs) {
  return operator_plus((mjz_Str &&) std::move(lhs),
                       (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const mjz_str_view &lhs,
                          const basic_mjz_String &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(const mjz_str_view &lhs, const StringSumHelper &rhs) {
  return operator_plus(mjz_Str(lhs), (const basic_mjz_String &)rhs);
}
StringSumHelper operator+(StringSumHelper &&lhs, const char *cstr) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!cstr || !a.concat(cstr)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, char c) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(c)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, unsigned char num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, int num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, unsigned int num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, long num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, unsigned long num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, long long num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, unsigned long long num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, float num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs, double num) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(num)) {
    a.invalidate();
  }
  return a;
}
StringSumHelper operator+(StringSumHelper &&lhs,
                          const __FlashStringHelper *rhs) {
  StringSumHelper &a = const_cast<StringSumHelper &>(lhs);
  if (!a.concat(rhs)) {
    a.invalidate();
  }
  return a;
}
/*********************************************/
/* Comparison */
/*********************************************/
int64_t basic_mjz_String::compareTo(const basic_mjz_String &s) const {
  if (!m_buffer || !s.m_buffer) {
    if (s.m_buffer && s.m_length > 0) {
      //// (this->*update_event_F_p)(); //departed
      return static_cast<int64_t>(0) - *(unsigned char *)s.m_buffer;
    }
    if (m_buffer && m_length > 0) {
      //// (this->*update_event_F_p)(); //departed
      return *(unsigned char *)m_buffer;
    }
    //// (this->*update_event_F_p)(); //departed
    return 0;
  }
  // if( len != s.len)return -1;
  //// (this->*update_event_F_p)(); //departed
  return MJZ_STRnCMP(m_buffer, s.m_buffer, m_length);
}
int64_t basic_mjz_String::compareTo(const char *cstr) const {
  if (!m_buffer || !cstr) {
    if (cstr && *cstr) {
      // // (this->*update_event_F_p)(); //departed
      return static_cast<int64_t>(0) - *(unsigned char *)cstr;
    }
    if (m_buffer && m_length > 0) {
      // // (this->*update_event_F_p)(); //departed
      return *(unsigned char *)m_buffer;
    }
    // // (this->*update_event_F_p)(); //departed
    return 0;
  }
  // // (this->*update_event_F_p)(); //departed
  return MJZ_STRCMP(m_buffer, cstr);
}
bool basic_mjz_String::equals(const basic_mjz_String &s2) const {
  // // (this->*update_event_F_p)(); //departed
  return (m_length == s2.m_length && compareTo(s2) == 0);
}
bool basic_mjz_String::equals(const char *cstr, size_t cstr_len) const {
  if (m_length == 0) {
    // // (this->*update_event_F_p)(); //departed
    return (cstr == NULL || *cstr == 0);
  }
  if (cstr == NULL) {
    // // (this->*update_event_F_p)(); //departed
    return m_buffer[0] == 0;
  }
  if (cstr_len != length()) return 0;
  // // (this->*update_event_F_p)(); //departed
  return MJZ_STRnCMP(m_buffer, cstr, cstr_len) == 0;
}
bool basic_mjz_String::equalsIgnoreCase(const basic_mjz_String &s2) const {
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
bool basic_mjz_String::startsWith(const basic_mjz_String &s2) const {
  if (m_length < s2.m_length) {
    // // (this->*update_event_F_p)(); //departed
    return false;
  }
  // // (this->*update_event_F_p)(); //departed
  return startsWith(s2, 0);
}
bool basic_mjz_String::startsWith(const basic_mjz_String &s2,
                                  size_t offset) const {
  if (offset > m_length - s2.m_length || !m_buffer || !s2.m_buffer) {
    // // (this->*update_event_F_p)(); //departed
    return false;
  }
  // // (this->*update_event_F_p)(); //departed
  return MJZ_STRnCMP(&m_buffer[offset], s2.m_buffer, s2.m_length) == 0;
}
bool basic_mjz_String::endsWith(const basic_mjz_String &s2) const {
  if (m_length < s2.m_length || !m_buffer || !s2.m_buffer) {
    // // (this->*update_event_F_p)(); //departed
    return false;
  }
  // // (this->*update_event_F_p)(); //departed
  return MJZ_STRnCMP(&m_buffer[m_length - s2.m_length], s2.m_buffer,
                     s2.m_length) == 0;
}
/*********************************************/
/* Character Access */
/*********************************************/
char basic_mjz_String::charAt(size_t loc) const {
  // // (this->*update_event_F_p)(); //departed
  return operator[](loc);
}
void mjz_Str::setCharAt(size_t loc, char c) {
  if (loc < m_length) {
    m_buffer[loc] = c;
  }
  // (this->*update_event_F_p)(); //departed
}
char basic_mjz_String::charAt(int64_t loc) const {
  // // (this->*update_event_F_p)(); //departed
  loc = signed_index_to_unsigned(loc);
  return operator[](loc);
}
void mjz_Str::setCharAt(int64_t loc, char c) {
  loc = signed_index_to_unsigned(loc);
  if ((size_t)loc < m_length) {
    m_buffer[loc] = c;
  }
  // (this->*update_event_F_p)(); //departed
}
char &mjz_Str::operator[](size_t index) {
  static char dummy_writable_char;
  if (index >= m_length || !m_buffer) {
    dummy_writable_char = 0;
    // (this->*update_event_F_p)(); //departed
    return dummy_writable_char;
  }
  // (this->*update_event_F_p)(); //departed
  return m_buffer[index];
}
char basic_mjz_String::operator[](size_t index) const {
  if (index >= m_length || !m_buffer) {
    // // (this->*update_event_F_p)(); //departed
    return 0;
  }
  // // (this->*update_event_F_p)(); //departed
  return m_buffer[index];
}
char &mjz_Str::operator[](int64_t index) {
  if (index < 0) {
    index = signed_index_to_unsigned(index);
  }
  static char dummy_writable_char;
  if ((size_t)index >= m_length || !m_buffer) {
    dummy_writable_char = 0;
    // (this->*update_event_F_p)(); //departed
    return dummy_writable_char;
  }
  // (this->*update_event_F_p)(); //departed
  return m_buffer[index];
}
char basic_mjz_String::operator[](int64_t index) const {
  if (index < 0) {
    index = signed_index_to_unsigned(index);
  }
  if ((size_t)index >= m_length || !m_buffer) {
    // // (this->*update_event_F_p)(); //departed
    return 0;
  }
  // // (this->*update_event_F_p)(); //departed
  return m_buffer[index];
}
void basic_mjz_String::getBytes(unsigned char *buf, size_t bufsize,
                                size_t index) const {
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
/*********************************************/
/* Search */
/*********************************************/
int64_t basic_mjz_String::indexOf(char c) const { return indexOf(c, 0); }
int64_t basic_mjz_String::indexOf(char ch, size_t fromIndex) const {
  if (fromIndex >= m_length) {
    return -1;
  }
  const char *temp = strchr(m_buffer + fromIndex, length() + fromIndex, ch);
  if (temp == NULL) {
    return -1;
  }
  return (int64_t)(temp - m_buffer);
}
int64_t basic_mjz_String::indexOf(const mjz_Str &s2) const {
  return indexOf(s2, 0);
}
int64_t basic_mjz_String::indexOf(const mjz_Str &s2, size_t fromIndex) const {
  return indexOf_cstr(s2.c_str(), s2.length(), fromIndex);
}
int64_t basic_mjz_String::indexOf_cstr(const char *c_str_, size_t len_str,
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
int64_t basic_mjz_String::lastIndexOf(char theChar) const {
  return lastIndexOf(theChar, m_length - 1);
}
int64_t basic_mjz_String::lastIndexOf(char ch, size_t fromIndex) const {
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
}
int64_t basic_mjz_String::lastIndexOf(const mjz_Str &s2) const {
  return lastIndexOf(s2, m_length - s2.m_length);
}
int64_t basic_mjz_String::lastIndexOf_cstr(const char *cstr__, size_t length__,
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
int64_t basic_mjz_String::lastIndexOf(const mjz_Str &s2,
                                      size_t fromIndex) const {
  return lastIndexOf_cstr(s2.c_str(), s2.length(), fromIndex);
}
mjz_Str basic_mjz_String::substring_beg_n(size_t beginIndex,
                                          size_t number) const {
  size_t endIndex = beginIndex + number;
  if (!number || length() < endIndex) return mjz_Str();
  return substring(beginIndex, beginIndex + number);
}
mjz_Str basic_mjz_String::substring(size_t beginIndex) const {
  return substring(beginIndex, m_length);
}
mjz_Str basic_mjz_String::substring_beg_n(int beginIndex, int number) const {
  return substring_beg_n((int64_t)beginIndex, (int64_t)number);
}
mjz_Str basic_mjz_String::substring(int beginIndex, int endIndex) const {
  return substring((int64_t)beginIndex, (int64_t)endIndex);
};
mjz_Str basic_mjz_String::substring(int beginIndex) const {
  return substring((int64_t)beginIndex);
}
mjz_Str basic_mjz_String::substring_beg_n(unsigned int beginIndex,
                                          unsigned int number) const {
  return substring_beg_n((size_t)beginIndex, (size_t)number);
}
mjz_Str basic_mjz_String::substring_beg_n(int64_t beginIndex,
                                          size_t number) const {
  beginIndex = signed_index_to_unsigned(beginIndex);
  return substring_beg_n((size_t)beginIndex, number);
}
mjz_Str basic_mjz_String::substring(int64_t beginIndex,
                                    int64_t endIndex) const {
  beginIndex = signed_index_to_unsigned(beginIndex);
  endIndex = signed_index_to_unsigned(endIndex);
  return substring((size_t)beginIndex, (size_t)endIndex);
}
mjz_Str basic_mjz_String::substring(int64_t beginIndex) const {
  beginIndex = signed_index_to_unsigned(beginIndex);
  return substring((size_t)beginIndex, length());
}
mjz_Str basic_mjz_String::substring(size_t left, size_t right) const {
  const char *c_str_out{};
  size_t len_out{};
  if (!substring_give_ptr(left, right, c_str_out, len_out)) return mjz_Str();
  mjz_Str out;
  out.copy(c_str_out, len_out);
  // // (this->*update_event_F_p)(); //departed
  return out;
}
size_t basic_mjz_String::signed_index_to_unsigned(int64_t input) const {
  if (0 <= input) {
    return input;
  }
  return input + length();
}
const char *basic_mjz_String::substring_give_ptr(int64_t left, int64_t right,
                                                 const char *&c_str_out,
                                                 size_t &len_out) const {
  return substring_give_ptrULL(signed_index_to_unsigned(left),
                               signed_index_to_unsigned(right), c_str_out,
                               len_out);
}
const char *basic_mjz_String::substring_give_ptrULL(size_t left, size_t right,
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
/*********************************************/
/* Modification */
/*********************************************/
mjz_Str &mjz_Str::insert(size_t pos, const mjz_Str &other) {
  return insert(pos, other, 0, length());
}
mjz_Str &mjz_Str::insert(size_t pos, const mjz_Str &other, size_t subpos,
                         size_t sublen) {
  return insert(pos, other.c_str() + subpos, sublen);
}
mjz_Str &mjz_Str::insert(size_t pos, const char *s, size_t n) {
  mjz_Str buffer_str_ = substring(0, pos);
  buffer_str_.write(s, n);
  buffer_str_ += substring(pos, length());
  return (*this = std::move(buffer_str_));
}
void mjz_Str::replace(char find, char replace_) {
  if (!m_buffer) {
    // (this->*update_event_F_p)(); //departed
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    if (*p == find) {
      *p = replace_;
    }
  }
  // (this->*update_event_F_p)(); //departed
}
// namespace mjz_ard
void mjz_Str::replace(const mjz_Str &find, const mjz_Str &replace_) {
  replace(find.c_str(), find.length(), replace_.c_str(), replace_.length());
}
void mjz_Str::remove(size_t index) {
  // Pass the biggest integer as the count. The remove method
  // below will take care of truncating it at the end of the
  // string.
  remove(index, (size_t)-1);
  // (this->*update_event_F_p)(); //departed
}
void mjz_Str::remove(size_t index, size_t count) {
  if (index >= m_length) {
    // (this->*update_event_F_p)(); //departed
    return;
  }
  if (count <= 0) {
    // (this->*update_event_F_p)(); //departed
    return;
  }
  if (count > m_length - index) {
    count = m_length - index;
  }
  char *writeTo = m_buffer + index;
  m_length = m_length - count;
  memmove(writeTo, m_buffer + index + count, m_length - index);
  m_buffer[m_length] = 0;
  // (this->*update_event_F_p)(); //departed
}
mjz_Str &mjz_Str::erase(size_t pos_, size_t len_) {
  if (len_ <= pos_ || length() < (len_ + pos_)) goto _end__;
  remove((size_t)pos_, (size_t)min(len_, length() - pos_));
_end__:
  return *this;
}
mjz_Str::iterator mjz_Str::erase(iterator p) {
  if (p < begin() || end() < p) return begin();
  size_t index_ = p - begin();
  remove(index_, 1);
  return p;
}
mjz_Str::iterator mjz_Str::erase(iterator first, iterator last) {
  if ((last <= first) || (end() < last) || (first < begin())) return begin();
  size_t index_first = first - begin();
  size_t index_last = last - begin();
  remove(index_first, index_last - index_first);
  return first;
}
mjz_Str &mjz_Str::erase_from_f_to_l(size_t first, size_t last) {
  first = signed_index_to_unsigned(first);
  last = signed_index_to_unsigned(last);
  if ((last <= first) || (length() < last)) return *this;
  remove(first, last - first);
  return *this;
}
void mjz_Str::toLowerCase(void) {
  if (!m_buffer) {
    // (this->*update_event_F_p)(); //departed
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    *p = (char)tolower(*p);
  }
  // (this->*update_event_F_p)(); //departed
}
void mjz_Str::toUpperCase(void) {
  if (!m_buffer) {
    // (this->*update_event_F_p)(); //departed
    return;
  }
  for (char *p = m_buffer; *p; p++) {
    *p = (char)toupper(*p);
  }
  // (this->*update_event_F_p)(); //departed
}
void mjz_Str::trim(void) {
  if (!m_buffer || m_length == 0) {
    // (this->*update_event_F_p)(); //departed
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
  // (this->*update_event_F_p)(); //departed
}
/*********************************************/
/* Parsing / Conversion */
/*********************************************/
long basic_mjz_String::toInt(void) const {
  if (m_buffer) {
    try {
      // // (this->*update_event_F_p)(); //departed
      return std::stol(m_buffer);
    } catch (...) {
      // // (this->*update_event_F_p)(); //departed
      return 0;
    }
  }
  // // (this->*update_event_F_p)(); //departed
  return 0;
}
long long basic_mjz_String::toLL(void) const {
  if (m_buffer) {
    return to_LL();
  }
  return 0;
}
long long basic_mjz_String::to_LL(int radix, bool *had_error,
                                  uint8_t error_level) const {
  if (m_buffer) {
    return C_STR_to_LL(c_str(), (uint8_t)min(length(), (uint64_t)255), radix,
                       had_error, error_level);
  }
  return 0;
}
float basic_mjz_String::toFloat(void) const {
  // // (this->*update_event_F_p)(); //departed
  return float(toDouble());
}
double basic_mjz_String::toDouble(void) const {
  if (m_buffer) {
    try {
      // // (this->*update_event_F_p)(); //departed
      return std::stod(m_buffer);
    } catch (...) {
      // // (this->*update_event_F_p)(); //departed
      return 0;
    }
  }
  // // (this->*update_event_F_p)(); //departed
  return 0;
}
void *mjz_Str::do_this_for_me(function_ptr function_ptr_, void *x) {
  // (this->*update_event_F_p)(); //departed
  return function_ptr_(*this, x);
}
const mjz_Str &mjz_Str::get_shift_op_rc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_shift_op_r() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_shift_op_lc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_shift_op_l() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_shift_op_r_sc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_shift_op_r_s() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_shift_op_l_sc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_shift_op_l_s() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_s_shift_op_rc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_s_shift_op_r() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_s_shift_op_lc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_s_shift_op_l() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_s_shift_op_r_sc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_s_shift_op_r_s() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
const mjz_Str &mjz_Str::get_s_shift_op_l_sc() const {
  // // (this->*update_event_F_p)(); //departed
  return (const mjz_Str &)*this;
}
mjz_Str &mjz_Str::get_s_shift_op_l_s() {
  // (this->*update_event_F_p)(); //departed
  return *this;
}
mjz_Str &mjz_Str::operator>>(mjz_Str &typeing) {
  // typeing.get_shift_op_r() += get_s_shift_op_r();
  helper__op_shift_input_(*this, get_s_shift_op_r(), typeing.get_shift_op_r());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_r();
}
mjz_Str &mjz_Str::operator>>(mjz_Str *typeing) {
  // typeing->get_shift_op_r() += get_s_shift_op_r();
  helper__op_shift_input_(*this, get_s_shift_op_r(), typeing->get_shift_op_r());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_r();
}
const mjz_Str &mjz_Str::operator>>(mjz_Str &typeing) const {
  // typeing.get_shift_op_r() += get_s_shift_op_rc();
  helper__op_shift_input_(*this, get_s_shift_op_rc(), typeing.get_shift_op_r());
  // // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_rc();
}
const mjz_Str &mjz_Str::operator>>(mjz_Str *typeing) const {
  // typeing->get_shift_op_r() += get_s_shift_op_rc();
  helper__op_shift_input_(*this, get_s_shift_op_rc(),
                          typeing->get_shift_op_r());
  // // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_rc();
}
mjz_Str &mjz_Str::operator<<(mjz_Str &typeing) {
  // get_s_shift_op_l() += typeing.get_shift_op_l();
  helper__op_shift_input_(*this, typeing.get_shift_op_l(), get_s_shift_op_l());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_l();
}
mjz_Str &mjz_Str::operator<<(mjz_Str *typeing) {
  // get_s_shift_op_l() += typeing->get_shift_op_l();
  helper__op_shift_input_(*this, typeing->get_shift_op_l(), get_s_shift_op_l());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_l();
}
mjz_Str &mjz_Str::operator<<(const mjz_Str &typeing) {
  // get_s_shift_op_l() += typeing.get_shift_op_lc();
  helper__op_shift_input_(*this, typeing.get_shift_op_lc(), get_s_shift_op_l());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_l();
}
mjz_Str &mjz_Str::operator<<(mjz_Str &&typeing) {
  // get_s_shift_op_l() += typeing.get_shift_op_lc();
  helper__op_shift_input_(*this, typeing.get_shift_op_lc(), get_s_shift_op_l());
  // (this->*update_event_F_p)(); //departed
  return get_s_shift_op_l();
}
#ifndef Arduino
// void mjz_Str::update_event() {
// update_event_ard_string();
// }
// void mjz_Str::update_event_ard_string() {
// std::string_view::operator=(buffer);//depatred
//}
#else
/*
void mjz_Str::update_event() {
 return;
}
void mjz_Str::update_event_ard_string() {
 return;
}
*/
#endif
mjz_Str mjz_Str::string_do_interpret() {
  mjz_Str out_str;
  str_helper__op_shift_input_(out_str, *this);
  return out_str;
}
void mjz_Str::string_do_interpret(mjz_Str &instr) {
  str_helper__op_shift_input_(*this, instr);
}
char *&mjz_Str::buffer_ref() {
  if (!m_capacity && !m_buffer) {
    return m_buffer;
  }
  if (m_capacity < (m_length + 1)) {
    reserve(m_capacity + 1, 1);
  }
  if (m_buffer) m_buffer[m_length] = '\0';
  return m_buffer;
}
void str_helper__op_shift_input_(mjz_Str &rhs, mjz_Str &CIN) {
  helper__op_shift_input_(rhs, CIN, rhs);
}
const mjz_Str &helper__op_shift_input_(const mjz_Str &rhs, const mjz_Str &CIN,
                                       mjz_Str &get_shift_op_s) {
  if (CIN.is_blank()) return CIN;
  //
  const char *CIN_c_str = CIN.c_str();
  size_t CURunt_index_{};
  size_t my_bfr_obj_length = CIN.length() + 4;
  malloc_wrapper my_bfr_obj_ptr(my_bfr_obj_length + 5, 0);
  char *bfr = (char *)my_bfr_obj_ptr.get_ptr();  // char bfr[2050];
  uint8_t is_reinterpreted{};
  constexpr uint8_t is_reinterpreted_and_is_int = 2;
  uint8_t value_reinterpreted_and_is_int{};
  auto &reinterpret_char_char_ref =
      rhs.drived_mjz_Str_DATA_storage_Obj_ptr->reinterpret_char_char;
  auto continue_event_is_reinterpreted_and_is_int = [&](uint16_t &i) -> bool {
    if (bfr[i] == reinterpret_char_char_ref) {
      bfr[i] = value_reinterpreted_and_is_int;
      is_reinterpreted = 0;
    } else {
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
        return 1;  // continue;
      }
    }
    return 0;
  };
  auto continue_event_is_reinterpreted =
      [&](uint16_t &i, uint8_t &is_reinterpreted_do_not_forbid) -> bool {
    int8_t vlal_bf = rhs.char_to_int_for_string(bfr[i]);
    if (vlal_bf == -1) {
      is_reinterpreted = 0;
      if (!rhs.char_to_char_for_reinterpret(bfr[i])) {
        i--;
        return 1;  // continue;
      } else {
        is_reinterpreted_do_not_forbid = 1;
      }
    } else {
      value_reinterpreted_and_is_int = vlal_bf;
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
std::istream &helper__op_shift_input_(const mjz_Str &rhs, std::istream &CIN,
                                      mjz_Str &get_shift_op_s) {
  char bfr[2050]{};
  uint8_t is_reinterpreted{};
  constexpr uint8_t is_reinterpreted_and_is_int = 2;
  uint8_t value_reinterpreted_and_is_int{};
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
    } else if (rhs.drived_mjz_Str_DATA_storage_Obj_ptr->reinterpret_char_char ==
               bfr[i]) {
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
// stream stuff
size_t mjz_Str::write(uint8_t c) {
  mjz_Str::operator+=(c);
  return 1;
}
size_t mjz_Str::write(const uint8_t *buf, size_t size_) {
  mjz_Str::operator+=(mjz_Str(buf, (size_t)size_));
  return size_;
}
size_t mjz_Str::write(const char *buf, size_t size_) {
  mjz_Str::operator+=(mjz_Str(buf, (size_t)size_));
  return size_;
}
size_t mjz_Str::write(const char *buf) {
  mjz_Str::operator+=(buf);
  return strlen(buf);
}
int64_t mjz_Str::availableLL() { return length(); }
int mjz_Str::read() {
  if (available()) {
    char x = charAt(0ULL);
    remove(0, 1);
    return x;
  }
  return -1;
}
int mjz_Str::peek() {
  int64_t len_ = length();
  if (len_ == 0) {
    return -1;
  } else {
    return operator[](0ULL);
  }
}
size_t mjz_Str::read(uint8_t *buf, size_t size_) {
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
  // buf[size_]= 0;
  remove(0, (size_t)size_);
  return size_;
}
void mjz_Str::flush() {}
void mjz_Str::begin(unsigned long) {}
void mjz_Str::begin(unsigned long, uint16_t) {}
// void mjz_Str::end() {}
mjz_Str &mjz_Str::ULL_LL_to_str_add(uint64_t value, int radix, bool is_signed,
                                    bool force_neg) {
  this->operator+=(ULL_LL_to_str(value, radix, is_signed, force_neg));
  return *this;
}
mjz_Str &mjz_Str::ULL_LL_to_str_rep(uint64_t value, int radix, bool is_signed,
                                    bool force_neg) {
  operator=(empty_STRING_C_STR);
  reserve(70, 1);
  auto ptr_ = b_U_lltoa(value, buffer_ref(), radix, is_signed, force_neg);
  if (!ptr_) return *this;
  addto_length((size_t)strlen(ptr_), 1);
  return *this;
}
const char mjz_Str::forbiden_chars_cnt[forbiden_chars_cnt_size] = {
    '\r', '\n', 0};  //{ 3, 4, 23, 24, 25, 27, 28, 29, 30, 31, 127, 0 };
const char
    mjz_Str::the_in_reinterpreted_char_cca[the_reinterpreted_char_cca_size] = {
        'n',
        'r',
        'a',
        'b',
        't',
        'v',
        'f',
        'e'  // world wide
        ,
        's',
        '@',
        '\\',
        '\"',
        '\'',
        'p',
        'c',
        '\0'};  // just me
const char
    mjz_Str::the_out_reinterpreted_char_cca[the_reinterpreted_char_cca_size] = {
        '\n', '\r', '\a', '\b', '\t', '\v',
        '\f',                                                  // world wide
        'e',  ' ',  '@',  '\\', '\"', '\'', '"', '\\', '\0'};  // just me
bool is_blank_characteres_default(char x_char_) {
  return !((get_num_from_char(x_char_) != 127) ||
           ((x_char_) != '\n' && (x_char_) != '\r' && (x_char_) != ' ' &&
            isPrintable(x_char_)));
}
char char_to_char_for_reinterpret_fnc_ptr_default(char x_char_) {
  for (int64_t i{}; i < mjz_Str::the_reinterpreted_char_cca_size; i++) {
    if (x_char_ == mjz_Str::the_in_reinterpreted_char_cca[i]) {
      return mjz_Str::the_out_reinterpreted_char_cca[i];
    }
  }
  return x_char_;
}
bool is_forbiden_character_default(char x_char_) {
  for (int64_t i{}; i < mjz_Str::forbiden_chars_cnt_size; i++) {
    if (x_char_ == mjz_Str::forbiden_chars_cnt[i]) {
      return 1;
    }
  }
  return 0;
}
bool mjz_Str::is_blank() const {
  for (size_t index_i{}; index_i < m_length; index_i++) {
    if (!drived_mjz_Str_DATA_storage_Obj_ptr->is_blank_character(
            m_buffer[index_i])) {
      return 0;
    }
  }
  return 1;
}
int8_t mjz_Str::char_to_int_for_string(char c_char) {
  if (c_char < '0' || c_char > '9') {
    return -1;
  }
  return (int8_t)(c_char - '0');
}
bool mjz_Str::change_reinterpret_char_char(char x) {
  if (x == 0) {
    return 0;
  }
  drived_mjz_Str_DATA_storage_Obj_ptr_set()->reinterpret_char_char = x;
  return 1;
}
bool mjz_Str::char_to_char_for_reinterpret(char &c_char) const {
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
bool mjz_Str::is_forbiden(char x) const {
  if (!x) {
    return 1;
  }
  return drived_mjz_Str_DATA_storage_Obj_ptr->is_forbiden_character(x);
}
mjz_Str mjz_Str::create_mjz_Str_char_array(size_t size_, char filler,
                                           bool do_fill) {
  mjz_Str ret_val;
  ret_val.addto_length(size_, 1);
  char *ret_val_bufer = (char *)ret_val;
  if (do_fill)
    for (size_t i{}; i < size_; i++) {
      ret_val_bufer[i] = filler;
    }
  return ret_val;
}
mjz_Str mjz_Str::create_mjz_Str_2D_char_array(size_t size_col, size_t size_row,
                                              char filler, bool do_fill) {
  size_t size_of_my_ptrs = (size_t)sizeof(void *) * size_col;
  size_t total_size_ = (size_row + 1) * size_col + size_of_my_ptrs;
  mjz_Str my_buufer_ =
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
void mjz_Str::replace(const char *find_cstr, size_t find_count,
                      const char *replace_cstr, size_t replace_count) {
  if (m_length == 0 || find_count == 0) {
    // (this->*update_event_F_p)(); //departed
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
      // (this->*update_event_F_p)(); //departed
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
      // (this->*update_event_F_p)(); //departed
      return;
    }
    if (size_ > m_capacity && !changeBuffer(size_, 0)) {
      // (this->*update_event_F_p)(); //departed
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
  // (this->*update_event_F_p)(); //departed
}
mjz_Str &mjz_Str::operator-=(const mjz_Str &othr_) {
  int64_t index_of_remove = lastIndexOf(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
mjz_Str &mjz_Str::operator-=(mjz_Str &&othr_) {
  int64_t index_of_remove = lastIndexOf(othr_);
  if (index_of_remove != -1) {
    remove(index_of_remove, othr_.length());
  }
  return *this;
}
mjz_Str &mjz_Str::operator*=(unsigned int number_of_it) {
  mjz_Str temperory_str;
  for (size_t i{}; i < number_of_it; i++) {
    temperory_str += *this;
  }
  *this = std::move(temperory_str);
  return *this;
}
mjz_Str mjz_Str::operator*(unsigned int number_of_it) {
  mjz_Str lhs = mjz_Str(*this);
  return lhs.operator*=(number_of_it);
}
mjz_Str &mjz_Str::operator/=(const mjz_Str &othr_) {
  replace(othr_, othr_.length(), empty_STRING_C_STR, 0);
  return *this;
}
mjz_Str &mjz_Str::operator/=(mjz_Str &&othr_) {
  replace(othr_, othr_.length(), empty_STRING_C_STR, 0);
  return *this;
}
mjz_Str &mjz_Str::operator++() {
  this->println();
  return *this;
}
mjz_Str mjz_Str::operator++(int) {
  mjz_Str tmp(*this);
  ++(*this);
  return tmp;
}
mjz_Str &mjz_Str::operator--() {
  this->read();
  return *this;
}
mjz_Str mjz_Str::operator--(int) {
  mjz_Str tmp(*this);
  --(*this);
  return tmp;
}
size_t basic_mjz_String::UN_ORDERED_compare(const basic_mjz_String &s) const {
  return UN_ORDERED_compare(s.m_buffer, s.m_length);
}
size_t basic_mjz_String::UN_ORDERED_compare(const char *s, size_t s_len) const {
  const unsigned char *ucs_ = (const unsigned char *)s;
  const unsigned char *ucbuffer_ = (const unsigned char *)this->m_buffer;
  size_t number_of_not_right{};
  malloc_wrapper my_num_buffer(static_cast<size_t>(2) * 256 * sizeof(size_t));
  size_t *NUMBER_OF_EACH_char_array[2] = {
      (my_num_buffer.get_ptr_as<size_t>()),
      (my_num_buffer.get_ptr_as<size_t>() + 256)};
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
hash_sha_512 hash_msg_to_sha_512(const char *dev_passwoed,
                                 const size_t dev_passwoedLength,
                                 mjz_Str output_name) {
  hash_sha_512 rtrn{};
  uint8_t(&shaResult)[64] = rtrn.Hashed;
  for (int64_t i{}; i < 64; i++) {
    shaResult[i] = 0;
  }
#ifdef Arduino
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA512;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)dev_passwoed,
                    dev_passwoedLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
  if (output_name != "Null") {
    Serial.print("const char " + output_name + " [] = { ");
    Serial.print((int)shaResult[0]);
    for (int64_t i = 1; i < sizeof(shaResult); i++) {
      Serial.print(",");
      Serial.print((int)shaResult[i]);
    }
    Serial.println(" };");
  }
#endif
  return rtrn;
}
hash_sha_512 hash_msg_to_sha_512(const char *dev_passwoed,
                                 const size_t dev_passwoedLength) {
  hash_sha_512 rtrn{};
  uint8_t(&shaResult)[64] = rtrn.Hashed;
  for (int64_t i{}; i < 64; i++) shaResult[i] = 0;
#ifdef Arduino
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA512;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)dev_passwoed,
                    dev_passwoedLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
#endif
  return rtrn;
}
hash_sha_512 hash_msg_to_sha_512_n(
    const char *dev_passwoed, const size_t dev_passwoedLength, uint8_t n,
    const mjz_Str &output_name) {  // intended copy
  if (n == 0) {
    return hash_msg_to_sha_512(dev_passwoed, dev_passwoedLength, output_name);
  }
  hash_sha_512 ret = hash_msg_to_sha_512(dev_passwoed, dev_passwoedLength);
  for (int64_t i{}; i < ((int64_t)n - 1); i++) {
    ret = hash_msg_to_sha_512((char *)ret.Hashed, 64);
  }
  return hash_msg_to_sha_512((char *)ret.Hashed, 64, output_name);
}
hash_sha_512 hash_msg_to_sha_512_n(const char *dev_passwoed,
                                   const size_t dev_passwoedLength,
                                   uint8_t n) {  // intended copy
  hash_sha_512 ret = hash_msg_to_sha_512(dev_passwoed, dev_passwoedLength);
  for (int64_t i{}; i < n; i++) {
    ret = hash_msg_to_sha_512((char *)ret.Hashed, 64);
  }
  return ret;
}
hash_sha_512 mjz_Str::mjz_hash(uint8_t n) const {
  return hash_msg_to_sha_512_n(c_str(), length(), n);
}
long long unsigned int mjz_millis() {
  static uint64_t my_time[2] = {0, 0};
  uint64_t ARD_millis_vr = ARD_millis();
  if (my_time[0] > ARD_millis_vr) {
    my_time[1] += 1;  //+=1;
  }
  my_time[0] = ARD_millis_vr;
  return *((uint64_t *)my_time);
}
void Set_nth_bit_andret32(void *data, uint64_t nthbt,
                          bool set_to) {  // set_to is a bool
  uint64_t &data_in_array = ((uint64_t *)data)[nthbt / 32];
  const uint8_t ofset = nthbt % 32;
  data_in_array &= static_cast<uint64_t>(0) << ofset;
  if (set_to) {
    data_in_array |= (static_cast<uint64_t>(1) << ofset);
  }
}
bool Get_nth_bit_andret32(const void *data,
                          uint64_t nthbt) {  // set_to is a bool
  uint64_t data_in_array = ((uint64_t *)data)[nthbt / 32];
  const uint8_t ofset = nthbt % 32;
  data_in_array &= static_cast<uint64_t>(1) << ofset;
  if (data_in_array) {
    return 1;
  }
  return 0;
}
void tgl_nth_bit_andret32(void *data, uint64_t nthbt) {  // set_to is a bool
  uint64_t *data_in_array = ((uint64_t *)data) + (nthbt / 32);
  const uint8_t ofset = nthbt % 32;
  *data_in_array ^= (static_cast<uint64_t>(1) << ofset);
}
void Set_nth_bit_andret8(void *data, uint64_t nthbt,
                         bool set_to) {  // set_to is a bool
  uint8_t *data_in_array = ((uint8_t *)data) + (nthbt / 8);
  const uint8_t ofset = nthbt % 8;
  *data_in_array &= (1 << ofset) ^ (-1);  // number &= ~(1UL << ofset);
  if (set_to) {
    *data_in_array = *data_in_array | (1 << ofset);
  }
}
void tgl_nth_bit_andret8(void *data, uint64_t nthbt) {  // set_to is a bool
  uint8_t *data_in_array = ((uint8_t *)data) + (nthbt / 8);
  const uint8_t ofset = nthbt % 8;
  *data_in_array ^= (1 << ofset);
}
bool Get_nth_bit_andret8(const void *data,
                         uint64_t nthbt) {  // set_to is a bool
  uint64_t data_in_array = *((uint8_t *)data) + (nthbt / 8);
  const uint8_t ofset = nthbt % 8;
  data_in_array &= (uint64_t)1 << ofset;
  if (data_in_array) {
    return 1;
  }
  return 0;
}
UINT64_X2_32_t mjz_random() {
  UINT64_X2_32_t ret{};
  ret.data32[0] = esp_random();
  ret.data32[1] = esp_random();
  return ret;
}
bool get_random_chanch_bool(double chance_var) {
  double random_var = (double)(abs(mjz_random().data64) % 1000001);
  random_var /= 1000000;
  return random_var <= chance_var;
}
std::shared_ptr<mjz_Str_DATA_storage_cls>
    mjz_Str::main_mjz_Str_DATA_storage_Obj_ptr =
        mjz_Str_DATA_storage_cls::create();
std::shared_ptr<mjz_Str_DATA_storage_cls>
    &mjz_Str::drived_mjz_Str_DATA_storage_Obj_ptr_set() {
  if (!did_drived_mjz_Str_DATA_storage_Obj_ptr_set) {
    drived_mjz_Str_DATA_storage_Obj_ptr = mjz_Str_DATA_storage_cls::create();
    did_drived_mjz_Str_DATA_storage_Obj_ptr_set = 1;
  }
  return drived_mjz_Str_DATA_storage_Obj_ptr;
}
// stream part
size_t mjz_Str::print(const __FlashStringHelper *ifsh) {
  return print(reinterpret_cast<const char *>(ifsh));
}
size_t mjz_Str::print(const mjz_Str &s) { return write(s.c_str(), s.length()); }
size_t mjz_Str::print(const char *str) { return write(str); }
size_t mjz_Str::print(char c) { return write(c); }
size_t mjz_Str::print(unsigned char b, int base) {
  return print((unsigned long)b, base);
}
size_t mjz_Str::print(int n, int base) { return print((long)n, base); }
size_t mjz_Str::print(unsigned int n, int base) {
  return print((unsigned long)n, base);
}
size_t mjz_Str::print(long n, int base) {
  if (base == 0) {
    return write(n);
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
size_t mjz_Str::print(unsigned long n, int base) {
  if (base == 0) {
    return write(n);
  } else {
    return printNumber(n, static_cast<uint8_t>(base));
  }
}
size_t mjz_Str::print(long long n, int base) {
  if (base == 0) {
    return write(n);
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
size_t mjz_Str::print(unsigned long long n, int base) {
  if (base == 0) {
    return write(n);
  } else {
    return printULLNumber(n, static_cast<uint8_t>(base));
  }
}
size_t mjz_Str::print(double n, int digits) { return printFloat(n, digits); }
size_t mjz_Str::println(const __FlashStringHelper *ifsh) {
  size_t n = print(ifsh);
  n += println();
  return n;
}
size_t mjz_Str::println() { return write("\r\n"); }
size_t mjz_Str::println(const mjz_Str &s) {
  size_t n = print(s);
  n += println();
  return n;
}
size_t mjz_Str::println(const char *c) {
  size_t n = print(c);
  n += println();
  return n;
}
size_t mjz_Str::println(char c) {
  size_t n = print(c);
  n += println();
  return n;
}
size_t mjz_Str::println(unsigned char b, int base) {
  size_t n = print(b, base);
  n += println();
  return n;
}
size_t mjz_Str::println(int num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(unsigned int num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(unsigned long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(long long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(unsigned long long num, int base) {
  size_t n = print(num, base);
  n += println();
  return n;
}
size_t mjz_Str::println(double num, int digits) {
  size_t n = print(num, digits);
  n += println();
  return n;
}
// Private Methods /////////////////////////////////////////////////////////////
size_t mjz_Str::printNumber(unsigned long n, uint8_t base) {
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
// size_t mjz_Str::printULLNumber(unsigned long long n, uint8_t base)
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
size_t mjz_Str::printULLNumber(unsigned long long n64, uint8_t base) {
  // if limited to base 10 and 16 the bufsize can be 20
  char buf[64]{};
  uint8_t i = 0;
  uint8_t innerLoops = 0;
  // Special case workaround
  // https://github.com/mjz_ard/ArduinoCore-API/issues/178
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
size_t mjz_Str::printFloat(double number, int digits) {
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
  // mjz_Str the decimal point, but only if there are digits beyond
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
int mjz_Str::timedRead() {
  unsigned long _startMillis;  // used for timeout measurement
  int c;
  _startMillis = millis();
  do {
    c = read();
    if (c >= 0) {
      return c;
    }
  } while (millis() - _startMillis <
           drived_mjz_Str_DATA_storage_Obj_ptr->_timeout);
  return -1;  // -1 indicates timeout
}
// private method to peek stream with timeout
int mjz_Str::timedPeek() {
  unsigned long _startMillis;  // used for timeout measurement
  int c;
  _startMillis = millis();
  do {
    c = peek();
    if (c >= 0) {
      return c;
    }
  } while (millis() - _startMillis <
           drived_mjz_Str_DATA_storage_Obj_ptr->_timeout);
  return -1;  // -1 indicates timeout
}
// returns peek of the next digit in the stream or -1 if timeout
// discards non-numeric characters
int mjz_Str::peekNextDigit(LookaheadMode lookahead, bool detectDecimal) {
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
void mjz_Str::setTimeout(
    unsigned long timeout) {  // sets the maximum number of milliseconds to wait
  drived_mjz_Str_DATA_storage_Obj_ptr_set()->_timeout = timeout;
}
// find returns true if the target string is found
bool mjz_Str::find(const char *target) {
  return findUntil(target, strlen(target), NULL, 0);
}
// reads data from the stream until the target string of given length is found
// returns true if target string is found, false if timed out
bool mjz_Str::find(const char *target, size_t length) {
  return findUntil(target, length, NULL, 0);
}
// as find but search ends if the terminator string is found
bool mjz_Str::findUntil(const char *target, const char *terminator) {
  return findUntil(target, strlen(target), terminator, strlen(terminator));
}
// reads data from the stream until the target string of the given length is
// found search terminated if the terminator string is found returns true if
// target string is found, false if terminated or timed out
bool mjz_Str::findUntil(const char *target, size_t targetLen,
                        const char *terminator, size_t termLen) {
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
long mjz_Str::parseInt(LookaheadMode lookahead, char ignore) {
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
    } else if (c >= '0' && c <= '9')  // is c a digit?
    {
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
float mjz_Str::parseFloat(LookaheadMode lookahead, char ignore) {
  bool isNegative = false;
  bool isFraction = false;
  double value = 0.0;
  int c;
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
        value = value + fraction * (c - '0');
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
 size_t mjz_Str::readBytes(char *buffer, size_t length)
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
size_t mjz_Str::readBytesUntil(char terminator, char *buffer_, size_t length) {
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
mjz_Str mjz_Str::readmjz_Str() {
  /*
  mjz_Str ret;
  int c = timedRead();
  while (c >= 0)
  {
  ret += (char)c;
  c = timedRead();
  }
  return ret;
  */
  mjz_Str ret = std::move(*this);  // be careful
  new (this) mjz_Str;              // be careful
  return ret;
}
mjz_Str mjz_Str::readmjz_StrUntil(char terminator) {
  mjz_Str ret;
  int c = timedRead();
  while (c >= 0 && (char)c != terminator) {
    ret += (char)c;
    c = timedRead();
  }
  return ret;
}
int mjz_Str::findMulti(struct mjz_Str::MultiTarget *targets, int tCount) {
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
typedef mjz_RingBufferN<SERIAL_BUFFER_SIZE> RingBuffer;
template <int N>
mjz_RingBufferN<N>::mjz_RingBufferN(void) {
  memset(_aucBuffer, 0, N);
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
#endif  // asdfghjklkjhgfdsasdfghjkjhgfdfghj
