
#include "my_main.h"
namespace mjz_ard {
class minimal_mjz_string_data : protected mjzt::algorithm {
  struct dynamic_DB_t {
    char *buffer{};
    size_t length{};
    size_t capacity{};
  };

 private:
  constexpr static const size_t power_of_2_offset = sizeof(size_t);
  constexpr static const size_t size_of_minimal_mjz_string_data =
      sizeof(dynamic_DB_t) + power_of_2_offset - 2 * sizeof(char);
  static_assert(size_of_minimal_mjz_string_data < 255);
  constexpr static const uint8_t static_storage_cap =
      size_of_minimal_mjz_string_data;
  constexpr static const uint8_t is_external_array_v{255};
  struct static_DB_t {
    char internal_array[static_storage_cap + 1]{};
    uint8_t internal_array_length{};
    void reset() {
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
    DB_t(DB_t &&other) : db_s(other.db_s) {
      // deafult copy
      other.db_s.reset();
    }
    DB_t &operator=(DB_t &&other) {
      db_s = other.db_s;
      other.db_s.reset();
    }
    DB_t &operator=(DB_t const &) = delete;

    DB_t(DB_t const &) = delete;
  };
  constexpr static const uint8_t real_static_size = sizeof(static_DB_t);
  constexpr static const uint8_t real_size = sizeof(DB_t);
  static_assert(real_static_size == real_size);
  DB_t m_db;

 public:
  minimal_mjz_string_data() {}
  inline bool is_dynamic() const {
    return m_db.db_s.internal_array_length == is_external_array_v;
  }
  inline dynamic_DB_t str_set_dynamic(char *buffer, size_t cap) {
    if (!buffer || !cap) return {};
    if (is_dynamic()) {
      dynamic_DB_t per = m_db.db_d;
      if (cap < per.length) return {};
      if (!per.buffer || !per.length || !per.capacity || per.buffer == buffer)
        return per;
      memmove(buffer, per.buffer, per.length);
      m_db.db_d.buffer = buffer;
      m_db.db_d.capacity = cap;
      return per;
    }
    if (m_db.db_s.internal_array_length)
      memmove(buffer, m_db.db_s.internal_array,
              m_db.db_s.internal_array_length);
    dynamic_DB_t ret;
    ret.buffer = 0;
    ret.capacity = static_storage_cap;
    ret.length = m_db.db_s.internal_array_length;
    m_db.db_s.internal_array_length = is_external_array_v;
    m_db.db_d.buffer = buffer;
    m_db.db_d.capacity = cap;
    m_db.db_d.length = ret.length;
    return ret;
  }
  inline size_t get_cap() const {
    if (is_dynamic()) return m_db.db_d.capacity;
    return static_storage_cap;
  }
  inline const char *get_str() const {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline char *get_str() {
    if (is_dynamic()) return m_db.db_d.buffer;
    return m_db.db_s.internal_array;
  }
  inline size_t get_len() const {
    if (is_dynamic()) return m_db.db_d.length;
    return m_db.db_s.internal_array_length;
  }
  inline bool set_len(size_t length) {
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
  inline minimal_mjz_string_data &move_to_me(minimal_mjz_string_data &&d) {
    m_db = std::move(d.m_db);
    return *this;
  }
};

template <typename T>
concept no_decay_c_str =
    requires {
      requires(std::is_same_v<T, const char *> || std::is_same_v<T, char *>);
    };
template <typename T>
concept mjz_Str_data_strorage = requires(T obj, const T cobj) {
                                  obj.get_buffer();
                                  cobj.get_buffer();
                                  obj.get_length();
                                  cobj.get_length();
                                };

template <mjz_Str_data_strorage Base_t>
class basic_mjz_Str_view : protected Base_t, protected static_str_algo {
 protected:
  using Base_t::get_buffer;
  using Base_t::get_length;  // the ptr length (not counting the '\0')
  using static_str_algo;
  Base_t &get_base() { return *this; }
  const Base_t &get_base() const { return *this; }

 public:
  constexpr static size_t npos = -1;
  constexpr inline const char *begining_of_str_ptr() const {
    return get_buffer();
  }
  constexpr inline const char *ending_of_str_ptr() const {
    return get_buffer() + get_length();
  }

 public:
  constexpr basic_mjz_Str_view(const char *const buffer, size_t length)
      : Base_t(buffer, length) {}
  template <size_t N>
  constexpr basic_mjz_Str_view(const char (&const a)[N])
      : basic_mjz_Str_view(a, N - (!a[N - 1])) {}
  constexpr basic_mjz_Str_view(const just_str_view_data &s)
      : basic_mjz_Str_view(s.buffer, s.len) {}
  constexpr explicit inline basic_mjz_Str_view(no_decay_c_str auto c_string)
      : basic_mjz_Str_view(c_string, strlen(c_string)) {}
  constexpr basic_mjz_Str_view()
      : basic_mjz_Str_view(static_str_algo::empty_STRING_C_STR, 0) {}
  constexpr inline ~basic_mjz_Str_view() = default;
  constexpr basic_mjz_Str_view &operator=(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view &operator=(const basic_mjz_Str_view &) = default;
  constexpr basic_mjz_Str_view(basic_mjz_Str_view &&) = default;
  constexpr basic_mjz_Str_view(const basic_mjz_Str_view &) = default;

 public:
  constexpr bool is_blank() const {
    size_t i{};
    while (i < get_length()) {
      if (!is_blank_characteres_default(get_buffer()[i])) {
        return 0;
      }
      i++;
    }
    return 1;
  }
  constexpr bool empty() const {
    return (!get_length() || get_buffer() == empty_STRING_C_STR ||
            get_buffer() == nullptr);
  }
  constexpr inline size_t length(void) const { return get_length(); }
  constexpr inline char *data() { return get_buffer(); }
  constexpr inline const char *data() const { return get_buffer(); }

 public:
  constexpr inline const char *c_str() const & { return get_buffer(); }
  constexpr inline const char *c_str() & { return get_buffer(); }
  constexpr inline char *C_str() & { return get_buffer(); }

 public:
  constexpr explicit operator const bool() const { return !is_blank(); }
  constexpr char operator[](int64_t index_) const {
    size_t index = signed_index_to_unsigned(index_);
    if ((size_t)index >= get_length() || !get_buffer()) {
      return 0;
    }
    return get_buffer()[index];
  }
  constexpr char operator[](size_t index) const {
    if (index >= get_length() || !get_buffer()) {
      return 0;
    }
    return get_buffer()[index];
  }
  constexpr bool operator!() const { return is_blank(); }
  // parsing/conversion
  constexpr long long toLL(void) const {
    if (get_buffer()) {
      return to_LL();
    }
    return 0;
  }
  constexpr long long to_LL(int radix = 10, bool *had_error = 0,
                            uint8_t error_level = 0) const {
    if (get_buffer()) {
      return C_STR_to_LL(c_str(), (uint8_t)min(length(), (uint64_t)255), radix,
                         had_error, error_level);
    }
    return 0;
  }
  constexpr long toInt(void) const { return (long)toLL(); }
  constexpr float toFloat(void) const {
    //
    return float(toDouble());
  }
  constexpr double toDouble(void) const {
    if (get_buffer()) {
      char *ptr{};
      return strtod(get_buffer(), &ptr);
    }
    return 0;
  }
  constexpr friend bool operator==(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return a.equals(b);
  }
  constexpr friend bool operator==(const basic_mjz_Str_view &a, const char *b) {
    return a.equals(b);
  }
  constexpr friend bool operator==(const char *a, const basic_mjz_Str_view &b) {
    return b == a;
  }
  constexpr friend bool operator<(const basic_mjz_Str_view &a,
                                  const basic_mjz_Str_view &b) {
    return a.compareTo(b) < 0;
  }
  constexpr friend bool operator<(const basic_mjz_Str_view &a, const char *b) {
    return a.compareTo(b) < 0;
  }
  constexpr friend bool operator<(const char *a, const basic_mjz_Str_view &b) {
    return b.compareTo(a) > 0;
  }
  constexpr friend bool operator!=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(a == b);
  }
  constexpr friend bool operator!=(const basic_mjz_Str_view &a, const char *b) {
    return !(a == b);
  }
  constexpr friend bool operator!=(const char *a, const basic_mjz_Str_view &b) {
    return !(a == b);
  }
  constexpr friend bool operator>(const basic_mjz_Str_view &a,
                                  const basic_mjz_Str_view &b) {
    return b < a;
  }
  constexpr friend bool operator>(const basic_mjz_Str_view &a, const char *b) {
    return b < a;
  }
  constexpr friend bool operator>(const char *a, const basic_mjz_Str_view &b) {
    return b < a;
  }
  constexpr friend bool operator<=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(b < a);
  }
  constexpr friend bool operator<=(const basic_mjz_Str_view &a, const char *b) {
    return !(b < a);
  }
  constexpr friend bool operator<=(const char *a, const basic_mjz_Str_view &b) {
    return !(b < a);
  }
  constexpr friend bool operator>=(const basic_mjz_Str_view &a,
                                   const basic_mjz_Str_view &b) {
    return !(a < b);
  }
  constexpr friend bool operator>=(const basic_mjz_Str_view &a, const char *b) {
    return !(a < b);
  }
  constexpr friend bool operator>=(const char *a, const basic_mjz_Str_view &b) {
    return !(a < b);
  }
  friend std::ostream &operator<<(std::ostream &COUT,
                                  const basic_mjz_Str_view &s) {
    COUT.write(s.data(), s.length());
    return COUT;
  }
  // hash function
  constexpr hash_sha256 mjz_hash(uint8_t n = 0) const {
    return SHA256_CTX::hash_msg_to_sha_512_n(c_str(), length(), n);
  }
  constexpr hash_sha256 hash() const { return mjz_hash(0); }
  constexpr int64_t compareTo(const basic_mjz_Str_view &s) const {
    return compare_two_str(get_buffer(), get_length(), s.get_buffer(),
                           s.get_length());
  }
  constexpr int64_t compareTo(const char *cstr) const {
    return compare_two_str(get_buffer(), get_length(), cstr, strlen(cstr));
  }
  constexpr inline int compare(const basic_mjz_Str_view &str) const {
    return compareTo(str);
  }
  constexpr inline int compare(size_t pos, size_t len,
                               const basic_mjz_Str_view &str) const {
    return substr_view_beg_n(pos, len).compareTo(str);
  }
  constexpr inline int compare(size_t pos, size_t len,
                               const basic_mjz_Str_view &str, size_t subpos,
                               size_t sublen) const {
    return substr_view_beg_n(pos, len).compareTo(
        str.substr_view_beg_n(subpos, sublen));
  }
  constexpr inline int compare(const char *s, size_t n) const {
    return compare_two_str(get_buffer(), get_length(), s, n);
  }
  constexpr inline int compare(size_t pos, size_t len, const char *s,
                               size_t n) const {
    return substr_view_beg_n(pos, len).compare(s, n);
  }
  constexpr inline int compare(size_t pos, size_t len, const char *s) const {
    return compare(pos, len, s, strlen(s));
  }
  constexpr inline int compare(const char *s) const {
    return compare(s, strlen(s));
  }
  constexpr inline bool starts_with(const char *s, size_t n) const {
    return substr_view(0ULL, n).equals(s, n);
  }
  constexpr inline bool starts_with(const basic_mjz_Str_view &s) const {
    return starts_with(s.c_str(), s.length());
  }
  constexpr inline bool starts_with(char c) const { return c == *c_str(); }
  constexpr inline bool starts_with(const char *s) const {
    return starts_with(s, strlen(s));
  }
  constexpr inline bool ends_with(const char *s, size_t n) const {
    return substr_view(length() - n, length()).equals(s, n);
  }
  constexpr inline bool ends_with(const basic_mjz_Str_view &s) const {
    return ends_with(s.c_str(), s.length());
  }
  constexpr inline bool ends_with(char c) const {
    return c == c_str()[length() - 1];
  }
  constexpr inline bool ends_with(const char *s) const {
    return ends_with(s, strlen(s));
  }
  constexpr inline int64_t indexOf(const basic_mjz_Str_view &s2) const {
    return indexOf(s2, 0);
  }
  constexpr inline int64_t indexOf(const basic_mjz_Str_view &s2,
                                   size_t fromIndex) const {
    return indexOf_cstr(s2.data(), s2.length(), fromIndex);
  }
  constexpr inline int64_t lastIndexOf(const basic_mjz_Str_view &s2) const {
    return lastIndexOf(s2, get_length() - s2.get_length());
  }
  constexpr inline int64_t lastIndexOf(const basic_mjz_Str_view &s2,
                                       size_t fromIndex) const {
    return lastIndexOf_cstr(s2.c_str(), s2.length(), fromIndex);
  }
  constexpr inline size_t find(const basic_mjz_Str_view &str,
                               size_t pos = 0) const {
    return indexOf(str, pos);
  }
  constexpr inline size_t find(const char *s, size_t pos = 0) const {
    return indexOf_cstr(s, strlen(s), pos);
  }
  constexpr inline size_t find(const char *s, size_t pos, size_t n) const {
    return indexOf_cstr(s, n, pos);
  }
  constexpr inline size_t find(char c, size_t pos = 0) const {
    return indexOf(c, pos);
  }
  constexpr inline size_t rfind(const basic_mjz_Str_view &str,
                                size_t pos = npos) const {
    return lastIndexOf(str, pos);
  }
  constexpr inline size_t rfind(const char *s, size_t pos = npos) const {
    return lastIndexOf_cstr(s, strlen(s), pos);
  }
  constexpr inline size_t rfind(const char *s, size_t pos, size_t n) const {
    return lastIndexOf_cstr(s, n, pos);
  }
  constexpr inline size_t rfind(char c, size_t pos = npos) const {
    return lastIndexOf(c, pos);
  }
  constexpr inline size_t find_first_of_in_str(const char *s, size_t n) const {
    return static_str_algo::find_first_of_in_str(get_buffer(), get_length(), s,
                                                 n);
  }
  constexpr inline size_t find_last_of_in_str(const char *s, size_t n) const {
    return static_str_algo::find_last_of_in_str(get_buffer(), get_length(), s,
                                                n);
  }
  constexpr inline size_t find_first_of_in_str(
      const basic_mjz_Str_view &str) const {
    return find_first_of_in_str(str.get_buffer(), str.get_length());
  }
  constexpr inline size_t find_first_of(const basic_mjz_Str_view &str,
                                        size_t pos = 0) const {
    return substr_view(pos).find_first_of_in_str(str);
  }
  constexpr inline size_t find_first_of(const char *s, size_t pos,
                                        size_t n) const {
    return substr_view(pos).find_first_of_in_str(s, n);
  }
  constexpr inline size_t find_first_of(const char *s, size_t pos = 0) const {
    return find_first_of(s, pos, strlen(s));
  }
  constexpr inline size_t find_first_of(char c, size_t pos = 0) const {
    return find_first_of(&c, pos, 1);
  }
  constexpr inline size_t find_last_of(const basic_mjz_Str_view &str,
                                       size_t pos = npos) const {
    return substr_view(pos).find_last_of_in_str(str.data(), str.length());
  }
  constexpr inline size_t find_last_of(const char *s, size_t pos = npos) const {
    return substr_view(pos).find_last_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_last_of(const char *s, size_t pos,
                                       size_t n) const {
    return substr_view(pos).find_last_of_in_str(s, n);
  }
  constexpr inline size_t find_last_of(char c, size_t pos = npos) const {
    return find_last_of(&c, pos, 1);
  }
  constexpr inline size_t find_last_not_of_in_str(const char *s,
                                                  size_t n) const {
    return static_str_algo::find_last_of_not_in_str(get_buffer(), get_length(),
                                                    s, n);
  }
  constexpr inline size_t find_last_not_of(const basic_mjz_Str_view &str,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(str.c_str(), str.length());
  }
  constexpr inline size_t find_last_not_of(const char *s,
                                           size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_last_not_of(const char *s, size_t pos,
                                           size_t n) const {
    return substr_view(pos).find_last_not_of_in_str(s, n);
  }
  constexpr inline size_t find_last_not_of(char c, size_t pos = npos) const {
    return substr_view(pos).find_last_not_of_in_str(&c, 1);
  }
  constexpr inline size_t find_first_not_of_in_str(const char *s,
                                                   size_t n) const {
    return static_str_algo::find_first_of_not_in_str(get_buffer(), get_length(),
                                                     s, n);
  }
  constexpr inline size_t find_first_not_of(const basic_mjz_Str_view &str,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(str.c_str(), str.length());
  }
  constexpr inline size_t find_first_not_of(const char *s,
                                            size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(s, strlen(s));
  }
  constexpr inline size_t find_first_not_of(const char *s, size_t pos,
                                            size_t n) const {
    return substr_view(pos).find_first_not_of_in_str(s, n);
  }
  constexpr inline size_t find_first_not_of(char c, size_t pos = 0) const {
    return substr_view(pos).find_first_not_of_in_str(&c, 1);
  }
  constexpr bool equals(const char *cstr, size_t cstr_len) const {
    return are_two_str_equale(get_buffer(), get_length(), cstr, cstr_len);
  }
  constexpr bool equals(const basic_mjz_Str_view &s2) const {
    //
    return equals(s2.data(), s2.length());
  }
  constexpr inline bool equals(const char *cstr) const {
    return equals(cstr, strlen(cstr));
  }
  constexpr inline size_t signed_index_to_unsigned(int64_t input) const {
    size_t index{length()};
    if (input < 0) {
      index = (size_t)(input + (int64_t)length());
    } else if (input <= (int64_t)length()) {
      index = (size_t)input;
    }
    return index;
  }
  constexpr size_t UN_ORDERED_compare(const basic_mjz_Str_view &s) const {
    return UN_ORDERED_compare(s.get_buffer(), s.get_length());
  }
  constexpr size_t UN_ORDERED_compare(const char *s, size_t s_len) const {
    const unsigned char *ucs_ = (const unsigned char *)s;
    const unsigned char *ucbuffer_ = (const unsigned char *)this->get_buffer();
    size_t number_of_not_right{};
    size_t NUMBER_OF_EACH_char_array[2][256]{};
    for (size_t i{}; i < s_len; i++) {
      NUMBER_OF_EACH_char_array[0][ucs_[i]]++;
    }
    for (size_t i{}; i < this->length(); i++) {
      NUMBER_OF_EACH_char_array[1][ucbuffer_[i]]++;
    }
    for (size_t i{}; i < 256; i++) {
      number_of_not_right +=
          (size_t)abs((int64_t)NUMBER_OF_EACH_char_array[1][i] -
                      (int64_t)NUMBER_OF_EACH_char_array[0][i]);
    }
    return number_of_not_right;
  }
  constexpr bool equalsIgnoreCase(const basic_mjz_Str_view &s2) const {
    if (this == &s2) {
      //
      return true;
    }
    if (get_length() != s2.get_length()) {
      //
      return false;
    }
    if (get_length() == 0) {
      //
      return true;
    }
    const char *p1 = get_buffer();
    const char *p2 = s2.get_buffer();
    const char *end_p1 = get_buffer() + length();
    while (p1 < end_p1) {
      if (tolower(*p1++) != tolower(*p2++)) {
        //
        return false;
      }
    }
    //
    return true;
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2) const {
    if (get_length() < s2.get_length()) {
      //
      return false;
    }
    //
    return startsWith(s2, 0);
  }
  constexpr bool startsWith(const basic_mjz_Str_view &s2, size_t offset) const {
    if (offset > get_length() - s2.get_length() || !get_buffer() ||
        !s2.get_buffer()) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&get_buffer()[offset], s2.get_buffer(),
                       s2.get_length()) == 0;
  }
  constexpr bool endsWith(const basic_mjz_Str_view &s2) const {
    if (get_length() < s2.get_length() || !get_buffer() || !s2.get_buffer()) {
      //
      return false;
    }
    //
    return MJZ_STRnCMP(&get_buffer()[get_length() - s2.get_length()],
                       s2.get_buffer(), s2.get_length()) == 0;
  }
  // Function that return the length
  constexpr size_t size() const { return length(); }
  constexpr char charAt(int64_t loc) const {
    return operator[](signed_index_to_unsigned(loc));
  }
  constexpr char charAt(size_t loc) const {
    //
    return operator[](loc);
  }
  constexpr void getBytes(unsigned char *buf, size_t bufsize,
                          size_t index = 0) const {
    if (!bufsize || !buf) {
      //
      return;
    }
    if (index >= get_length()) {
      buf[0] = 0;
      //
      return;
    }
    size_t n = bufsize - 1;
    if (n > get_length() - index) {
      n = get_length() - index;
    }
    memmove((char *)buf, get_buffer() + index, min_macro_(n, get_length()));
    buf[n] = 0;
    //
  }
  constexpr inline void toCharArray(char *buf, size_t bufsize,
                                    size_t index = 0) const {
    getBytes((unsigned char *)buf, bufsize, index);
  }
  constexpr int64_t indexOf(char c) const { return indexOf(c, 0); }
  constexpr int64_t indexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *temp =
        strchr(get_buffer() + fromIndex, length() + fromIndex, ch);
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - get_buffer());
  }
  constexpr int64_t indexOf_cstr(const char *c_str_, size_t len_str,
                                 size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *found =
        strstr(get_buffer() + fromIndex, length() - fromIndex, c_str_, len_str);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - get_buffer());
  }
  constexpr int64_t lastIndexOf_cstr(const char *cstr__, size_t length__,
                                     size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *found = r_strstr(get_buffer() + fromIndex, length() - fromIndex,
                                 cstr__, length__);
    if (found == NULL) {
      return -1;
    }
    return (int64_t)(found - get_buffer());
  }
  constexpr inline int64_t lastIndexOf_cstr(const char *cstr__,
                                            size_t length__) const {
    return lastIndexOf_cstr(cstr__, length__, 0);
  }
  constexpr int64_t lastIndexOf(char theChar) const {
    return lastIndexOf(theChar, get_length() - 1);
  }
  constexpr int64_t lastIndexOf(char ch, size_t fromIndex) const {
    if (fromIndex >= get_length()) {
      return -1;
    }
    const char *temp = strrchr(get_buffer() + fromIndex, length(), ch);
    if (temp == NULL) {
      return -1;
    }
    return (int64_t)(temp - get_buffer());
  };
  constexpr const char *substring_give_ptrULL(size_t left, size_t right,
                                              const char *&c_str_out,
                                              size_t &len_out) const {
    if (left > right) {
      size_t temp = right;
      right = left;
      left = temp;
    }
    if (left >= get_length()) {
      //
      len_out = 0;
      return (c_str_out = 0);
    }
    if (right > get_length()) {
      right = get_length();
    }
    c_str_out = get_buffer() + left;
    len_out = right - left;
    return c_str_out;
  }
  constexpr const char *substring_give_ptr(int64_t left, int64_t right,
                                           const char *&c_str_out,
                                           size_t &len_out) const {
    return substring_give_ptrULL(signed_index_to_unsigned(left),
                                 signed_index_to_unsigned(right), c_str_out,
                                 len_out);
  }
  constexpr const char at(int64_t i) { return operator[](i); }
  constexpr const char back() { return get_buffer()[0]; }
  constexpr const char front() { return operator[]((int64_t)-1); }
  constexpr inline basic_mjz_Str_view substr_view(size_t beginIndex,
                                                  size_t endIndex) const {
    const char *out_ptr{};
    size_t out_len{};
    substring_give_ptrULL(beginIndex, endIndex, out_ptr, out_len);
    return out_len ? basic_mjz_Str_view(out_ptr, out_len)
                   : basic_mjz_Str_view();
  }
  constexpr inline basic_mjz_Str_view substr_view(int64_t beginIndex,
                                                  int64_t endIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(endIndex));
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  constexpr inline basic_mjz_Str_view substr_view(int64_t beginIndex) const {
    return substr_view(signed_index_to_unsigned(beginIndex));
  }
  constexpr inline basic_mjz_Str_view substr_view(int beginIndex,
                                                  int endIndex) const {
    return substr_view((int64_t)beginIndex, (int64_t)endIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int beginIndex,
                                                        int number) const {
    return substr_view_beg_n((int64_t)beginIndex, (size_t)number);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(size_t beginIndex,
                                                        size_t number) const {
    return substr_view(beginIndex, number + beginIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view(size_t beginIndex) const {
    return substr_view(beginIndex, length() - beginIndex);
  }
  constexpr inline basic_mjz_Str_view substr_view_beg_n(int64_t beginIndex,
                                                        size_t number) const {
    return substr_view(signed_index_to_unsigned(beginIndex),
                       signed_index_to_unsigned(beginIndex) + number);
  }
  constexpr inline size_t max_size() const { return (((size_t)(-1)) >> 1) - 1; }
};
/*
Update :
i have to do a hole string rewrite
because of the memory inefficiency of the Arduino string class
and my support on its api
and the mess that is mjz_String
*/
}  // namespace mjz_ard
int my_main::main(int argc, const char* const* const argv) {
  USE_MJZ_NS();
  minimal_mjz_string_data d;
  char s[40+1]{};
  {
    char* ptr = d.get_str();
    char* ptr_ = d.get_str();
    for (const char& c : "1234567890abcdef") {
    *ptr++ = c;
    }
    d.set_len(ptr - ptr_);
    d.str_set_dynamic(s, NumberOf(s));
  }
  auto str=  std::string_view(d.get_str(), d.get_len());
auto ptr= str.data();
  println(str);

  return 0;
}
