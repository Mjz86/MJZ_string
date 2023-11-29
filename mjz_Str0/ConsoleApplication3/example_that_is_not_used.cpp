// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//
#include "start.h"
/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>

/* fstream header file for ifstream, ofstream,
 fstream classes */
#include <fstream>

class my_main : public main_class {
 public:
  int main(int argc, const char* const* const argv) override;
  template<typename T>
  inline static const T& print(const T& obj) {
    std::cout << obj;
    return obj;
  }
  template <typename T>
  inline static const T& println(const T& obj) {
    std::cout << obj << '\n';
    return obj;
  }
};

std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
  return std::make_unique<my_main>();
}


#include "fstream"
int my_main::main(int argc, const char* const* const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  println("   _________________________________________    ");
 
  mjz_Str test(".");
  while (test) {
    operation_reporter op;
  getline(std::cin, test());
  println(test);
  op || op;
  }
  
  println("   _________________________________________    ");
  return 0;
}
