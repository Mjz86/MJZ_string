// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
// 
#include "mjzString.hpp"
#include "pch.h"
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
};

std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
  return std::make_unique<my_main>();
}

#include "fstream"
int my_main::main(int argc, const char* const* const argv) {
	using namespace mjz_ard::have_mjz_ard_removed;
  array<operation_reporter, 1000> a[2];
        a[0].swap(a[2]);
  a[0].fill({1});

  mjz_str_view s((const char*)a[0][0].UUID(), sizeof(a));

        std::cout << s;


  return 0; 
}
