#pragma once
#include "my_vm_generic_instructions.h"
#include "vm_generic_oprations.h"
namespace mjz_ard {
namespace mjz_vm {

namespace mjz_instruction_no_generic {

struct reg_to_reg : public instruction {
  static inline instruction *get() {
    static reg_to_reg singleton;
    return &singleton;
  }
  template <typename T1, typename T2>
  inline void op(T1 &r, T2 l) {
    r = mjz_ard::static_str_algo::bit_cast<T1>(l);
  }
  template <typename T1, typename T2>
  inline void op_cast(T1 &r, T2 l) {
    r = static_cast<T1>(l);
  }
  context &perform(context &C) {
    using it = instruction_type;
    constexpr uint8_t is_64Bit = it::is_64bit;
    constexpr uint8_t is_8Bit = it::is_byte;
    constexpr uint8_t is_sined = it::is_sined;
    constexpr uint8_t is_vector = it::is_vector;
    constexpr uint8_t result_is_ptr = it::result_is_ptr;
    constexpr uint8_t is_2_typed = it::type_2_op_code;
    constexpr uint8_t is_float = is_vector;
    constexpr uint8_t is_double = is_vector | is_64Bit;
    constexpr uint8_t is_32Bit = it::NONE;
    constexpr uint8_t filter = (~is_sined & ~is_2_typed);

    constexpr uint16_t _8_32_ = is_8Bit | ((int)is_32Bit << 8);
    constexpr uint16_t _32_8_ = is_32Bit | ((int)is_8Bit << 8);
    constexpr uint16_t _64_32_ = is_64Bit | ((int)is_32Bit << 8);
    constexpr uint16_t _32_64_ = is_32Bit | ((int)is_64Bit << 8);
    constexpr uint16_t _8_64_ = is_8Bit | ((int)is_64Bit << 8);
    constexpr uint16_t _64_8_ = is_64Bit | ((int)is_8Bit << 8);

    constexpr uint16_t _p_32_ = result_is_ptr | ((int)is_32Bit << 8);
    constexpr uint16_t _32_p_ = is_32Bit | ((int)result_is_ptr << 8);
    constexpr uint16_t _p_64_ = result_is_ptr | ((int)is_64Bit << 8);
    constexpr uint16_t _64_p_ = is_64Bit | ((int)result_is_ptr << 8);
    constexpr uint16_t _p_8_ = result_is_ptr | ((int)is_8Bit << 8);
    constexpr uint16_t _8_p_ = is_8Bit | ((int)result_is_ptr << 8);

    constexpr uint16_t _f_32_ = is_float | ((int)is_32Bit << 8);
    constexpr uint16_t _32_f_ = is_32Bit | ((int)is_float << 8);
    constexpr uint16_t _f_64_ = is_float | ((int)is_64Bit << 8);
    constexpr uint16_t _64_f_ = is_64Bit | ((int)is_float << 8);
    constexpr uint16_t _f_8_ = is_float | ((int)is_8Bit << 8);
    constexpr uint16_t _8_f_ = is_8Bit | ((int)is_float << 8);

    constexpr uint16_t _d_32_ = is_double | ((int)is_32Bit << 8);
    constexpr uint16_t _32_d_ = is_32Bit | ((int)is_double << 8);
    constexpr uint16_t _d_64_ = is_double | ((int)is_64Bit << 8);
    constexpr uint16_t _64_d_ = is_64Bit | ((int)is_double << 8);
    constexpr uint16_t _d_8_ = is_double | ((int)is_8Bit << 8);
    constexpr uint16_t _8_d_ = is_8Bit | ((int)is_double << 8);

    constexpr uint16_t _d_f_ = is_double | ((int)is_float << 8);
    constexpr uint16_t _f_d_ = is_float | ((int)is_double << 8);
    constexpr uint16_t _d_p_ = is_double | ((int)result_is_ptr << 8);
    constexpr uint16_t _p_d_ = result_is_ptr | ((int)is_double << 8);
    constexpr uint16_t _f_p_ = is_float | ((int)result_is_ptr << 8);
    constexpr uint16_t _p_f_ = result_is_ptr | ((int)is_float << 8);

    bool it_is_2_typed = C.type & is_2_typed;
    uint8_t t1 = C.type & filter;
    uint8_t t2 = C.type_reg_move & filter;
    if (t1 & result_is_ptr) t1 = result_is_ptr;
    if (t2 & result_is_ptr) t2 = result_is_ptr;
    if ((t1 | is_sined) == (t2 | is_sined)) return C;
    uint8_t dest = C.DOR_result;
    uint8_t src = C.DOR_result;
    uint16_t types = t1 | ((int)t2 << 8);
    if (it_is_2_typed) {
      switch (types) {
        case _32_64_:
          op(C.r32[dest], C.r64[src]);
          break;
        case _32_8_:
          op(C.r32[dest], C.r8[src]);
          break;
        case _32_p_:
          op(C.r32[dest], C.address_regs_u[src]);
          break;
        case _32_f_:
          op(C.r32[dest], C.rf[src]);
          break;
        case _32_d_:
          op(C.r32[dest], C.rd[src]);
          break;
        case _64_32_:
          op(C.r64[dest], C.r32[src]);
          break;
        case _64_8_:
          op(C.r64[dest], C.r8[src]);
          break;
        case _64_p_:
          op(C.r64[dest], C.address_regs_u[src]);
          break;
        case _64_f_:
          op(C.r64[dest], C.rf[src]);
          break;
        case _64_d_:
          op(C.r64[dest], C.rd[src]);
          break;
        case _8_32_:
          op(C.r8[dest], C.r32[src]);
          break;
        case _8_64_:
          op(C.r8[dest], C.r64[src]);
          break;
        case _8_p_:
          op(C.r8[dest], C.address_regs_u[src]);
          break;
        case _8_f_:
          op(C.r8[dest], C.rf[src]);
          break;
        case _8_d_:
          op(C.r8[dest], C.rd[src]);
          break;
        case _f_32_:
          op(C.rf[dest], C.r32[src]);
          break;
        case _f_64_:
          op(C.rf[dest], C.r64[src]);
          break;
        case _f_p_:
          op(C.rf[dest], C.address_regs_u[src]);
          break;
        case _f_8_:
          op(C.rf[dest], C.r8[src]);
          break;
        case _f_d_:
          op(C.rf[dest], C.rd[src]);
          break;
        case _d_32_:
          op(C.rd[dest], C.r32[src]);
          break;
        case _d_64_:
          op(C.rd[dest], C.r64[src]);
          break;
        case _d_p_:
          op(C.rd[dest], C.address_regs_u[src]);
          break;
        case _d_8_:
          op(C.rd[dest], C.r8[src]);
          break;
        case _d_f_:
          op(C.rd[dest], C.rf[src]);
          break;
        case _p_32_:
          op(C.address_regs_u[dest], C.r32[src]);
          break;
        case _p_64_:
          op(C.address_regs_u[dest], C.r64[src]);
          break;
        case _p_d_:
          op(C.address_regs_u[dest], C.rd[src]);
          break;
        case _p_8_:
          op(C.address_regs_u[dest], C.r8[src]);
          break;
        case _p_f_:
          op(C.address_regs_u[dest], C.rf[src]);
          break;
        default:
          break;
      }
      return C;
    }
    switch (types) {
      case _32_64_:
        op_cast(C.r32[dest], C.r64[src]);
        break;
      case _32_8_:
        op_cast(C.r32[dest], C.r8[src]);
        break;
      case _32_p_:
        op_cast(C.r32[dest], C.address_regs_u[src]);
        break;
      case _32_f_:
        op_cast(C.r32[dest], C.rf[src]);
        break;
      case _32_d_:
        op_cast(C.r32[dest], C.rd[src]);
        break;
      case _64_32_:
        op_cast(C.r64[dest], C.r32[src]);
        break;
      case _64_8_:
        op_cast(C.r64[dest], C.r8[src]);
        break;
      case _64_p_:
        op_cast(C.r64[dest], C.address_regs_u[src]);
        break;
      case _64_f_:
        op_cast(C.r64[dest], C.rf[src]);
        break;
      case _64_d_:
        op_cast(C.r64[dest], C.rd[src]);
        break;
      case _8_32_:
        op_cast(C.r8[dest], C.r32[src]);
        break;
      case _8_64_:
        op_cast(C.r8[dest], C.r64[src]);
        break;
      case _8_p_:
        op_cast(C.r8[dest], C.address_regs_u[src]);
        break;
      case _8_f_:
        op_cast(C.r8[dest], C.rf[src]);
        break;
      case _8_d_:
        op_cast(C.r8[dest], C.rd[src]);
        break;
      case _f_32_:
        op_cast(C.rf[dest], C.r32[src]);
        break;
      case _f_64_:
        op_cast(C.rf[dest], C.r64[src]);
        break;
      case _f_p_:
        op_cast(C.rf[dest], C.address_regs_u[src]);
        break;
      case _f_8_:
        op_cast(C.rf[dest], C.r8[src]);
        break;
      case _f_d_:
        op_cast(C.rf[dest], C.rd[src]);
        break;
      case _d_32_:
        op_cast(C.rd[dest], C.r32[src]);
        break;
      case _d_64_:
        op_cast(C.rd[dest], C.r64[src]);
        break;
      case _d_p_:
        op_cast(C.rd[dest], C.address_regs_u[src]);
        break;
      case _d_8_:
        op_cast(C.rd[dest], C.r8[src]);
        break;
      case _d_f_:
        op_cast(C.rd[dest], C.rf[src]);
        break;
      case _p_32_:
        op_cast(C.address_regs_u[dest], C.r32[src]);
        break;
      case _p_64_:
        op_cast(C.address_regs_u[dest], C.r64[src]);
        break;
      case _p_d_:
        op_cast(C.address_regs_u[dest], C.rd[src]);
        break;
      case _p_8_:
        op_cast(C.address_regs_u[dest], C.r8[src]);
        break;
      case _p_f_:
        op_cast(C.address_regs_u[dest], C.rf[src]);
        break;
      default:
        break;
    }

    return C;
  }
};



}  // namespace mjz_instruction_no_generic
}  // namespace mjz_vm
}  // namespace mjz_ard