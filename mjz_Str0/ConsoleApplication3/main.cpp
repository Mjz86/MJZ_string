
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
  result<operation_reporter> r = valopt; 
  /*
  * why i hate padding :
  * this could be half the size
  * (char*)&r :
  * [0] operation_reporter padding / char (Error_t)
  * [1] operation_reporter char
  * [2] result uint8_t (state)
  * [3] result padding
  */
  char*p=r.uuep();
  println( *r);
  assert(sizeof(operation_reporter) == 2);
  p[0] = '%';// padding  in here
  p[1] = '#';// inner char  in here
  println(*r);
  return 0;
}
