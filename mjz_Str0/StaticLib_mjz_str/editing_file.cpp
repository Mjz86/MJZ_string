
template <typename T, C_mjz_obj_manager obj_crtr, C_mjz_obj_manager err_crtr,
          mjz_stack_obj_warper_template_t_data_states m_in_uinion,
          typename in_Error_t = void>
struct mjz_stack_obj_warper_deafualt_data_storage_template_t
    : protected obj_crtr,
      protected err_crtr {
 private:
  template <typename Error_T>
  using select_err_t =
      mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t<
          Error_T>;
  using BS = obj_crtr;

 public:
  using Error_t = typename select_err_t<in_Error_t>::Error_t;
  constexpr static const bool class_has_Error =
      select_err_t<in_Error_t>::class_has_Error;

 private:
  using err_crtr_t = err_crtr;
  using obj_crtr_t = obj_crtr;
  using unsafe_err_t =typename
      err_crtr_t::template simple_unsafe_init_obj_wrpr<false, false>;
  template <mjz_stack_obj_warper_template_t_data_states is_in_uinion>
  union UDB_t {};
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_aligned_chars> {
    const constexpr static size_t alinement_v =
        calucate::KMM_algo(BS::alignment, alignof(unsafe_err_t));
    uint8_t f;
    alignas(alinement_v) /*aliens with T */ mutable uint8_t
        mm_data[BS::size_of_type()];
    alignas(alinement_v) uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return *(T *)m_data; }
    constexpr inline const T &obj_ref() const { return *(const T *)m_data; }
    constexpr inline unsafe_err_t &err_ref() { return *(unsafe_err_t *)m_data; }
    constexpr inline const unsafe_err_t &err_ref() const {
      return *(const unsafe_err_t *)m_data;
    }
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_chars> {
    uint8_t f;
    alignas(1) /*no alinement*/ mutable uint8_t mm_data[BS::size_of_type()];
    alignas(1) uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return *(T *)m_data; }
    constexpr inline const T &obj_ref() const { return *(const T *)m_data; }

    constexpr inline unsafe_err_t &err_ref() { return *(unsafe_err_t *)m_data; }
    constexpr inline const unsafe_err_t &err_ref() const {
      return *(const unsafe_err_t *)m_data;
    }
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  template <>
  union UDB_t<mjz_stack_obj_warper_template_t_data_states::in_union> {
    uint8_t f;
    mutable uint8_t mm_data[BS::size_of_type()];
    uint8_t m_data[BS::size_of_type()];
    constexpr inline T &obj_ref() { return obj; }
    constexpr inline const T &obj_ref() const { return obj; }

    constexpr inline unsafe_err_t &err_ref() { return error; }
    constexpr inline const unsafe_err_t &err_ref() const { return error; }
    T obj;
    unsafe_err_t error;
    UDB_t() {}   // no init
    ~UDB_t() {}  // no deinit
  };
  UDB_t<m_in_uinion> m_obj_buf{};

 public:
  enum class data_state : uint8_t { none = 0, value, error };
  using Type = T;
  using BS = obj_crtr;
  static_assert(BS::size_of_type() && sizeof(T) <= BS::size_of_type());

 protected:
  union {
    data_state m_state{false};
    mutable data_state mm_state;
  };

 public:
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
    return this->addressof(m_obj_buf.obj_ref());
  }
  template <>
  constexpr inline const Type *m_data<Type>() const {
    return obj_crtr_t::addressof(m_obj_buf.obj_ref());
  }

  template <>
  constexpr inline Type *m_data<Type>() {
    return obj_crtr_t::addressof(m_obj_buf.obj_ref());
  }

  template <>
  constexpr inline Error_t *mm_data<Error_t>() {
    return m_obj_buf.err_ref().ptr();
  }
  template <>
  constexpr inline const Error_t *m_data<Error_t>() const {
    return m_obj_buf.err_ref().ptr();
  }

  template <>
  constexpr inline Error_t *m_data<Error_t>() {
    return m_obj_buf.err_ref().ptr();
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
  constexpr inline bool get_has_error() const volatile noexcept {
    return m_state == data_state::error;
  }
  constexpr inline bool get_has_error() const noexcept {
    return m_state == data_state::error;
  }
  constexpr inline bool get_has_data() const volatile noexcept {
    return m_state == data_state::value;
  }
  constexpr inline bool get_has_data() const noexcept {
    return m_state == data_state::value;
  }
  mjz_stack_obj_warper_deafualt_data_storage_template_t() {}
  ~mjz_stack_obj_warper_deafualt_data_storage_template_t() {
    if (get_has_data()) {
          
  obj_crtr_t::destroy_at(m_data<Type>());
      /* i really dont like this condition*/
      /*Throw<const char*const>(" unexpected leaked object ");*/
    } else if constexpr (!class_has_Error) {
      m_state = data_state::none;
      return;
    }
    if (get_has_error())
      destrouy_err();
    else
      m_state = data_state::none;
  }

 private:
  constexpr inline void destrouy_err() {
    if constexpr (!class_has_Error) {
      data_state::none;
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
      m_state = data_state::none;
    }
  }

 public:
  constexpr inline void base_unsafe_unsafe_set_type(data_state cast_to_type) {
    m_state = cast_to_type;
  }

 public:
  constexpr inline void base_notify_value_unsafe_init_start() {
    if (get_has_data()) {
      obj_crtr_t::destroy_at(m_data<Type>());
    } else if constexpr (!class_has_Error)
      return;
    else if (get_has_error())
      destrouy_err();
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_deinit_start() {
    if constexpr (!class_has_Error) {
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
    }
    /* else if (!get_has_data()) {
    * i really dont like this condition
    *Throw<const char*const>(" unexpected leaked object ");
    }*/
  }
  constexpr inline void base_notify_value_unsafe_init_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_deinit_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_value_unsafe_init_end() {
    m_state = data_state::value;
  }
  constexpr inline void base_notify_value_unsafe_deinit_end() {
    m_state = data_state::none;
  }
  
  constexpr inline void base_notify_error_unsafe_init_start() {
    if (get_has_data()) {
      obj_crtr_t::destroy_at(m_data<Type>());
    } else if constexpr (!class_has_Error)
      return;
    else if (get_has_error())
      destrouy_err();
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_deinit_start() {
    if constexpr (!class_has_Error) {
      return;
    }
    if (get_has_error()) {
      m_obj_buf.err_ref().unsafe_destroy();
    }
    /* else if (!get_has_data()) {
    * i really dont like this condition
    *Throw<const char*const>(" unexpected leaked object ");
    }*/
  }
  constexpr inline void base_notify_error_unsafe_init_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_deinit_fail() {
    m_state = data_state::none;
  }
  constexpr inline void base_notify_error_unsafe_init_end() {
    m_state = data_state::error;
  }
  constexpr inline void base_notify_error_unsafe_deinit_end() {
    m_state = data_state::none;
  }
};
template <typename T, bool can_be_void>
concept C_is_valid_optional_type_helper =
    requires() {
      requires std::is_same_v<std::remove_cvref_t<T>, T>;
      requires !std::is_array_v<T>;
    };
template <typename T, bool can_be_void = 0>
concept C_is_valid_optional_type =
    C_is_valid_optional_type_helper<T, can_be_void> ||
    (can_be_void && std::is_same_v<void, T>);

template <
    C_is_valid_optional_type my_iner_Type_,
    bool construct_obj_on_constructor = true,
    C_mjz_temp_type_obj_algorithims_warpper my_obj_creator_t =
        mjz_temp_type_obj_algorithims_warpper_t< my_iner_Type_ >,
    bool do_error_check = 1, bool use_object_in_union = false,
    template <typename,class,class,mjz_stack_obj_warper_template_t_data_states,class>
    class mjz_stack_obj_warper_data_storage_template_t =
        mjz_stack_obj_warper_deafualt_data_storage_template_t,
    C_is_valid_optional_type<1> data_strorage_in_Error_t = void,
    C_mjz_temp_type_obj_algorithims_warpper my_err_creator_t =
        mjz_temp_type_obj_algorithims_warpper_t<
            mjz_stack_obj_warper_deafualt_data_storage_template_t_select_err_t<
                data_strorage_in_Error_t>>
     >
struct mjz_stack_obj_warper_template_class_t
    : private mjz_stack_obj_warper_data_storage_template_t<
          my_iner_Type_, my_obj_creator_t,
          my_err_creator_t,
          use_object_in_union
              ? mjz_stack_obj_warper_template_t_data_states::in_union
              : mjz_stack_obj_warper_template_t_data_states::in_aligned_chars,
          data_strorage_in_Error_t> {
 public:
  using Type = my_iner_Type_;
  using my_totaly_uniuqe_type_name_of_content_type = Type;
  using my_totaly_uniuqe_type_name_of_content_type_for_mjz_stack_obj_warper_template_class_t =
      Type;
  static constexpr size_t sizeof_Type = my_obj_creator_t::size_of_type();
  constexpr inline my_obj_creator_t get_obj_creator() {
    return my_obj_creator_t(*((const my_obj_creator_t *)this));
  }

 protected:
  using my_obj_helper_t = mjz_stack_obj_warper_data_storage_template_t<my_iner_Type_, my_obj_creator_t, my_err_creator_t,
      use_object_in_union
          ? mjz_stack_obj_warper_template_t_data_states::in_union
          : mjz_stack_obj_warper_template_t_data_states::in_aligned_chars,
      data_strorage_in_Error_t>;
      using my_err_helper_t=my_obj_helper_t;

 public:
  using err_crtr_t = my_obj_helper_t::err_crtr_t;
  using Error_t = my_obj_helper_t::Error_t;
  constexpr static const bool class_has_Error =
      my_obj_helper_t::class_has_Error;

 protected:
  constexpr inline const my_obj_helper_t &m_obj_helper() const & {
    return *this;
  }
  constexpr inline my_obj_helper_t &m_obj_helper() & { return *this; }
  constexpr inline const my_obj_helper_t &&m_obj_helper() const && {
    return move_me();
  }
  constexpr inline my_obj_helper_t &&m_obj_helper() && { return move_me(); }

  constexpr inline const my_err_helper_t &m_err_helper() const & {
    return *this;
  }
  constexpr inline my_err_helper_t &m_err_helper() & { return *this; }
  constexpr inline const my_err_helper_t &&m_err_helper() const && {
    return move_me();
  }
  constexpr inline my_err_helper_t &&m_err_helper() && { return move_me(); }







  constexpr inline const my_obj_creator_t &m_obj_creator() const & {
    return *this;
  }
  constexpr inline my_obj_creator_t &m_obj_creator() & { return *this; }
  constexpr inline const my_obj_creator_t &&m_obj_creator() const && {
    return move_me();
  }
  constexpr inline my_obj_creator_t &&m_obj_creator() && { return move_me(); }

 constexpr inline const my_err_creator_t &m_err_creator() const & {
    return *this;
  }
  constexpr inline my_err_creator_t &m_err_creator() & { return *this; }
  constexpr inline const my_err_creator_t &&m_err_creator() const && {
    return move_me();
  }
  constexpr inline my_err_creator_t &&m_err_creator() && { return move_me(); }

 

 private:
  constexpr inline Type *OP() & { return m_obj_helper().m_data<Type>(); }
  constexpr inline Type &O() & { return *OP(); }
  constexpr inline const Type *OP() const & {
    return m_obj_helper().m_data<Type>();
  }
  constexpr inline const Type &O() const & { return *OP(); }
  constexpr inline Type &&O() && {
    return std::move(*m_obj_helper().m_data<Type>());
  }
  constexpr inline const Type &&O() const && {
    return std::move(*m_obj_helper().m_data<Type>());
  }
  constexpr inline Error_t *EP() & { return m_err_helper().m_data<Error_t>(); }
  constexpr inline Error_t &E() & { return *EP(); }
  constexpr inline const Error_t *EP() const & {
    return m_err_helper().m_data<Error_t>();
  }
  constexpr inline const Error_t &E() const & { return *EP(); }
  constexpr inline Error_t &&E() && {
    return std::move(*m_err_helper().m_data<Error_t>());
  }
  constexpr inline const Error_t &&E() const && {
    return std::move(*m_err_helper().m_data<Error_t>());
  }



  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type &&src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, std::move(src));
      return place;
    }
    return m_obj_creator().construct_at(place, std::move(src));
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type &src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_obj_creator().construct_at(place, src);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            const Type &src) {
    if (plc_has_obj) {
      m_obj_creator().obj_go_to_obj(*place, src);
      return place;
    }
    return m_obj_creator().construct_at(place, src);
  }
  template <typename... args_t>
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            args_t &&...args) {
    if (plc_has_obj) {
   alignas(my_obj_creator_t::alignment)   uint8_t buf[sizeof_Type];
      Type *that = (Type *)buf;
      m_obj_creator().construct_at(that, std::forward<args_t>(args)...);
      m_obj_creator().obj_go_to_obj(*place, std::move(*that));
      m_obj_creator().destroy_at(that);
      return place;
    }
    return m_obj_creator().construct_at(place, std::forward<args_t>(args)...);
  }
  template <typename U, typename = std::enable_if_t<std::is_fundamental<Type>>>
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            U &&arg) {
    if (plc_has_obj) {
      Type buf = m_obj_creator().constructor(std::forward<U>(arg));
      m_obj_creator().obj_go_to_obj(*place, std::move(buf));
      return place;
    }
    return m_obj_creator().construct_at(place, std::forward<U>(arg));
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj) {
    if (plc_has_obj) {
      m_obj_creator().destroy_at(place);
      m_obj_creator().construct_at(place);
      return place;
    }
    return m_obj_creator().construct_at(place);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            Type *src) {
    return construct_in_place(place, *src);
  }
  constexpr inline Type *construct_in_place(Type *place, bool plc_has_obj,
                                            const Type *src) {
    return construct_in_place(place, *src);
  }
  constexpr inline void destroy_at_place(Type *place) {
    m_obj_creator().destroy_at(place);
  }
  template <typename... args_t>
  constexpr inline void construct(args_t &&...args) {
    notify_unsafe_value_init_in([&](const auto &) {
      return pointer_to_unsafe_data() ==
             construct_in_place(pointer_to_unsafe_data(), get_has_data(),
                                std::forward<args_t>(args)...);
    });
  }
  constexpr inline void destroy() {
    notify_unsafe_value_deinit_in_if(get_has_data(), [&](const auto &) {
      destroy_at_place(pointer_to_unsafe_data());
      return true;
    });
  }
  constexpr inline Type *move_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj,
                                std::move(*pointer_to_unsafe_data()));
    destroy();
    return 0;
  }
  constexpr inline Type *copy_to_place(Type *dest, bool dest_has_obj) {
    if (has_data())
      return construct_in_place(dest, dest_has_obj, *pointer_to_unsafe_data());
    return 0;
  }




 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
  Error_t  &&src) {
 if (plc_has_obj) {
  m_err_creator().obj_go_to_obj(*place, std::move(src));
 return place;
 }
 return  m_err_creator().construct_at(place, std::move(src));
 }
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
  Error_t  &src) {
 if (plc_has_obj) {
  m_err_creator().obj_go_to_obj(*place, src);
 return place;
 }
 return  m_err_creator().construct_at(place, src);
 }
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
 const  Error_t  &src) {
 if (plc_has_obj) {
  m_err_creator().obj_go_to_obj(*place, src);
 return place;
 }
 return  m_err_creator().construct_at(place, src);
 }
 template <typename... args_t>
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
 args_t &&...args) {
 if (plc_has_obj) {
alignas(my_err_creator_t::alignment) uint8_t buf[sizeof_Type];
  Error_t  *that = ( Error_t  *)buf;
  m_err_creator().construct_at(that, std::forward<args_t>(args)...);
  m_err_creator().obj_go_to_obj(*place, std::move(*that));
  m_err_creator().destroy_at(that);
 return place;
 }
 return  m_err_creator().construct_at(place, std::forward<args_t>(args)...);
 }
 template <typename U, typename = std::enable_if_t<std::is_fundamental< Error_t >>>
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
 U &&arg) {
 if (plc_has_obj) {
  Error_t  buf =  m_err_creator().constructor(std::forward<U>(arg));
  m_err_creator().obj_go_to_obj(*place, std::move(buf));
 return place;
 }
 return  m_err_creator().construct_at(place, std::forward<U>(arg));
 }
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj) {
 if (plc_has_obj) {
  m_err_creator().destroy_at(place);
  m_err_creator().construct_at(place);
 return place;
 }
 return  m_err_creator().construct_at(place);
 }
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
  Error_t  *src) {
 return err_construct_in_place(place, *src);
 }
 constexpr inline Error_t*err_construct_in_place( Error_t  *place, bool plc_has_obj,
 const  Error_t  *src) {
 return err_construct_in_place(place, *src);
 }
 constexpr inline void err_destroy_at_place( Error_t  *place) {
  m_err_creator().destroy_at(place);
 }
 template <typename... args_t>
 constexpr inline void err_construct(args_t &&...args) {
 notify_unsafe_error_init_in([&](const auto &) {
 return EP() ==
 err_construct_in_place(EP(), get_has_error(),
 std::forward<args_t>(args)...);
 });
 }
 constexpr inline void err_destroy() {
 notify_unsafe_error_deinit_in_if(get_has_error(), [&](const auto &) {
 err_destroy_at_place(EP());
 return true;
 });
 }
 constexpr inline Error_t*err_move_to_place( Error_t  *dest, bool dest_has_obj) {
 if (has_error())
 return err_construct_in_place(dest, dest_has_obj,
 std::move(*EP()));
 err_destroy();
 return 0;
 }
 constexpr inline Error_t*err_copy_to_place( Error_t  *dest, bool dest_has_obj) {
 if (has_error())
 return err_construct_in_place(dest, dest_has_obj, *EP());
 return 0;
 }




 public:
  constexpr inline Type &o() & { return **this; }
  constexpr inline Type *op() & { return operator->(); }
  constexpr inline const Type &o() const & { return **this; }
  constexpr inline const Type *op() const & { return operator->(); }
  constexpr inline Type &&o() && { return std::move(*move_me()); }
  constexpr inline Type *op() && = delete;
  constexpr inline const Type &&o() const && { return std::move(*move_me()); }
  constexpr inline const Type *op() const && = delete;
  // unsafe object functions begin

  constexpr inline Type &uo() &noexcept { return *uop(); }
  constexpr inline Type *uop() &noexcept {
    if (get_has_data()) return OP();
    return nullptr;
  }
  constexpr inline const Type &uo() const &noexcept { return *uop(); }
  constexpr inline const Type *uop() const &noexcept {
    if (get_has_data()) return OP();
    return nullptr;
  }
  constexpr inline Type &&uo() &&noexcept { return std::move(uo()); }
  constexpr inline Type *uop() && = delete;
  constexpr inline const Type &&uo() const &&noexcept {
    return std::move(uo());
  }
  constexpr inline const Type *uop() const && = delete;

  // Ultra nunsafe
  using data_state_t = typename my_obj_helper_t::data_state;
  using data_state = data_state_t;
  constexpr const static data_state_t cast_to_none = data_state_t::none;

  constexpr const static data_state_t cast_to_value = data_state_t::value;

  constexpr const static data_state_t cast_to_error = data_state_t::error;
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_unsafe_set_type(data_state cast_to_state) & {
    m_obj_helper().base_unsafe_unsafe_set_type(cast_to_state);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_start() & {
    m_obj_helper().base_notify_value_unsafe_init_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_start() & {
    m_obj_helper().base_notify_value_unsafe_deinit_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_start() && {
    m_obj_helper().base_notify_value_unsafe_init_start();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_start() && {
    m_obj_helper().base_notify_value_unsafe_deinit_start();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_fail() & {
    m_obj_helper().base_notify_value_unsafe_init_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_fail() & {
    m_obj_helper().base_notify_value_unsafe_deinit_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_fail() && {
    m_obj_helper().base_notify_value_unsafe_init_fail();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_fail() && {
    m_obj_helper().base_notify_value_unsafe_deinit_fail();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_end() & {
    m_obj_helper().base_notify_value_unsafe_init_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_end() & {
    m_obj_helper().base_notify_value_unsafe_deinit_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_init_end() && {
    m_obj_helper().base_notify_value_unsafe_init_end();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_value_unsafe_deinit_end() && {
    m_obj_helper().base_notify_value_unsafe_deinit_end();
    return move_me();
  }



constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_start() & {
    m_err_helper().base_notify_error_unsafe_init_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_start() & {
    m_err_helper().base_notify_error_unsafe_deinit_start();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_start() && {
    m_err_helper().base_notify_error_unsafe_init_start();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_start() && {
    m_err_helper().base_notify_error_unsafe_deinit_start();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_fail() & {
    m_err_helper().base_notify_error_unsafe_init_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_fail() & {
    m_err_helper().base_notify_error_unsafe_deinit_fail();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_fail() && {
    m_err_helper().base_notify_error_unsafe_init_fail();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_fail() && {
    m_err_helper().base_notify_error_unsafe_deinit_fail();
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_end() & {
    m_err_helper().base_notify_error_unsafe_init_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_end() & {
    m_err_helper().base_notify_error_unsafe_deinit_end();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_init_end() && {
    m_err_helper().base_notify_error_unsafe_init_end();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &
  unsafe_notify_error_unsafe_deinit_end() && {
    m_err_helper().base_notify_error_unsafe_deinit_end();
    return move_me();
  }







  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      me_notify_unsafe_value_init_or_deinit_in(bool is_init,
                                         function_type &&inner_function,
                                         Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(*this, std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              *this, std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            *this, std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      me_notify_unsafe_value_init_or_deinit_in(bool is_init,
                                         function_type &&inner_function,
                                         Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->move_me(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              move_me(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            move_me(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                        function_type &&inner_function,
                                        Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                        function_type &&inner_function,
                                        Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_value_init_or_deinit_in(false,
                                    std::forward<function_type>(inner_function),
                                    std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().me_notify_unsafe_value_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &notify_unsafe_value_init_in(
      function_type &&inner_function, Ts... args) & {
    notify_unsafe_value_init_or_deinit_in(true,
                                    std::forward<function_type>(inner_function),
                                    std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_in(function_type &&inner_function, Ts... args) && {
    move_me().me_notify_unsafe_value_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_deinit_in_if(bool do_notify, function_type &&inner_function,
                             Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_deinit_in_if(bool do_notify, function_type &&inner_function,
                             Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_deinit_in_if(bool do_notify, function_type &&inner_function,
                                Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_deinit_in_if(bool do_notify, function_type &&inner_function,
                                Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                           Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                           Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                              Ts... args) & {
    if (do_notify)
      me_notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_value_init_in_if(bool do_notify, function_type &&inner_function,
                              Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_value_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      notify_unsafe_value_init_or_deinit_in(bool is_init,
                                      function_type &&inner_function,
                                      Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      notify_unsafe_value_init_or_deinit_in(bool is_init,
                                      function_type &&inner_function,
                                      Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_value_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_value_unsafe_init_end();
      } else {
        unsafe_notify_value_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_value_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_value_unsafe_deinit_end();
    } else {
      unsafe_notify_value_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                     function_type &&inner_function,
                                     Ts... args) & {
    if (do_notify)
      notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_value_init_or_deinit_in_if(bool do_notify, bool is_init,
                                     function_type &&inner_function,
                                     Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_value_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }


  

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      me_notify_unsafe_error_init_or_deinit_in(bool is_init,
                                         function_type &&inner_function,
                                         Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(*this, std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              *this, std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            *this, std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      me_notify_unsafe_error_init_or_deinit_in(bool is_init,
                                         function_type &&inner_function,
                                         Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->move_me(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              move_me(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            move_me(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                        function_type &&inner_function,
                                        Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                        function_type &&inner_function,
                                        Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) & {
    notify_unsafe_error_init_or_deinit_in(false,
                                    std::forward<function_type>(inner_function),
                                    std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_deinit_in(function_type &&inner_function, Ts... args) && {
    move_me().me_notify_unsafe_error_init_or_deinit_in(
        false, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &notify_unsafe_error_init_in(
      function_type &&inner_function, Ts... args) & {
    notify_unsafe_error_init_or_deinit_in(true,
                                    std::forward<function_type>(inner_function),
                                    std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) && {
    move_me().notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) & {
    me_notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_in(function_type &&inner_function, Ts... args) && {
    move_me().me_notify_unsafe_error_init_or_deinit_in(
        true, std::forward<function_type>(inner_function),
        std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_deinit_in_if(bool do_notify, function_type &&inner_function,
                             Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_deinit_in_if(bool do_notify, function_type &&inner_function,
                             Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_deinit_in_if(bool do_notify, function_type &&inner_function,
                                Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_deinit_in_if(bool do_notify, function_type &&inner_function,
                                Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          false, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                           Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                           Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                              Ts... args) & {
    if (do_notify)
      me_notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_notify_unsafe_error_init_in_if(bool do_notify, function_type &&inner_function,
                              Ts... args) && {
    if (do_notify)
      move_me().me_notify_unsafe_error_init_or_deinit_in(
          true, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }

  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &
      notify_unsafe_error_init_or_deinit_in(bool is_init,
                                      function_type &&inner_function,
                                      Ts... args) &
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return *this;
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return *this;
  }
  template <class function_type, typename... Ts>
      constexpr inline mjz_stack_obj_warper_template_class_t &&
      notify_unsafe_error_init_or_deinit_in(bool is_init,
                                      function_type &&inner_function,
                                      Ts... args) &&
        requires requires(function_type &&inner_function) {
                   {
                     inner_function(this->uuop(), std::forward<Ts>(args)...)
                     } -> std::convertible_to<bool>;
                 }
  {
    if (is_init) {
      unsafe_notify_error_unsafe_init_start();
      if (std::forward<function_type>(inner_function)(
              uuop(), std::forward<Ts>(args)...)) {
        unsafe_notify_error_unsafe_init_end();
      } else {
        unsafe_notify_error_unsafe_init_fail();
      }
      return move_me();
    }
    unsafe_notify_error_unsafe_deinit_start();
    if (std::forward<function_type>(inner_function)(
            uuop(), std::forward<Ts>(args)...)) {
      unsafe_notify_error_unsafe_deinit_end();
    } else {
      unsafe_notify_error_unsafe_deinit_fail();
    }
    return move_me();
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                     function_type &&inner_function,
                                     Ts... args) & {
    if (do_notify)
      notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return *this;
  }
  template <class function_type, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  notify_unsafe_error_init_or_deinit_in_if(bool do_notify, bool is_init,
                                     function_type &&inner_function,
                                     Ts... args) && {
    if (do_notify)
      move_me().notify_unsafe_error_init_or_deinit_in(
          is_init, std::forward<function_type>(inner_function),
          std::forward<Ts>(args)...);
    return move_me();
  }



  constexpr inline uint8_t *pointer_to_unsafe_data_buffer() & {
    return (uint8_t *)(m_obj_helper().m_data<uint8_t>());
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const & {
    return (uint8_t *)(m_obj_helper().m_data<uint8_t>());
  }
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() && = delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data_buffer() const && =
      delete;
  constexpr inline const uint8_t *pointer_to_unsafe_data() const && = delete;
  constexpr inline Type *pointer_to_unsafe_data() & { return OP(); }
  constexpr inline const Type *pointer_to_unsafe_data() const & { return OP(); }

  constexpr inline mjz_stack_obj_warper_template_class_t &&move_me() {
    return std::move(*this);
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &&move_me()
      const {
    return std::move(*this);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_me() & {
    return *this;
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &copy_me()
      const & {
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_me() && {
    return *this;
  }

  constexpr inline const mjz_stack_obj_warper_template_class_t &copy_me()
      const && {
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() & {
    return {*this};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() const & {
    return {*this};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t clone_me() const && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t temp_me() const {
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t temp_me() {
    return *this;
  }

 public:  // unsafe may cuse undefined behavior
  mjz_stack_obj_warper_template_class_t &remove_const() const & {
    return *mjz_ard::remove_const(this);
  }
  mjz_stack_obj_warper_template_class_t &&remove_const() const && {
    return std::move(*mjz_ard::remove_const(this));
  }
  mjz_stack_obj_warper_template_class_t &remove_const() & { return *this; }
  mjz_stack_obj_warper_template_class_t &&remove_const() && {
    return std::move(*this);
  }
  Type &remove_const_obj() const & { return *m_obj_helper().mm_data<Type>(); }
  Type &&remove_const_obj() const && {
    return std::move(*m_obj_helper().mm_data<Type>());
  }
  Type &remove_const_obj() & { return *m_obj_helper().mm_data<Type>(); }
  Type &&remove_const_obj() && {
    return std::move(*m_obj_helper().mm_data<Type>());
  }
  const Type &&move() const & { return std::move(o()); }
  const Type &&move() const && { return std::move(o()); }
  Type &&move() & { return std::move(o()); }
  Type &&move() && { return std::move(o()); }

  constexpr inline Type &uuo() &noexcept { return *uuop(); }
  constexpr inline Type *uuop() &noexcept { return OP(); }
  constexpr inline const Type &uuo() const &noexcept { return *uuop(); }
  constexpr inline const Type *uuop() const &noexcept { return OP(); }
  constexpr inline Type &&uuo() &&noexcept { return std::move(O()); }
  constexpr inline const Type &&uuo() const &&noexcept {
    return std::move(O());
  }
  constexpr inline const Type *uuop() const && = delete;
  constexpr inline Type *uuop() && = delete;

  // no safe use case
  constexpr inline const Type *invalid_object_pointer() const && {
    return uuop();
  };
  // no safe use case
  constexpr inline Type *invalid_object_pointer() && { return uuop(); }

  // unsafe object functions end

  constexpr inline mjz_stack_obj_warper_template_class_t() {
    if constexpr (construct_obj_on_constructor) {
      construct();
    }
  };
  constexpr inline mjz_stack_obj_warper_template_class_t(mjz_init_optional_t) {
    construct();
  };
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_init_optional_if_t do_init) {
    if (do_init) construct();
  };

  template <size_t init_if_iam_not_zero_the_base_with_me_minus_one>
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_no_init_optional_t<init_if_iam_not_zero_the_base_with_me_minus_one>)
    requires(init_if_iam_not_zero_the_base_with_me_minus_one != 0)
  {
    static_assert(
        requires {
          construct(mjz_no_init_optional_t<
                    init_if_iam_not_zero_the_base_with_me_minus_one - 1>());
        });
    construct(
        mjz_no_init_optional_t<init_if_iam_not_zero_the_base_with_me_minus_one -
                               1>());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_no_init_optional_t<0>) {
    notify_unsafe_value_deinit_in([&](const auto &) {});
  };

  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(T0 &&arg0,
                                                         T_s &&...args) {
    construct(std::move(arg0), std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(const T0 &arg0,
                                                         T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  template <typename T0, typename... T_s>
  constexpr inline mjz_stack_obj_warper_template_class_t(T0 &arg0,
                                                         T_s &&...args) {
    construct(arg0, std::forward<T_s>(args)...);
  };
  constexpr inline ~mjz_stack_obj_warper_template_class_t() { destroy(); }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) {
    if (s_obj_w.has_data()) construct(std::move(s_obj_w.operator*()));
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(
      mjz_stack_obj_warper_template_class_t &s_obj_w) {
    if (s_obj_w.has_data()) construct(s_obj_w.operator*());
  }

 public:
  template <class Func_t, typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t s_create_op(
      Func_t Func, Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t obj(
        std::forward<Ts>(class_constructor_args)...);
    std::forward<Func_t>(Func)(obj);
    return obj;
  }
  template <class Func_t, typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t
  s_create_op_ignore_args_if_not_fn(Func_t Func,
                                    Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t o;
    mjz_stack_obj_warper_template_class_t *a = &o;
    static_assert(
        std::is_same_v<decltype(!!std::forward<Func_t>(Func)(*a)), bool>);
    if (!!std::forward<Func_t>(Func)(*this)) {
      a->~mjz_stack_obj_warper_template_class_t();
      auto *p = new (a) mjz_stack_obj_warper_template_class_t(
          std::forward<Ts>(class_constructor_args)...);
      if (!p) Throw("failed to construct ");
    }
    return o;
  }
  template <typename... Ts>
  static constexpr inline mjz_stack_obj_warper_template_class_t
  s_create_op_ignore_args_if_not(bool b, Ts &&...class_constructor_args) {
    mjz_stack_obj_warper_template_class_t o;
    if (b) o.replace_me(std::forward<Ts>(class_constructor_args)...);
    return o;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &replace_me(
      Ts &&...args) & {
    this->~mjz_stack_obj_warper_template_class_t();
    auto *p = new (this)
        mjz_stack_obj_warper_template_class_t(std::forward<Ts>(args)...);
    if (!p) Throw("failed to construct ");
    return *this;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &replace_obj(
      Ts &&...args) & {
    emplace(std::forward<Ts>(args)...);
    return *this;
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&replace_me(
      Ts &&...args) && {
    return replace_me(std::forward<Ts>(args)...).move_me();
  }
  template <typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&replace_obj(
      Ts &&...args) && {
    return replace_obj(std::forward<Ts>(args)...).move_me();
  }
  template <class Func_t, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_with_me(
      Func_t Func, Ts &&...args) & {
    std::forward<Func_t>(Func)(*this, std::forward<Ts>(args)...);
    return *this;
  }
  template <class Func_t, typename... Ts>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_with_me(
      Func_t Func, Ts &&...args) const & {
    std::forward<Func_t>(Func)(*this, std::forward<Ts>(args)...);
    return *this;
  }
  template <class Func_t, typename... Ts>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_with_me(
      Func_t Func, Ts &&...args) && {
    std::forward<Func_t>(Func)(move_me(), std::forward<Ts>(args)...);
    return move_me();
  }
  template <class Func_t, typename... Ts>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_with_me(
      Func_t Func, Ts &&...args) const && {
    std::forward<Func_t>(Func)(move_me(), std::forward<Ts>(args)...);
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t(const Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(Type &obj) {
    construct(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t(Type &&obj) {
    construct(std::move(obj));
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      Type &&obj) && {
    construct(std::move(obj));
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      Type &obj) && {
    construct(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      const Type &obj) && {
    construct(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      Type &&obj) & {
    construct(std::move(obj));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      Type &obj) & {
    construct(obj);
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      const Type &obj) & {
    construct(obj);
    return *this;
  }
  template <class U>
  inline constexpr
      typename std::enable_if<std::conjunction_v<std::is_constructible<Type, U>,
                                                 std::is_assignable<Type &, U>>,
                              mjz_stack_obj_warper_template_class_t &>::type
      operator=(U &&Obj) & {
    construct(std::forward<U>(Obj));
    return *this;
  }

  template <class U>
  inline constexpr typename std::enable_if<
      std::conjunction_v<
          std::is_constructible<mjz_stack_obj_warper_template_class_t, U>,
          std::negation<std::conjunction<std::is_constructible<Type, U>,
                                         std::is_assignable<Type &, U>>>>,
      mjz_stack_obj_warper_template_class_t &>::type
  operator=(U &&Obj) & {
    operator=(mjz_stack_obj_warper_template_class_t(std::forward<U>(Obj)));
    return *this;
  }

  template <class U>
  inline constexpr
      typename std::enable_if<std::conjunction_v<std::is_constructible<Type, U>,
                                                 std::is_assignable<Type &, U>>,
                              mjz_stack_obj_warper_template_class_t &&>::type
      operator=(U &&Obj) && {
    construct(std::forward<U>(Obj));
    return move_me();
  }

  template <class U>
  inline constexpr typename std::enable_if<
      std::conjunction_v<
          std::is_constructible<mjz_stack_obj_warper_template_class_t, U>,
          std::negation<std::conjunction<std::is_constructible<Type, U>,
                                         std::is_assignable<Type &, U>>>>,
      mjz_stack_obj_warper_template_class_t &&>::type
  operator=(U &&Obj) && {
    operator=(mjz_stack_obj_warper_template_class_t(std::forward<U>(Obj)));
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) && {
    if (!!s_obj_w) operator=(std::move(s_obj_w.operator*()));
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) && {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator=(
      mjz_stack_obj_warper_template_class_t &s_obj_w) && {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return move_me();
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      mjz_stack_obj_warper_template_class_t &&s_obj_w) & {
    if (!!s_obj_w) operator=(std::move(s_obj_w.operator*()));
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      const mjz_stack_obj_warper_template_class_t &s_obj_w) & {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator=(
      mjz_stack_obj_warper_template_class_t &s_obj_w) & {
    if (!!s_obj_w) operator=(s_obj_w.operator*());
    return *this;
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      const mjz_stack_obj_warper_template_class_t &obj) & {
    return operator=(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      mjz_stack_obj_warper_template_class_t &obj) & {
    return operator=(obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      mjz_stack_obj_warper_template_class_t &&obj) & {
    return operator=(std::move(obj));
  }
  template <typename arg_T, typename Type>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      std::initializer_list<arg_T> list) & {
    construct(list);
    return *this;
  }
  template <typename arg_T>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      iterator_template_t<arg_T> list) & {
    construct(list);
    return *this;
  }
  template <typename... arguments_types>
  constexpr inline mjz_stack_obj_warper_template_class_t &init(
      arguments_types &&...args) & {
    construct(std::forward<arguments_types>(args)...);

    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      const mjz_stack_obj_warper_template_class_t &obj) && {
    operator=(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      mjz_stack_obj_warper_template_class_t &obj) && {
    operator=(obj);
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      mjz_stack_obj_warper_template_class_t &&obj) && {
    operator=(std::move(obj));
    return move_me();
  }
  template <typename arg_T, typename Type>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      std::initializer_list<arg_T> list) && {
    construct(list);
    return move_me();
  }
  template <typename arg_T>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      iterator_template_t<arg_T> list) && {
    construct(list);
    return move_me();
  }
  template <typename... arguments_types>
  constexpr inline mjz_stack_obj_warper_template_class_t &&init(
      arguments_types &&...args) && {
    construct(std::forward<arguments_types>(args)...);

    return move_me();
  }

 public:
  constexpr inline mjz_stack_obj_warper_template_class_t &de_init() & {
    destroy();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &de_init(
      int fill_VAL) & {
    destroy();
    static_str_algo::memset(m_obj_helper().m_data<uint8_t>(), fill_VAL,
                            sizeof_Type);
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&de_init() && {
    destroy();

    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&de_init(
      int fill_VAL) && {
    destroy();
    static_str_algo::memset(m_obj_helper().m_data<uint8_t>(), fill_VAL,
                            sizeof_Type);

    return move_me();
  }

 public:
  using value_type = Type;
  using reference = value_type &;
  using pointer = value_type *;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using T = Type;
  using type = Type;

 public:
 public:
  constexpr inline Type &if_no_obj_then_create() & {
    if (!get_has_data()) construct();
    return *pointer_to_unsafe_data();
  }
  Type &&if_no_obj_then_create() && {
    if (!get_has_data()) construct();
    return std::move(move_me().O());
  }

 public:
 public:
  constexpr inline const Type *throw_if_no_data_or_give_data() && = delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const && =
      delete;
  constexpr inline const Type *throw_if_no_data_or_give_data() const & {
    if (!get_has_data()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
            "access");
      } else {
        return (const Type *)nullptr;  // :(
      }
    }
    return pointer_to_unsafe_data();
  }
  constexpr inline const Type *pointer_to_data() && = delete;
  constexpr inline const Type *pointer_to_data() const && = delete;
  constexpr inline const Type *pointer_to_data() const & {
    return throw_if_no_data_or_give_data();
  }
  constexpr inline Type *pointer_to_data() & {
    return mjz_ard::remove_const(throw_if_no_data_or_give_data());
  }

 public:
  constexpr inline Type *operator->() & { return pointer_to_data(); }
  constexpr inline Type &&operator->() && {
    if (has_data()) return std::move(move_me().O());
    Throw<std::out_of_range>(
        "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data bad "
        "access");
  }  // overload dosnt give ptr
  constexpr inline const Type &&operator->() const && {
    if (has_data()) return std::move(move_me().O());
    Throw<std::out_of_range>(
        "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data bad "
        "access");
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  constexpr inline auto operator->*(my_type my_var) const & {
    return pointer_to_data()->*my_var;
  }
  template <typename my_type>
  constexpr inline void operator->*(my_type my_var) && = delete;
  template <typename my_type>
  constexpr inline void operator->*(my_type my_var) const && = delete;
  constexpr inline Type &&operator*() && {
    return std::move(move_me().operator->());
  }
  constexpr inline const Type &&operator*() const && {
    return std::move(move_me().operator->());
  }
  constexpr inline Type &operator*() & { return *operator->(); }

  constexpr inline const Type *operator->() const & {
    return pointer_to_data();
  }
  constexpr inline const Type &operator*() const & { return *operator->(); }
  constexpr inline const Type &operator()() const & { return **this; }
  constexpr inline Type &&operator()() && { return std::move(*move_me()); }
  constexpr inline const Type &&operator()() const && {
    return std::move(*move_me());
  }
  constexpr inline Type &operator()() & { return **this; }
  using my_Type_t = mjz_stack_obj_warper_template_class_t;

  constexpr inline my_Type_t &destroy_if_valid() & {
    de_init();
    return *this;
  }
  constexpr inline my_Type_t &create_if_empty() & {
    if (has_data()) {
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &if_valid_destroy_else_create() & {
    if (has_data()) {
      de_init();
      return *this;
    }
    construct();
    return *this;
  }
  constexpr inline my_Type_t &&destroy_if_valid() && {
    de_init();
    return move_me();
  }
  constexpr inline my_Type_t &&create_if_empty() && {
    if (has_data()) {
      return move_me();
    }
    construct();
    return move_me();
  }
  constexpr inline my_Type_t &&if_valid_destroy_else_create() && {
    if (has_data()) {
      de_init();
      return move_me();
    }
    construct();
    return move_me();
  }

  constexpr inline my_Type_t &operator~() & { return destroy_if_valid(); }
  constexpr inline my_Type_t &operator+() & { return create_if_empty(); }
  constexpr inline my_Type_t &operator-() & {
    return if_valid_destroy_else_create();
  }
  constexpr inline my_Type_t &&operator~() && {
    return move_me().destroy_if_valid();
  }
  constexpr inline my_Type_t &&operator+() && {
    return move_me().create_if_empty();
  }
  constexpr inline my_Type_t &&operator-() && {
    return move_me().if_valid_destroy_else_create();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator--() & {
    de_init();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator--() && {
    de_init();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator--()
      const && = delete;
  constexpr inline mjz_stack_obj_warper_template_class_t operator--(int) {
    mjz_stack_obj_warper_template_class_t temp(move_me());
    de_init();
    return temp;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator++() & {
    // de_init();
    construct();
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator++() && {
    // de_init();
    construct();
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator++()
      const && = delete;
  constexpr inline mjz_stack_obj_warper_template_class_t operator++(int) {
    mjz_stack_obj_warper_template_class_t temp(move_me());
    // you may want to reuse data soo
    // de_init();
    construct();
    return temp;
  }

  /*
  IMPORTANT NOTE :
  use iter_me to set up an iterator
  (or clone_me because they are the same .
  but use iter_me because its more readable.)
  */

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() & {
    return {*this};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() const & {
    return {*this};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t new_me() && {
    return {move_me()};
  }

  constexpr inline mjz_stack_obj_warper_template_class_t new_me() const && {
    return {move_me()};
  }
  constexpr inline iterator_template_t<Type> get_iter() & {
    if (!!*this) return {uop(), uop(), uop() + 1};
    return {};
  }
  constexpr inline iterator_template_t<const Type> get_iter() const & {
    if (!!*this) return {uop(), uop(), uop() + 1};
    return {};
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *begin() & {
    return this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *end() & {
    return begin() + 1;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin()
      const & {
    return this;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() const & {
    return begin() + 1;
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin()
      const & {
    return begin();
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend() const & {
    return end();
  }
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend() && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *begin()
      const && = delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *end() const && =
      delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cbegin()
      const && = delete;
  const constexpr inline mjz_stack_obj_warper_template_class_t *cend()
      const && = delete;
  constexpr inline static size_t size() { return 1; }  // for iterator
  constexpr inline static size_t my_size() { return sizeof_Type; }
  constexpr inline static size_t size_T() { return my_size(); }
  constexpr inline Type &operator()(Type &&moved) & {
    return emplace(std::move(moved));
  }
  constexpr inline Type &operator()(const Type &&moved) & {
    return emplace(std::move(moved));
  }
  template <typename... args_t>
  constexpr inline Type &operator()(args_t &&...args) & {
    return emplace(std::forward<args_t>(args)...);
  }
  constexpr inline Type &operator()(Type &moved) & { return emplace(moved); }
  constexpr inline Type &operator()(const Type &moved) & {
    return emplace(moved);
  }
  constexpr inline Type &operator()(Type &&moved) && {
    emplace(std::move(moved));
    return move_me();
  }
  constexpr inline Type &operator()(const Type &&moved) && {
    emplace(std::move(moved));
    return move_me();
  }
  constexpr inline Type &operator()(Type &moved) && {
    emplace(moved);
    return move_me();
  }
  constexpr inline Type &operator()(const Type &moved) && {
    emplace(moved);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) & {
    return f(**this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class function_type,
            typename... args_t>
  constexpr inline return_type do_or_throw(function_type f,
                                           args_t &&...args) const && {
    return f(std::move(*move_me()), std::forward<args_t>(args)...);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &operator[](
      std::function<void(Type &)> f) & {
    if (has_data()) f(O());
    return *this;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&operator[](
      std::function<void(Type &&)> f) && {
    if (has_data()) f(std::move(move_me().O()));
    return move_me();
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &&operator[](
      std::function<void(const Type &&)> f) const && {
    if (has_data()) f(std::move(move_me().O()));

    return move_me();
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &operator[](
      std::function<void(const Type &)> f) const & {
    if (has_data()) f(O());
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) const & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return *this;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) & {
    if (do_throw) {
      f(**this, std::forward<args_t>(args)...);
      return *this;
    }
    if (has_data()) f(O(), std::forward<args_t>(args)...);
    return *this;
  }
  template <class function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) && {
    if (do_throw) {
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return move_me();
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&i_do_nt_know(
      bool do_throw, function_type f, args_t &&...args) const && {
    if (do_throw) {
      f(std::move(*move_me()), std::forward<args_t>(args)...);
      return move_me();
    }
    if (has_data()) f(std::move(move_me().O()), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return no_data_function(std::forward<args_t>(args)...);
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(std::forward<args_t>(args)...);
    return move_me();
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  do_first_if_true_or_second_if_false(has_data_function_type has_data_function,
                                      no_data_function_type no_data_function,
                                      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(std::forward<args_t>(args)...);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return no_data_function(*this, std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return no_data_function(move_me(), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return no_data_function(move_me(), std::forward<args_t>(args)...);
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            class no_data_function_type, typename... args_t>
  constexpr inline return_type me_do_first_if_true_or_second_if_false_ret(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return no_data_function(*this, std::forward<args_t>(args)...);
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(*this, std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }
    no_data_function(*this, std::forward<args_t>(args)...);
    return *this;
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(move_me(), std::forward<args_t>(args)...);
    return move_me();
  }
  template <class has_data_function_type, class no_data_function_type,
            typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  me_do_first_if_true_or_second_if_false(
      has_data_function_type has_data_function,
      no_data_function_type no_data_function, args_t &&...args) const && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    no_data_function(move_me(), std::forward<args_t>(args)...);
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(std::move(move_me().O()),
                               std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (has_data()) {
      return has_data_function(O(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) & {
    if (has_data()) {
      has_data_function(O(), std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(std::move(move_me().O()),
                        std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }

  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (!has_data()) {
      return no_data_function(std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const & {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) & {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) && {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const && {
    if (!has_data()) {
      no_data_function(std::forward<args_t>(args)...);
      return move_me();
    }
    return move_me();
  }

  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (has_data()) {
      return has_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class has_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_valid_ret(
      has_data_function_type has_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (has_data()) {
      return has_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) & {
    if (has_data()) {
      has_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class has_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&me_do_if_valid(
      has_data_function_type has_data_function,

      args_t &&...args) const && {
    if (has_data()) {
      has_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }

  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const & {
    if (!has_data()) {
      return no_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) && {
    if (!has_data()) {
      return no_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) const && {
    if (!has_data()) {
      return no_data_function(move_me(), std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class return_type = (decltype(auto)), class no_data_function_type,
            typename... args_t>
  constexpr inline return_type me_do_if_empty_ret(
      no_data_function_type no_data_function, return_type false_ret_val,
      args_t &&...args) & {
    if (!has_data()) {
      return no_data_function(*this, std::forward<args_t>(args)...);
    }
    return false_ret_val;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const & {
    if (!has_data()) {
      no_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) & {
    if (!has_data()) {
      no_data_function(*this, std::forward<args_t>(args)...);
      return *this;
    }

    return *this;
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline mjz_stack_obj_warper_template_class_t &&me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) && {
    if (!has_data()) {
      no_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }

    return move_me();
  }
  template <class no_data_function_type, typename... args_t>
  constexpr inline const mjz_stack_obj_warper_template_class_t &&me_do_if_empty(
      no_data_function_type no_data_function,

      args_t &&...args) const && {
    if (!has_data()) {
      no_data_function(move_me(), std::forward<args_t>(args)...);
      return move_me();
    }
    return move_me();
  }

 public:
  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_class_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_class_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this != *other;
  }
  constexpr inline bool operator<(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_stack_obj_warper_template_class_t &other) const {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_class_t &other) const {
return **this<=> (*other);
}
#endif  // ! Arduino

  constexpr inline bool operator==(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      const mjz_stack_obj_warper_template_class_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>(const mjz_stack_obj_warper_template_class_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
  constexpr inline bool operator==(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this == *other;
  }
  constexpr inline bool operator!=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this != *other;
  }
  constexpr inline bool operator<(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this < (*other);
  }
  constexpr inline bool operator<=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this <= (*other);
  }
  constexpr inline bool operator>(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this > (*other);
  }
  constexpr inline bool operator>=(
      mjz_stack_obj_warper_template_class_t &other) {
    return **this >= (*other);
  }
#if 0
 constexpr inline bool operator<=>( mjz_stack_obj_warper_template_class_t &other) {
return **this<=> (*other);
}
#endif  // ! Arduino
 protected:
  constexpr inline bool get_has_data() const volatile noexcept {
    return m_obj_helper().get_has_data();
  }
  constexpr inline bool get_has_data() const noexcept {
    return m_obj_helper().get_has_data();
  }
  constexpr inline bool get_has_error() const volatile noexcept {
    return m_obj_helper().get_has_error();
  }
  constexpr inline bool get_has_error() const noexcept {
    return m_obj_helper().get_has_error();
  }

 public:
  constexpr inline Error_t &uue() &noexcept { return *uuep(); }
  constexpr inline Error_t *uuep() &noexcept { return EP(); }
  constexpr inline const Error_t &uue() const &noexcept { return *uuep(); }
  constexpr inline const Error_t *uuep() const &noexcept { return EP(); }
  constexpr inline Error_t &&uue() &&noexcept { return std::move(E()); }
  constexpr inline const Error_t &&uue() const &&noexcept {
    return std::move(E());
  }
  constexpr inline const Error_t *uuep() const && = delete;
  constexpr inline Error_t *uuep() && = delete;


 public:
  constexpr inline Error_t &e() & { return *ep(); }
  constexpr inline Error_t *ep() & { return throw_if_no_error_or_give_error(); }
  constexpr inline const Error_t &e() const & { return *ep(); }
  constexpr inline const Error_t *ep() const & {
    return throw_if_no_error_or_give_error();
  }
  constexpr inline Error_t &&e() && { return std::move(*move_me()); }
  constexpr inline Error_t *ep() && = delete;
  constexpr inline const Error_t &&e() const && {
    return std::move(*move_me());
  }
  constexpr inline const Error_t *ep() const && = delete;
  // unsafe object functions begin

  constexpr inline Error_t &ue() &noexcept { return *uep(); }
  constexpr inline Error_t *uep() &noexcept {
    if (get_has_error()) return ep();
    return nullptr;
  }
  constexpr inline const Error_t &ue() const &noexcept { return *uep(); }
  constexpr inline const Error_t *uep() const &noexcept {
    if (get_has_error()) return ep();
    return nullptr;
  }
  constexpr inline Error_t &&ue() &&noexcept { return std::move(ue()); }
  constexpr inline Error_t *uep() && = delete;
  constexpr inline const Error_t &&ue() const &&noexcept {
    return std::move(ue());
  }
  constexpr inline const Error_t *uep() const && = delete;

  constexpr inline const Error_t *throw_if_no_error_or_give_error() const & {
    if (!get_has_error()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
            "access");
      } else {
        return (const Error_t *)nullptr;  // :(
      }
    }
    return uuep();
  }

  constexpr inline Error_t *throw_if_no_error_or_give_error() & {
    if (!get_has_error()) {
      if constexpr (do_error_check) {
        Throw<std::out_of_range>(
            "mjz_ard::mjz_stack_obj_warper_template_class_t::pointer_to_data "
            "bad "
            "access");
      } else {
        return (Error_t *)nullptr;  // :(
      }
    }
    return uuep();
  }
  constexpr inline bool has_error() const noexcept { return get_has_error(); }
  constexpr inline bool has_error() const volatile noexcept {
    return get_has_error();
  }
  constexpr inline bool has_data() const noexcept { return get_has_data(); }
  constexpr inline bool has_data() const volatile noexcept {
    return get_has_data();
  }
  constexpr inline bool operator!() const noexcept { return !get_has_data(); }
  constexpr inline operator Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() & { return pointer_to_data(); }
  constexpr inline operator const Type *() const & { return pointer_to_data(); }
  constexpr inline operator Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() & { return *pointer_to_data(); }
  constexpr inline operator const Type &() const & {
    return *pointer_to_data();
  }
  constexpr inline operator Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() && { return std::move(*move_me()); }
  constexpr inline operator const Type &&() const && {
    return std::move(*move_me());
  }
  constexpr inline operator Type() & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() const & {
    return get_obj_creator().obj_constructor(*pointer_to_data());
  }
  constexpr inline operator Type() && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr inline operator Type() const && {
    return get_obj_creator().obj_constructor(std::move(*move_me()));
  }
  constexpr explicit operator bool() &noexcept { return has_data(); }
  constexpr explicit operator bool() &&noexcept { return has_data(); }
  constexpr explicit operator bool() const &noexcept { return has_data(); }
  constexpr explicit operator bool() const &&noexcept { return has_data(); }

  constexpr inline mjz_stack_obj_warper_template_class_t &
  remove_volatile() volatile &noexcept {
    return *const_cast<mjz_stack_obj_warper_template_class_t *>(this);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &&
  remove_volatile() volatile &&noexcept {
    return *const_cast<mjz_stack_obj_warper_template_class_t *>(this);
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &
  remove_volatile() volatile const &noexcept {
    return std::move(
        *const_cast<const mjz_stack_obj_warper_template_class_t *>(this));
  }
  constexpr inline const mjz_stack_obj_warper_template_class_t &&
  remove_volatile() volatile const &&noexcept {
    return std::move(
        *const_cast<const mjz_stack_obj_warper_template_class_t *>(this));
  }
  constexpr explicit operator bool() volatile &noexcept { return has_data(); }
  constexpr explicit operator bool() volatile &&noexcept { return has_data(); }
  constexpr explicit operator bool() const volatile &noexcept {
    return has_data();
  }
  constexpr explicit operator bool() const volatile &&noexcept {
    return has_data();
  }
  constexpr bool has_value() const noexcept { return has_data(); }

  template <typename... Ts>
  struct deafult_construct_at_fn : private my_obj_creator_t {
    inline constexpr bool operator()(T *dest, Ts &&...args) {
      return !!my_obj_creator_t::construct_at(dest, std::forward<Ts>(args)...);
    }
  };
  template <>
  struct deafult_construct_at_fn<void> : private my_obj_creator_t {
    inline constexpr bool operator()(T *dest) {
      return !!my_obj_creator_t::construct_at(dest);
    }
  };
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &&fn_emplace(
      std::function<bool(T *, Args...)> construct_at_fn, Args &&...args) && {
    ~*this;
    notify_unsafe_value_init_in([&](const auto &) {
      return construct_at_fn(uuop(), std::forward<Args>(args)...);
    });
    return move_me();
  }
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &fn_emplace(
      std::function<bool(T *, Args...)> construct_at_fn, Args &&...args) & {
    ~*this;
    notify_unsafe_value_init_in([&](const auto &) {
      return construct_at_fn(uuop(), std::forward<Args>(args)...);
    });
    return *this;
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&fn_emplace(
      std::function<bool(T *)> construct_at_fn =
          deafult_construct_at_fn<void>()) && {
    ~*this;
    notify_unsafe_value_init_in(
        [&](const auto &) { return (construct_at_fn(uuop())); });
    return move_me();
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &fn_emplace(
      std::function<bool(T *)> construct_at_fn =
          deafult_construct_at_fn<void>()) & {
    ~*this;
    notify_unsafe_value_init_in(
        [&](const auto &) { return (construct_at_fn(uuop())); });
    return *this;
  }

  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &&dfn_emplace(
      Args &&...args) && {
    return move_me().fn_emplace<Args...>(deafult_construct_at_fn<Args...>(),
                                         std::forward<Args>(args)...);
  }
  template <class... Args>
  constexpr inline mjz_stack_obj_warper_template_class_t &dfn_emplace(
      Args &&...args) & {
    return fn_emplace<Args...>(deafult_construct_at_fn<Args...>(),
                               std::forward<Args>(args)...);
  }

  constexpr inline mjz_stack_obj_warper_template_class_t &&dfn_emplace() && {
    return move_me().fn_emplace(deafult_construct_at_fn<void>());
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &dfn_emplace() & {
    return fn_emplace(deafult_construct_at_fn<void>());
  }
  template <class... Args>
  constexpr inline T &emplace(Args &&...args) & {
    ~*this;
    construct(std::forward<Args>(args)...);
    return o();
  }
  template <class U, class... Args>

  constexpr inline T &emplace(std::initializer_list<U> ilist,
                              Args &&...args) & {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return o();
  }
  template <class... Args>
  constexpr inline T &&emplace(Args &&...args) && {
    ~*this;
    construct(std::forward<Args>(args)...);
    return move_me().o();
  }
  template <class U, class... Args>

  constexpr inline T &&emplace(std::initializer_list<U> ilist,
                               Args &&...args) && {
    ~*this;
    construct(ilist, std::forward<Args>(args)...);
    return move_me().o();
  }
  void reset() noexcept {
    try {
      ~(*this);
    } catch (...) {
    }
  }
  template <class F>
  mjz_stack_obj_warper_template_class_t or_else(F &&f) const & {
    return has_data() ? *this : std::forward<F>(f)();
  }
  template <class F>
  mjz_stack_obj_warper_template_class_t or_else(F &&f) && {
    return has_data() ? move_me() : std::forward<F>(f)();
  }
  constexpr Type &&value() && { return *move_me(); }
  constexpr const Type &value() const & { return **this; }
  constexpr Type &value() & { return **this; }
  template <class U>
  constexpr Type value_or(U &&default_value) const & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) & {
    return has_data() ? **this : static_cast<T>(std::forward<U>(default_value));
  }
  template <class U>
  constexpr Type value_or(U &&default_value) && {
    return has_data() ? std::move(*move_me())
                      : static_cast<T>(std::forward<U>(default_value));
  }

  template <class... Ts>
  inline constexpr Type &value_or_emplace(Ts &&...args) & {
    if (!!*this) return o();
    emplace(std::forward<Ts>(args)...);
    return o();
  }
  template <class... Ts>
  inline constexpr Type &&value_or_emplace(Ts &&...args) && {
    if (!!*this) return std::move(o());
    emplace(std::forward<Ts>(args)...);
    return std::move(o());
  }

  template <class... Ts>
  inline constexpr mjz_stack_obj_warper_template_class_t &optional_or_emplace(
      Ts &&...args) & {
    if (!!*this) return *this;
    emplace(std::forward<Ts>(args)...);
    return *this;
  }
  template <class... Ts>
  inline constexpr mjz_stack_obj_warper_template_class_t &&optional_or_emplace(
      Ts &&...args) && {
    if (!!*this) return move_me();
    emplace(std::forward<Ts>(args)...);
    return move_me();
  }

  template <class F>
  constexpr auto transform(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const & {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          O()))>(std::forward<F>(f)(O()));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    if (has_data()) {
      return mjz_stack_obj_warper_template_class_t<decltype(std::forward<F>(f)(
          std::move(move_me().O())))>(
          std::forward<F>(f)(std::move(move_me().O())));
    }
    return {};
  }

 public:
  constexpr inline Type *copy_to(Type *dest, bool dest_has_obj) & {
    return copy_to_place(dest, dest_has_obj);
  }
  constexpr inline Type *move_to(Type *dest, bool dest_has_obj) {
    return move_to_place(dest, dest_has_obj);
  }
  constexpr inline Type &copy_to(Type &dest, bool dest_has_obj) & {
    return *copy_to(&dest, dest_has_obj);
  }
  constexpr inline Type &move_to(Type &dest, bool dest_has_obj) {
    return *move_to(&dest, dest_has_obj);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &copy_to(
      mjz_stack_obj_warper_template_class_t &dest) & {
    if (this != &dest)
      dest.notify_unsafe_value_init_in([&](const auto &) {
        return copy_to(dest.pointer_to_unsafe_data(), dest.has_data()) ==
               dest.pointer_to_unsafe_data();
      });
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t &move_to(
      mjz_stack_obj_warper_template_class_t &dest) {
    if (this != &dest)
      dest.notify_unsafe_value_init_in([&](const auto &) {
        return move_to(dest.pointer_to_unsafe_data(), dest.has_data()) ==
               dest.pointer_to_unsafe_data();
      });
    return dest;
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *copy_to(
      mjz_stack_obj_warper_template_class_t *dest) & {
    return &copy_to(*dest);
  }
  constexpr inline mjz_stack_obj_warper_template_class_t *move_to(
      mjz_stack_obj_warper_template_class_t *dest) {
    return &move_to(*dest);
  }
};