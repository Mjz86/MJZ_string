
#include "my_main.h"
namespace mjz_ard {};  // namespace mjz_ard
static bool fn_mjz(mjzt::calee_ret<mjzt::operation_reporter> ret) {
  if (!ret) return false;
  ret->emplace("i am an object in my_main::main stack frame ");
  return true;
}
int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz;
  using namespace mjzt;
  using namespace mjz::ssco;
  caler_ret<operation_reporter> ret_;
  if (!fn_mjz(ret_)) return -1;
  operation_reporter& ret=ret_;
  ret++;

  return 0;

}


