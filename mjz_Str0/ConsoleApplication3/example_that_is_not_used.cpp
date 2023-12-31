
#include "my_main.h"

int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;
  mjz_str s;
  s.ULL_LL_to_str_rep(40, 10,1,0);
  println(s);




  return 0;
}