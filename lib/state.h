#ifndef STATE_H
#define STATE_H

#include "shared.h"

typedef struct {
	uint32_t encoded_instr;
	instr_t *instr;
	exec_t exec;
} state_t;

state_t *state_init(void);

void state_deinit(state_t **state_ptr);

#endif
