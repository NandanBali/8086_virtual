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
void add(CPU &cpu, const char *reg_src, const char *reg_dest);
void add_with_carry(CPU &cpu, const char *reg_src, const char *reg_dest);
} // namespace Instructions::Arithmetic

#endif // !INSTRUCTION_SET_H
