#pragma once

namespace mjz_ard {
namespace mjz_vm {
namespace oprations {
struct add {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l + r;
  }
};
struct sub {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l - r;
  }
};
struct mul {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l * r;
  }
};

struct div {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l / r;
  }
};
struct pow {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = std::pow(l, r);
  }
};
struct And {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l & r;
  }
};
struct Or {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l | r;
  }
};
struct Xor {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l ^ r;
  }
};
struct Not {
  template <typename T>
  void inline operator()(T &a, T l, const T &) {
    a = ~l;
  }
};
struct NAnd {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = ~(l & r);
  }
};
struct NOr {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = ~(l | r);
  }
};
struct XNor {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = ~(l ^ r);
  }
};
struct SH_r {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l >> r;
  }
};
struct SH_l {
  template <typename T>
  void inline operator()(T &a, T l, T r) {
    a = l << r;
  }
};
struct load {
  template <typename T>
  inline void operator()(T &v, ptr_type p, const ptr_type &, context &C) {
    v = *((T *)C.to_real_pointer(p.ptr_representation));
  }
  template <typename T>
  inline void operator()(ptr_type &v, T p, const T &, context &C) {
    v.ptr_representation = p;
  }
};
struct store {
  template <typename T>
  inline void operator()(T &v, ptr_type p, const ptr_type &, context &C) {
    *((T *)C.to_real_pointer(p.ptr_representation)) = v;
  }
  template <typename T>
  inline void operator()(ptr_type &v, T p, const T &, context &C) {
    p = v.ptr_representation;
  }
};

}  // namespace oprations
}  // namespace mjz_vm
}  // namespace mjz_ard