
#include "my_main.h"

namespace mjz_ard {

};  // namespace mjz_ard
namespace test {
mjzt::callee_ret<char> hfdskjLAHS(mjzt::callee_ret<char>ret) {
  CE_NE_RETURN_IF0(ret);
  CE_NE_RETURN_EMPLACE(ret, 0);
}
}
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  auto named_operation_reporter_for_main = named_operation_reporter(
      "int my_main::main(int argc, const char* const* const argv)");

  return 0;
}
