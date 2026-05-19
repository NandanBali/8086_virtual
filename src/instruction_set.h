#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H
#include "cpu.h"
#include <cstdint>

// MOV instructions
namespace Instructions::MOV {
void direct_16(CPU &cpu, const char *reg, uint16_t value);
void direct_8(CPU &cpu, const char *reg, uint8_t value);
void reg_to_reg(CPU &cpu, const char *reg_src, const char *reg_dest);
} // namespace Instructions::MOV

namespace Instructions::Arithmetic {
void add_reg_to_reg(CPU &cpu, const char *reg_src, const char *reg_dest);
void add_with_carry(CPU &cpu, const char *reg_src, const char *reg_dest);
void add_with_val(CPU &cpu, const char *reg_dest, uint16_t value);
// AX = AX - val
void sub_with_val(CPU &cpu, const char *reg_dest, uint16_t value);
} // namespace Instructions::Arithmetic

namespace Instructions::ControlFlow {
void jump_unc(CPU &cpu, const char *label);
void add_label(CPU &cpu, const char *label);
void compare_value_16(CPU &cpu, const char *reg, uint16_t value);
void compare_value_reg_to_reg(CPU &cpu, const char *reg1, const char *reg2);
void jump_if_not_zero(CPU &cpu, const char *label);
} // namespace Instructions::ControlFlow

#endif // !INSTRUCTION_SET_H
