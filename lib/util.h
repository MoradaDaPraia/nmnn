#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

uint32_t util_extract(uint32_t src, uint8_t ms, uint8_t ls);

uint32_t util_extend_signed(uint32_t value, uint8_t bits);

#endif
