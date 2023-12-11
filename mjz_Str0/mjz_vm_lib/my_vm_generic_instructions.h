#pragma once
#include "my_vm_data.h"
namespace mjz_ard {
namespace mjz_vm {
namespace mjz_generic_instructions {

template <typename OP>
struct generic_op : public instruction {
  static inline instruction *get() {
    static generic_op singleton;
    return &singleton;
  }
  OP op_obj;
  context &perform(context &C) {
    using it = instruction_type;
    uint8_t it_tp = C.type;
    constexpr uint8_t is_64bit = it::is_64bit;
    constexpr uint8_t is_sined = it::is_sined;
    constexpr uint8_t is_vector = it::is_vector;
    constexpr uint8_t none_ = it::NONE;
    constexpr uint8_t is_float = is_vector | is_sined;
    constexpr uint8_t is_double = is_vector | is_64bit | is_sined;
    constexpr uint8_t is_u_float = is_vector;
    constexpr uint8_t is_u_double = is_vector | is_64bit;
    constexpr uint8_t is_32_i = is_sined;
    constexpr uint8_t is_32_u = none_;
    constexpr uint8_t is_64_i = is_64bit | is_sined;
    constexpr uint8_t is_64_u = is_64bit;
    switch (it_tp) {
      case is_double:
        op_obj(C.rd[C.DOR_result], C.rd[C.DOR_lhs], C.rd[C.DOR_rhs]);
        break;
      case is_float:
        op_obj(C.rf[C.DOR_result], C.rf[C.DOR_lhs], C.rf[C.DOR_rhs]);
        break;
      case is_32_i:
        op_obj(C.r32[C.DOR_result], C.r32[C.DOR_lhs], C.r32[C.DOR_rhs]);
        break;
      case is_32_u:
        op_obj(C.ru32[C.DOR_result], C.ru32[C.DOR_lhs], C.ru32[C.DOR_rhs]);
        break;
      case is_64_i:
        op_obj(C.r64[C.DOR_result], C.r64[C.DOR_lhs], C.r64[C.DOR_rhs]);
        break;
      case is_64_u:
        op_obj(C.ru64[C.DOR_result], C.ru64[C.DOR_lhs], C.ru64[C.DOR_rhs]);
        break;
      case is_u_double:
        op_obj(C.rd[C.DOR_result], abs(C.rd[C.DOR_lhs]), abs(C.rd[C.DOR_rhs]));
        break;
      case is_u_float:
        op_obj(C.rf[C.DOR_result], abs(C.rf[C.DOR_lhs]), abs(C.rf[C.DOR_rhs]));
        break;
      default:
        break;
    }
    return C;
  }
};
template <typename OP>
struct generic_logical_op : public instruction {
  static inline instruction *get() {
    static generic_logical_op singleton;
    return &singleton;
  }
  OP op_obj;
  context &perform(context &C) {
    using it = instruction_type;
    uint8_t it_tp = C.type;
    constexpr uint8_t is_64bit = it::is_64bit;
    constexpr uint8_t is_sined = it::is_sined;
    constexpr uint8_t none_ = it::NONE;
    constexpr uint8_t is_32_i = is_sined;
    constexpr uint8_t is_32_u = none_;
    constexpr uint8_t is_64_i = is_64bit | is_sined;
    constexpr uint8_t is_64_u = is_64bit;
    switch (it_tp) {
      case is_32_i:
        op_obj(C.r32[C.DOR_result], C.r32[C.DOR_lhs], C.r32[C.DOR_rhs]);
        break;
      case is_32_u:
        op_obj(C.ru32[C.DOR_result], C.ru32[C.DOR_lhs], C.ru32[C.DOR_rhs]);
        break;
      case is_64_i:
        op_obj(C.r64[C.DOR_result], C.r64[C.DOR_lhs], C.r64[C.DOR_rhs]);
        break;
      case is_64_u:
        op_obj(C.ru64[C.DOR_result], C.ru64[C.DOR_lhs], C.ru64[C.DOR_rhs]);
        break;
      default:
        break;
    }
    return C;
  }
};
template <typename OP>
struct generic_ptr_op : public instruction {
  static inline instruction *get() {
    static generic_ptr_op singleton;
    return &singleton;
  }
  OP op_obj;
  context &perform(context &C) {
    using it = instruction_type;
    uint8_t it_tp = C.type;
    constexpr uint8_t is_64bit = it::is_64bit;
    constexpr uint8_t is_8bit = it::is_byte;
    constexpr uint8_t is_sined = it::is_sined;
    constexpr uint8_t is_vector = it::is_vector;
    constexpr uint8_t result_is_ptr = it::result_is_ptr;
    constexpr uint8_t none_ = it::NONE;
    constexpr uint8_t is_float = is_vector | is_sined;
    constexpr uint8_t is_double = is_vector | is_64bit | is_sined;
    constexpr uint8_t is_32_i = is_sined;
    constexpr uint8_t is_32_u = none_;
    constexpr uint8_t is_64_i = is_64bit | is_sined;
    constexpr uint8_t is_64_u = is_64bit;
    constexpr uint8_t is_8_c = is_8bit | is_sined;
    constexpr uint8_t is_8_u = is_8bit;

    if (result_is_ptr & it_tp) {
      it_tp &= ~result_is_ptr;
      switch (it_tp) {
        case is_32_i:
          op_obj(C.address_regs[C.DOR_result], C.r32[C.DOR_lhs],
                 C.r32[C.DOR_rhs], C);
          break;
        case is_32_u:
          op_obj(C.address_regs[C.DOR_result], C.ru32[C.DOR_lhs],
                 C.ru32[C.DOR_rhs], C);
          break;
        case is_64_i:
          op_obj(C.address_regs[C.DOR_result], C.r64[C.DOR_lhs],
                 C.r64[C.DOR_rhs], C);
          break;
        case is_64_u:
          op_obj(C.address_regs[C.DOR_result], C.ru64[C.DOR_lhs],
                 C.ru64[C.DOR_rhs], C);
          break;
        case is_8_c:
          op_obj(C.address_regs[C.DOR_result], C.r8[C.DOR_lhs], C.r8[C.DOR_rhs],
                 C);
          break;
        case is_8_u:
          op_obj(C.address_regs[C.DOR_result], C.ru8[C.DOR_lhs],
                 C.ru8[C.DOR_rhs], C);
          break;

        default:
          break;
      }
      return C;
    }
    if (it_tp & is_vector) it_tp |= is_sined;
    switch (it_tp) {
      case is_double:
        op_obj(C.rd[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_float:
        op_obj(C.rf[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_32_i:
        op_obj(C.r32[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_32_u:
        op_obj(C.ru32[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_64_i:
        op_obj(C.r64[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_64_u:
        op_obj(C.ru64[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_8_u:
        op_obj(C.ru8[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      case is_8_c:
        op_obj(C.r8[C.DOR_result], C.address_regs[C.DOR_lhs],
               C.address_regs[C.DOR_rhs], C);
        break;
      default:
        break;
    }
    return C;
  }
};










}  // namespace mjz_generic_instructions
}  // namespace mjz_vm
}  // namespace mjz_ard