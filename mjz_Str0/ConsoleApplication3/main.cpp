
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

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  mjz_String o;

 char*b= o.get_buffer();
  memset(b, '@', 10);
 o.get_Base_t().set_len(10);
 println((void*)o.get_buffer(), ' ', o.get_length(), ' ', o.get_cap());
  o.reallocate_bigger_buffer(90);
  println((void*)o.get_buffer(), ' ', o.get_length(), ' ', o.get_cap());
  return 0;
}
