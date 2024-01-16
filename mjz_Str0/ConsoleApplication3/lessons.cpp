
#include "my_main.h"

namespace lesson {
void show_lifetime() {
  USE_MJZ_NS();
  operation_reporter var("var");
  optional<operation_reporter> optional_var;
  std::unique_ptr<operation_reporter> up;
  up = std::make_unique<operation_reporter>("up");
  auto ptr_show = [](optional<std::shared_ptr<operation_reporter>> optr) {
    std::weak_ptr<operation_reporter> w_ptr_s0;
    {
      std::weak_ptr<operation_reporter> w_ptr_s1;
      std::shared_ptr<operation_reporter> ptr_s1;
      {
        std::weak_ptr<operation_reporter> w_ptr_s2;
        std::shared_ptr<operation_reporter> ptr_s2;

        if (optr) {
          std::shared_ptr<operation_reporter>& ptr = optr.o();
          ptr_s2 = ptr;
          w_ptr_s2 = ptr;

          ptr_s1 = ptr;
          w_ptr_s1 = ptr;

          w_ptr_s0 = ptr;
          optr.de_init();
        }
        println("w_ptr_s0.use_count():", w_ptr_s0.use_count());
      }
      println("w_ptr_s0.use_count():", w_ptr_s0.use_count());
    }
    println("w_ptr_s0.use_count():", w_ptr_s0.use_count());
  };
  optional_var = "optional_var";
  ptr_show({});
  ptr_show(std::make_shared<operation_reporter>());
}

void show_exceptions_effect() {
  USE_MJZ_NS();
  try {
    try {
      println("stack_unwinding_shower_object create:");
      operation_reporter stack_unwinding_shower_object[10]{
          "SUSO0", "SUSO1", "SUSO2", "SUSO3", "SUSO4",
          "SUSO5", "SUSO6", "SUSO7", "SUSO8", "SUSO9"};
      println("stack_unwinding_shower_object created");
      println("throw : ");
      throw operation_reporter("i am thrown");
      println(" this will never run/print ");

    } catch (...) {
      println("first catch: ");
      println("rethrow_shower_object create:");
      operation_reporter rethrow_shower_object[10]{
          "RTSO0", "RTSO1", "RTSO2", "RTSO3", "RTSO4",
          "RTSO5", "RTSO6", "RTSO7", "RTSO8", "RTSO9"};
      println("rethrow_shower_object created");
      println("rethrow : ");
      throw;
      println(" this will never run/print becuse of throw; that rethrows ");
    }
  } catch (const operation_reporter& ex) {
    println("second catch (const operation_reporter& ex): ");
    println("catch_shower_object create:");
    operation_reporter catch_shower_object[10]{"CSO0", "CSO1", "CSO2", "CSO3",
                                               "CSO4", "CSO5", "CSO6", "CSO7",
                                               "CSO8", "CSO9"};
    println("catch_shower_object created");

    println("themporary create:");
    ex += {" temporary object "};
    println("themporary died after line");
    println("second catch end ");
  } catch (...) {
    println("second catch (const operation_reporter& ex): ");
    println("this will never run ");
    println("second catch end ");
  }
}

typedef void (*lesson_fn)(void);
lesson_fn lessons[]{show_lifetime, show_exceptions_effect};

void show_all() {
  USE_MJZ_NS();
  int i_{};
  for (auto l : lessons) {
    const int i = i_++;
    println("lesson ", i, " begin:");
    l();
    println("lesson ", i, " end");
  }
}

};  // namespace lesson