
#include "my_main.h"
namespace mjz_ard {
}  // namespace mjz_ard
template<class T,class...Ts>
concept mjz_is_one_of = mjz::mjz_get_type_index_v<T, Ts...> != size_t(-1);

template <bool B>
concept to_concept = B;

template<typename...Types>
struct type_safe_union{
  static const constexpr size_t max_alinement =
      mjz::calucate::KMM_algo(1,alignof(size_t), alignof(Types)...);
  static const constexpr size_t max_size=
      mjz::calucate::max(sizeof(Types)...);
  static const constexpr size_t no_content = size_t(-1) >> 1;

  template <class T>
  static const constexpr bool is_one_of_types = mjz_is_one_of<T, Types...>;
  template <class T>
  static const constexpr size_t to_index = mjz::mjz_get_type_index_v<T, Types...>;
  template <size_t I>
  using to_type = mjz::mjz_get_template_argument<I, Types...>;
  template <size_t I>
  static const constexpr bool is_one_of_indexes = is_one_of_types<to_type<I>>;
  template <class T>
using  inizilizer_t= mjz::mjz_obj_manager_template_t<T>;
	union {
    alignas(max_alinement) uint8_t m_data[max_size];
  };
  size_t current_type_index{no_content};
  type_safe_union() {}
  type_safe_union(type_safe_union&&) { }
  type_safe_union(const type_safe_union&) {}
  type_safe_union(type_safe_union&) {}
  type_safe_union(const type_safe_union&&) {}
  ~type_safe_union() { destroy();}
  bool runtime_is_one_of_indexes(size_t i) { return i < sizeof...(Types);
  }
  template<template<class>class temp_function>
  void uruntime_do_at(size_t i) {
      using fn_ptr=void(*)(void*);
    fn_ptr list[] = {
          [](void* ptr) { temp_function<Types>()((Types*)ptr);}...};
    list[i](m_data);
  }
  
  template <size_t I>
  to_type<I>* uptr_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    return reinterpret_cast<to_type<I>*>(&m_data[0]);
  }
  template <size_t I>
  to_type<I>& uref_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    return *uptr_at<I>();
  }
  template <class T>
  T* uptr_as()
    requires(to_concept<is_one_of_types<T>>)
  {
    return reinterpret_cast<T*>(&m_data[0]);
  }
  template <class T>
  T& uref_as()
    requires(to_concept<is_one_of_types<T>>)
  {
    return *uptr_as<T>();
  }
  template <size_t I>
  to_type<I>* ptr_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    if (current_type_index == I) return uptr_at<I>();
    return nullptr;
  }
  template <size_t I>
  to_type<I>& ref_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    if (current_type_index == to_index<I>) return *uptr_at<I>();
    mjz::Throw("bad variant accsess !!");
  }
  template <class T>
  T* ptr_as()
    requires(to_concept<is_one_of_types<T>>)
  {
    if (current_type_index == to_index<T>) uptr_as<T>();
    return nullptr;
  }
  template <class T>
  T& ref_as()
    requires(to_concept<is_one_of_types<T>>)
  {
    if (current_type_index == to_index<T>) return *uptr_as<T>();
    mjz::Throw("bad variant accsess !!");
  }
  template <size_t I>
  void uto_type_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    current_type_index = I;
  }
  void uruntime_to_type_at(size_t I)
  {
    current_type_index = I;
  }

  template <class T>
  void uto_type_as()
    requires(to_concept<is_one_of_types<T>>)
  {
    current_type_index = to_index<T>;
  }
  template <size_t I>
  void to_type_at()
    requires(to_concept<is_one_of_indexes<I>>)
  {
    return to_type_as<to_type<I>>();
  }
  template <class T>
  struct construct {
    void operator()(T*p) { 
        inizilizer_t<T>::construct_at(p);
    }
  };
  template <class T>
  struct destruct {
    void operator()(T* p) { inizilizer_t<T>::destroy_at(p); }
  };
  template <class T>
  void to_type_as()
    requires(to_concept<is_one_of_types<T>>)
  {
  constexpr    size_t const new_index = to_index<T>;
    size_t const per_index = current_type_index;
    if (per_index == new_index) return; 
    if (per_index != no_content) {
    uruntime_do_at<destruct>(per_index);
    }
    uto_type_at<new_index>();
    construct<T>()((T*)m_data);
  } 

   bool  runtime_to_type_at(size_t new_index)
  { 
    if (!runtime_is_one_of_indexes(new_index)) return false;
    size_t const per_index = current_type_index;
    if (per_index == new_index) return true;
    if (per_index != no_content) {
    uruntime_do_at<destruct>(per_index);
    }
    uruntime_to_type_at(new_index);
    uruntime_do_at<construct>(new_index);
    return 1;
  } 
  void destroy() 
  { 
    size_t const per_index = current_type_index; 
    if (per_index == no_content) return;
    uruntime_do_at<destruct>(per_index);
    current_type_index = no_content;
  }



}; 

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS(); 
if (optional<operation_reporter> o = init_optional()) {
    o()++;



}

  return 0;
}
