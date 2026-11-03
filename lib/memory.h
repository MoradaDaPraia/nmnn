#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef struct {
	uint8_t heap[4096];
} leaf_t;

typedef struct __node_t {
	struct __node_t *nodes[16];
	leaf_t **leaves;
} node_t;

typedef struct {
	node_t *head;
} memory_t;

void load_batch(uint32_t address, void *start, uint32_t size);

void store_batch(uint32_t address, void *start, uint32_t size);

#endif
