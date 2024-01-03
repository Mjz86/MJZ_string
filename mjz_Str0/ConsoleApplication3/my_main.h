#pragma once

#include "start.h"
class my_main : public main_class {
 public:
  int main(int argc, const char* const* const argv) override;
  bool catch_exceptions() override;

  my_main(){};
  my_main(const my_main&) = delete;
  my_main&operator=(const my_main&) = delete;
};

