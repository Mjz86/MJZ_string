
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

 protected:
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
template <typename Char_t,class mjz_reallocator_t = mjz::default_string_allocator>
class mjz_String_memory_class : public mjz_str_DB_t<Char_t> {

};




int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();

  return 0;
}
