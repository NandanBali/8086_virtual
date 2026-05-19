#include "instruction_set.h"

#include "helpers.h"
#include <cstdint>

namespace Instructions::ControlFlow {
void add_label(CPU &cpu, const char *label) {
  cpu.label_table.insert({label, cpu.instruction_exec_pointer});
}

void jump_unc(CPU &cpu, const char *label) {
  cpu.load_instr([&cpu, label]() {
    auto label_pos = cpu.label_table.find(label);
    if (label_pos != cpu.label_table.end()) {
      // exec_instr will ++ after this returns, so step back one.
      cpu.instruction_exec_pointer = label_pos->second - 1;
    }
  });
}

void compare_value_16(CPU &cpu, const char *reg, uint16_t value) {
  cpu.load_instr([&cpu, reg, value]() {
    auto rd = access_register(cpu, reg);
    if (rd.has_value()) {
      uint16_t orig = *rd.value();
      uint16_t res = orig - value;
      handle_flag(cpu, "ZF", orig == value);
      handle_flag(cpu, "CF", orig < value);
      handle_flag(cpu, "SF", res & 0x8000);
    }
  });
}

void jump_if_not_zero(CPU &cpu, const char *label) {
  cpu.load_instr([&cpu, label]() {
    if (!(cpu.reg.flag_reg & 0x0040)) {
      auto label_pos = cpu.label_table.find(label);
      if (label_pos != cpu.label_table.end()) {
        cpu.instruction_exec_pointer = label_pos->second - 1;
      }
    }
  });
}
} // namespace Instructions::ControlFlow
