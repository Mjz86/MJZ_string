#pragma once

#include <fstream>
#include <iostream>
#include "mjzString.hpp"
#include "fstream"
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
        return_val = ptr->main(argc, argv);
      } catch (std::exception ex) {
        std::cout << "\n\n\n EXEPTION : " << ex.what();
      } catch (mjz_ard::StringSumHelper ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
      } catch (mjz_ard::mjz_Str ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
      } catch (mjz_ard::basic_mjz_Str_view ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
      } catch (void* ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
        if (ex == 0) {
          mjz_ard::static_str_algo::trap_crash();
        }
      } catch (int ex) {
        std::cout << "\n\n\n EXEPTION : " << ex;
        if (ex == -1) {
          mjz_ard::static_str_algo::trap_crash();
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
namespace mjz_ard {
template <typename T>
const T& print(const T& obj) {
  std::cout << obj;
  return obj;
}
template <typename T>
const T& println(const T& obj) {
  std::cout << obj << '\n';
  return obj;
}
inline void print() {}
inline void println() { std::cout << '\n'; }
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
void print_it(const T begin, T end) {
  auto& it = begin;
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
T for_each(T begin, T end, FNT function) {
  auto  it = begin;
  if (it == end) return;
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return begin;
}
template <class T>
void println_it_FE(T begin, T end) {
  auto& it = begin;
  if (it == end) return;
  for (;;) {
    std::cout << (*it) << '\n';
    if (++it < end) {
      continue;
    }
    break;
  }
}
template <class T>
const T& print_it(const T& obj) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;
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
  return obj;
}
template <class T, typename FNT>
const T& for_each(const T& obj, FNT function) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return obj;
}
template <class T, typename FNT>
T& for_each(T& obj, FNT function) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return obj;  
  for (;;) {
    function(*it);
    if (++it < end) {
      continue;
    }
    break;
  }
  return obj;
}
template <class T>
void println_it_FE(const T& obj) {
  auto it = obj.begin(), end = obj.end();
  if (it == end) return;
  for (;;) {
    std::cout << (*it) << '\n';
    if (++it < end) {
      continue;
    }
    break;
  }
}
template <typename... argT>
void println_FE(argT&&... args) {  // println_for_each
  auto list =
      std::initializer_list<std::function<void(void)>>{[&](void) -> void {
        std::cout << std::forward<argT>(args);
        std::cout << '\n';
        return;
      }...};  // do all tasks in thr rigth order
  auto it = list.begin(), end = list.end();
  if (it == end) return;
  for (auto& f : list) f();
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
}  // namespace mjz_ard

namespace API_of_obj_cnstructor_example {
class P {// no one can do anything with P except  mjz_obj_destructor<P> and mjz_obj_constructor<P> and thy provide a good api for all normal obj stuf
 private:
  P() {}
  ~P() {}
  P &operator=(P &) {}
  P &operator=(const P &) {}
  P &operator=(P &&) {}
  P(P &) {}
  P(const P &) {}
  P(P &&) {}
  inline static [[nodiscard]] void *operator new(size_t m_size) {}
  inline static [[nodiscard]] void *operator new[](size_t m_size) {}
  inline static void *operator new(size_t, void *where) { return where; }
  inline static void operator delete(void *p) {}
  inline static void operator delete[](void *p) {}
  inline static void operator delete(void *p, size_t) {}
  inline static void operator delete[](void *p, size_t) {}
  inline static void operator delete(void *, void *) {}
  inline static void operator delete(void *, size_t, void *) {}

 public:
  inline void f() { ::mjz_ard::println(" \n f called \n"); }
  friend class ::mjz_ard::mjz_obj_destructor<P>;
  friend class ::mjz_ard::mjz_obj_constructor<P>;
};
}  // namespace API_of_obj_cnstructor_example
namespace mjz_ard {// template spesilization  for API_of_obj_cnstructor_example::P
template <>
class mjz_obj_destructor<::API_of_obj_cnstructor_example::P> {
    using P=::API_of_obj_cnstructor_example::P;
 public:
  inline bool destroy_at(P *ptr) noexcept {
    ptr->~P();
    return true;
  }
  inline bool destroy_at(P &ptr) noexcept {
    ptr.~P();
    return true;
  }
  static inline void destroy(mjz_obj_destructor &a, P *p) { a.destroy_at(p); }
  inline void destroy(P *p) { this->destroy_at(p); }
  template <class ForwardIt, class Size>
  ForwardIt destroy_n(ForwardIt first, Size n) {
    for (; n > 0; (void)++first, --n) this->destroy_at(&(*first));
    return first;
  }

  template <class ForwardIt>
  void destroy(ForwardIt first, ForwardIt last) {
    for (; first != last; ++first) this->destroy_at(&(*first));
  }
};
template <>
class mjz_obj_constructor<::API_of_obj_cnstructor_example::P> {
    using P=::API_of_obj_cnstructor_example::P;
 public:
  template <typename... args_t>
  inline P *construct_at(P *dest, args_t &&...args) noexcept {
    return (new (dest) P(std::forward<args_t>(args)...));
  }
  template <typename... args_t>
  inline [[nodiscard]] P obj_constructor(args_t &&...args) {
    return P(std::forward<args_t>(args)...);
  }
  template <class T, class... Args>
  static inline void construct(mjz_obj_constructor &a, T *p, Args &&...args) {
    a.construct_at(p, std::forward<Args>(args)...);
  }
  friend constexpr P *addressof(P &obj) { return &obj; }
  friend constexpr const P *addressof(const P &obj) { return &obj; }
  static constexpr P *addressof(P &obj) { return &obj; }
  static constexpr const P *addressof(const P &obj) { return &obj; }
  friend constexpr P *to_address(P *p) noexcept { return p; }
  friend constexpr const P *to_address(const P &obj) noexcept { return &obj; }
  friend constexpr const P *to_address(const P *p) noexcept { return p; }
  friend constexpr P *to_address(P &obj) noexcept { return &obj; }

  friend P *pointer_to(P &r) noexcept { return &r; }
};
}  // namespace mjz_ard