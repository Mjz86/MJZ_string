#pragma once

#include "mjzString.hpp" 
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
extern void get_main_argguments(int& argc, const char* const*& argv);

class main_class {
  std::unique_ptr<main_class> run(int argc, const char* const* const argv);

 protected:
  inline virtual int main(int argc, const char* const* const argv) { return 0; }
  inline virtual bool catch_exceptions() { return false; }

 public:
  inline int setup(int argc, const char* const* const argv) {
    int return_val{-1};
    std::unique_ptr<main_class> ptr = run(argc, argv);
    if (ptr->catch_exceptions()) {
      try {
        USE_MJZ_NS();
        auto named_operation_reporter_for_main = named_operation_reporter( "int main_class_Base::main(int argc, const char* const* const argv)");
        return_val = ptr->main(argc, argv);
      } catch (std::exception ex) {
        std::cout << "\n\n\n EXEPTION : " << ex.what();
      } catch (const char*const ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
      } catch (void* ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
        if (ex == 0) {
          return -1;
        }
      } catch (int ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
        if (ex == -1) {
          return -1;
        }
      } catch (...) {
        std::cout << "\n\n\n EXEPTION happened :( ";
      }
    } else {
      return_val = ptr->main(argc, argv);
    }
    std::cout << std::endl << std::endl;
    return return_val;
  }
  inline virtual ~main_class() = default;
  main_class() = default;
  main_class(main_class&&) = delete;
  main_class(const main_class&) = delete;
  main_class operator=(main_class&&) = delete;
  main_class operator=(const main_class&) = delete;
};

namespace API_of_obj_cnstructor_example {
class P {// no one can do anything with P except  mjz_obj_destructor<P> and mjz_obj_constructor<P> and thy provide a good api for all normal obj stuf
 private:
  P() {}
  ~P() {}
  P& operator=(P&) { return *this; }
  P& operator=(const P&) { return *this; }
  P& operator=(P&&) { return *this; }
  P& operator=(const P&&) { return *this; }
  P(P &) {}
  P(const P &) {}
  P(P&&) {}
  P(const P&&) {}
  inline static 
      
      
      
      
      
      
      
      void* operator new(size_t  ) { return 0; }
  inline static [[nodiscard]] void* operator new[](size_t  ) { return 0; }
  inline static void *operator new(size_t, void *where) { return where; }
  inline static void operator delete(void * ) {}
  inline static void operator delete[](void * ) {}
  inline static void operator delete(void * , size_t) {}
  inline static void operator delete[](void * , size_t) {}
  inline static void operator delete(void *, void *) {}
  inline static void operator delete(void *, size_t, void *) {}

 public:
  inline void f() { ::mjz_ard::iostream::println(" \n f called \n"); }
  friend class ::mjz_ard::mjz_obj_manager_template_t<P>;
};
}  // namespace API_of_obj_cnstructor_example
