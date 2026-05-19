#include "helpers.h"

#include "cpu.h"
#include <cstdint>
#include <cstring>
#include <optional>

std::optional<uint16_t *> access_register(CPU &cpu, const char *reg_name) {
  if (strcmp(reg_name, "AX") == 0) {
    return &(cpu.reg.ax);
  } else if (strcmp("BX", reg_name) == 0) {
    return &cpu.reg.bx;
  } else if (strcmp("CX", reg_name) == 0) {
    return &(cpu.reg.cx);
  } else if (strcmp("DX", reg_name) == 0) {
    return &(cpu.reg.dx);
  } else if (strcmp("SP", reg_name) == 0) {
    return &(cpu.reg.sp);
  } else if (strcmp(reg_name, "BP") == 0) {
    return &(cpu.reg.bp);
  } else if (strcmp("CS", reg_name) == 0) {
    return &(cpu.reg.cs);
  } else if (strcmp(reg_name, "DS") == 0) {
    return &(cpu.reg.ds);
  } else if (strcmp("SS", reg_name) == 0) {
    return &(cpu.reg.ss);
  } else if (strcmp(reg_name, "ES") == 0) {
    return &(cpu.reg.es);
  } else if (strcmp(reg_name, "SI") == 0) {
    return &(cpu.reg.si);
  } else if (strcmp(reg_name, "DI") == 0) {
    return &(cpu.reg.di);
  }

  return std::nullopt;
}

void handle_flag(CPU &cpu, const char *flag_name, bool set_bit) {
  uint16_t fl_current = cpu.reg.flag_reg;
  auto fc = [flag_name](const char *flag) {
    return (strcmp(flag_name, flag) == 0);
  };

  uint16_t bm;
  auto sv = [set_bit, &bm](uint16_t v1, uint16_t v2) {
    bm = set_bit ? v1 : v2;
  };

  if (fc("CF")) {
    sv(0x01, 0xFFFE);
  } else if (fc("PF")) {
    sv(0x04, 0xFFFB);
  } else if (fc("AF")) {
    sv(0x0010, 0xFFEF);
  } else if (fc("ZF")) {
    sv(0x0040, 0xFFBF);
  } else if (fc("SF")) {
    sv(0x0080, 0xFF7F);
  } else if (fc("TF")) {
    sv(0x0100, 0xFEFF);
  } else if (fc("IF")) {
    sv(0x0200, 0xFDFF);
  } else if (fc("DF")) {
    sv(0x0400, 0xFBFF);
  } else if (fc("OF")) {
    sv(0x0800, 0xF7FF);
  } else {
    sv(0x0, 0xFFFF);
  }

  if (set_bit) {
    cpu.reg.flag_reg = fl_current | bm;
  } else {
    cpu.reg.flag_reg = fl_current & bm;
  }
}
