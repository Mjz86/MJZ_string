
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
  optional_ce<operation_reporter> r = valopt;
  r.err_emplace();
  r.me_do_first_if_true_or_second_if_false( [](optional_ce<operation_reporter>& o) { o()++; },[](optional_ce<operation_reporter>& o) {
        if (o.has_error()) println("ERR:",(int)o.error()[0]);
        else
          println("ERR");});
  return 0;
}
