
#include "my_main.h"
namespace mjz_ard {}  // namespace mjz_ard
/*
 *for some unknown reason the auto complete doesn't work on my msvc for big
 *classes like the optional  . PLANS :
 *time is limited so  the minimal  mjz string should wait a bit  for
 *the revamp of the allocators
 *and some mess that should be cleaned and then for the vectors and the classes
 *dependent on dynamic allocators to update api and finally then the minimal mjz
 *string will be written
 * and the unique and sheared and weak ptr will be next
 */

struct DB_t {
 private:
  constexpr static const bool
      S_flag0_will_allocate_a_sharable_string_by_default = true;

  constexpr static const bool S_flag1_can_throw_if_error_by_default = false;
  constexpr static const bool S_flag2_can_allocate_more_that_needed_by_default =
      true;
  constexpr static const bool static_flag_default[3]{
      S_flag0_will_allocate_a_sharable_string_by_default,
      S_flag1_can_throw_if_error_by_default,
      S_flag2_can_allocate_more_that_needed_by_default};
  constexpr static const uint8_t static_flag_mask[3]{(1 << 7), (1 << 6),
                                                     (1 << 5)};
#define MJZ_static_flag_mask_bit_default_I_(I) \
  (MJZ_logic_bit_to_8_bits(static_flag_default[I]) & static_flag_mask[I])
  constexpr static const uint8_t static_flag_mask_bit_default[3]{
      MJZ_static_flag_mask_bit_default_I_(0),
      MJZ_static_flag_mask_bit_default_I_(1),
      MJZ_static_flag_mask_bit_default_I_(2)};
#undef MJZ_static_flag_mask_bit_default_I_
  constexpr static const size_t max_internal_storable_uint_in_control_byte =
      (1 << 5) - 1;
  constexpr static const size_t is_dynamic_control_value =
      max_internal_storable_uint_in_control_byte;
  /*
bits:____________7____6____5________4____3____2____1____0__;
Control byte:__[SF0][SF1][SF2]____[len][len][len][len][len];
 */
  constexpr static const uint8_t control_default =
      static_flag_mask_bit_default[0] | static_flag_mask_bit_default[1] |
      static_flag_mask_bit_default[2];
  constexpr static const uint8_t nullen = 1;
  constexpr static const char nulchr = '\0';
  constexpr static const size_t internal_storage_cap =
      sizeof(char*) + 3 * sizeof(size_t) - nullen - sizeof(control_default);
  static_assert(internal_storage_cap < is_dynamic_control_value,
                "cant store until capacity in this architecture");
  union DATA_t {
    union {
      struct {
        char* m_dy_data;
        size_t m_dy_length;
        size_t m_dy_capacity;
      };
      struct {
        uint8_t no_use___pervious_dy_data_____[sizeof(m_dy_data) +
                                               sizeof(m_dy_length) +
                                               sizeof(m_dy_capacity)];
        uint8_t m_dy_flags;
      };
      char internal_storage[internal_storage_cap + nullen];
      struct {
        uint8_t no_use__all_pervious_data_____[internal_storage_cap];
        char internal_null_terminator;
        uint8_t control_byte;
      };
    };
  };
  static_assert(sizeof(size_t) <= sizeof(char*), " what is this place ");
  static_assert(alignof(size_t) <= alignof(char*), " what is this place ");
  static_assert(alignof(char) == 1 && 1 == sizeof(char),
                " what is this place ");
  static_assert(alignof(uint8_t) == 1 && 1 == sizeof(uint8_t),
                " what is this place ");
  static_assert(alignof(DATA_t) <= alignof(char*), " what is this place ");
  static_assert(internal_storage_cap + nullen + sizeof(DATA_t::control_byte) ==
                    sizeof(DATA_t),
                " some packing was added but how ");

  const static constexpr uint8_t static_length_mask_for_control =
      max_internal_storable_uint_in_control_byte;
  const static constexpr uint8_t inverted_static_length_mask_for_control =
      ~static_length_mask_for_control;
  const static constexpr uint8_t inverted_static_flag_mask[]{
      (uint8_t)~static_flag_mask[0], (uint8_t)~static_flag_mask[1],
      (uint8_t)~static_flag_mask[2]};
  constexpr static const uint8_t dynamic_flag_mask[8]{
      (1 << 7), (1 << 6), (1 << 5), (1 << 4),
      (1 << 3), (1 << 2), (1 << 1), (1 << 0)};
  const static constexpr uint8_t inverted_dynamic_flag_mask[8]{
      (uint8_t)~dynamic_flag_mask[0], (uint8_t)~dynamic_flag_mask[1],
      (uint8_t)~dynamic_flag_mask[2], (uint8_t)~dynamic_flag_mask[3],
      (uint8_t)~dynamic_flag_mask[4], (uint8_t)~dynamic_flag_mask[5],
      (uint8_t)~dynamic_flag_mask[6], (uint8_t)~dynamic_flag_mask[7]};

  DATA_t m_data;

 public:
  enum static_flags : uint8_t {
    S_flag0_will_allocate_a_sharable_string = 0,
    S_flag1_can_throw_if_error = 1,
    S_flag2_can_allocate_more_that_needed = 2,
    S_is_in_dynamic_state = 255
  };
  enum dynamic_flags : uint8_t {
    D_flag0_can_have_sheared_string_data = 0,
    D_flag1_can_sheare_string_data = 1,
    D_flag1_has_atomic = D_flag1_can_sheare_string_data,
    D_flag2,
    D_flag3,
    D_flag4,
    D_flag5,
    D_flag6,
    D_flag7,
    D_free_mask_a = D_flag4,
    D_free_mask_b,
    D_free_mask_c,
    D_free_mask_d,
  };
  template <static_flags flag>
  inline void set_static_flag(bool bit_) { /* can clear*/
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<static_flags>>(flag);
    uint8_t& bytes_ = m_data.control_byte;
    bytes_ &= inverted_static_flag_mask[index];
    bytes_ |= bit_ ? static_flag_mask[index] : 0;
  }

  template <static_flags flag>
  inline bool get_static_flag() const {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<static_flags>>(flag);
    uint8_t bytes_ = m_data.control_byte;
    return !!(bytes_ & static_flag_mask[index]);
  }

  template <static_flags flag>
  inline void set_static_flag(bool to_dynamic)
    requires(flag == static_flags::S_is_in_dynamic_state)
  {
    uint8_t& bytes_ = m_data.control_byte;
    bytes_ = to_dynamic ? (bytes_ | static_length_mask_for_control)
                        : (bytes_ & inverted_static_length_mask_for_control);
  }

  template <static_flags flag>
  inline bool get_static_flag() const
    requires(flag == static_flags::S_is_in_dynamic_state)
  {
    uint8_t bytes_ = m_data.control_byte;
    return (bytes_ & static_length_mask_for_control) ==
           static_length_mask_for_control;
  }

  typedef bool succsess_t;
  inline void set_static_length(size_t len) {
    /* if (internal_storage_cap < len)
    return ;*/
    uint8_t& bytes_ = m_data.control_byte;
    bytes_ &= inverted_static_length_mask_for_control;
    bytes_ |= (uint8_t)len;
  }
  inline size_t set_static_length() const {
    uint8_t bytes_ = m_data.control_byte;
    return uint8_t(bytes_ & static_length_mask_for_control);
  }

  template <dynamic_flags flag>
  inline void set_dynamic_flag(bool bit_) {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<dynamic_flags>>(flag);
    uint8_t& bytes_ = m_data.m_dy_flags;
    bytes_ &= inverted_dynamic_flag_mask[index];
    bytes_ |= bit_ ? dynamic_flag_mask[index] : 0;
  }

  template <dynamic_flags flag>
  inline bool get_dynamic_flag() const {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<dynamic_flags>>(flag);
    uint8_t bytes_ = m_data.m_dy_flags;
    return !!(bytes_ & dynamic_flag_mask[index]);
  }

  inline DB_t() {
    m_data.control_byte = control_default;
    m_data.internal_null_terminator = nulchr;
  }

  inline void move_to_me(DB_t&& other) {
    operator=(std::move(other));
    other.m_data.control_byte = control_default;
  }
  inline static DB_t move_to_new(DB_t&& other) {
    DB_t ret = DB_t(std::move(other));
    other.m_data.control_byte = control_default;
    return ret;
  }
  inline ~DB_t() { m_data.control_byte = control_default; }

 private:
  inline DB_t& operator=(DB_t&&) = default;
  inline DB_t& operator=(const DB_t&) = default;
  inline DB_t(DB_t&&) = default;
  inline DB_t(const DB_t&) = default;
};
/*
requires mjz any to work as a mjz::function
*/
template <typename ret_t, class Alocator = mjz::default_new_allocator,
          typename... arguments_t>
struct mjz_function_data;
template <typename return_t, class Alocator, typename... arguments_t>
struct mjz_function_data<return_t(arguments_t...), Alocator> {
 private:
  Alocator get_allocator() const {
    if constexpr (std::is_constructible_v<
                      Alocator,
                      mjz::mjz_object_pointer_for_internal_realloc_id>)
      return Alocator(
          mjz::mjz_object_pointer_for_internal_realloc_id{.ptr = this});
    else
      return Alocator{};
  }
  typedef bool succsess_t;

  template <class FN_t>
  static return_t call(void* This_of_fn, arguments_t&&... args) {
    return (*((FN_t*)This_of_fn))(std::forward<arguments_t>(args)...);
  }
  template <class FN_t>
 static inline void destroy_val(mjz_function_data*This,void* This_of_fn) {
    This->get_allocator().mjz_delete_single((FN_t*)This_of_fn);
  }
  template <class FN_t>
  static inline void copy_val(mjz_function_data* This, void* This_of_fn) {
    This->set(*((FN_t*)This_of_fn));
  }
  enum what_t{
  destroy,
  copy_construct,
  };
  template <class FN_t>
  static inline void controler_val(what_t what, mjz_function_data* This,
                                   void* This_of_fn) {
    switch (what) { case what_t::copy_construct:
        return copy_val < FN_t>(This, This_of_fn);
        break;
      case what_t::destroy:
        return destroy_val < FN_t>(This, This_of_fn);
        break;
      default:
        break;
    }
  }


  using call_t = return_t (*)(void*, arguments_t&&...);
  using controler_t = void (*)(what_t what, mjz_function_data* This,
                               void* This_of_fn);

  controler_t function_controler{};
  call_t function_caller{};
  void* my_object{}; /* removed if any was added*/
                     /*mjzt::any   potential_object ;*/
 public:
 inline mjz_function_data() {}

  inline mjz_function_data(mjz_function_data&& other)
     : function_caller(std::exchange(other.function_caller, nullptr)),
       function_controler(std::exchange(other.function_controler, nullptr)),
       my_object(std::exchange(other.my_object, nullptr)) {
  
  } 
    inline mjz_function_data& operator=(mjz_function_data&& other){
    destruct();
    function_caller =(std::exchange(other.function_caller, nullptr));
    function_controler=(std::exchange(other.function_controler, nullptr));
    my_object=(std::exchange(other.my_object, nullptr));
 }

 inline ~mjz_function_data() { 
      destruct(); /*any destructor*/
  }
  inline return_t operator()(arguments_t&&... args) {
    if (!function_caller) mjz::Throw("no function");
    return function_caller(my_object, std::forward<arguments_t>(args)...);
  }

  inline void destruct() {
    function_caller = nullptr;
    if (function_controler && my_object) function_controler(what_t::destroy,this, my_object);
    function_controler = nullptr;
    my_object = nullptr;
  }
  /*will be hidden and set to mjz any's object */
  template <class FN_t>
  succsess_t set(FN_t&& my_func)
    requires requires(arguments_t&&... args) {
               {
                 my_func(std::forward<arguments_t>(args)...)
                 } -> std::same_as<return_t>;
             }
  {
   using fn_t = std::remove_reference_t<FN_t>;
    function_caller = &call<fn_t>;
    if constexpr (std::is_convertible_v<fn_t,
                                        return_t (*)(arguments_t && ...)>) {
      my_object = (return_t(*)(arguments_t && ...)) my_func;
      function_controler = nullptr;
    } else if constexpr (std::is_convertible_v<fn_t,
                                               return_t (*)(arguments_t...)>) {
      my_object = (return_t(*)(arguments_t...))my_func;
      function_controler = nullptr;
    } else if constexpr (std::is_destructible_v<
                             fn_t> ||
                         std::is_class_v<fn_t> ||
                         std::is_union_v<fn_t>) {
      my_object =
          get_allocator().mjz_new_single<fn_t>(std::forward<FN_t>(my_func));
      if (!my_object) return false;
      function_controler = &controler_val<fn_t>;
    } else {
      static_assert(" wrong type ");
    }
    return true;
  }
};

#include <any>
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  std::any s;
  // no delete why ?
  mjz_function_data<operation_reporter()> fn;
  auto f = [o=operation_reporter{}]() mutable -> operation_reporter { return {}; };
  fn.set(f);
  fn()++;
  // sf()++;
  return 0;
}
