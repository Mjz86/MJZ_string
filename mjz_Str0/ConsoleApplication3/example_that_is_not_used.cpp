
#include "my_main.h"

int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;
  using namespace mjz::ssco;
  int i{}, j{};
  std::cin >> i>>j;
  mjz_str s = mjz_str::ULL_LL_to_str(i, j, 1, 0);
  println(s);



  return 0;
}