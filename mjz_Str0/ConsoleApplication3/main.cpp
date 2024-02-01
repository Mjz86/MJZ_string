
#include "my_main.h"
namespace mjz_ard {}  // namespace mjz_ard
/*
 *for some unknown reason the auto complete doesn't work on my msvc for big
 *classes like the optional  . PLANS : the error part of the optional is in
 *construction and may take a while to complete because of the mjz optional
 *complexity and powerful unsafe-safe  user API functions .(aka a good and
 *featureful type safe union of {Type,Error_t}+1byte+padding )
 *
 *time is limited so  the minimal  mjz string should wait a bit  for the
 *optional / Result  class to complete and then for the revamp of the allocators
 *and some mess that should be cleaned and then for the vectors and the classes
 *dependent on dynamic allocators to update api and finally then the minimal mjz
 *string will be written
 * and the unique and sheared and weak ptr will be next
 */
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS(); 
  return 0;
}
