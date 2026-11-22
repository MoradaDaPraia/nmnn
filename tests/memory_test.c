#include "../lib/memory.h"
#include <assert.h>
#include <string.h>

void test_memory_init(void) {
	memory_t *memory = memory_init();
	assert(memory != NULL);
	assert(memory->data != NULL);
	assert(memory->size == 0xA0000);
	assert(load_byte(memory, 0) == 0);
	memory_deinit(&memory);
}

void test_memory_deinit(void) {
	memory_t *memory = memory_init();
	memory_deinit(&memory);
	assert(memory == NULL);
	memory_deinit(&memory);
	memory_deinit(NULL);
}

void test_load_batch(void) {
	memory_t *memory = memory_init();
	uint8_t source[] = {0x12, 0x34, 0x56, 0x78};
	uint8_t result[sizeof(source)] = {0};
	store_batch(memory, 0x100, source, sizeof(source));
	load_batch(memory, 0x100, result, sizeof(result));
	assert(memcmp(source, result, sizeof(source)) == 0);

	uint8_t wrapped_source[] = {0xAA, 0xBB, 0xCC, 0xDD};
	store_batch(memory, memory->size - 2, wrapped_source,
	            sizeof(wrapped_source));
	memset(result, 0, sizeof(result));
	load_batch(memory, memory->size - 2, result, sizeof(result));
	assert(memcmp(wrapped_source, result, sizeof(wrapped_source)) == 0);
	memory_deinit(&memory);
}

void test_store_batch(void) {
	memory_t *memory = memory_init();
	uint8_t source[] = {0xAB, 0xCD, 0xEF};
	uint8_t result[sizeof(source)] = {0};
	store_batch(memory, 0x200, source, sizeof(source));
	load_batch(memory, 0x200, result, sizeof(result));
	assert(memcmp(source, result, sizeof(source)) == 0);

	store_batch(memory, memory->size - 2, source, sizeof(source));
	memset(result, 0, sizeof(result));
	load_batch(memory, memory->size - 2, result, 2);
	assert(result[0] == source[0] && result[1] == source[1]);
	uint8_t wrapped_byte = 0;
	load_batch(memory, 0, &wrapped_byte, 1);
	assert(wrapped_byte == source[2]);
	memory_deinit(&memory);
}

void test_batch_larger_than_memory(void) {
	uint32_t data[1] = {0x44332211};
	memory_t memory = {.data = data, .size = sizeof(data)};
	uint8_t source[sizeof(data) + 1] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
	uint8_t result[sizeof(source)];
	memset(result, 0xFF, sizeof(result));

	load_batch(&memory, 0, result, sizeof(result));
	for (uint32_t i = 0; i < sizeof(result); i++) {
		assert(result[i] == 0xFF);
	}

	store_batch(&memory, 0, source, sizeof(source));
	assert(data[0] == 0x44332211);
}

void test_load_byte(void) {
	memory_t *memory = memory_init();
	store_byte(memory, 0x10, 0xA5);
	assert(load_byte(memory, 0x10) == 0xA5);
	assert(load_byte(memory, memory->size) == 0);
	memory_deinit(&memory);
}

void test_store_byte(void) {
	memory_t *memory = memory_init();
	store_byte(memory, 0x20, 0x5A);
	assert(load_byte(memory, 0x20) == 0x5A);
	store_byte(memory, memory->size, 0xFF);
	assert(load_byte(memory, memory->size - 1) == 0);
	memory_deinit(&memory);
}

void test_load_half(void) {
	memory_t *memory = memory_init();
	uint16_t expected = 0xB2A1;
	uint8_t bytes[sizeof(expected)];
	memcpy(bytes, &expected, sizeof(expected));
	store_batch(memory, 0x30, bytes, sizeof(bytes));
	assert(load_half(memory, 0x30) == expected);
	assert(load_half(memory, memory->size - 1) == 0);
	memory_deinit(&memory);
}

void test_store_half(void) {
	memory_t *memory = memory_init();
	uint8_t bytes[2] = {0};
	uint16_t expected = 0xA1B2;
	uint8_t expected_bytes[sizeof(expected)];
	memcpy(expected_bytes, &expected, sizeof(expected));
	store_half(memory, 0x40, expected);
	load_batch(memory, 0x40, bytes, sizeof(bytes));
	assert(memcmp(bytes, expected_bytes, sizeof(bytes)) == 0);
	assert(load_half(memory, 0x40) == expected);
	memory_deinit(&memory);
}

void test_load_word(void) {
	memory_t *memory = memory_init();
	uint32_t expected = 0x78563412;
	uint8_t bytes[sizeof(expected)];
	memcpy(bytes, &expected, sizeof(expected));
	store_batch(memory, 0x50, bytes, sizeof(bytes));
	assert(load_word(memory, 0x50) == expected);
	assert(load_word(memory, memory->size - 3) == 0);
	memory_deinit(&memory);
}

void test_store_word(void) {
	memory_t *memory = memory_init();
	uint8_t bytes[4] = {0};
	uint32_t expected = 0x12345678;
	uint8_t expected_bytes[sizeof(expected)];
	memcpy(expected_bytes, &expected, sizeof(expected));
	store_word(memory, 0x60, expected);
	load_batch(memory, 0x60, bytes, sizeof(bytes));
	assert(memcmp(bytes, expected_bytes, sizeof(bytes)) == 0);
	assert(load_word(memory, 0x60) == expected);
	memory_deinit(&memory);
}

int main(void) {
	test_memory_init();
	test_memory_deinit();
	test_load_batch();
	test_store_batch();
	test_batch_larger_than_memory();
	test_load_byte();
	test_store_byte();
	test_load_half();
	test_store_half();
	test_load_word();
	test_store_word();
	return 0;
}
