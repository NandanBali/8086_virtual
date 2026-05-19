#include "instruction_set.h"

#include "helpers.h"
#include <cstdint>

namespace Instructions::ControlFlow {
void add_label(CPU &cpu, const char *label) {
  cpu.label_table.insert({label, cpu.instruction_exec_pointer});
}

void jump_unc(CPU &cpu, const char *label) {
  auto label_pos = cpu.label_table.find(label);
  if (label_pos != cpu.label_table.end()) {
    cpu.instruction_exec_pointer = label_pos->second;
  }
}

void compare_value_16(CPU &cpu, const char *reg, uint16_t value) {
  auto rd = access_register(cpu, reg);
  if (rd.has_value()) {
    uint16_t orig = *rd.value();
    Instructions::Arithmetic::sub_with_val(cpu, reg, value);
    Instructions::MOV::direct_16(cpu, reg, orig);
  }
}

void jump_if_not_zero(CPU &cpu, const char *label) {
  if (!(cpu.reg.flag_reg & 0x0040)) {
    jump_unc(cpu, label);
  }
}
} // namespace Instructions::ControlFlow
