
#include "my_main.h"
namespace mjz_ard {}  // namespace mjz_ard
/*
 *for some unknown reason the auto complete doesn't work on my msvc for big
 *classes like the optional  . PLANS :
 *time is limited so  the minimal  mjz string should wait a bit  for
 *the revamp of the allocators
 *and some mess that should be cleaned and then for the vectors and the classes
 *dependent on dynamic allocators to update api and finally then the minimal mjz
 *string will be written
 * and the unique and sheared and weak ptr will be next
 */
#define Printlnvar(X) println(__LINE__, ':', #X,':', (X))
#define Printlnstr(X) println(__LINE__, ':', #X, ':', X, '<', X.length(), '>')

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  mjz_String str;
  int i{};
  while (!str.is_dynamic()) {
    str.concat(static_str_algo::GET_CHAR_from_int(i++,0));
  }
  println(str);
  
  return 0;
}
