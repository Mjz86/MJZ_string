
template <typename T, typename Allocator = mjz_reallocator_t<T>>
struct mjz_Vector {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename Allocator::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  constexpr inline mjz_Vector() : mjz_Vector(Allocator{}) {}
  constexpr inline explicit mjz_Vector(const Allocator &a)
      : m_allocator{a}, m_size{0}, m_capacity{0}, m_data{nullptr} {}
  constexpr inline explicit mjz_Vector(size_type n, const T &val,
                                       const Allocator &a = Allocator{})
      : m_allocator{a},
        m_size{n},
        m_capacity{n},
        m_data{m_allocator.allocate(n)} {
    m_allocator.uninitialized_fill_n(m_data, n, val);
  }
  constexpr inline explicit mjz_Vector(size_type n,
                                       const Allocator &a = Allocator{})
      : m_allocator{a},
        m_size{n},
        m_capacity{n},
        m_data{m_allocator.allocate_obj_array(n)} {}
  template <typename InputIt>
  constexpr inline mjz_Vector(InputIt first, InputIt last,
                              const Allocator &a = Allocator{})
      : mjz_Vector(std::distance(first, last), a) {
    m_allocator.copy(first, last, begin());
  }
  constexpr inline mjz_Vector(const mjz_Vector &other)
      : m_allocator{other.m_allocator},
        m_size{other.m_size},
        m_capacity{other.m_capacity} {
    m_data = m_allocator.allocate(m_capacity);
    m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
  }
  constexpr inline mjz_Vector(const mjz_Vector &other, const Allocator &a)
      : m_allocator{a}, m_size{other.m_size}, m_capacity{other.m_capacity} {
    m_data = m_allocator.allocate(m_capacity);
    m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
  }
  constexpr inline mjz_Vector(mjz_Vector &&other) noexcept
      : m_allocator{std::move(other.m_allocator)},
        m_size{std::exchange(other.m_size, 0)},
        m_capacity{std::exchange(other.m_capacity, 0)},
        m_data{std::exchange(other.m_data, nullptr)} {}
  constexpr inline mjz_Vector(mjz_Vector &&other, const Allocator &a)
      : m_allocator{a},
        m_size{std::exchange(other.m_size, 0)},
        m_capacity{std::exchange(other.m_capacity, 0)},
        m_data{std::exchange(other.m_data, nullptr)} {}
  constexpr inline mjz_Vector(std::initializer_list<T> il,
                              const Allocator &a = Allocator{})
      : mjz_Vector(il.begin(), il.end(), a) {}
  constexpr inline ~mjz_Vector() {
    clear();
    deallocate();
  }
  constexpr inline mjz_Vector &operator=(const mjz_Vector &other) {
    if (this != &other) {
      clear();
      deallocate();
      m_allocator = other.m_allocator;
      m_size = other.m_size;
      m_capacity = other.m_capacity;
      m_data = m_allocator.allocate(m_capacity);
      m_allocator.uninitialized_copy(other.begin(), other.end(), begin());
    }
    return *this;
  }
  constexpr inline mjz_Vector &operator=(mjz_Vector &&other) noexcept {
    if (this != &other) {
      clear();
      deallocate();
      m_allocator = std::move(other.m_allocator);
      m_size = std::exchange(other.m_size, 0);
      m_capacity = std::exchange(other.m_capacity, 0);
      m_data = std::exchange(other.m_data, nullptr);
    }
    return *this;
  }
  constexpr inline mjz_Vector &operator=(std::initializer_list<T> il) {
    *this = mjz_Vector(il);
    return *this;
  }
  constexpr inline reference operator[](size_type pos) { return m_data[pos]; }
  constexpr inline const_reference operator[](size_type pos) const {
    return m_data[pos];
  }
  constexpr inline reference at(size_type pos) {
    check_range(pos);
    return m_data[pos];
  }
  constexpr inline const_reference at(size_type pos) const {
    check_range(pos);
    return m_data[pos];
  }
  constexpr inline reference front() { return *begin(); }
  constexpr inline const_reference front() const { return *begin(); }
  constexpr inline reference back() { return *(end() - 1); }
  constexpr inline const_reference back() const { return *(end() - 1); }
  constexpr inline T *data() noexcept { return m_data; }
  constexpr inline const T *data() const noexcept { return m_data; }
  constexpr inline iterator begin() noexcept { return m_data; }
  constexpr inline const_iterator begin() const noexcept { return m_data; }
  constexpr inline iterator end() noexcept { return m_data + m_size; }
  constexpr inline const_iterator end() const noexcept {
    return m_data + m_size;
  }
  constexpr inline reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr inline const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr inline reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr inline const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr inline const_iterator cbegin() const noexcept { return begin(); }
  constexpr inline const_iterator cend() const noexcept { return end(); }
  constexpr inline const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }
  constexpr inline const_reverse_iterator crend() const noexcept {
    return rend();
  }
  constexpr inline bool empty() const noexcept { return m_size == 0; }
  constexpr inline size_type size() const noexcept { return m_size; }
  constexpr inline size_type max_size() const noexcept {
    return ((((size_type)-1) >> 1) -
            1);  // std::allocator_traits<Allocator>::max_size(m_allocator);
  }
  constexpr inline void reserve(size_type n) {
    if (n > m_capacity) reallocate(n);
  }
  constexpr inline size_type capacity() const noexcept { return m_capacity; }
  constexpr inline void shrink_to_fit() {
    if (m_size < m_capacity) reallocate(m_size);
  }
  constexpr inline void clear() noexcept {
    destroy(begin(), end());
    m_size = 0;
  }
  constexpr inline iterator insert(const_iterator pos, const T &val) {
    return insert(pos, 1, val);
  }
  constexpr inline iterator insert(const_iterator pos, T &&val) {
    emplace(pos, std::move(val));
    return pos;
  }
  constexpr inline iterator insert(const_iterator pos, size_type count,
                                   const T &val) {
    auto offset = pos - cbegin();
    reallocate(m_size + count);
    m_allocator.move_backward(begin() + offset, end() - count, end());
    m_allocator.uninitialized_fill_n(begin() + offset, count, val);
    m_size += count;
    return begin() + offset;
  }
  template <typename InputIt>
  constexpr inline iterator insert(const_iterator pos, InputIt first,
                                   InputIt last) {
    auto offset = pos - cbegin();
    auto count = std::distance(first, last);
    reallocate(m_size + count);
    m_allocator.move_backward(begin() + offset, end() - count, end());
    m_allocator.uninitialized_copy(first, last, begin() + offset);
    m_size += count;
    return begin() + offset;
  }
  constexpr inline iterator insert(const_iterator pos,
                                   std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
  }
  template <typename... Args>
  constexpr inline iterator emplace(const_iterator pos, Args &&...args) {
    auto offset = pos - cbegin();
    reallocate(m_size + 1);
    m_allocator.move_backward(begin() + offset, end() - 1, end());
    m_allocator.construct(m_allocator, m_data + offset,
                          std::forward<Args>(args)...);
    ++m_size;
    return begin() + offset;
  }
  constexpr inline iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  }
  constexpr inline iterator erase(const_iterator first, const_iterator last) {
    auto offset = first - cbegin();
    auto count = last - first;
    destroy(begin() + offset, end());
    std::move(begin() + offset + count, end(), begin() + offset);
    m_size -= count;
    return begin() + offset;
  }
  template <typename... Args>
  constexpr inline reference emplace_back(Args &&...args) {
    if (m_size == m_capacity) {
      reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
    m_allocator.construct_at(m_data + m_size, std::forward<Args>(args)...);
    ++m_size;
    return back();
  }
  constexpr inline void pop_back() {
    if (m_size > 0) {
      --m_size;
      m_allocator.destroy(m_data + m_size);
    }
  }
  constexpr inline bool add_size(size_type n) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    auto it = begin() + size();
    auto it_e = it + n;
    while (it < it_e) {
      m_allocator.construct_at(it);
      ++it;
    }
    m_size += n;
    return 1;
  }
  constexpr inline bool add_size(size_type n, const value_type &value) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    m_allocator.uninitialized_fill_n(begin() + size(), n, value);
    m_size += n;
    return 1;
  }
  constexpr inline bool add_size(size_type n, value_type &&value) {
    if (n == m_size) return 1;
    reserve(size() + n);
    if ((size() + n) > m_capacity) return 0;
    if ((n - 1) > 0)
      m_allocator.uninitialized_fill_n(begin() + size(), n - 1, value);
    m_allocator.construct(m_allocator, begin() + size() + n - 1,
                          std::move(value));
    m_size += n;
    return 1;
  }
  constexpr inline bool resize(size_type n) {
    if (m_size < n) return add_size(n - m_size);
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, const value_type &value) {
    if (m_size < n) return add_size(n - m_size, std::move(value));
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline bool resize(size_type n, value_type &&value) {
    if (m_size < n) return add_size(n - m_size, std::move(value));
    if (m_size > n) {
      destroy(begin() + n, end());
      m_size = n;
      return 1;
    }
    return 1;
  }
  constexpr inline reference push_back(const T &value) {
    return emplace_back(value);
  }
  constexpr inline reference push_back(T &&value) {
    return emplace_back(std::move(value));
  }

 private:
  allocator_type m_allocator;
  size_type m_size;
  size_type m_capacity;
  pointer m_data;
  // Helper functions

  constexpr inline void deallocate() {
    if (m_data) {
      for (size_type i = 0; i < m_size; ++i) {
        m_allocator.destroy(m_data + i);
      }
      m_allocator.deallocate(m_data, m_capacity);
      m_data = nullptr;
      m_size = 0;
      m_capacity = 0;
    }
  }
  constexpr inline void destroy(iterator first, iterator last) {
    while (first < last) {
      m_allocator.destroy(first++);
    }
  }
  constexpr inline void reallocate(size_type newCapacity) {
    pointer newData = m_allocator.allocate(newCapacity);
    m_allocator.uninitialized_move_n(m_data, m_size, newData);
    size_t s = m_size;
    deallocate();
    m_size = s;
    m_data = newData;
    m_capacity = newCapacity;
  }
  constexpr inline void check_range(size_type size) {
    if (m_size < size) Throw<std::out_of_range>(" bad accesses");
  }
};

namespace smart_ptr {
template <typename T>
union data_storage {
  using Type = T;
  char f;
  Type object;
  inline data_storage() {}  // no initilization

  inline ~data_storage() {}  // no deinitilization

  inline operator Type &() & { return *((Type *)&f); }

  inline operator Type &&() && { return (Type &&)*((Type *)&f); }

  inline operator const Type &() & { return *((const Type *)&f); }

  inline operator const Type &&() && {
    return (const Type &&)*((const Type *)&f);
  }

  inline operator const Type &() const & { return *((const Type *)&f); }

  inline operator const Type &&() const && {
    return (const Type &&)*((const Type *)&f);
  }

  inline operator Type *() & { return ((Type *)&f); }

  inline operator const Type *() & { return ((const Type *)&f); }

  inline operator const Type *() const & { return ((const Type *)&f); }

  inline Type &operator*() & { return operator Type &(); }

  inline const Type &operator*() const & { return *((const Type *)&f); }

  inline Type *operator->() & { return operator Type *(); }

  inline const Type *operator->() const & { return operator const Type *(); }

  inline Type *ptr() & { return operator Type *(); }

  inline Type &get() & { return operator Type &(); }

  inline const Type *ptr() const & { return operator const Type *(); }

  inline const Type &get() const & { return *((const Type *)&f); }

  inline Type &&get() && { return (Type &&)*((Type *)&f); }

  inline const Type &&get() const && {
    return (const Type &&)*((const Type *)&f);
  }
};
template <typename T>
struct smart_data_structure_base {
  using Type = T;
  using size_lt = uint32_t;
  size_lt shared_ptr_ref_count{};
  size_lt weak_ptr_ref_count{};
  data_storage<T> *data;
  // a char will exist in both derived classes at this spot
  // like this  char first_of_obj_if_allocted_both_in_one_go = pure variable;
  smart_data_structure_base() {}
  ~smart_data_structure_base() {}
};
template <typename T>
struct smart_data_structure_sepereat : public smart_data_structure_base<T> {
  char first_of_obj_if_allocted_both_in_one_go{};
};
template <typename T>
struct smart_data_structure_both : public smart_data_structure_base<T> {
  union data_storage_t {
    data_storage_t() {}   // no initilization of T
    ~data_storage_t() {}  // no deinitilization of T
    T Data_of_var;
    char first_of_obj_if_allocted_both_in_one_go;
  };
  data_storage_t buff;
};
template <typename T>
bool is_sepereat(const smart_data_structure_base<T> &obj) {
  static_assert(offsetof(smart_data_structure_both<T>,
                         buff.first_of_obj_if_allocted_both_in_one_go) ==
                offsetof(smart_data_structure_sepereat<T>,
                         first_of_obj_if_allocted_both_in_one_go));

  return ((void *)(&((smart_data_structure_sepereat<T> *)&obj)
                        ->first_of_obj_if_allocted_both_in_one_go)) !=
         (void *)obj.data;
}

template <typename T>
class smart_ptr_base_template_t : private mjz_reallocator_t<T> {
 private:
  void deallocate_seperate() {
    if (!m_data) return;
    if (m_data->data) {
      try {
        operator delete(m_data->data);  // no destructor called
      } catch (...) {
      }
    }
    try {
      delete m_data;
    } catch (...) {
    }
    m_data = 0;
  }
  void allocate_seperate(data_storage<T> *p) {
    if (m_data) trap_crash();  // if has object ??
    try {
      m_data = new DB_t;
    } catch (...) {
      m_data = 0;
    }
    if (!m_data) return;
    m_data->data = p;
  }
  void deallocate_both() {
    if (!m_data) return;
    try {
      delete ((smart_data_structure_both<T> *)(m_data));
    } catch (...) {
    }
    m_data = 0;
  }
  void allocate_both() {
    if (m_data) trap_crash();  // if has object ??
    try {
      m_data = new smart_data_structure_both<T>;
    } catch (...) {
      m_data = 0;
    }
    if (!m_data) return;
    try {
      m_data->data =
          (decltype(m_data
                        ->data))(((char *)m_data) +
                                 offsetof(
                                     smart_data_structure_both<T>,
                                     buff.first_of_obj_if_allocted_both_in_one_go));
    } catch (...) {
      m_data->data = 0;
    }

    if (!m_data->data) {
      deallocate_both();
    }
  }

 public:
  void deallocate() {
    if (is_sepereat(*m_data)) return deallocate_seperate();
    return deallocate_both();
  }
  void allocate(data_storage<T> *p = 0) {
    if (p) return allocate_seperate(p);
    return allocate_both();
  }

 public:
  using DB_t = smart_data_structure_base<T>;
  DB_t *m_data{};
  using Type = T;

  void destroy() {
    if (!m_data) return;
    if (!m_data->data) return;
    get_object_allocator().destroy_at(m_data->data->ptr());
  }
  template <typename... args_t>
  bool create(args_t &&...args) {
    return create_with([&, this](Type *p) {
      get_object_allocator().construct_at(p, std::forward<args_t>(args)...);
      return true;
    });
  }
  bool create_with(std::function<bool(Type *)> FN_construct_at) {
    if (!m_data) return false;
    bool sucsses_full{false};
    try {
      sucsses_full = FN_construct_at(m_data->data->ptr());
    } catch (...) {
    }
    return sucsses_full;
  }

  void weak_add(DB_t *p) {
    if (!p) return;
    m_data = p;
    m_data->weak_ptr_ref_count++;
  }
  bool sheared_add(DB_t *p) {
    if (!p) return false;
    m_data = p;
    m_data->shared_ptr_ref_count++;
    return true;
  }

 protected:
  smart_ptr_base_template_t() {}
  ~smart_ptr_base_template_t() {}
  mjz_temp_type_allocator_warpper_t<T> &get_object_allocator() { return *this; }
  const mjz_temp_type_allocator_warpper_t<T> &get_object_allocator() const {
    return *this;
  }
  void update_weak() {
    if (!m_data) return;
    if (!m_data->shared_ptr_ref_count) {
      weak_free();
    }
  }
  void weak_set(DB_t *p) { weak_add(p); }

  void weak_free() {
    if (!m_data) return;
    DB_t &data = *m_data;
    data.weak_ptr_ref_count--;
    if (data.shared_ptr_ref_count) {
    } else if (!data.weak_ptr_ref_count) {
      deallocate();
    }

    m_data = nullptr;
  }
  void sheared_free() {
    if (!m_data) return;
    DB_t &data = *m_data;

    data.shared_ptr_ref_count--;
    if (!data.shared_ptr_ref_count) {
      destroy();
      if (!data.weak_ptr_ref_count) {
        deallocate();
      }
    }
    m_data = nullptr;
  }
};
template <class T>
class weak_ptr_template_t;
template <typename T>
class sheared_ptr_template_t : protected smart_ptr_base_template_t<T> {
  using DB_t = typename smart_ptr_base_template_t<T>::DB_t;
  using Type = T;
  DB_t *&ptr() { return this->m_data; }
  const DB_t *const &ptr() const { return this->m_data; }

 public:
  sheared_ptr_template_t() : smart_ptr_base_template_t<T>() {}
  ~sheared_ptr_template_t() { operator~(); }
  void operator~() { this->sheared_free(); }
  sheared_ptr_template_t(sheared_ptr_template_t &&obj) {
    ptr() = std::exchange(obj.ptr(), {0});
  }
  sheared_ptr_template_t(const sheared_ptr_template_t &&obj) {
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  sheared_ptr_template_t(sheared_ptr_template_t &obj) {
    this->sheared_add(obj.ptr());
  }
  sheared_ptr_template_t(const sheared_ptr_template_t &obj) {
    this->sheared_add(remove_const(obj.ptr()));
  }

  sheared_ptr_template_t &operator=(sheared_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(obj.ptr(), {0});
    return *this;
  }
  sheared_ptr_template_t &operator=(const sheared_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  sheared_ptr_template_t &operator=(sheared_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->sheared_add(obj.ptr());
    return *this;
  }
  sheared_ptr_template_t &operator=(const sheared_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->sheared_add(remove_const(obj.ptr()));
    return *this;
  }

  void reset() noexcept { operator~(); }
  template <class T_der>
  void reset(smart_data_structure_base<T_der> *p)
    requires(std::is_base_of<T, T_der>)
  {
    operator~();
    if (p) {
      ptr() = (smart_data_structure_base<T> *)
          p;  // dont worry its just a {s_lt,s_lt,der ptr}
    }
  }
  T *get() {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  T &operator*() {
    return *get();  // may derefrence null;
  }
  T *operator->() {
    return get();  // may derefrence null;
  }
  long use_count() const noexcept {
    if (!ptr()) return 0;
    return ptr()->shared_ptr_ref_count;
  }

  const T *get() const {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  const T &operator*() const {
    return *get();  // may derefrence null;
  }
  const T *operator->() const {
    return get();  // may derefrence null;
  }

  explicit operator bool() const { return !!ptr(); }
  bool operator!() const { return !ptr(); }

 public:
  template <typename... args_t>
  [[nodiscard]] static sheared_ptr_template_t mjz_make_sheared(
      args_t &&...args) {
    sheared_ptr_template_t ret;
    ret.allocate();
    ret.create(std::forward<args_t>(args)...);
    ret.sheared_add(ret.m_data);
    return ret;
  }

  friend class weak_ptr_template_t<T>;
};

template <typename T, typename... args_t>
sheared_ptr_template_t<T> make_sheared(args_t &&...args) {
  return sheared_ptr_template_t<T>::mjz_make_sheared(
      std::forward<args_t>(args)...);
}

template <typename T>
class weak_ptr_template_t : protected smart_ptr_base_template_t<T> {
  using DB_t = typename smart_ptr_base_template_t<T>::DB_t;
  using Type = T;
  friend class sheared_ptr_template_t<T>;

  DB_t *&ptr() {
    UW();
    return this->m_data;
  }
  const DB_t *const &ptr() const {
    UW();
    return this->m_data;
  }
  void UW() const  // not really
  {
    remove_const(this)->update_weak();
  }

 public:
  weak_ptr_template_t() : smart_ptr_base_template_t<T>() {}
  ~weak_ptr_template_t() { operator~(); }
  void operator~() { this->weak_free(); }
  weak_ptr_template_t(weak_ptr_template_t &&obj) {
    ptr() = std::exchange(obj.ptr(), {0});
  }
  weak_ptr_template_t(const weak_ptr_template_t &&obj) {
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  weak_ptr_template_t(weak_ptr_template_t &obj) { this->weak_add(obj.ptr()); }
  weak_ptr_template_t(const weak_ptr_template_t &obj) {
    this->weak_add(remove_const(obj.ptr()));
  }

  weak_ptr_template_t(const sheared_ptr_template_t<T> &obj) {
    this->weak_add(remove_const(obj.ptr()));
  }

  weak_ptr_template_t &operator=(weak_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(obj.ptr(), {0});
    return *this;
  }
  weak_ptr_template_t &operator=(const weak_ptr_template_t &&obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    ptr() = std::exchange(remove_const(obj.ptr()), {0});
  }
  weak_ptr_template_t &operator=(weak_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(obj.ptr());
    return *this;
  }
  weak_ptr_template_t &operator=(const weak_ptr_template_t &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(remove_const(obj.ptr()));
    return *this;
  }

  weak_ptr_template_t &operator=(const sheared_ptr_template_t<T> &obj) {
    if (ptr() == obj.ptr()) return *this;
    operator~();
    this->weak_add(remove_const(obj.ptr()));
    return *this;
  }

  void reset() noexcept { operator~(); }
  template <class T_der>
  void reset(smart_data_structure_base<T_der> *p)
    requires(std::is_base_of<T, T_der>)
  {
    operator~();
    if (p) {
      ptr() = (smart_data_structure_base<T> *)
          p;  // dont worry its just a {s_lt,s_lt,der ptr}
    }
  }
  T *get() {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  T &operator*() {
    return *get();  // may derefrence null;
  }
  T *operator->() {
    return get();  // may derefrence null;
  }

  const T *get() const {
    if (!ptr()) return nullptr;
    if (!ptr()->data) return nullptr;
    return ptr()->data->get();
  }
  const T &operator*() const {
    return *get();  // may derefrence null;
  }
  const T *operator->() const {
    return get();  // may derefrence null;
  }

  explicit operator bool() const { return !!ptr(); }
  bool operator!() const { return !ptr(); }
};

};  // namespace smart_ptr