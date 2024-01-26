
template <bool B, class T, class BS>
struct mjz_stack_obj_or_err_warper_template_t_data : public BS {
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());
  static_assert(B);
  static_assert(!B);
};

template <class T, class BS>
struct mjz_stack_obj_or_err_warper_template_t_data<true, T, BS> : public BS {
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());

 protected:
  union {
    bool m_Has_data{false};
    mutable bool mm_Has_data;
  };

 public:
  union UDB_t {
    alignas(BS::alignment) /*aliens with T */ mutable uint8_t
        mm_data[BS::size_of_type()];
    alignas(BS::alignment) uint8_t m_data[BS::size_of_type()];
    T m_obj____;
    mutable T mm_obj____;
  };
  UDB_t m_obj_buf{};

  constexpr inline uint8_t *mm_data_b() {
    return (uint8_t *)&m_obj_buf.mm_obj____;
  }

  constexpr inline const uint8_t *m_data_b() const {
    return (const uint8_t *)&m_obj_buf.m_obj____;
  }

  constexpr inline uint8_t *m_data_b() {
    return (uint8_t *)&m_obj_buf.m_obj____;
  }
};
template <class T, class BS>
struct mjz_stack_obj_or_err_warper_template_t_data<false, T, BS> : public BS {
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());

 protected:
  union {
    bool m_Has_data{false};
    mutable bool mm_Has_data;
  };

 public:
  union UDB_t {
    alignas(1) /*no alinement*/ mutable uint8_t mm_data[BS::size_of_type()];
    alignas(1) uint8_t m_data[BS::size_of_type()];
  };
  UDB_t m_obj_buf{};
  constexpr inline uint8_t *mm_data_b() { return m_obj_buf.mm_data; }
  constexpr inline const uint8_t *m_data_b() const { return m_obj_buf.m_data; }
  constexpr inline uint8_t *m_data_b() { return m_obj_buf.m_data; }
};

template <typename T, class obj_crtr, bool use_object_in_union>
struct mjz_stack_obj_or_err_warper_template_t_helper {};
template <typename T, class obj_crtr>
struct mjz_stack_obj_or_err_warper_template_t_helper<T, obj_crtr, false>
    : protected mjz_stack_obj_or_err_warper_template_t_data<std::is_fundamental_v<T>,
                                                     T, obj_crtr> {
  constexpr bool static const has_data_v = 1;
 protected:
  template <typename ret_t>
  constexpr inline ret_t *mm_data() {
    return (ret_t *)this->mm_data_b();
  }
  template <typename ret_t>
  constexpr inline const ret_t *m_data() const {
    return (const ret_t *)this->m_data_b();
  }

  template <typename ret_t>
  constexpr inline ret_t *m_data() {
    return (ret_t *)this->m_data_b();
  }

 public:
  constexpr inline bool get_has_data() const volatile noexcept {
    return this->m_Has_data == has_data_v;
  }
  constexpr inline bool get_has_data() const noexcept {
    return this->m_Has_data == has_data_v;
  }
  constexpr  inline bool set_has_data(bool B) volatile noexcept {
    if (B) this->m_Has_data = has_data_v;
    return B;
  }
  constexpr inline bool set_has_data(bool B) noexcept {
    if (B) this->m_Has_data = has_data_v;
    return B;
  }
};
template <typename T, class obj_crtr>
struct mjz_stack_obj_or_err_warper_template_t_helper<T, obj_crtr, true>
    : protected obj_crtr {
  constexpr bool static const has_data_v = 1;
  using Type = T;
  using BS = obj_crtr;
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());

 protected:
  union {
    bool m_Has_data{false};
    mutable bool mm_Has_data;
  };

 public:
  union UDB_t {
    uint8_t f;
    mutable uint8_t mm_data[BS::size_of_type()];
    uint8_t m_data[BS::size_of_type()];
    T obj;
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  UDB_t m_obj_buf{};

 protected:
  template <typename ret_t>
  constexpr inline ret_t *mm_data() {
    return (ret_t *)&m_obj_buf.f;
  }
  template <typename ret_t>
  constexpr inline const ret_t *m_data() const {
    return (const ret_t *)&m_obj_buf.f;
  }
  template <typename ret_t>
  constexpr inline ret_t *m_data() {
    return (ret_t *)&m_obj_buf.f;
  }
  template <>
  constexpr inline Type *mm_data<Type>() {
    return this->addressof(m_obj_buf.obj);
  }
  template <>
  constexpr inline const Type *m_data<Type>() const {
    return this->addressof(m_obj_buf.obj);
  }

  template <>
  constexpr inline Type *m_data<Type>() {
    return this->addressof(m_obj_buf.obj);
  }

  template <>
  constexpr inline uint8_t *mm_data<uint8_t>() {
    return &m_obj_buf.f;
  }
  template <>
  constexpr inline const uint8_t *m_data<uint8_t>() const {
    return &m_obj_buf.f;
  }

  template <>
  constexpr inline uint8_t *m_data<uint8_t>() {
    return &m_obj_buf.f;
  }

 public:
  constexpr inline bool get_has_data() const volatile noexcept {
    return this->m_Has_data == has_data_v;
  }
  constexpr inline bool get_has_data() const noexcept {
    return this->m_Has_data == has_data_v;
  }
  constexpr inline bool set_has_data(bool B) volatile noexcept {
    if (B) this->m_Has_data = has_data_v;
    return B;
  }
  constexpr inline bool set_has_data(bool B) noexcept {
    if (B) this->m_Has_data = has_data_v;
    return B;
  }
};
