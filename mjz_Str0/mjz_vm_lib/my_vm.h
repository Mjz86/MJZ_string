#include"vm_non_generic_instructions.h"
namespace mjz_ard {
namespace mjz_vm {

namespace opration_instructions {
namespace op = oprations;
struct add : public mjz_generic_instructions::generic_op<op::add> {};
struct sub : public mjz_generic_instructions::generic_op<op::sub> {};
struct mul : public mjz_generic_instructions::generic_op<op::mul> {};
struct div : public mjz_generic_instructions::generic_op<op::div> {};
struct pow : public mjz_generic_instructions::generic_op<op::pow> {};
struct And : public mjz_generic_instructions::generic_logical_op<op::And> {};
struct Or : public mjz_generic_instructions::generic_logical_op<op::Or> {};
struct Xor : public mjz_generic_instructions::generic_logical_op<op::Xor> {};
struct Not : public mjz_generic_instructions::generic_logical_op<op::Not> {};
struct NAnd : public mjz_generic_instructions::generic_logical_op<op::NAnd> {};
struct NOr : public mjz_generic_instructions::generic_logical_op<op::NOr> {};
struct XNor : public mjz_generic_instructions::generic_logical_op<op::XNor> {};
struct SH_r : public mjz_generic_instructions::generic_logical_op<op::SH_r> {};
struct SH_l : public mjz_generic_instructions::generic_logical_op<op::SH_l> {};
struct load : public mjz_generic_instructions::generic_ptr_op<op::load> {};
struct store : public mjz_generic_instructions::generic_ptr_op<op::store> {};
struct nop : public instruction {
  static inline instruction *get() {
    static nop singleton;
    return &singleton;
  }
  context &perform(context &C) { return C; }
};
struct RTR : public mjz_instruction_no_generic::reg_to_reg {};
}  // namespace opration_instructions
inline instruction *get_instruction(instruction_names t) {
  namespace op = opration_instructions;
  static instruction *instruction_array[] = {
      op::nop::get(),   op::add::get(),  op::sub::get(),  op::mul::get(),
      op::div::get(),   op::pow::get(),  op::And::get(),  op::Or::get(),
      op::Xor::get(),   op::Not::get(),  op::NAnd::get(), op::NOr::get(),
      op::XNor::get(),  op::SH_r::get(), op::SH_l::get(), op::load::get(),
      op::store::get(), op::RTR::get(),
  };

  return instruction_array[t];
}
}  // namespace mjz_vm
}  // namespace mjz_ard