
#include "my_main.h"

int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;
  malloc_wrapper m(1);
  using mjz_ard ::private_dont_use_it::log_functions::get_real_mem;
  size_t *sizes = m.get_ptr_as<size_t>();
  for (int i{2}; i < 7; i++) {
    std::cout << sizes[-i] <<'\n';
  }

  return 0;
}
