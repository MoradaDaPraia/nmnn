#include "state.h"
#include "decode.h"
#include <stdlib.h>
#include <string.h>

state_t *state_init(void) {
	state_t *state = malloc(sizeof(state_t));

	state->encoded_instr = 0;
	state->instr = NULL;
	state->exec = 0;
	memset(state->regs, 0, sizeof(state->regs));

	return state;
}

void state_deinit(state_t **state_ptr) {
	if (state_ptr == NULL || *state_ptr == NULL) {
		return;
	}

	state_t *state = *state_ptr;

	instr_deinit(&(state->instr));

	free(state);
	*state_ptr = NULL;
}
