#include "../lib/util.h"
#include <assert.h>

void test_util_extract(void) {
	assert(util_extract(0xD6, 6, 2) == 0x15);
	assert(util_extract(0xFFFFFFFF, 31, 0) == 0xFFFFFFFF);
	assert(util_extract(0xFFFFFFFF, 3, 4) == 0);
	assert(util_extract(0xFFFFFFFF, 32, 0) == 0);
}

void test_util_extend_signed(void) {
	assert(util_extend_signed(0x7FF, 12) == 0x000007FF);
	assert(util_extend_signed(0x800, 12) == 0xFFFFF800);
	assert(util_extend_signed(0xFFF, 12) == 0xFFFFFFFF);
}

void test_util_clear_bits(void) {
	assert(util_clear_bits(0xFFFFFFFF, 4) == 0xFFFFFFF0);
	assert(util_clear_bits(0x12345678, 0) == 0x12345678);
	assert(util_clear_bits(0xFFFFFFFF, 32) == 0);
	assert(util_clear_bits(0xFFFFFFFF, 40) == 0);
}

int main(void) {
	test_util_extract();
	test_util_extend_signed();
	test_util_clear_bits();
	return 0;
}
