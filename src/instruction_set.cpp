#include "instruction_set.h"
#include "helpers.h"
#include <cstdint>
#include <iostream>
#include <string.h>
uint16_t mov_handle(uint8_t val_src, uint16_t val_reg, bool lower) {
  if (lower) {
    return (val_reg & 0xF0) | (uint16_t)val_src;
  } else {
    return (val_src) | (val_reg & 0x0F);
  }
}

namespace Instructions::MOV {
void direct_16(CPU &cpu, const char *reg, uint16_t value) {
  if (strcmp(reg, "AX") == 0) {
    cpu.reg.ax = value;
  } else if (strcmp(reg, "BX") == 0) {
    cpu.reg.bx = value;
  } else if (strcmp(reg, "CX") == 0) {
    cpu.reg.cx = value;
  } else if (strcmp(reg, "DX") == 0) {
    cpu.reg.dx = value;
  }
}

void direct_8(CPU &cpu, const char *reg, uint8_t value) {
  if (strcmp(reg, "AH") == 0) {
    direct_16(cpu, "AX", mov_handle(value, cpu.reg.ax, false));
  } else if (strcmp(reg, "AL") == 0) {
    direct_16(cpu, "AX", mov_handle(value, cpu.reg.ax, true));
  } else if (strcmp(reg, "BH") == 0) {
    direct_16(cpu, "BX", mov_handle(value, cpu.reg.bx, false));
  } else if (strcmp(reg, "BL") == 0) {
    direct_16(cpu, "BX", mov_handle(value, cpu.reg.bx, true));
  } else if (strcmp(reg, "CH") == 0) {
    direct_16(cpu, "CX", mov_handle(value, cpu.reg.cx, false));
  } else if (strcmp(reg, "CL") == 0) {
    direct_16(cpu, "CX", mov_handle(value, cpu.reg.cx, true));
  } else if (strcmp(reg, "DH") == 0) {
    direct_16(cpu, "DX", mov_handle(value, cpu.reg.dx, false));
  } else if (strcmp(reg, "DL") == 0) {
    direct_16(cpu, "DX", mov_handle(value, cpu.reg.dx, true));
  } else {
    std::cerr << "Invalid move instruction\n";
  }
}
// handles only 16bit to 16bit moves
void reg_to_reg(CPU &cpu, const char *reg_src, const char *reg_dest) {
  const char *const registers_16bit[] = {"AX", "BX", "CX", "DX",  "SI",
                                         "SP", "BP", "DI", "CS",  "IP",
                                         "SS", "DS", "ES", "FLAG"};

  bool reg_src_16 = false;
  bool reg_dest_16 = false;
  for (auto &r : registers_16bit) {
    if (strcmp(reg_src, r) == 0) {
      if (!reg_src_16) {
        reg_src_16 = true;
      }
    }
    if (strcmp(reg_dest, r) == 0) {
      if (!reg_dest_16) {
        reg_dest_16 = true;
      }
    }

    if (reg_dest_16 && reg_src_16) {
      break;
    }
  }

  if (reg_src_16 && reg_dest_16) {
    auto rs = access_register(cpu, reg_src);
    auto rd = access_register(cpu, reg_dest);

    if (rs.has_value() && rd.has_value()) {
      *rd.value() = *rs.value();
    } else {
      std::cerr << "Failed to find appropriate 16 bit register for: " << reg_src
                << " & " << reg_dest << std::endl;
    }
  } else {
    std::cout << "Error parsing registers" << std::endl;
  }
}
} // namespace Instructions::MOV
  //
  //

namespace Instructions::Arithmetic {
void add(CPU &cpu, const char *reg_src, const char *reg_dest) {
  auto rs = access_register(cpu, reg_src);
  auto rd = access_register(cpu, reg_dest);

  if (rs.has_value() && rd.has_value()) {
    // handle flags
    *rd.value() = *rd.value() + *rs.value();
  } else {
    std::cerr << "Invalid registers\n";
  }
}

void add_with_carry(CPU &cpu, const char *reg_src, const char *reg_dest) {
  auto rs = access_register(cpu, reg_src);
  auto rd = access_register(cpu, reg_dest);

  if (rs.has_value() && rd.has_value()) {

    add(cpu, reg_src, reg_dest);
    if ((uint32_t)(*rs.value()) + (uint32_t)(*rd.value()) > UINT16_MAX) {
      auto flags = cpu.reg.flag_reg;
    }
  }
}
} // namespace Instructions::Arithmetic
