#include "memory.h"
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE ((uint32_t)0xA0000)

memory_t *memory_init(void) {
	memory_t *memory = malloc(sizeof(memory_t));
	memory->size = MEMORY_SIZE;
	memory->data = malloc(memory->size);
	memset(memory->data, 0, memory->size);
	return memory;
}

void memory_deinit(memory_t **memory_ptr) {
	if (memory_ptr == NULL || *memory_ptr == NULL) {
		return;
	}

	free((*memory_ptr)->data);
	free(*memory_ptr);
	*memory_ptr = NULL;
}

void load_batch(memory_t *memory, uint32_t address, void *start,
                uint32_t size) {
	if (start == NULL || size == 0 || memory == NULL || memory->data == NULL ||
	    memory->size == 0 || size > memory->size) {
		return;
	}

	uint8_t *source = (uint8_t *)memory->data;
	uint32_t offset = address % memory->size;
	if (size <= memory->size - offset) {
		memcpy(start, source + offset, size);
		return;
	}

	uint32_t first_size = memory->size - offset;
	uint32_t wrapped_size = size - first_size;
	memcpy(start, source + offset, first_size);
	memcpy((uint8_t *)start + first_size, source, wrapped_size);
}

void store_batch(memory_t *memory, uint32_t address, void *start,
                 uint32_t size) {
	if (memory == NULL || memory->data == NULL || memory->size == 0 ||
	    start == NULL || size == 0 || size > memory->size) {
		return;
	}

	uint8_t *destination = (uint8_t *)memory->data;
	uint32_t offset = address % memory->size;
	if (size <= memory->size - offset) {
		memcpy(destination + offset, start, size);
		return;
	}

	uint32_t first_size = memory->size - offset;
	uint32_t wrapped_size = size - first_size;
	memcpy(destination + offset, start, first_size);
	memcpy(destination, (uint8_t *)start + first_size, wrapped_size);
}

uint8_t load_byte(memory_t *memory, uint32_t address) {
	uint8_t byte = 0;
	load_batch(memory, address, &byte, sizeof(byte));
	return byte;
}

void store_byte(memory_t *memory, uint32_t address, uint8_t byte) {
	store_batch(memory, address, &byte, sizeof(byte));
}

uint16_t load_half(memory_t *memory, uint32_t address) {
	uint16_t half = 0;
	load_batch(memory, address, &half, sizeof(half));
	return half;
}

void store_half(memory_t *memory, uint32_t address, uint16_t half) {
	store_batch(memory, address, &half, sizeof(half));
}

uint32_t load_word(memory_t *memory, uint32_t address) {
	uint32_t word = 0;
	load_batch(memory, address, &word, sizeof(word));
	return word;
}

void store_word(memory_t *memory, uint32_t address, uint32_t word) {
	store_batch(memory, address, &word, sizeof(word));
}
