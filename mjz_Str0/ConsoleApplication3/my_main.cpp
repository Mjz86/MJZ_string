#include "my_main.h"
std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
  return std::make_unique<my_main>();
}
bool my_main::catch_exceptions() {
  return false;  // true false
}