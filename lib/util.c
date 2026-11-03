#include "util.h"

inline uint32_t util_extract(uint32_t src, uint8_t ms, uint8_t ls) {
	if (ms > 31 || ls > 31 || ms < ls) {
		return 0;
	}

	uint32_t mask = 0xffffffff >> ((sizeof(uint32_t) * 8) - (ms + 1 - ls));

	return (src >> ls) & mask;
}

uint32_t util_extend_signed(uint32_t value, uint8_t bits) {
	uint32_t sign = 1u << (bits - 1);
	uint32_t mask = UINT32_MAX >> (32 - bits);

	value &= mask;

	return (value ^ sign) - sign;
}
