#include "cpu.h"
#include "instruction_set.h"
int main() {
  CPU cpu{};
  Instructions::MOV::direct_16(cpu, "AX", 0xFFBA);
  Instructions::MOV::direct_16(cpu, "BX", 0x00FF);
  std::cout << "before executing reg2reg " << cpu << std::endl;
  Instructions::MOV::reg_to_reg(cpu, "AX", "BX");
  std::cout << "after reg2reg" << cpu << std::endl;
}
