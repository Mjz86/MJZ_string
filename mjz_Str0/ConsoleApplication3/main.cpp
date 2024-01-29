
#include "my_main.h"
namespace mjz_ard {
}  // namespace mjz_ard
template<class T,class...Ts>
concept mjz_is_one_of = mjz::mjz_get_type_index_v<T, Ts...> != size_t(-1);

template <bool B>
concept to_concept = B;

template <typename T,template<typename>typename B>
concept t_to_concept = B<T>::value;
/*
* TODO:
* add invalid operations for the moment that they aren't convertible  
*/
template<typename...Types>
struct type_safe_union{
  static const constexpr size_t max_alinement =
      mjz::calucate::KMM_algo(1,alignof(size_t), alignof(Types)...);
  static const constexpr size_t max_size=
      mjz::calucate::max(sizeof(Types)...);
  static const constexpr size_t no_content = size_t(-1) >> 1;

  template <class T>
  static const constexpr bool is_one_of_types = mjz_is_one_of <std::remove_cvref_t< T >
      , Types... > ;
  template <class T>
  static const constexpr size_t to_index =
      mjz::mjz_get_type_index_v<std::remove_cvref_t<T>, Types...>;
  template <size_t I>
  using to_type = mjz::mjz_get_template_argument<I, Types...>;
  template <size_t I>
  static const constexpr bool is_one_of_indexes = is_one_of_types<to_type<I>>;
  template <class T>
  using inizilizer_t = mjz::mjz_obj_manager_template_t<std::remove_cvref_t< T >>;
  template <class T>
  struct is_one_of_types_t {
    static const constexpr bool value = is_one_of_types<std::remove_cvref_t<T>>;
  };
	union {
    alignas(max_alinement) uint8_t m_data[max_size];
  };
  size_t current_type_index{no_content};
  type_safe_union() {}
  template <class T>
  struct move_construct_op {
    void*
    operator()(T* p, void* to_place) {
      inizilizer_t<T>::construct_at((T*)to_place, std::move(*p));
      return to_place;
    }
  };
  template <class T>
  struct copy_construct_op {
    void* operator()(const T* p, void* to_place) {
      inizilizer_t<T>::construct_at((T*)to_place, *p);
      return to_place;
    }
  };
  template <class T>
  struct temp_copy_construct_op {
    void* operator()(const T* p, void* to_place) {
      inizilizer_t<T>::construct_at((T*)to_place, (const T&&)std::move(*p));
      return to_place;
    }
  };
  template <class T>
  struct mut_copy_construct_op {
    void* operator()(  T* p, void* to_place) {
      inizilizer_t<T>::construct_at((T*)to_place, *p);
      return to_place;
    }
  };
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union(T&& other) {
    move_construct_op<T>()(inizilizer_t<T>::addressof(other),
                        m_data);
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union(T const& other) {
    copy_construct_op<T>()(inizilizer_t<T>::addressof(other),
                        m_data);
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union(T& other) {
    mut_copy_construct_op<T>()(inizilizer_t<T>::addressof(other),
                            m_data);
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union(T const&& other) {
    temp_copy_construct_op<T>()(inizilizer_t<T>::addressof(other),
                             m_data);
  }
  template <t_to_concept<is_one_of_types_t> T>
  bool u_init_with(T&&obj) {
    if (runtime_is_one_of_indexes(current_type_index)) return false;
    if (inizilizer_t<T>::construct_at((std::remove_cvref_t<T>*)m_data, std::forward<T>(obj))) {
      uto_type_as<T>();
      return true;
    }
    return false;
  } 
  bool u_runtime_init_with(void*obj,size_t index) {
    if (runtime_is_one_of_indexes(current_type_index) ||
        !runtime_is_one_of_indexes(index))
      return false;
    using fn_t = bool (*)(void*, void*);
    
  fn_t list[] = {[](void* obj, void* m_data) -> bool {
      return inizilizer_t<Types>::construct_at((Types*)m_data,*(Types*)obj);
    }...};
    if (list[index](obj,m_data)) {
      current_type_index = index;
      return true;
    }
    return false;
  }
  bool u_runtime_init_with(const void* obj, size_t index) {
    if (runtime_is_one_of_indexes(current_type_index) ||
        !runtime_is_one_of_indexes(index))
      return false;
    using fn_t = bool (*)(const void*, void*);
    fn_t list[] = {[](const void* obj, void* m_data) -> bool {
      return inizilizer_t<Types>::construct_at((Types*)m_data, *(const Types*)obj);
    }...};
    if (list[index](obj, m_data)) {
      current_type_index = index;
      return true;
    }
    return false;
  }
  bool u_runtime_move_init_with(void* obj, size_t index) {
    if (runtime_is_one_of_indexes(current_type_index) ||
        !runtime_is_one_of_indexes(index))
      return false;  using fn_t = bool (*)(void*, void*);
    fn_t list[] = {[](void* obj, void* m_data) -> bool {
      return inizilizer_t<Types>::construct_at((Types*)m_data,
                                               std::move (* (Types*)obj));
    }...};
    if (list[index](obj, m_data)) {
      current_type_index = index;
      return true;
    }
    return false;
  }
  bool u_runtime_move_init_with(const void* obj, size_t index) {
    if (runtime_is_one_of_indexes(current_type_index) ||
        !runtime_is_one_of_indexes(index))
      return false;  using fn_t = bool (*)(const void*, void*);
    fn_t list [] = {[](const void* obj, void* m_data) -> bool {
      return inizilizer_t<Types>::construct_at((Types*)m_data,
                                               std::move(*(const Types*)obj));
    }...};
    if (list[index](obj, m_data)) {
      current_type_index = index;
      return true;
    }
    return false;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& set(T&& other) {
    if (!u_init_with(std::move(other)))
      uruntime_do_at_rev<move_rev_assinement_op>(
          inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& set(const T& other) {
    if (!u_init_with(other))
      uruntime_do_at_rev<copy_rev_assinement_op>(
          inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& set(T& other) {
    if (!u_init_with(other))
      uruntime_do_at_rev<mut_copy_rev_assinement_op>(
          inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& set(const T&& other) {
    if (!u_init_with(std::move(other)))
      uruntime_do_at_rev<temp_copy_rev_assinement_op>(
          inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& operator=(T&& other) {
    if (!u_init_with(std::move(other)))
    uruntime_do_at_rev<move_rev_assinement_op>(
        inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& operator=(const T& other) {
    if (!u_init_with(other))
    uruntime_do_at_rev<copy_rev_assinement_op>(
        inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& operator=(T& other) {
    if (!u_init_with(other))
    uruntime_do_at_rev<mut_copy_rev_assinement_op>(
        inizilizer_t<T>::addressof(other));
    return *this;
  }
  template <t_to_concept<is_one_of_types_t> T>
  type_safe_union& operator=(const T&& other) {
    if (!u_init_with(std::move(other)))
    uruntime_do_at_rev<temp_copy_rev_assinement_op>(
        inizilizer_t<T>::addressof(other));
    return *this;
  }
  type_safe_union(type_safe_union&&other) { 
     if (other.uruntime_do<move_construct_op>(m_data))
      uto_type_at(other.current_type_index);
  }
  type_safe_union(const type_safe_union& other) {
     if (other.uruntime_do<copy_construct_op>(m_data))
      uto_type_at(other.current_type_index);
  }
  type_safe_union(type_safe_union& other) {
     if (other.uruntime_do<mut_copy_construct_op>(m_data))
      uto_type_at(other.current_type_index);
  }
  type_safe_union(const type_safe_union&& other) {
     if (other.uruntime_do<temp_copy_construct_op>(m_data))
      uto_type_at(other.current_type_index);
  }
  template <class T, class U>
  struct move_assinement_op {
     void* operator()(T* p, U* o) {
      inizilizer_t<T>::obj_equals(*p, std::move(*o));
      return p;
     }
  };
  template <class T, class U>
  struct copy_assinement_op {
     void* operator()(T* p, const U* o) {
      inizilizer_t<T>::obj_equals(*p, (*o));
      return p;
  }
  };
  template <class T, class U>
  struct temp_copy_assinement_op {
  void* operator()(T* p, const U* o) {
      inizilizer_t<T>::obj_equals(*p, std::move(*o));
      return p;
     }
  };
  template <class T, class U>
  struct mut_copy_assinement_op {
     void* operator()(T* p, U* o) {
      inizilizer_t<T>::obj_equals(*p, *o);
      return p;
     }
  };
  template <class T, class U>
  struct move_rev_assinement_op {
     void* operator()(T* o, U* p) {
      inizilizer_t<U>::obj_equals(*p, std::move(*o));
      return p;
     }
  };
  template <class T, class U>
  struct copy_rev_assinement_op {
     void* operator()(const T* o, U* p) {
      inizilizer_t<U>::obj_equals(*p, (*o));
      return p;
     }
  };
  template <class T, class U>
  struct temp_copy_rev_assinement_op {
       void* operator()(const T* o, U* p) {
      inizilizer_t<U>::obj_equals(*p, std::move(*o));
      return p;
     }
  };
  template <class T, class U>
  struct mut_copy_rev_assinement_op {
     void* operator()(T* o, U* p) {
      inizilizer_t<U>::obj_equals(*p, *o);
      return p;
     }
  };
  // the type complexity is O(n^2) that n=sizeof...(Types);
  type_safe_union& operator=(type_safe_union&& other) {
     if (!u_runtime_move_init_with(other.m_data,other.current_type_index))
     uruntime_do<move_assinement_op>(other);
     return *this;
  }
  type_safe_union& operator=(const type_safe_union& other) {
     if (!u_runtime_init_with(other.m_data, other.current_type_index))
     uruntime_do<copy_assinement_op>(other);
     return *this;
  }
  type_safe_union& operator=(type_safe_union& other) {
     if (!u_runtime_init_with(other.m_data, other.current_type_index))
     uruntime_do<mut_copy_assinement_op>(other);
     return *this;
  }
  type_safe_union& operator=(const type_safe_union&& other) {
     if (!u_runtime_move_init_with(other.m_data, other.current_type_index))
     uruntime_do<temp_copy_assinement_op>(other);
     return *this;
  }
  ~type_safe_union() { destroy();}
  bool runtime_is_one_of_indexes(size_t i) { return i < sizeof...(Types); }
  template <template <class, class> class temp_function, class T0>
 void* uruntime_do_at_rev(T0* ptr) {
     if (runtime_is_one_of_indexes(current_type_index))
      return uruntime_do_at_helper<temp_function>(ptr,current_type_index,m_data);
     return 0;
  }
  template <template <class, class> class temp_function, class T0>
static  void* uruntime_do_at_helper(T0* ptr, size_t j, void* other) {
     using fn_ptr = void* (*)(T0*, void*);
     fn_ptr list[] = {[](T0* ptr, void* other_) -> void* {
       return mjz::remove_const(
           temp_function<T0, Types>()(ptr, (Types*)other_));
     }...};
     return list[j](ptr, other);
  }
  template <template <class,class> class temp_function>
   void* uruntime_do_at(size_t i, size_t j, void* other) {
     using fn_ptr = void* (*)(void*, void*, size_t j);
     fn_ptr list[] = {[](void* ptr, void* other, size_t j) -> void* {
return       uruntime_do_at_helper<temp_function, Types>((Types*)ptr,j, other);
     }...};
     return list[i](m_data, other, j);
  }

  template <template <class> class temp_function>
  void uruntime_do_at(size_t i) {
      using fn_ptr=void(*)(void* );
    fn_ptr list[] = {
          [](void* ptr) { temp_function<Types>()((Types*)ptr); }...};
      list[i](m_data);
  }
  template <template <class> class temp_function>
  void* uruntime_do_at(size_t i, void* other) {
      using fn_ptr = void* (*)(void*, void*);
      fn_ptr list[] = {[](void* ptr, void* other_) -> void* {
        temp_function<Types>()((Types*)ptr, other_);
      }...};
     return list[i](m_data, other);
  }
  template <template <class> class temp_function>
 inline void uruntime_do() {
    if (runtime_is_one_of_indexes(current_type_index))
      return uruntime_do_at <temp_function>(current_type_index);
  }
  template <template <class> class temp_function>
  inline void* uruntime_do(void*other) {
    if (runtime_is_one_of_indexes(current_type_index))
      return uruntime_do_at<temp_function>(current_type_index, other);
  }
  template <template <class,class> class temp_function>
  inline void* uruntime_do(type_safe_union& other) {
    if (runtime_is_one_of_indexes(current_type_index)&&runtime_is_one_of_indexes(other.current_type_index))
      return uruntime_do_at<temp_function>(current_type_index,
                                           other.current_type_index, other.m_data);
  }
  template <template <class, class> class temp_function>
  inline void* uruntime_do(const type_safe_union& other) {
    if (runtime_is_one_of_indexes(current_type_index) &&
        runtime_is_one_of_indexes(other.current_type_index))
      return uruntime_do_at<temp_function>(current_type_index,
                                           other.current_type_index, other);
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
void show_if_with_optional() {
  USE_MJZ_NS();

  if (optional<operation_reporter> op; op) {
    op()++;
  } else {
    op.init()()--;
  }
  if (optional<operation_reporter> op; !op) {
    op.init()()++;
  } else {
    op()--;
  }
  for (optional<operation_reporter> op; auto& o : op.get_iter()) {
    if (o) o++;
  }
  for (optional<operation_reporter> op; auto& o : op) {
    if (o) o()++;
  }
  for (auto op : optional<operation_reporter>()) {
    if (op) op()++;
  }
}
#include <fstream>
#include <iostream>
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  return 0;
}
