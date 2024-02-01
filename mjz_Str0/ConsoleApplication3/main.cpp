
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
#include<any>
#include<variant>
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  tiny_scoped_timer t("");
  in_union<result<int, std::string>> o = null_union;
  o.unsafe_create(valopt);
  return 0;
}
