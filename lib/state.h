#ifndef STATE_H
#define STATE_H

#include "memory.h"
#include "shared.h"

typedef struct {
	uint32_t encoded_instr;
	instr_t *instr;
	exec_t exec;
	uint32_t regs[32];
	uint32_t pc;
	uint32_t next_pc;
	memory_t *memory;
} state_t;

state_t *state_init(void);

void state_deinit(state_t **state_ptr);

#endif
