// StaticLib_wr.cpp : Defines the functions for the static library.
//

#include "compiher_wr.h"
using namespace mjz_ard::have_mjz_ard_removed;

namespace mjz_ard {
void print(WRState* w, const WRValue* argv, const int argn, WRValue& retVal,
           void* usr) {
  char buf[16*1024];
  for (int i = 0; i < argn; ++i) {
    printf("%s", argv[i].asString(buf, 16 * 1024));
  }
}

int run_code(mjz_str_view code, size_t stack_size) {
  WRState* w = wr_newState(stack_size);  // create the state

  wr_registerFunction(w, "print", print);  // bind a function
  wr_loadAllLibs(w);
  unsigned char* outBytes;  // compiled code is alloc'ed
  int outLen;

  int err = wr_compile(code.data(), code.length()-1, &outBytes,
                       &outLen);  // compile it
  if (err == 0) {
    wr_run(w, outBytes, outLen);  // load and run the code!
    delete[] outBytes;            // clean up
  }

  wr_destroyState(w);

  return 0;
}
[[nodiscard]] mjz_Str get_byte_code(mjz_str_view code, size_t stack_size) {
  unsigned char* outBytes;  // compiled code is alloc'ed
  int outLen;

  int err = wr_compile(code.data(), code.length() - 1, &outBytes,
                       &outLen);  // compile it
  mjz_stack_obj_warper<mjz_Str,0> retval;
  if (err == 0) {
    retval.init((const char*)outBytes, outLen);
    delete[] outBytes;            // clean up
  } else {
  retval.init();
  }
  return *retval;
}

int run_byte_code(mjz_str_view code, size_t stack_size) {
  WRState* w = wr_newState(stack_size);  // create the state

  wr_registerFunction(w, "print", print);  // bind a function
  wr_loadAllLibs(w);

  wr_run(w, (const uint8_t*)code.data(),
         code.length());  // load and run the code!

  wr_destroyState(w);

  return 0;
}
}  // namespace mjz_ard