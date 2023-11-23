
#include "pch.h"
#include "mjzString.hpp"
#include "wrench.h"

  namespace mjz_ard {
  int run_code(
      have_mjz_ard_removed::mjz_str_view code,
      size_t stack_size = 1024);
  int run_byte_code(have_mjz_ard_removed::mjz_str_view code,
                    size_t stack_size = 1024);


[[nodiscard]] have_mjz_ard_removed::mjz_Str get_byte_code(
      have_mjz_ard_removed::mjz_str_view code, size_t stack_size = 1024);
  }
