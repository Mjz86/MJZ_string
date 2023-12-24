
#include "my_main.h"




template<typename T>
  union data_storage {
      using Type=T;
  char f;
  Type object;
  inline   data_storage() {}  // no initilization

  inline   ~data_storage() {}  // no deinitilization

  inline   operator Type &() & { return *((Type *)&f); }

  inline   operator Type &&() && { return (Type &&) * ((Type *)&f); }

  inline   operator const Type &() & { return *((const Type *)&f); }

  inline   operator const Type &&() && {
    return (const Type &&)*((const Type *)&f);
  }

  inline   operator const Type &() const & {
    return *((const Type *)&f);
  }

  inline   operator const Type &&() const && {
    return (const Type &&)*((const Type *)&f);
  }

  inline   operator Type *() & { return ((Type *)&f); }

  inline   operator const Type *() & { return ((const Type *)&f); }

  inline   operator const Type *() const & {
    return ((const Type *)&f);
  }

  inline   Type &operator*() & { return operator Type &(); }

  inline   const Type &operator*() const & {
    return *((const Type *)&f);
  }

  inline   Type *operator->() & { return operator Type *(); }

  inline   const Type *operator->() const & {
    return operator const Type *();
  }

  inline   Type *ptr() & { return operator Type *(); }

  inline   Type &get() & { return operator Type &(); }

  inline   const Type *ptr() const & { return operator const Type *(); }

  inline   const Type &get() const & { return *((const Type *)&f); }

  inline   Type &&get() && { return (Type &&) * ((Type *)&f); }

  inline   const Type &&get() const && {
    return (const Type &&)*((const Type *)&f);
  }
};
template<typename T>
struct smart_data_structure_base {
  using Type = T;
  using size_lt = uint32_t;
  size_lt shared_ptr_ref_count{};
  size_lt week_ptr_ref_count{};
  data_storage<T> *data;
  // a char will exist in both derived classes at this spot
  //like this  char first_of_obj_if_allocted_both_in_one_go = pure variable;
  smart_data_structure_base() {}
  ~smart_data_structure_base() {}
  
};
template <typename T>
struct smart_data_structure_sepereat : public smart_data_structure_base<T> {
  char first_of_obj_if_allocted_both_in_one_go{};
 
};
template <typename T>
struct smart_data_structure_both : public smart_data_structure_base<T> {
  union data_storage_t{
    data_storage_t() {}//no initilization of T
    ~data_storage_t() {}  // no deinitilization of T
    T Data_of_var;
    char first_of_obj_if_allocted_both_in_one_go;
  };
  data_storage_t buff;
};
template<typename T>
bool is_sepereat(const smart_data_structure_base<T>&obj) {
  static_assert(offsetof(smart_data_structure_both<T>,
                         buff.first_of_obj_if_allocted_both_in_one_go) ==
                offsetof(smart_data_structure_sepereat<T>,
                         first_of_obj_if_allocted_both_in_one_go));

  return ((void *)(&((smart_data_structure_sepereat<T> *)&obj)
                ->first_of_obj_if_allocted_both_in_one_go)) != (void *)obj.data;
}

template <typename T> 
class smart_ptr_base_template_t
    : private mjzt::mjz_allocator_warpper<T> {
    private:
  void deallocate_seperate() {
    if (!m_data) return;
    if (m_data->data) {
      try {
         operator delete (m_data->data);//no destructor called
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
    if (m_data) mjz::static_str_algo::trap_crash();  // if has object ??
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
    if (m_data) mjz::static_str_algo::trap_crash();  // if has object ??
    try {
      m_data = new smart_data_structure_both<T>;
    } catch (...) {
      m_data = 0;
    }
    if (!m_data) return;
    try {
      m_data->data =
          (decltype(m_data->data))(((char *)m_data) +
                      offsetof(smart_data_structure_both<T>,
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
  using Type=T;

  void destroy() {
      if (!m_data) return;
      if (!m_data->data) return;
    get_object_allocator().destroy_at(m_data->data->ptr());
  }
  template<typename...args_t>
  bool create(args_t&&...args) {
    return create_with([&,this](Type *p) {
      get_object_allocator().construct_at(p,std::forward<args_t>(args)...);
          return true;
                  });
  }
  bool create_with(std::function<bool(Type*)>FN_construct_at) {
    if (!m_data) return false;
    bool sucsses_full{false};
    try {
      sucsses_full= FN_construct_at(m_data->data->ptr());
    } catch (...) {
    }
    return sucsses_full;
  }
 
  void week_add(DB_t *p) {
    if (!p) return;
    m_data = p;
    m_data->week_ptr_ref_count++;
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
  mjz::mjz_temp_type_allocator_warpper_t<T>& get_object_allocator( ) {
    return *this;
  }
 const mjz::mjz_temp_type_allocator_warpper_t<T> &get_object_allocator()const {
    return *this;
  }
 void update( ) {
    if (!m_data) return; 
    if (!m_data->shared_ptr_ref_count) {
      m_data = 0;
    }
  
 }
 void week_set(DB_t *p) { week_add(p); }
 
 void week_free() {
    if (!m_data) return;
    DB_t &data = *m_data; 
      data.week_ptr_ref_count--;
      if (data.shared_ptr_ref_count) {
      } else if (!data.week_ptr_ref_count) {
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
        if (!data.week_ptr_ref_count) {
          deallocate();
        }
      }
      m_data = nullptr; 
 }

};

template <typename T>
class sheared_ptr_template_t :protected smart_ptr_base_template_t<T> {
    using DB_t =typename smart_ptr_base_template_t<T>::DB_t;
    using Type=T;
    DB_t*& ptr( ) { return this->m_data;
 }
const DB_t *&ptr() const{ return this->m_data; }

public:
 sheared_ptr_template_t() : smart_ptr_base_template_t<T>(){}
 ~sheared_ptr_template_t() {
     operator~(); 
 }
 void operator~() { this->sheared_free(); }
 sheared_ptr_template_t(sheared_ptr_template_t &&obj) {
      ptr( ) = std::exchange(obj.ptr( ), {0});
 }
 sheared_ptr_template_t(const sheared_ptr_template_t &&obj) {
      ptr( ) = std::exchange(mjz::remove_const(obj.ptr( )), {0});
 }
 sheared_ptr_template_t(sheared_ptr_template_t &obj) {
     this->
      sheared_add(obj.ptr( ));
 }
 sheared_ptr_template_t(const sheared_ptr_template_t &obj) {
     this->sheared_add(mjz::remove_const(obj.ptr()));
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
      ptr( ) = std::exchange(mjz::remove_const(obj.ptr( )), {0});
 }
 sheared_ptr_template_t &operator=(sheared_ptr_template_t &obj) {
      if (ptr( ) == obj.ptr( )) return *this;
      operator~();
      this->sheared_add(obj.ptr());
      return *this;
 }
 sheared_ptr_template_t &operator=(const sheared_ptr_template_t &obj) {
      if (ptr() == obj.ptr()) return *this;
      operator~();
      this->sheared_add(mjz::remove_const(obj.ptr()));
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
      if (!ptr( )) return nullptr;
      if (!ptr( )->data) return nullptr;
      return ptr( )->data->get();
 }
 T &operator*() {
      return *get();  // may derefrence null;
 }
 T *operator->() {
      return get();  // may derefrence null;
 }
 long use_count() const noexcept {
      if (!ptr( )) return 0;
      return ptr( )->shared_ptr_ref_count;
 }
 explicit operator bool() const { return !!ptr(); }
  bool operator !() const { return !ptr(); }
 public:
  template < typename... args_t>
  [[nodiscard]]
      static sheared_ptr_template_t mjz_make_sheared(
      args_t &&...args) {
      sheared_ptr_template_t  ret;
      ret.allocate();
      ret.create(std::forward<args_t>(args)...);
      ret.sheared_add(ret.m_data);
        return ret;
 }
};

 template <typename T, typename... args_t>
  sheared_ptr_template_t<T>  make_sheared(args_t &&...args) {
 return sheared_ptr_template_t<T>::mjz_make_sheared(
     std::forward<args_t>(args)...);
}




  int my_main::main(int argc, const char *const *const argv) {
  using namespace mjz;
  using namespace mjzt;
 if(0) {
        Scoped_speed_Timer t("____________________________");
        t("____________________________");
        {
        std::weak_ptr<operation_reporter> w;
        {
          std::shared_ptr<operation_reporter> s;

          s = std::make_shared<operation_reporter>(" S ");

          w = s;
        }
        }
 }
 {
        sheared_ptr_template_t<operation_reporter> s0;
        {
        auto s = make_sheared<operation_reporter>();

        s0 = s;
        }
 }
  return 0;
 }
