#ifndef STACK_H
#define STACK_H

// A uint8_t stack
#include <cstdint>
#include <iostream>
class CPUStack {
public:
  CPUStack(uint16_t size) {
    // heap allocate
    this->stack = new uint8_t[size + 1];
    this->size = size;
  }

  ~CPUStack() { delete this->stack; }

  uint16_t push_byte_at(uint8_t val, uint16_t index) {
    if (index > size) {
      std::cerr << "Failed to push byte at " << val
                << ": Index required exceeds size\n";
      return index;
    } else {
      this->stack[index - 1] = val;
      return index - 1;
    }
  }

  uint8_t pop_byte_at(uint16_t *index) {
    auto val = this->stack[*index];
    index -= 1;
    return val;
  }

private:
  uint8_t *stack;
  uint16_t size;
};

#endif // !STACK_H
