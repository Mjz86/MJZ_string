#include "start.h"
static std::function<void(int&, const char* const*&)  >
    get_argguments_fn_v{};
static std::function<void(int&, const char* const*&)  >& get_argguments_fn( ) {
 return get_argguments_fn_v;
}
void get_main_argguments(int& argc, const char* const*& argv) {
 return get_argguments_fn()(argc, argv);
}



int main(
    int argc, const char* const* const argv) {
 get_argguments_fn() = [&](int& argc_get, const char* const*& argv_get) -> void{
    argc_get = argc;
    argv_get = argv;
  };
  using namespace mjz_ard;
  using namespace have_mjz_ard_removed;
  mjz_stack_obj_warper<Scoped_speed_Timer> total =
      mjz_stack_obj_warper<Scoped_speed_Timer>("creation of main_class\n\n\n",
                                               false);
   delete new char;
  main_class user;
   return [&]() mutable -> int {
        total()("total time spent\n\n\n", false);
    int ret = user.setup(argc, argv);
    ~total;
    return ret;
  }();
  }
