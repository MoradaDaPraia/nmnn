#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct {
	uint32_t *data;
	uint32_t size;
} memory_t;

memory_t *memory_init(void);

void memory_deinit(memory_t **memory_ptr);

void load_batch(memory_t *memory, uint32_t address, void *start, uint32_t size);

void store_batch(memory_t *memory, uint32_t address, void *start, uint32_t size);

uint8_t load_byte(memory_t *memory, uint32_t address);

void store_byte(memory_t *memory, uint32_t address, uint8_t byte);

uint16_t load_half(memory_t *memory, uint32_t address);

void store_half(memory_t *memory, uint32_t address, uint16_t half);

uint32_t load_word(memory_t *memory, uint32_t address);

void store_word(memory_t *memory, uint32_t address, uint32_t word);

#endif
