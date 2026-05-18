#ifndef CPU_H
#define CPU_H

#include "stack.h"
#include <cstdint>
#include <ostream>

struct Registers {
  // General registers
  uint16_t ax;
  uint16_t bx;
  uint16_t cx;
  uint16_t dx;

  // Segment registers
  uint16_t cs;
  uint16_t ds;
  uint16_t ss;
  uint16_t es;

  // Index registers
  uint16_t si;
  uint16_t di;

  // Base pointers
  uint16_t sp;
  uint16_t bp;

  uint16_t ip;
  uint16_t flag_reg;
};

class CPU {
public:
  Registers reg;

  CPUStack *stack;

  uint8_t *memory;

  CPU() {

    this->stack = new CPUStack(UINT16_MAX);
    this->memory = new uint8_t[1024 * 1024];
    // by default we split the memory into 4 equally sized segments
    this->reg.cs = 0;
    this->reg.ds = (uint16_t)(1024 * 256 >> 16);
    this->reg.ss = (uint16_t)(1024 * 512 >> 16);
    this->reg.es = (uint16_t)(1024 * 768 >> 16);
  }

  ~CPU() {
    delete stack;
    delete memory;
  }

  friend std::ostream &operator<<(std::ostream &os, const CPU &cpu) {
    os << "AX: " << cpu.reg.ax << "\nBX: " << cpu.reg.bx
       << "\nCX: " << cpu.reg.cx << "\nDX: " << cpu.reg.dx
       << "\nSS: " << cpu.reg.ss;

    return os;
  }
};

#endif // !CPU_H
