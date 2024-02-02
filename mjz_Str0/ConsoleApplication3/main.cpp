
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
  Result<std::string, to_error_t<std::string>> o;
  scan(*+o);
  if (*o == "@E")
    ~o;
  else if (o->at(0) == '@')
    o.err_emplace(o.move().substr(1));
  auto [value_ptr, error_ptr] = o.ref();
  if (error_ptr) {
    println("Error: ", std::string_view(*error_ptr));
  }
  else if (value_ptr) {
    auto& value = *value_ptr;
    if (promptlnp<decltype(value)>('"', value,'"', " is:") == value)
      println("correct");
    else
      println("incorrect");   
  } else {
    println("Error?");
  }  

  return 0;
}
