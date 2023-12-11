#pragma once
#pragma once
#include "mjzString.hpp"
namespace mjz_ard {
namespace mjz_vm {

enum instruction_names : uint16_t {
  nop = 0,
  add,
  sub,
  mul,
  div,
  pow,
  And,
  Or,
  Xor,
  Not,
  NAnd,
  NOr,
  XNor,
  SH_r,
  SH_l,
  load,
  store,
  RTR,
};

class context;

class instruction {
 public:
  virtual context &perform(context &C) = 0;
  inline context &operator()(context &C) { return perform(C); }
  //  instruction() {}
  // virtual   ~instruction() {}
};
struct ptr_type {
  ptr_type(uint64_t p = 0) : ptr_representation(p) {}
  uint64_t ptr_representation;
};
class instruction;
enum instruction_type : uint8_t {
  NONE = 0,
  is_sined = MJZ_logic_BIT(1),
  is_vector = MJZ_logic_BIT(2),
  is_64bit = MJZ_logic_BIT(3),
  result_is_ptr = MJZ_logic_BIT(4),
  is_byte = MJZ_logic_BIT(5),
  type_2_op_code = MJZ_logic_BIT(6),
};

struct context_data {
  constexpr static size_t NumberOf_ll_regs = 2;
  constexpr static size_t NumberOf_lf_regs = 2;
  constexpr static size_t NumberOf_ptr_regs = 2;
  struct {
    union {
      int64_t r64[NumberOf_ll_regs];
      uint64_t ru64[NumberOf_ll_regs];
    };
    union {
      int16_t r16[NumberOf_ll_regs * 4];
      uint16_t ru16[NumberOf_ll_regs * 4];
    };
    union {
      uint32_t ru32[NumberOf_ll_regs * 2];
      int32_t r32[NumberOf_ll_regs * 2];
    };
    union {
      uint8_t ru8[NumberOf_ll_regs * 8];
      int8_t r8[NumberOf_ll_regs * 8];
    };
  };
  struct {
    double rd[NumberOf_lf_regs];
    float rf[NumberOf_lf_regs * 2];
  };
  union {
    uint8_t DOR[3];
    struct {
      uint8_t DOR_result;
      uint8_t DOR_rhs;
      uint8_t DOR_lhs;
    };
  };
  union {
    ptr_type address_regs[NumberOf_ptr_regs];
    uint64_t address_regs_u[NumberOf_ptr_regs];
  };
  context_data() {}
};
struct instruction_data {
  uint64_t instruction;
  uint64_t data;
};
struct context : public context_data {
  uint64_t pc;  // program counter
  dynamic_arena_allocator my_heap;
  ptr_type to_fake_pointer(void *p_) {
    uint8_t *p = (uint8_t *)p_;
    if (p && hp_unsafe.begin() <= p && p < hp_unsafe.end()) {
      return ptr_type{(size_t)(p - hp_unsafe.get_pointer())};
    }
    return {};
  }
  void *to_real_pointer(ptr_type p) {
    if (p.ptr_representation) {
      return &hp_unsafe[p.ptr_representation];
    }
    return nullptr;
  }
  iterator_template_t<uint8_t> sp;         // stack_pointer
  iterator_template_t<uint8_t> hp_unsafe;  // stack_pointer
  instruction_type type;
  instruction_type type_reg_move;
  context(uint64_t pc_, iterator_template_t<uint8_t> data_p_, size_t stack_size)
      : pc([&]() -> uint64_t {
          if (data_p_.size() < dynamic_arena_allocator::block_required_size ||
              data_p_.size() < stack_size) {
            stack_size = 0;
            throw std::exception(" low given memory ");
          }
          return pc_;
        }()),
        my_heap(data_p_.get_pointer(), data_p_.size()),
        sp((uint8_t *)my_heap.malloc(stack_size), stack_size),
        hp_unsafe((uint8_t *)my_heap.unsafe_data(), my_heap.unsafe_size()),context_data() {}
  context &operator()(instruction &inst) {
    inst.perform(*this);
    return *this;
  }
};
}  // namespace mjz_vm
}  // namespace mjz_ard