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

 ///////////////////////////////////////////////////////////////////////////////////////
 ////////////////////// create a array data structure/////////////////////////////////////
 return 0; //////////////////////////////////////////////////////////////////////////
}
