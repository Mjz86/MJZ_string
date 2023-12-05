#include <iostream>

#include "my_main.h"

int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;
  struct S{
    mjz_stack_obj_warper<mjz_str> s;
    array<char, 100> canary;
    S() : s(), canary() {
      for (auto& c : canary) c = 'C';
    }
  }c;
  auto& s = *c.s;
  void* pt = c.canary.data();
  s(" hello world \n");
  s.insert(s.length(), 10, 'F');
 // s.insert(16, 10, '$');
  println(s);
  s->replace(s->find("hello"), 5, (basic_mjz_Str_view) "replace_cstr");
  println(s);
  s->replace(s->find("world"), 5, (basic_mjz_Str_view) "my world with you");
  println(s);
  println(s *= 100);
  println(s /= "you");
  s.find_and_replace("with", "@!WITH!@");
  println(s);



  return 0;
}
