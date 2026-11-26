#include "../lib/fetch.h"
#include "../lib/memory.h"
#include "../lib/state.h"
#include <assert.h>

static void test_fetch_advances_and_loads_instruction(void) {
	state_t *state = state_init();
	state->pc = 0x200;
	state->next_pc = 0x204;
	store_word(state->memory, 0x200, 0x00508193);
	store_word(state->memory, 0x204, 0x002081b3);

	fetch(state);
	assert(state->pc == 0x200);
	assert(state->next_pc == 0x204);
	assert(state->encoded_instr == 0x00508193);

	state->pc = 0x204;
	fetch(state);
	assert(state->pc == 0x204);
	assert(state->next_pc == 0x204);
	assert(state->encoded_instr == 0x002081b3);

	state_deinit(&state);
}

int main(void) {
	test_fetch_advances_and_loads_instruction();
	return 0;
}
