#include "cpu.h"
#include "instruction_set.h"
int main() {
  CPU cpu{};
  cpu.load_instr([&cpu]() { Instructions::MOV::direct_16(cpu, "AX", 0xFFBA); });
  //  cpu.load_instr([&cpu]() { Instructions::MOV::direct_16(cpu, "AX", 0xFF00);
  //  });
  cpu.load_instr([&cpu]() { Instructions::MOV::direct_16(cpu, "BX", 0x00FF); });
  cpu.load_instr([&cpu]() { Instructions::MOV::reg_to_reg(cpu, "AX", "BX"); });
  std::cout << "after reg2reg\n" << cpu << std::endl;
  cpu.load_instr([&cpu]() { Instructions::MOV::direct_16(cpu, "CX", 0x01); });
  cpu.load_instr([&cpu]() { Instructions::MOV::direct_16(cpu, "DX", 0x03); });
  cpu.load_instr(
      [&cpu]() { Instructions::Arithmetic::add_reg_to_reg(cpu, "CX", "DX"); });
  cpu.load_instr(
      [&cpu]() { Instructions::Arithmetic::add_with_val(cpu, "DX", 12); });
  std::cout << "after adding\n" << cpu << std::endl;
  cpu.exec_instr();
}
