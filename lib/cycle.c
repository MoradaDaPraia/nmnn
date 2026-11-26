#include "cycle.h"
#include "decode.h"
#include "execute.h"
#include "fetch.h"

void cycle_advance(state_t *state) {
	state->pc = state->next_pc;
	state->next_pc = state->pc + 4;
}

void cycle(state_t *state) {
	cycle_advance(state);
	fetch(state);
	decode(state);
	execute(state);
}
