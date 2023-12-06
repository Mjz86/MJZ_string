
#include "my_main.h"
namespace mjz_ard
{


}
int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;
  static_vector<operation_reporter, 1000> v;
  v.add_to_length(100);
  for (auto& obj : v) {
    obj % obj + obj&& obj* obj - obj;
    obj < obj / obj || obj < obj;
  }
  return 0;
}
