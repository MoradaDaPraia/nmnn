#include "util.h"

inline uint32_t util_extract(uint32_t src, uint8_t ms, uint8_t ls) {
	if (ms > 31 || ls > 31 || ms < ls) {
		return 0;
	}

	uint32_t mask = 0xffffffff >> ((sizeof(uint32_t) * 8) - (ms + 1 - ls));

	return (src >> ls) & mask;
}
