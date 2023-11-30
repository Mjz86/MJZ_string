#pragma once

#include "compiher_wr.h"
class main_class {
  std::unique_ptr<main_class> run(int argc, const char* const* const argv);

 protected:
  inline virtual int main(int argc, const char* const* const argv) { return 0; }
  inline virtual bool catch_exceptions() { return false; }

 public:
  template <typename T>
  inline static const T& print(const T& obj) {
    std::cout << obj;
    return obj;
  }
  template <typename T>
  inline static const T& println(const T& obj) {
    std::cout << obj << '\n';
    return obj;
  }
  inline static void print() {}
  inline static void println() { std::cout << '\n'; }
  template <typename... argT>
  void print(argT&&... args) {
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          std::cout << std::forward<argT>(args);
          return;
        }...};  // do all tasks in thr rigth order
    for (auto& f : list) f();
  }
  template <typename... argT>
  void print_arr(argT&&... args) {
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          std::cout << std::forward<argT>(args);
          return;
        }...};  // do all tasks in thr rigth order
    auto it = list.begin(), end = list.end();
    if (it == end) return;
    std::cout << "{ ";
    for (;;) {
      (*it)();
      if (++it < end) {
          std::cout << " , ";
          continue;
      }
      std::cout << " }";
      break;
    }
  }
  template <class T>
  void print_it(const T&obj ) {
    auto it = obj.begin(), end = obj.end();
    if (it == end) return;
    std::cout << "{ ";
    for (;;) {
      std::cout << (*it);
      if (++it < end) {
          std::cout << " , ";
          continue;
      }
      std::cout << " }";
      break;
    }
  }
  template <class T, typename FNT>
  void for_each(const T& obj, FNT function) {
    auto it = obj.begin(), end = obj.end();
    if (it == end) return;
    for (;;) {
      function(*it);
      if (++it < end) {
          continue;
      }
      break;
    }
  }
  template <class T,typename FNT>
  void for_each(T& obj, FNT function) {
    auto it = obj.begin(), end = obj.end();
    if (it == end) return;
    for (;;) {
      function(*it);
      if (++it < end) {
          continue;
      }
      break;
    }
  }
  template <class T>
  void println_it_FE(const T& obj) {
    auto it = obj.begin(), end = obj.end();
    if (it == end) return;
    for (;;) {
      std::cout << (*it) <<'\n';
      if (++it < end) {
          continue;
      }
      break;
    }
  }
  template <typename... argT>
  void println_FE(argT&&... args) {//println_for_each
    auto list =
        std::initializer_list<std::function<void(void)>>{[&](void) -> void {
          std::cout << std::forward<argT>(args);
          std::cout << '\n';
          return;
        }...};  // do all tasks in thr rigth order
    auto it = list.begin(), end = list.end();
    if (it == end) return;
    for (auto& f:list)f();
  }
  template <typename... argT>
  void println_arr(argT&&... args) {
    print_arr(std::forward<argT>(args)...);
    std::cout << '\n';
  }
  template <class T>
  void println_it(const T& obj) {
    print_it(obj);
    std::cout << '\n';
  }
  template <typename... argT>
  void println(argT&&... args) {
    print(std::forward<argT>(args)...);
    std::cout << '\n';
  }
  inline int setup(int argc, const char* const* const argv) {
    int return_val{-1};
    std::unique_ptr<main_class> ptr = run(argc, argv);
    if (ptr->catch_exceptions()) {
      try {
        try {
          return_val = ptr->main(argc, argv);
        } catch (std::exception ex) {
          std::cout << "\n\n\n EXEPTION : " << ex.what();
        } catch (mjz_ard::StringSumHelper ex) {
          std::cout << "\n\n\n EXEPTION : " << ex;
        } catch (mjz_ard::basic_mjz_Str_view ex) {
          std::cout << "\n\n\n EXEPTION : " << ex;
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
