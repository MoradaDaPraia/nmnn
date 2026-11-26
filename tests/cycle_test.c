#include "../lib/cycle.h"
#include "../lib/memory.h"
#include "../lib/state.h"
#include <assert.h>

static void test_cycle_runs_fetch_decode_execute_and_branch(void) {
	state_t *state = state_init();
	store_word(state->memory, 0, 0x00500093);  /* addi x1, x0, 5 */
	store_word(state->memory, 4, 0x00308113);  /* addi x2, x1, 3 */
	store_word(state->memory, 8, 0x00108463);  /* beq x1, x1, 8 */
	store_word(state->memory, 12, 0x06300193); /* addi x3, x0, 99 */
	store_word(state->memory, 16, 0x00700193); /* addi x3, x0, 7 */

	cycle(state);
	assert(state->pc == 0);
	assert(state->next_pc == 4);
	assert(state->regs[1] == 5);

	cycle(state);
	assert(state->pc == 4);
	assert(state->next_pc == 8);
	assert(state->regs[2] == 8);

	cycle(state);
	assert(state->pc == 8);
	assert(state->next_pc == 16);

	cycle(state);
	assert(state->pc == 16);
	assert(state->next_pc == 20);
	assert(state->regs[3] == 7);

	state_deinit(&state);
}

int main(void) {
	test_cycle_runs_fetch_decode_execute_and_branch();
	return 0;
}
