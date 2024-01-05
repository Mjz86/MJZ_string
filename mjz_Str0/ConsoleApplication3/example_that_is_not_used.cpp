
#include "my_main.h"
namespace mjz_ard {};  // namespace mjz_ard
namespace test {
static bool fn_mjz(mjzt::calee_ret<mjzt::operation_reporter> ret,
                   int condition) noexcept {
  NE_RETURN_IF0(ret);
  switch (condition) {
    case 0:
      NE_RETURN_EMPLACE(ret, "i am returned {0}");
      break;
    case 1:
      try {  // safer for outside even if emplace is noexcept
        ret->emplace(" initialized {1}");
      } catch (...) {
      }
      break;
    case 2:
      NE_RETURN_EMPLACE(ret, "i am returned {2}");
      break;
    default:
      try {
        ret->emplace('D');
        ~ret();
      }
      catch (...) {
      }
      break;
  } 
  NE_RETURN_WITH(ret);
}
static void run_ret() {
  int i{};
  while (i != 3) {
    mjz::println("give an integer {3 to exit}");
    char buf[4]{};
    std::cin.getline(buf, 3);
    {
      for (auto c : buf) {
        if (c == 0) break;
        if ('0' <= c && c <= '9') continue;
        i = 3;
      }
      if (i == 3 || !*buf) {
        mjz::println(" :( ");
        break;
      }
      i = mjzt::mjz_str(buf, 3).toLL();
    }

    mjzt::caler_ret<mjzt::operation_reporter> ret_;
    if (!fn_mjz(ret_, i)) {
      mjz::println("no return {!(1 || 0 || 3)}");
      continue;
    }
    mjzt::operation_reporter& ret = ret_;
    ret++;
  }
}
};  // namespace test

int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz;
  using namespace mjzt;
  using namespace mjz::ssco;
  test::run_ret();
   
  return 0;
}
