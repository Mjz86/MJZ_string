#include <iostream>

#include "my_main.h"

int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;
  int a[10001];
  for (int i{}; i < 10000; i++) a[i] = i;
  auto obj = (to_revurse(
      filter_iterator<int>(
      a, 10000, [](int x) { return ((x % 40) == 0) ;
  })));

  println_it_FE(obj);
  
  return 0;
}

 
