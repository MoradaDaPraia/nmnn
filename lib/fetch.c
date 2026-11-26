#include "fetch.h"
#include "memory.h"

void fetch(state_t *state) {
	state->encoded_instr = load_word(state->memory, state->pc);
}
