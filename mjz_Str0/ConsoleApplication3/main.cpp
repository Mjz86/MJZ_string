
#include "my_main.h"
class minimal_mjz_string_data:protected mjzt::algorithm {
  struct dynamic_DB_t {
    char* buffer{};
    size_t length{};
    size_t capacity{};
  };
 private:
 
  constexpr static const size_t power_of_2_offset = sizeof(size_t);
  constexpr static const size_t size_of_minimal_mjz_string_data =    sizeof(dynamic_DB_t) + power_of_2_offset - 2 * sizeof(char);
  static_assert(size_of_minimal_mjz_string_data < 255);
  constexpr static const uint8_t static_storage_cap =
      size_of_minimal_mjz_string_data;
  constexpr static const uint8_t is_external_array_v{255};
  struct static_DB_t {
    char internal_array[static_storage_cap+1]{};
    char internal_array_length{};
    void reset( ) {
      mjz::mjz_obj_manager_template_t<static_DB_t>::obj_destructor_on(this);
      mjz::mjz_obj_manager_template_t<static_DB_t>::obj_constructor_on(this);
    }
  };
  struct DB_t {
    union {  
    dynamic_DB_t db_d; 
    static_DB_t db_s{};
    static_assert(sizeof(dynamic_DB_t) < sizeof(static_DB_t));
    };
    DB_t() {}
    ~DB_t() {}
    DB_t(DB_t&& other) : db_s(other.db_s) {
    // deafult copy
    other.db_s.reset();
    }
    DB_t& operator=(DB_t&& other) {
    db_s = other.db_s;
    other.db_s.reset();
    }
    DB_t& operator=(DB_t const&) = delete;

    DB_t(DB_t const&) = delete;
  };
  constexpr static const uint8_t real_static_size = sizeof(static_DB_t);
  constexpr static const uint8_t real_size = sizeof(DB_t);
  static_assert(real_static_size == real_size);
  DB_t m_db;
 public:
 minimal_mjz_string_data( ) {
 
 }
  inline bool is_dynamic()const {
    return m_db.db_s.internal_array_length == is_external_array_v;
  }
  inline dynamic_DB_t str_set_dynamic(char* buffer, size_t cap) {
    if (!buffer || !cap) return {};
    if (is_dynamic()) {
    dynamic_DB_t per = m_db.db_d;
    if (cap < per.length) return {};
    if (!per.buffer ||!per.length||!per.capacity||per.buffer==buffer)  return per;
    memmove(buffer, per.buffer, per.length);
    m_db.db_d.buffer = buffer;
    m_db.db_d.capacity = cap;
    return per;
    }
    if (m_db.db_s.internal_array_length) 
    memmove(buffer, m_db.db_s.internal_array, m_db.db_s.internal_array_length); 
   dynamic_DB_t ret;
    ret.buffer = 0;
    ret.capacity = static_storage_cap;
    ret.length = m_db.db_s.internal_array_length;
    return ret;
  }
  inline size_t get_cap() const {
    if (is_dynamic()) return m_db.db_d.capacity;
    return static_storage_cap;
  }
  inline const  char* get_str() const {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline   char* get_str()   {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline size_t get_len() const {
    if (is_dynamic()) return m_db.db_d.length;
    return m_db.db_s.internal_array_length;
  }
  inline bool set_len(size_t length)   {
    if (!can_have_len(length)) return false;
    if (is_dynamic()) {
    m_db.db_d.length = length;
    return true;
    }
    m_db.db_s.internal_array_length = length;
    return true; 
  }
  inline bool add_len(size_t add_length) {
    return set_len(get_len() + add_length);
  }
  inline bool can_have_len(size_t length) const { return length <= get_cap(); }
  inline bool can_add_len(size_t add_length) const {
    return can_have_len(get_len() + add_length);
  }
  inline minimal_mjz_string_data& move_to_me(minimal_mjz_string_data&& d) {
    m_db = std::move(d.m_db);
    return *this;
  }
  inline mjz::basic_mjz_Str_view  view() const {
    return {(char*)get_str(), get_len()};
  }

};
/*
Update :
i have to do a hole string rewrite 
because of the memory inefficiency of the Arduino string class 
and my support on its api  
and the mess that is mjz_String 
*/

int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  minimal_mjz_string_data d;
  char* p = d.get_str();
  size_t c = d.get_cap();
  while (c--) {
    *p++ = c +'0';
  }
  d.set_len(d.get_cap());
 auto view= d.view();
  println(view);

  return 0;
}
