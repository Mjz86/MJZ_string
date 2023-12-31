
#include "my_main.h"
bool get_the_object(mjzt::optional_ref<mjzt::operation_reporter&> uninitialized_obj) {

    new (uninitialized_obj.ptr()) mjzt::operation_reporter('#');
  return true;
}


int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;

  optional<operation_reporter> object_holder;
 if( get_the_object(object_holder.uuop()))
  object_holder.notify_unsafe_init();




  return 0;
}