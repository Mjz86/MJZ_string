// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//
#include "start.h"

main_class* main_class::
run() {
  mjz_ard::heap_obj_warper<mjz_ard::mjz_str_view> my_str_ptr;	
	my_str_ptr.data_init_mv(" data ");
  std::cout << *my_str_ptr;
	my_str_ptr.data_de_init();
  return this;
	}

