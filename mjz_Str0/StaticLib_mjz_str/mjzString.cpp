
#include "pch.h"
//
#include "mjzString.hpp"
my_data_randomizer_union my_data_randomizer_uni;

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
constexpr inline long map(long x, long in_min, long in_max, long out_min,
                          long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#ifndef Arduino
unsigned long static time_MIlis_{};
  inline unsigned long millis() {
  time_MIlis_ += 10;
  return time_MIlis_;
}
#endif
#ifdef Arduino
constexpr inline unsigned long long millisR() { return std::chrono::millisR(); }
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

/*********************************************/
/* Search */
/*********************************************/
inline std::ostream& operator<<(std::ostream& CIN,
                                const mjz_ard::SHA256_CTX& obj) {
  char buffer[2 * 1025]{};
  CIN << obj.copy_to_c_str(buffer, 2 * 1024);
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

uint32_t num_allocations;
static_str_algo::cpu_endian static_str_algo::my_endian = get_cpu_endian();
}  // namespace mjz_ard

#endif  // asdfghjklkjhgfdsasdfghjkjhgfdfghj
