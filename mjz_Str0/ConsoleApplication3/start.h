#pragma once

#include "mjzString.hpp"
class main_class {
  main_class* run();

 protected:
 inline virtual int main() { return 0; }
 public:
 inline int setup() { return run()->main(); }
  inline virtual ~main_class() = default;
  main_class() = default;
  main_class(main_class&&) = delete;
  main_class(const main_class&) = delete;
  main_class operator=(main_class&&) = delete;
  main_class operator=(const main_class&) = delete;
};
