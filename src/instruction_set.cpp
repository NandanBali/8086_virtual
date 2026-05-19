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
  cpu.load_instr([&cpu, reg, value]() {
    if (strcmp(reg, "AX") == 0) {
      cpu.reg.ax = value;
    } else if (strcmp(reg, "BX") == 0) {
      cpu.reg.bx = value;
    } else if (strcmp(reg, "CX") == 0) {
      cpu.reg.cx = value;
    } else if (strcmp(reg, "DX") == 0) {
      cpu.reg.dx = value;
    }
  });
}

void direct_8(CPU &cpu, const char *reg, uint8_t value) {
  cpu.load_instr([&cpu, reg, value]() {
    auto rd = access_register(cpu, "AX");
    (void)rd;
    if (strcmp(reg, "AH") == 0) {
      cpu.reg.ax = mov_handle(value, cpu.reg.ax, false);
    } else if (strcmp(reg, "AL") == 0) {
      cpu.reg.ax = mov_handle(value, cpu.reg.ax, true);
    } else if (strcmp(reg, "BH") == 0) {
      cpu.reg.bx = mov_handle(value, cpu.reg.bx, false);
    } else if (strcmp(reg, "BL") == 0) {
      cpu.reg.bx = mov_handle(value, cpu.reg.bx, true);
    } else if (strcmp(reg, "CH") == 0) {
      cpu.reg.cx = mov_handle(value, cpu.reg.cx, false);
    } else if (strcmp(reg, "CL") == 0) {
      cpu.reg.cx = mov_handle(value, cpu.reg.cx, true);
    } else if (strcmp(reg, "DH") == 0) {
      cpu.reg.dx = mov_handle(value, cpu.reg.dx, false);
    } else if (strcmp(reg, "DL") == 0) {
      cpu.reg.dx = mov_handle(value, cpu.reg.dx, true);
    } else {
      std::cerr << "Invalid move instruction\n";
    }
  });
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
    cpu.load_instr([&cpu, reg_src, reg_dest]() {
      auto rs = access_register(cpu, reg_src);
      auto rd = access_register(cpu, reg_dest);

      if (rs.has_value() && rd.has_value()) {
        *rd.value() = *rs.value();
      } else {
        std::cerr << "Failed to find appropriate 16 bit register for: "
                  << reg_src << " & " << reg_dest << std::endl;
      }
    });
  } else {
    std::cout << "Error parsing registers" << std::endl;
  }
}
} // namespace Instructions::MOV
  //
  //

namespace Instructions::Arithmetic {
void add_reg_to_reg(CPU &cpu, const char *reg_src, const char *reg_dest) {
  cpu.load_instr([&cpu, reg_src, reg_dest]() {
    auto rs = access_register(cpu, reg_src);
    auto rd = access_register(cpu, reg_dest);

    if (rs.has_value() && rd.has_value()) {
      uint32_t res = (uint32_t)(*rd.value()) + (uint32_t)(*rs.value());
      handle_flag(cpu, "ZF", (res == 0));
      handle_flag(cpu, "SF", (((uint16_t)res & 0x8000) == 1));
      handle_flag(cpu, "CF", (res > 0xffff));
      handle_flag(cpu, "OF",
                  ((~(*rd.value() ^ *rs.value()) &
                    (*rd.value() ^ (uint16_t)res) & 0x8000) != 0));

      handle_flag(cpu, "AF", (*rs.value() ^ *rd.value() ^ (uint16_t)(res)));
      handle_flag(cpu, "PF", ((uint16_t)res & 0xFF));
      *rd.value() = (uint16_t)res;
    } else {
      std::cerr << "Invalid registers\n";
    }
  });
}

void add_with_carry(CPU &cpu, const char *reg_src, const char *reg_dest) {
  cpu.load_instr([&cpu, reg_src, reg_dest]() {
    if (cpu.reg.flag_reg & 0x0001) {
      auto rs = access_register(cpu, reg_src);
      auto rd = access_register(cpu, reg_dest);
      if (rs.has_value() && rd.has_value()) {
        *rd.value() = *rd.value() + *rs.value() + 1;
      }
    }
  });
}

void add_with_val(CPU &cpu, const char *reg_dest, uint16_t value) {
  cpu.load_instr([&cpu, reg_dest, value]() {
    auto rs = access_register(cpu, reg_dest);
    if (rs.has_value()) {
      uint32_t res = (uint32_t)(*rs.value()) + (uint32_t)(value);
      handle_flag(cpu, "ZF", (res == 0));
      handle_flag(cpu, "SF", (((uint16_t)res & 0x8000) == 1));
      handle_flag(cpu, "CF", (res > 0xffff));
      handle_flag(
          cpu, "OF",
          ((~(value ^ *rs.value()) & (value ^ (uint16_t)res) & 0x8000) != 0));

      handle_flag(cpu, "AF", (*rs.value() ^ value ^ (uint16_t)(res)));
      handle_flag(cpu, "PF", ((uint16_t)res & 0xFF));
      *rs.value() = (uint16_t)res;
    } else {
      std::cerr << "Invalid register\n";
    }
  });
}

void sub_with_val(CPU &cpu, const char *reg_dest, uint16_t value) {
  cpu.load_instr([&cpu, reg_dest, value]() {
    auto rd = access_register(cpu, reg_dest);
    if (rd.has_value()) {
      uint16_t res = *rd.value() - value;
      handle_flag(cpu, "ZF", *rd.value() == value);
      handle_flag(cpu, "CF", *rd.value() < value);
      handle_flag(cpu, "SF", res & 0x8000);

      *rd.value() = res;
    } else {
      std::cout << "Invalid register name\n";
    }
  });
}
} // namespace Instructions::Arithmetic
