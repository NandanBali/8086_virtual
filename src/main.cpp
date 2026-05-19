#include "cpu.h"
#include "instruction_set.h"
int main() {
  CPU cpu{};
  Instructions::MOV::direct_16(cpu, "AX", 10);
  Instructions::MOV::direct_16(cpu, "BX", 0);
  Instructions::ControlFlow::add_label(cpu, "loop1");
  Instructions::Arithmetic::add_reg_to_reg(cpu, "AX", "BX");
  Instructions::Arithmetic::sub_with_val(cpu, "AX", 1);
  Instructions::ControlFlow::jump_if_not_zero(cpu, "loop1");
  cpu.exec_instr();
  std::cout << "after adding\n" << cpu << std::endl;
}
