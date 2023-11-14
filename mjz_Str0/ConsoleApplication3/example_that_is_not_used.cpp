// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//
#include "pch.h"
#include "mjzString.hpp"
#include "start.h"
/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>

/* fstream header file for ifstream, ofstream,
  fstream classes */
#include <fstream>
mjz_ard::mjz_Str float_get_bits_interpretation(float x) {
  auto bits = mjz_ard::get_bit_representation<float>(x);
  mjz_ard::mjz_Str str("  ");
  for (auto cr : bits) {
    str.print((char)cr);
    str.write(' ');
  }
  str.print(
      "\n |- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -|\n "
      "|s|      exp      |                  mantissa                   |\n\n");
  return str;
}

class my_main : public main_class {
 public:
  int main(int argc, const char* const* const argv) override;
};

std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
  return std::make_unique<my_main>();
}

#include "fstream"

int my_main::main(int argc, const char* const* const argv) {
 using namespace  mjz_ard;

 return 0;
}