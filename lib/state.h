#ifndef STATE_H
#define STATE_H

#include "constants.h"

typedef struct {
	uint32_t encoded_instr;
	instr_t *instr;
	exec_t exec;
} state_t;

#endif
