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
  ;
  heap_obj_warper<mjz_Str> str = mjz_Str(" hi mom i am a string ")(
      [](mjz_Str& obj,
         heap_obj_warper<mjz_Str>&&  val) -> heap_obj_warper<mjz_Str> {
        val.init(std::move(obj));
	return val; 
      }, heap_obj_warper<mjz_Str>());
  std::cout << str <<'\n';
  mjz_ard::iterator_template<char> it((char*)str.pointer_to_unsafe_data_buffer(),
                                      str.size);
  for (auto c: it) {
    bool c_is_null = (c == 0);
    std::cout << (char)((c & MJZ_logic_BL_bit_to_64_bits(!c_is_null)) |
                  ('?'& MJZ_logic_BL_bit_to_64_bits(c_is_null)));
  }
  std::cout << '\n';
  

  return 0;
}
