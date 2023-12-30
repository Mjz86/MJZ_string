
#include "my_main.h"
namespace mjz_ard {

/*
NOTE THIS CLASS IS JUST A (REFRENCE / POINTER ) YOU SHALL USE IT LIKE ONE aka
1.DONT RETURN THIS FROM A FUNCTION INTERNAL (STACK) VARIABLE
2.USE THIS WHEN VARIABLE IS REFENECED WITHIN THIS TIME FRAME :
 {THE REFRENED OBJECT SHUOLD  EXSIST  } -  CREATION ----------USE... ----------
LAST USE - {THE REFRENED OBJECT COULD SAFELY DESTROY HEARE }  ... DESTRUCTION
... {THE REFRENED OBJECT COULD SAFELY DESTROY HEARE }
*/

template <typename T_ref>
class optional_pointer_template_t
    : private mjz_non_internal_obj_manager_template_t<
          std::remove_const_t<std::remove_reference_t<T_ref>>> {

 public:
  using Type =   std::remove_reference_t<T_ref>;
  static_assert(std::is_same_v<Type &, T_ref>);

 private:
  Type *m_ptr{};
    public:
  template <typename T = Type>
     Type *const get_ptr_to_valid_object_or_throw() = delete;
  template <>
  Type *const get_ptr_to_valid_object_or_throw<void>() {
      return m_ptr;
  }
  template<>
  Type *const get_ptr_to_valid_object_or_throw<Type>() {
    if (m_ptr)
         return m_ptr;
    throw "no object ";
  }
  inline constexpr optional_pointer_template_t() {}

  inline constexpr optional_pointer_template_t(
      Type *valid_pointer_to_object_that_meates_the_requirements)
      : m_ptr(
            valid_pointer_to_object_that_meates_the_requirements) {}
  inline constexpr optional_pointer_template_t(
      Type &valid_refrence_to_object_that_meates_the_requirements)
      : optional_pointer_template_t(this->addressof(
            valid_refrence_to_object_that_meates_the_requirements)) {}
  
  inline constexpr optional_pointer_template_t(
      optional_pointer_template_t 
          &valid_refrence_to_object_that_meates_the_requirements)
      : optional_pointer_template_t(
            valid_refrence_to_object_that_meates_the_requirements
                .ptr_to_valid_object_if_not_nul) {}
  inline constexpr optional_pointer_template_t(
      optional_pointer_template_t 
          &&valid_refrence_to_object_that_meates_the_requirements)
      : optional_pointer_template_t(
            valid_refrence_to_object_that_meates_the_requirements
                .ptr_to_valid_object_if_not_nul) {}
  inline constexpr optional_pointer_template_t(Type &&) = delete;

  inline constexpr optional_pointer_template_t &operator=(
      Type *valid_pointer_to_object_that_meates_the_requirements) {
    m_ptr =
        (valid_pointer_to_object_that_meates_the_requirements);
    return *this;
  }
  inline constexpr optional_pointer_template_t &operator=(
      Type &valid_refrence_to_object_that_meates_the_requirements) {
    m_ptr =
        this->addressof(valid_refrence_to_object_that_meates_the_requirements);
    return *this;
  }
  optional_pointer_template_t &operator=(Type &&) = delete; 
  inline constexpr optional_pointer_template_t &operator=(
      optional_pointer_template_t 
          &valid_refrence_to_object_that_meates_the_requirements) {
    m_ptr =
        valid_refrence_to_object_that_meates_the_requirements
            .get_ptr_to_valid_object_or_throw();
    return *this;
  } 
  inline constexpr optional_pointer_template_t &operator=(
      optional_pointer_template_t 
          &&valid_refrence_to_object_that_meates_the_requirements) {
    m_ptr =
        valid_refrence_to_object_that_meates_the_requirements
            .get_ptr_to_valid_object_or_throw();
    return *this;
  }






  inline constexpr ~optional_pointer_template_t() {}

  inline constexpr explicit operator bool() const {
    return !!get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr bool operator!() const {
    return !get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr operator Type *() {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr operator const Type *() const {
    return get_ptr_to_valid_object_or_throw<void>();
  }

  inline constexpr Type *ptr() {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr const Type *ptr() const {
    return get_ptr_to_valid_object_or_throw<void>();
  }
  inline constexpr Type &get() {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr const Type &get() const {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr operator Type &() {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr operator const Type &() const {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type *operator->() {
    return get_ptr_to_valid_object_or_throw();
  }
  inline constexpr Type &operator*() {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr const Type *operator->() const {
    return get_ptr_to_valid_object_or_throw();
  }
  inline constexpr const Type &operator*() const {
    return *get_ptr_to_valid_object_or_throw();
  }

  inline constexpr Type &operator()() {
    return *get_ptr_to_valid_object_or_throw();
  }
  
  inline constexpr void operator()(std::function<void(Type &)> f) {
    if (m_ptr) {
      f(*m_ptr);
    }
  }
  inline constexpr const Type &operator()() const {
    return *get_ptr_to_valid_object_or_throw();
  }
  inline constexpr void operator~() { m_ptr = 0; }

  
  inline constexpr operator optional_pointer_template_t<std::add_const_t<Type>&>( ) {
    return {m_ptr};
  }
  using merf = optional_pointer_template_t &;
  inline constexpr friend bool operator==(merf a, merf b) { return *a == *b; }
  inline constexpr friend bool operator!=(merf a, merf b) { return *a != *b; }
  inline constexpr friend bool operator<=(merf a, merf b) { return *a <= *b; }
  inline constexpr friend bool operator>=(merf a, merf b) { return *a >= *b; }
  inline constexpr friend bool operator<(merf a, merf b) { return *a < *b; }
  inline constexpr friend bool operator>(merf a, merf b) { return *a > *b; }
  inline constexpr friend auto operator&(merf a, merf b) { return *a & *b; }
  inline constexpr friend auto operator|(merf a, merf b) { return *a | *b; }
  inline constexpr friend auto operator^(merf a, merf b) { return *a ^ *b; }
  inline constexpr friend auto operator%(merf a, merf b) { return *a % *b; }
  inline constexpr friend auto operator*(merf a, merf b) { return *a * *b; }
  inline constexpr friend auto operator/(merf a, merf b) { return *a / *b; }
  inline constexpr friend auto operator-(merf a, merf b) { return *a - *b; }
  inline constexpr friend auto operator+(merf a, merf b) { return *a + *b; }
  inline constexpr auto operator&=(merf b) { return (**this) &= *b; }
  inline constexpr auto operator|=(merf b) { return (**this) |= *b; }
  inline constexpr auto operator^=(merf b) { return (**this) ^= *b; }
  inline constexpr auto operator%=(merf b) { return (**this) %= *b; }
  inline constexpr auto operator*=(merf b) { return (**this) *= *b; }
  inline constexpr auto operator/=(merf b) { return (**this) /= *b; }
  inline constexpr auto operator-=(merf b) { return (**this) -= *b; }
  inline constexpr auto operator+=(merf b) { return (**this) += *b; }
};

template <typename T, size_t m_size,
          class obj_cnstructor_t = mjz_temp_type_obj_algorithims_warpper_t<T>>
class safe_array {
  using value_type = T;
  using Type = T;

  struct DATA_base : private obj_cnstructor_t {
    union DATA_base_UN {
      char f;
      T m_data[m_size];
      template <typename... args_t,
                typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
      constexpr inline DATA_base_UN(args_t &&...args)
          : m_data{std::forward<args_t>(args)...} {}
      constexpr inline DATA_base_UN() {}
      constexpr inline ~DATA_base_UN() {}
    };
    constexpr inline obj_cnstructor_t &creator() { return *this; }
    constexpr inline obj_cnstructor_t &creator() const { return *this; }
    constexpr inline DATA_base() {
      creator().construct_array_at(ptr(), m_size);
    }

    template <typename... args_t,
              typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
    constexpr inline DATA_base(args_t &&...args)
        : db(std::forward<args_t>(args)...) {}
    constexpr inline ~DATA_base() {
      creator().destruct_array_at(ptr(), m_size);
    }

    DATA_base_UN db;
    constexpr inline T *ptr() { return db.m_data; }
    constexpr inline const T *const ptr() const { return db.m_data; }
  };

 public:
  template <typename... args_t,
            typename = std::enable_if_t<((sizeof...(args_t)) <= m_size)>>
  constexpr inline safe_array(args_t &&...args)
      : db(std::forward<args_t>(args)...) {}

  constexpr inline safe_array() {}

  constexpr inline ~safe_array() {}
  

  using ref_t=optional_pointer_template_t<Type &>;
  using cref_t=optional_pointer_template_t<const Type &>;

  template <size_t I>
  constexpr inline std::enable_if_t<I < m_size, Type &> at() noexcept {
    return ptr()[I];
  }
  template <size_t I>
  constexpr inline std::enable_if_t<I < m_size, const Type &> at()
      const noexcept {
    return ptr()[I];
  }

  

  constexpr inline ref_t at(size_t i) {
    if (i > m_size) return NULL_type;
    return ptr() + i;
  }
  constexpr inline cref_t at(size_t i) const  {
    if (i > m_size) return NULL_type;
    return ptr() + i;
  }

  constexpr inline ref_t operator[](size_t i) { return at(i);
  }
  constexpr inline cref_t operator[](size_t i) const 
    { return at(i);
  }
  Type &operator[](ptrdiff_t i) = delete;
  const Type &operator[](ptrdiff_t i) const = delete;
  constexpr inline obj_cnstructor_t &creator() { return db.creator(); }
  constexpr inline obj_cnstructor_t &creator() const { return db.creator(); }
  using array_type = typename Type (&)[m_size];
  constexpr inline const array_type &toa() const { return db.db.m_data; }
  constexpr inline array_type &toa() { return db.db.m_data; }

 private:
  static constexpr Type *const NULL_type = nullptr;
  DATA_base db;
  constexpr inline T *ptr() { return db.ptr(); }
  constexpr inline const T *const ptr() const { return db.ptr(); }
};
};  // namespace mjz_ard
int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;

 
  return 0;
}