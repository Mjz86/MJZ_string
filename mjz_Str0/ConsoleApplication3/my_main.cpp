#include "my_main.h"
std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
  return [&, this]() mutable -> std::unique_ptr<main_class> {
    return std::make_unique<my_main>();
  }();
}
bool my_main::catch_exceptions() {
  return true;  // true false
}