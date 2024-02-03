
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

mjzt::Result<std::string, const char*> get_user_input() {
  USE_MJZ_NS();
  std::string password = prompt<std::string>("give password:");
  constexpr const size_t hash_of_password =
      5411718394350379800;  // the password is "password"
  if (password.at(0) == '@')
    return {erropt, "not a valid input"};
  else if (std::hash<std::string>()(password) == hash_of_password)
    return {" default user name "};
  return nullopt;
}
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  auto input = get_user_input();
  if (auto [is_user, user_name, has_err, err] = input.uref4(); has_err) {
    println("[Error]:", err);
  } else if (is_user) {
    println("hello ", user_name, " hear is on cmd command:");
    std::string cmd = scanv<std::string>();
    println(system(cmd.c_str()), "\n bye.");
  } else {
    println("not  a valid user");
  }

  return 0;
}
