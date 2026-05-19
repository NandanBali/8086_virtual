#ifndef HELPERS_H
#define HELPERS_H
#include "cpu.h"
#include <cstdint>
#include <cstring>
#include <optional>

std::optional<uint16_t *> access_register(CPU &cpu, const char *reg_name);

void handle_flag(CPU &cpu, const char *flag_name, bool set_bit);

#endif
