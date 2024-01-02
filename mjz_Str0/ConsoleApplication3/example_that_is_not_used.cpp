
#include "my_main.h"


int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz;
  using namespace mjzt;
  using namespace mjz::ssco;
     get_big_endian_data<uint64_t> g(256);
  println(g.get_default(),' ', sizeof(g));
  return 0;
}