#include "state.h"
#include "decode.h"
#include <stdlib.h>
#include <string.h>

state_t *state_init(void) {
	state_t *state = malloc(sizeof(state_t));

	state->memory = memory_init();
	state->encoded_instr = 0;
	state->instr = NULL;
	state->exec = 0;
	state->pc = 0;
	state->next_pc = 0;
	state->text_end = 0;
	memset(state->regs, 0, sizeof(state->regs));

	return state;
}

void state_load_text(state_t *state, const uint32_t *text, uint32_t word_count) {
	if (state == NULL || state->memory == NULL || text == NULL ||
	    word_count == 0 || word_count > state->memory->size / sizeof(*text)) {
		return;
	}

	for (uint32_t i = 0; i < word_count; i++) {
		store_word(state->memory, i * sizeof(*text), text[i]);
	}

	state->text_end = (word_count - 1) * sizeof(*text);
}

void state_deinit(state_t **state_ptr) {
	if (state_ptr == NULL || *state_ptr == NULL) {
		return;
	}

	state_t *state = *state_ptr;

	instr_deinit(&(state->instr));
	memory_deinit(&(state->memory));

	free(state);
	*state_ptr = NULL;
}
