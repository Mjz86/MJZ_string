
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

} // namespace mjz_ard
#endif
/*
 String library for Wiring & Arduino
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan. All rights reserved.
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
} // namespace mjz_ard
} // namespace std::chrono
#endif
#ifndef Arduino
unsigned long static time_MIlis_{};
unsigned long millis() {
 time_MIlis_ += 10;
 return time_MIlis_;
}
#endif
#ifdef Arduino
unsigned long long millisR() { return std::chrono::millisR(); }
#endif
namespace mjz_ard {

/*********************** FUNCTION DEFINITIONS ***********************/

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
char* dtostrf(double __val, signed char __width, unsigned char __prec,
    char* __s) {
 char buffer_for_not_overflowing[65]{};
 char string_format[227]{};
 char* ptr{string_format};
 //b_U_lltoa(value, (char*)ret_var, radix, is_signed, force_neg);
 *ptr++ = '%'; 
 ptr +=static_str_algo::strlen(b_U_lltoa(__width, ptr, 10, 0, 0));
 *ptr++ = '.';
 ptr += static_str_algo::strlen(b_U_lltoa(__prec, ptr, 10, 0, 0));
 *ptr++ = 'l';
 *ptr++ = 'f';
 sprintf_alt_(buffer_for_not_overflowing, 64, string_format, __val);
 memmove(__s, buffer_for_not_overflowing, __width);
 return __s;
}
#define end_of_transmission_char 4
#define null_char 0
char GET_CHAR_from_int(uint8_t intager_in, bool is_upper_case) {
 const char volatile static* const volatile alphabett_table_lower =
  "0123456789abcdefghijklmnopqrstuvwxyz\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0";
 const char volatile static* const volatile alphabett_table_upper =
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0";
 const char volatile* const volatile alphabett_table =
  (is_upper_case ? alphabett_table_upper : alphabett_table_lower);

 if (intager_in == 255) {
 return '-';
 } else if (36 < intager_in) {
 return ' ';
 }

 return (char)alphabett_table[intager_in];
}
char* b_U_lltoa(uint64_t value, char* BFR_buffer, int radix, bool is_signed,
    bool force_neg, bool is_upper_case) {
 bool IS_NEGITIVE__ =
  (is_signed && (*((int64_t*)&value) < 0) && ((radix == 10) || force_neg));

 if (IS_NEGITIVE__) {
 *((int64_t*)&value) =
  ((-1) * (*((int64_t*)&value))); // use a positive insted of the - sign
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
   buffer[k] = buffer_[j]; // when its done we reverse the string
   k++;
  }
  }

  break;
 }
 }

 buffer[number_of_numbers] = 0;

 if (IS_NEGITIVE__) {
 BFR_buffer[0] = '-';
 memmove(BFR_buffer + 1, buffer, number_of_numbers + 1); //+null
 } else {
 memmove(BFR_buffer, buffer, number_of_numbers + 1); //+null
 }

 return BFR_buffer;
}


uint8_t get_num_from_char(uint8_t in_CHAR_, bool* to_neg) {
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
 return (uint8_t)127; // note : 127 is delete in ascii
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
long long C_STR_to_LL(const char* buffer, uint8_t buffer_len, int radix,
      bool* had_error, uint8_t error_level) {
 if (!had_error) {
 had_error = &dummy_bool;
 }

 const double MAX_LL_IN_D = 9.223372036854775807e+18;
 uint8_t buffer_for_number[70]{};

 for (int64_t i{}; i < 70; i++) {
 buffer_for_number[i] = 127;
 }

 bool is_neg_bool{};
 bool stack_ovf_prtt = !MJZ_logic_BIT_CHECK(error_level, 0);
 bool LL_ovf_prtt = !MJZ_logic_BIT_CHECK(error_level, 1);
 bool len_ovf_prtt = (!MJZ_logic_BIT_CHECK(error_level, 2) && stack_ovf_prtt);

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
   HOLDER_does_not_do_any_thing; // TODO: V2007
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
int MJZ_STRCMP(const char* p1, const char* p2) {
 const unsigned char* s1 = (const unsigned char*)p1;
 const unsigned char* s2 = (const unsigned char*)p2;
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

const char
 mjz_ard::basic_mjz_String::forbiden_chars_cnt[forbiden_chars_cnt_size] = {
  '\r', '\n', 0};
const char mjz_ard::basic_mjz_String::the_in_reinterpreted_char_cca
 [mjz_ard::basic_mjz_String::the_reinterpreted_char_cca_size] = {
  'n',
  'r',
  'a',
  'b',
  't',
  'v',
  'f',
  'e' // world wide
  ,
  's',
  '@',
  '\\',
  '\"',
  '\'',
  'p',
  'c',
  '\0'}; // just me
const char mjz_ard::basic_mjz_String::the_out_reinterpreted_char_cca
 [the_reinterpreted_char_cca_size] = {'\n', '\r', '\a', '\b', '\t', '\v',
           '\f', // world wide
           'e', ' ', '@', '\\', '\"', '\'',
           '"', '\\', '\0'}; // just me
bool is_blank_characteres_default(char x_char_) {
 return !((mjz_ard::get_num_from_char(x_char_) != 127) ||
   ((x_char_) != '\n' && (x_char_) != '\r' && (x_char_) != ' ' &&
   mjz_ard::isPrintable(x_char_)));
}
char char_to_char_for_reinterpret_fnc_ptr_default(char x_char_) {
 for (int64_t i{};
  i < mjz_ard::static_str_algo::the_reinterpreted_char_cca_size; i++) {
 if (x_char_ ==
  mjz_ard::basic_mjz_String::the_in_reinterpreted_char_cca[i]) {
  return mjz_ard::basic_mjz_String::the_out_reinterpreted_char_cca[i];
 }
 }

 return x_char_;
}
bool is_forbiden_character_default(char x_char_) {
 for (int64_t i{}; i < mjz_ard::static_str_algo ::forbiden_chars_cnt_size;
  i++) {
 if (x_char_ == mjz_ard::basic_mjz_String ::forbiden_chars_cnt[i]) {
  return 1;
 }
 }

 return 0;
}

std::shared_ptr<mjz_Str_DATA_storage_cls> main_mjz_Str_DATA_storage_Obj_ptr =
 mjz_Str_DATA_storage_cls::create();
;
 


long long unsigned int mjz_millis() {
 static uint64_t my_time[2] = {0, 0};
 uint64_t ARD_millis_vr = mjz_ard::ARD_millis();

 if (my_time[0] > ARD_millis_vr) {
 my_time[1] += 1;
 }

 my_time[0] = ARD_millis_vr;
 return *((uint64_t*)my_time);
}
void Set_nth_bit_andret32(void* data, uint64_t nthbt,
       bool set_to) { // set_to is a bool
 uint64_t& data_in_array = ((uint64_t*)data)[nthbt / 32];
 const uint8_t ofset = nthbt % 32;
 data_in_array &= static_cast<uint64_t>(0) << ofset;

 if (set_to) {
 data_in_array |= (static_cast<uint64_t>(1) << ofset);
 }
}
bool Get_nth_bit_andret32(const void* data,
       uint64_t nthbt) { // set_to is a bool
 uint64_t data_in_array = ((uint64_t*)data)[nthbt / 32];
 const uint8_t ofset = nthbt % 32;
 data_in_array &= static_cast<uint64_t>(1) << ofset;

 if (data_in_array) {
 return 1;
 }

 return 0;
}
void tgl_nth_bit_andret32(void* data, uint64_t nthbt) { // set_to is a bool
 uint64_t* data_in_array = ((uint64_t*)data) + (nthbt / 32);
 const uint8_t ofset = nthbt % 32;
 *data_in_array ^= (static_cast<uint64_t>(1) << ofset);
}
void Set_nth_bit_andret8(void* data, uint64_t nthbt,
       bool set_to) { // set_to is a bool
 uint8_t* data_in_array = ((uint8_t*)data) + (nthbt / 8);
 const uint8_t ofset = nthbt % 8;
 *data_in_array &= (1 << ofset) ^ (-1); // number &= ~(1UL << ofset);

 if (set_to) {
 *data_in_array = static_cast<uint8_t>(*data_in_array | (1 << ofset));
 }
}
void tgl_nth_bit_andret8(void* data, uint64_t nthbt) { // set_to is a bool
 uint8_t* data_in_array = ((uint8_t*)data) + (nthbt / 8);
 const uint8_t ofset = nthbt % 8;
 *data_in_array ^= (1 << ofset);
}
bool Get_nth_bit_andret8(const void* data,
       uint64_t nthbt) { // set_to is a bool
 uint64_t data_in_array = *((uint8_t*)data) + (nthbt / 8);
 const uint8_t ofset = nthbt % 8;
 data_in_array &= (uint64_t)1 << ofset;

 if (data_in_array) {
 return 1;
 }

 return 0;
}
/*********************************************/
/* Search */
/*********************************************/







std::ostream& operator<<(std::ostream& CIN, const mjz_ard::SHA256_CTX& obj) {
 char buffer[1024]{};
 CIN << obj.copy_to_c_str(buffer, 1024);
 return CIN;
}

mjz_ard::UINT64_X2_32_t mjz_random() {
 mjz_ard::UINT64_X2_32_t ret{};
 ret.data32[0] = (int32_t)esp_random();
 ret.data32[1] = (int32_t)esp_random();
 return ret;
}
bool get_random_chanch_bool(double chance_var) {
 double random_var = (double)(abs(mjz_random().data64) % 1000001);
 random_var /= 1000000;
 return random_var <= chance_var;
}

char* ulltoa(uint64_t value, char* buffer, int radix) {
 return b_U_lltoa((uint64_t)value, buffer, radix, 0);
}
char* ultoa(uint32_t value, char* buffer, int radix) {
 return ulltoa((uint64_t)value, buffer, radix);
}

char* utoa(uint32_t value, char* buffer, int radix) {
 return ultoa(value, buffer, radix);
}

char* lltoa(long long value, char* buffer, int radix) {
 return b_U_lltoa((uint64_t)value, buffer, radix, 1);
}

char* ltoa(long value, char* buffer, int radix) {
 return lltoa((long long)value, buffer, radix);
}

char* itoa(int value, char* buffer, int radix) {
 return lltoa((long long)value, buffer, radix);
}

char* SHA256_CTX::copy_to_c_str(char* buf, size_t len) const {
 if (len < 1024) {
 return 0;
 }

 static_str_algo::memset(buf, 0, len);
 return to_c_string(buf);
}
char* mjz_ard::SHA256_CTX::to_c_string(char* buf_) const {
 char* buf = buf_;
 auto paste_str = [&](mjz_ard::mjz_str_view str) {
 static_str_algo::memmove(buf, str.data(), str.length());
 buf += str.length();
 };
 auto str_left = [&]() { return 1024 - (size_t)buf_ + (size_t)buf; };
 paste_str("const char ");
 paste_str("hash");
 paste_str(" [] = { ");
 buf += sprintf_s(buf, str_left(), "%d", (int)hashed_data[0]);

 for (int64_t i = 1; i < NumberOf(hashed_data); i++) {
 buf += sprintf_s(buf, str_left(), ",");
 buf += sprintf_s(buf, str_left(), "%d", (int)hashed_data[i]);
 }

 buf += sprintf_s(buf, str_left(), " };\n");
 return buf_;
}

















 uint32_t num_allocations;
static_str_algo::cpu_endian static_str_algo::my_endian = get_cpu_endian();
} // namespace mjz_ard

#endif // asdfghjklkjhgfdsasdfghjkjhgfdfghj
