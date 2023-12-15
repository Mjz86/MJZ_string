
#define get_the_absoulot_typed_name(X) ((const char *const)#X)
#include "my_main.h"
int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;
  mjz_heap_obj_warper<operation_reporter> o;
  
static size_t constexpr s=  o.my_size();
  std::cout << s;
  return 0;
}