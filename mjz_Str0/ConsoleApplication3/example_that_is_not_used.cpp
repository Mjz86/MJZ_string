
#include "my_main.h"
  const char*   get_rand_string() { 
  using namespace mjz_ard::have_mjz_ard_removed;
    using c_a=array<char, 5>;  
    using c_aa=array<c_a, 1000>;
    static std::unique_ptr<c_aa> p =
      []() -> std::unique_ptr<c_aa> {
    std::unique_ptr<c_aa> ptr =
        std::make_unique<c_aa>();
    mjz_ard::for_each(*ptr, [](c_a& a) {
      mjz_ard::for_each(a, [](char& o) { o = rand (); });
      a[a.size() - 1] = '\0';
    });
    return ptr;
    }();

    return p->at(rand ()%p->size()).data();
}

int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz_ard::have_mjz_ard_removed;
  using namespace mjz_ard;
  
  Scoped_speed_Timer tm("_tm_");
  tm("_tm_");
  tm(" get_rand_string()");
  get_rand_string();
  tm(" get_rand_string()");
  get_rand_string();
  tm("_tm_");
  {
    tm("Str"); 
  std::vector<mjz_Str> String(10000);
    tm("S");
    for(auto& o : String) {
      o =  get_rand_string();
      o +=  get_rand_string();
  }
    tm("S2");
    for (auto& o : String) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("S3");
    for (auto& o : String) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("S4");
    for (auto& o : String) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("S5");
    for (auto& o : String) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("~Str");
  }
  tm("_tm_");
  tm("_tm_");
  tm("_tm_");
  {
    tm("str");
    std::vector<std::string> string(10000);
    tm("s");
    for (auto& o : string) {
      o =  get_rand_string();
      o +=  get_rand_string();
    }
    tm("s2");
    for (auto& o : string) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("s3");
    for (auto& o : string) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("s4");
    for (auto& o : string) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("s5");
    for (auto& o : string) {
      o +=  get_rand_string();
      o +=  get_rand_string();
    }
    tm("~str");
  }
  tm("ret");
  return 0;
}