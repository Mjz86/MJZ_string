#include "start.h"


int main(int argc, const char* const* const argv) {
  using namespace mjz_ard;
  using namespace have_mjz_ard_removed;
  mjz_stack_obj_warper<Scoped_speed_Timer>total("creation of main_class\n\n\n",false);
   delete new char;
  main_class user;
  return [&]() mutable -> int {
    total()("total time spent\n\n\n", false);
    int ret = user.setup(argc, argv);
    ~total;
    return ret;
  }();
  }
