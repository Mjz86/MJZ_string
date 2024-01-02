
#include "my_main.h"
template <class... T>
size_t f(int i) {
  size_t s []{sizeof(T)...};
  return s[i];
}
int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz;
  using namespace mjzt;
  using namespace mjz::ssco;
  println(f<bool ,std::bitset<999999999>,std::array<int,0>>(1));
  return 0;
}