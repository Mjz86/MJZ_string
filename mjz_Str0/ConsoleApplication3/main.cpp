
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
template <typename Char_t> class mjz_str_DB_t {
 public:
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

  constexpr static const uint8_t dynamic_flags_default =0;
  constexpr static const uint8_t nullen = 1;
  constexpr static const Char_t nulchr = '\0';
  constexpr static const size_t internal_storage_cap_ =
      2 * sizeof(Char_t*) + 2 * sizeof(size_t) - nullen -
      sizeof(control_default);
  static_assert(internal_storage_cap_ < is_dynamic_control_value,
                "cant store until capacity in this architecture");

  template <typename C_T = Char_t>
  static constexpr const size_t internal_storage_cap_for =
      max_macro_(1, size_t((internal_storage_cap_ + nullen) / sizeof(C_T))) -
      nullen;

  static constexpr const size_t internal_storage_cap_of_t =
      internal_storage_cap_for<Char_t>;
  union DATA_t {
    union {
      struct {
        Char_t* m_dy_data;
        size_t m_dy_length;
        size_t m_dy_capacity;
      };
      struct {
        uint8_t no_use___pervious_dy_data_____[sizeof(m_dy_data) +
                                               sizeof(m_dy_length) +
                                               sizeof(m_dy_capacity)];
        uint8_t m_dy_flags;
      }; 
      struct {
        Char_t internal_storage_[internal_storage_cap_of_t + nullen];
      };
      char internal_storage_cr[internal_storage_cap_ + nullen];
      struct {
        uint8_t no_use__all_pervious_data_____[internal_storage_cap_];
        char internal_null_terminator_____;
        uint8_t control_byte;
      };
    };
  };
  static_assert(sizeof(size_t) <= sizeof(Char_t*), " what is this place ");
  static_assert(alignof(size_t) <= alignof(Char_t*), " what is this place ");
  static_assert(alignof(char) == 1 && 1 == sizeof(char),
                " what is this place ");
  static_assert(alignof(uint8_t) == 1 && 1 == sizeof(uint8_t),
                " what is this place ");
  static_assert(alignof(DATA_t) <= alignof(Char_t*), " what is this place ");
  static_assert(internal_storage_cap_ + nullen + sizeof(DATA_t::control_byte) ==
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

  DATA_t m_data_strorage;
   
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
  constexpr inline void set_static_flag(bool bit_) noexcept { /* can clear*/
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<static_flags>>(flag);
    uint8_t& bytes_ = m_data_strorage.control_byte;
    bytes_ &= inverted_static_flag_mask[index];
    bytes_ |= bit_ ? static_flag_mask[index] : 0;
  }

  template <static_flags flag>
  constexpr inline bool get_static_flag() const noexcept {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<static_flags>>(flag);
    uint8_t bytes_ = m_data_strorage.control_byte;
    return !!(bytes_ & static_flag_mask[index]);
  }

  template <static_flags flag>
 constexpr inline void set_static_flag(bool to_dynamic) noexcept
    requires(flag == static_flags::S_is_in_dynamic_state)
  {
    uint8_t& bytes_ = m_data_strorage.control_byte;
    bytes_ = to_dynamic ? (bytes_ | static_length_mask_for_control)
                        : (bytes_ & inverted_static_length_mask_for_control);
  }

  template <static_flags flag>
  constexpr inline bool get_static_flag() const noexcept
    requires(flag == static_flags::S_is_in_dynamic_state)
  {
    uint8_t bytes_ = m_data_strorage.control_byte;
    return (bytes_ & static_length_mask_for_control) ==
           static_length_mask_for_control;
  }

  typedef bool succsess_t;
  constexpr inline void set_static_length(size_t len) noexcept {
    /* if (internal_storage_cap < len)
    return ;*/
    uint8_t& bytes_ = m_data_strorage.control_byte;
    bytes_ &= inverted_static_length_mask_for_control;
    bytes_ |= (uint8_t)len;
  }
  constexpr inline size_t get_static_length() const noexcept {
    uint8_t bytes_ = m_data_strorage.control_byte;
    return uint8_t(bytes_ & static_length_mask_for_control);
  }

  template <dynamic_flags flag>
  constexpr inline void set_dynamic_flag(bool bit_) noexcept {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<dynamic_flags>>(flag);
    uint8_t& bytes_ = m_data_strorage.m_dy_flags;
    bytes_ &= inverted_dynamic_flag_mask[index];
    bytes_ |= bit_ ? dynamic_flag_mask[index] : 0;
  }

  template <dynamic_flags flag>
  constexpr inline bool get_dynamic_flag() const noexcept {
    const constexpr uint8_t index =
        static_cast<std::underlying_type_t<dynamic_flags>>(flag);
    uint8_t bytes_ = m_data_strorage.m_dy_flags;
    return !!(bytes_ & dynamic_flag_mask[index]);
  }
  constexpr inline bool is_dynamic() const noexcept {
    return get_static_flag<static_flags::S_is_in_dynamic_state>();
  }

  
  template<typename Char_t=Char_t>
  inline static constexpr bool can_be_static(size_t len) noexcept {
    return len <= internal_storage_cap_of_t;
  }

constexpr inline size_t get_dynamic_length() const noexcept {
    return m_data_strorage.m_dy_length;
  }

constexpr inline bool get_DF0( ) const {
    return get_dynamic_flag<
        dynamic_flags::D_flag0_can_have_sheared_string_data>();
  }
constexpr inline bool get_DF1() const {
    return get_dynamic_flag<
        dynamic_flags::D_flag1_can_sheare_string_data>();
}

 
  inline Char_t* get_dynamic_string() noexcept {
    return  (m_data_strorage.m_dy_data);
}
 
  inline const Char_t* get_static_string() const noexcept {
    return  (m_data_strorage.internal_storage_);
}
 
  inline   Char_t* get_static_string() noexcept {
    return  (m_data_strorage.internal_storage_);
}
 
  inline constexpr static size_t get_static_capacity() noexcept {
    return internal_storage_cap_of_t;
}
 
  inline const Char_t* get_dynamic_string() const noexcept {
    return  (m_data_strorage.m_dy_data);
  }
constexpr inline size_t get_dynamic_capacity() const noexcept {
    return m_data_strorage.m_dy_capacity;
  }

constexpr inline size_t m_length() const noexcept {
    return is_dynamic() ? get_dynamic_length() : get_static_length();
  }

constexpr inline size_t m_capacity() const noexcept {
    return is_dynamic() ? get_dynamic_capacity() : get_static_capacity();
  }

constexpr inline const Char_t* m_string() const noexcept {
    return is_dynamic() ? get_dynamic_string() : get_static_string();
  }

constexpr inline Char_t* m_string() noexcept {
    return is_dynamic() ? get_dynamic_string() : get_static_string();
  }

constexpr inline void set_dynamic_length(size_t len) noexcept {
      m_data_strorage.m_dy_length = len;
  }

constexpr inline void set_dynamic_string(Char_t* mjz_str) noexcept {
      m_data_strorage.m_dy_data = mjz_str;
  }

constexpr inline void set_dynamic_capacity(size_t cap) noexcept {
      m_data_strorage.m_dy_capacity = cap;
  }

constexpr inline void set_all_to_dynamic(Char_t* mjz_data_string, size_t length,
                                 size_t capacity, bool DF0_did_share_string_data,
                                 bool DF1_has_a_live_atomic) noexcept {
    m_data_strorage.m_dy_flags = dynamic_flags_default;
    set_static_flag<static_flags::S_is_in_dynamic_state>(true);
    set_dynamic_flag<dynamic_flags::D_flag0_can_have_sheared_string_data>(DF0_did_share_string_data);
    set_dynamic_flag<dynamic_flags::D_flag1_has_atomic>(DF1_has_a_live_atomic);
    set_dynamic_capacity(capacity);
    set_dynamic_length(length);
    set_dynamic_string(mjz_data_string);
    mjz_data_string[length] = nulchr;
    mjz_data_string[capacity] = nulchr;
} 
/*msvc c++23*/
#define mjz_assume __assume
constexpr inline void set_all_to_static(size_t length) noexcept {
    [[mjz_assume(length <= internal_storage_cap_of_t)]];
    set_static_flag<static_flags::S_is_in_dynamic_state>(false);
    set_static_length(length);
    m_data_strorage.internal_storage_[length] = nulchr;
    m_data_strorage.internal_storage_[internal_storage_cap_of_t] = nulchr;
  }



constexpr inline void move_to_me(mjz_str_DB_t&& other) noexcept {
    operator=(std::move(other));
    other.m_data_strorage.control_byte = control_default;
  }

constexpr inline static mjz_str_DB_t move_to_new(
    mjz_str_DB_t&& other) noexcept {
    mjz_str_DB_t ret = mjz_str_DB_t(std::move(other));
    other.m_data_strorage.control_byte = control_default;
    return ret;
  }

public:
  constexpr inline mjz_str_DB_t() noexcept {
    m_data_strorage.control_byte = control_default;
    m_data_strorage.internal_storage_[internal_storage_cap_of_t] = nulchr;
  }
  constexpr inline ~mjz_str_DB_t() noexcept {
    m_data_strorage.control_byte = control_default;
}

 private:
constexpr inline mjz_str_DB_t& operator=(mjz_str_DB_t&&) noexcept = default;

  constexpr inline mjz_str_DB_t& operator=(const mjz_str_DB_t&) noexcept =
      default;

constexpr inline mjz_str_DB_t(mjz_str_DB_t&&) noexcept = default;
constexpr inline mjz_str_DB_t(const mjz_str_DB_t&) noexcept = default;
};
template <typename Char_t_,class mjz_reallocator_t = mjz::default_string_allocator>
class mjz_String_memory_class : public mjz_str_DB_t<Char_t_> {
public:
  inline constexpr mjz_str_DB_t<Char_t_>& B() { return *this; }
const inline constexpr mjz_str_DB_t<Char_t_>& B()const { return *this; }
using B_t= mjz_str_DB_t<Char_t_>;
mjz_reallocator_t Allocator() const {
    return mjz_reallocator_t(
        mjz::mjz_String_pointer_for_internal_realloc_id{.ptr = this});
};
inline void A_mjz_free(void* pr_real_ptr, size_t n) noexcept {
    Allocator().mjz_free(pr_real_ptr, n);
}
inline _NODISCARD void* A_mjz_realloc(void* pr_real_ptr, size_t preveious_size,
                                    size_t new_size) noexcept {
return    Allocator().mjz_realloc(pr_real_ptr,   preveious_size,   new_size);
}
using sheared_int_t =
    mjz::mjz_simple_unsafe_init_obj_wrpr_t<std::atomic_uint_fast64_t, 0, 0>;
using sheared_copy_int_t = uint64_t;
static constexpr const sheared_copy_int_t nops = (-1) >> 1;
inline static constexpr size_t capacity_to_real_size(size_t cap) {
    return cap + B_t::nullen + sizeof(sheared_int_t);
}
inline static constexpr size_t real_size_to_capacity(size_t real_size) {
    return real_size - capacity_to_real_size(0);
}
inline static   void *mjz_str_to_real_ptr(Char_t_ *my_mjz_c_str) {
    if (!my_mjz_c_str) return nullptr;
    return (char*)(my_mjz_c_str) - sizeof(sheared_int_t);
}
inline static Char_t_ *real_ptr_to_mjz_str(void *real_ptr) {
    if (!real_ptr) return nullptr;
    return (Char_t_*)((char*)real_ptr + sizeof(sheared_int_t));
}
inline static sheared_int_t *get_sheared_int_from_real(void*real_ptr) {
    return (sheared_int_t *)real_ptr;
}
inline static sheared_int_t *get_sheared_int_from_str(Char_t_ *my_mjz_c_str) {
    if (!my_mjz_c_str) return nullptr;
    return get_sheared_int_from_real(mjz_str_to_real_ptr(my_mjz_c_str));
}
inline static void*init_real_ptr_int_then_get_real(void*real_ptr) {
    if (!real_ptr) return nullptr;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    int_ptr->unsafe_create(1);
    return real_ptr;
}
inline static void*deinit_real_ptr_int_then_get_real(void*real_ptr) {
    if (!real_ptr) return nullptr;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    int_ptr->get() = nops;
    int_ptr->unsafe_destroy();
    return real_ptr;
}
inline static sheared_copy_int_t increment_real_ptr_int_and_then_get(
    void*real_ptr) {
    if (!real_ptr) return nops;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return ++int_ptr->get();
}
inline static sheared_copy_int_t decrement_real_ptr_int_and_then_get(
    void*real_ptr) {
    if (!real_ptr) return nops;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return --int_ptr->get();
}
inline static bool decrement_real_ptr_int_and_then_get_is_none(void*real_ptr) {
    if (!real_ptr) return 0;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return ((--int_ptr->get()) == 0);
}
inline static sheared_copy_int_t real_ptr_int_get(void*real_ptr) {
    if (!real_ptr) return 0;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return (sheared_copy_int_t)int_ptr->get();
}
inline static bool real_ptr_int_is_alone(void*real_ptr) {
    if (!real_ptr) return 0;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return (1 == int_ptr->get());
}
inline static bool real_ptr_int_is_none(void*real_ptr) {
    if (!real_ptr) return 0;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return (0 == int_ptr->get());
}

inline static bool real_ptr_int_has_friend(void*real_ptr) {
    if (!real_ptr) return 0;
    sheared_int_t *int_ptr = get_sheared_int_from_real(real_ptr);
    return (1 < int_ptr->get());
}

inline static bool real_ptr_is_sheared(bool is_dynamic,bool DF0_can_be_sheared,
                                       bool DF1_has_atomic, void* real_ptr) {
    if (!is_dynamic || !DF0_can_be_sheared || !DF1_has_atomic) return false;
    return real_ptr_int_has_friend(real_ptr);
}
inline static bool real_ptr_is_owned(bool is_dynamic,bool DF0_can_be_sheared,
                                       bool DF1_has_atomic, void* real_ptr) {
    return !real_ptr_is_sheared(is_dynamic,DF0_can_be_sheared, DF1_has_atomic,
                                real_ptr);
}
enum states{
    Static,
    dy_owend,
  dy_sheared,
};
inline static states real_ptr_is_state(bool is_dynamic, bool DF0_can_be_sheared,
                                     bool DF1_has_atomic, void* real_ptr) {
    [[mjz_assume((DF0_can_be_sheared && !DF1_has_atomic) == false)]];
    if (!is_dynamic) {
      return states::Static;
    }
    if (!DF0_can_be_sheared || !DF1_has_atomic) return states::dy_owend;
    if (real_ptr_int_has_friend(real_ptr)) return states::dy_sheared;
        return states::dy_owend;
}
inline   states get_state() {
        if (!B().is_dynamic()) {
        return states::Static;
        }
        void* real_ptr = mjz_str_to_real_ptr(B().get_dynamic_string());
        
        states state =
            real_ptr_is_state(false, B().get_DF0(), B().get_DF1(),
            real_ptr);
        if (state == states::dy_owend) {
        set_dynamic_flag<
            B().dynamic_flags::D_flag0_can_have_sheared_string_data>(false);
        return states::dy_owend;
        }
        return states::dy_sheared;
}

inline   states get_state() const {
        return mjz::remove_const(this)->get_state();
}
inline static constexpr size_t to_optimal_cap(size_t new_cap, size_t pr_cap,
                                              size_t pr_len) {
        pr_cap = min(pr_cap, 32768 + new_cap);
        pr_len=min(pr_cap, pr_len);
        if (new_cap < pr_cap) return pr_cap;
        return max(new_cap,max(64, max(pr_cap + pr_cap - pr_len, pr_cap + pr_len)));
}
using succsess_t=B_t::succsess_t;
struct String_temp_data_t {
      inline  String_temp_data_t(mjz_String_memory_class* ptr,
                           bool free_data_ = false)
            : This(ptr) , free_data(free_data_) {
        was_static = !This->is_dynamic();
        m_static_data = This->B().get_static_string();
        m_static_len = This->B().get_static_length();
        m_length = This->B().m_length();
        m_capacity = This->B().m_capacity();
        m_data = This->B().m_string();

        DF0 = This->B().get_DF0();
        DF1 = This->B().get_DF1();
        }
      inline  ~String_temp_data_t() {
        if (!was_static && free_data) free_memory_at_old(This, this);
        }
        mjz_String_memory_class* This{};
        Char_t_* m_data{};
        Char_t_* m_static_data{};
        constexpr static const size_t m_static_cap = B_t::get_static_capacity();
        size_t m_static_len{};
        size_t m_length{};
        size_t m_capacity{};
        bool flags[4]{};
        bool DF0{};
        bool DF1{};
        bool was_static{};
        bool free_data{};
};
inline
static void free_memory_at_old(mjz_String_memory_class* This,
                           String_temp_data_t*old_This) {
        [[mjz_assume(This && old_This)]];
        if (old_This->was_static) return;
        bool can_have_sheared_string_data = old_This->DF0;
        bool has_live_atomic = old_This->DF1;
        void* real_ptr = mjz_str_to_real_ptr(old_This->m_data);
       bool is_shered= real_ptr_is_sheared(false, can_have_sheared_string_data,
                            has_live_atomic,
                            real_ptr);
        auto delete_str = [&]() mutable -> void {
        real_ptr= deinit_real_ptr_int_then_get_real(real_ptr);
        This->A_mjz_free(real_ptr, capacity_to_real_size(old_This->m_capacity));
        return;
        };
        if (!is_shered) return delete_str();
        if (decrement_real_ptr_int_and_then_get_is_none(real_ptr)) {
        return delete_str();
        }
        return;

}
inline succsess_t realloc_memory(size_t new_cap, bool force_shrink,
                                 bool can_throw_) {
        if (B().can_be_static(new_cap)) {
        if (!B().is_dynamic()) return true;
        String_temp_data_t past(this, true);
        size_t new_len = min(new_cap, past.m_length);
        memmove(past.m_static_data, past.m_data, new_len);
        B().set_all_to_static(new_len);
        return true;
        }
        bool SF0_can_share =B().get_static_flag<B_t::S_flag0_will_allocate_a_sharable_string>();
        bool can_throw = can_throw_ && B().get_static_flag<B_t::S_flag1_can_throw_if_error>();
        bool alloc_more =
            force_shrink?0:B()
                .get_static_flag<B_t::S_flag2_can_allocate_more_that_needed>();
        size_t pr_cap_ = m_capacity();
        if (alloc_more) new_cap = to_optimal_cap(new_cap, pr_cap_, m_length());
        if (new_cap == pr_cap_) {
        return true;
        }
        String_temp_data_t past(this);
        if(past.was_static) {
     void*real_ptr=   A_mjz_realloc(nullptr, 0, capacity_to_real_size(new_cap));
        if (!real_ptr) {
        if (can_throw) mjz::Throw("bad alloc");
        return false;
        }
        size_t new_len = min(new_cap, past.m_length); 
        if (SF0_can_share) {
        real_ptr = init_real_ptr_int_then_get_real(real_ptr); 
        }
      Char_t_* mjz_str_data=  real_ptr_to_mjz_str(real_ptr);
        B().set_all_to_dynamic(mjz_str_data, new_len, new_cap, false,
                               SF0_can_share);
      memmove(mjz_str_data, past.m_static_data, new_len);
        mjz_str_data[new_len] = B().nulchr;
            return true;
        }
        void* others_real_ptr = mjz_str_to_real_ptr(B().get_dynamic_string());   
        if (real_ptr_is_sheared(false, B().get_DF0(), B().get_DF1(),
                                others_real_ptr)) {
            String_temp_data_t past_free_obj(this, true);
                 void* real_ptr =A_mjz_realloc(nullptr, 0, capacity_to_real_size(new_cap));
            if (!real_ptr) {
        if (can_throw) mjz::Throw("bad alloc");
        return false;
            }
            size_t new_len = min(new_cap, past.m_length);
            if (SF0_can_share) {
        real_ptr = init_real_ptr_int_then_get_real(real_ptr);
            }
            Char_t_* mjz_str_data = real_ptr_to_mjz_str(real_ptr);
            B().set_all_to_dynamic(mjz_str_data, new_len, new_cap, false,
                                   SF0_can_share);
            memmove(mjz_str_data, past.m_data, new_len);
            mjz_str_data[new_len] = B().nulchr;
            return true;
        }
        if (past.DF1)
            others_real_ptr=deinit_real_ptr_int_then_get_real(others_real_ptr);
        void* real_ptr =
            A_mjz_realloc(others_real_ptr, capacity_to_real_size(past.m_capacity), capacity_to_real_size(new_cap));
        if (!real_ptr) {
          B().set_dynamic_flag<B_t::D_flag1_has_atomic>(false);
            String_temp_data_t past_free_obj(this,true);
            if (can_throw) mjz::Throw("bad alloc");
            return false;
        }
        if (SF0_can_share) {
            real_ptr = init_real_ptr_int_then_get_real(real_ptr);
        }
        size_t new_len = min(new_cap, past.m_length);
        Char_t_* mjz_str_data = real_ptr_to_mjz_str(real_ptr);
        B().set_all_to_dynamic(mjz_str_data, new_len, new_cap, false,
                               SF0_can_share);
        mjz_str_data[new_len] = B().nulchr;
        return true;
}





constexpr inline size_t m_length() const noexcept { return B().m_length(); }

constexpr inline size_t m_capacity() const noexcept { return B().m_capacity(); }

constexpr inline const Char_t_* m_string() const noexcept {
        return B().m_string();
}
constexpr inline   Char_t_* m_string()   noexcept {
        return B().m_string();
}




    mjz_String_memory_class() { B().set_all_to_static(0);
    }
 ~mjz_String_memory_class() { String_temp_data_t deleter(this, true); }
};




int my_main::main(int argc, const char* const* const argv) {
 USE_MJZ_NS();
 mjz_String_memory_class<char> string{};
 auto p = [&]() {
   println('{', string.m_length(), ',', string.m_capacity(), ',',
         string.m_string(),'}');
 };
 for (int i{}; i < 327680000; i += 29) {
        string.realloc_memory(i, 0, 0);
 }
 p();

  return 0;
}
