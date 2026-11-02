#include "lib/decode.h"
#include "lib/state.h"
#include <stdio.h>

int main(void) {
	state_t *state = state_init();
	state->encoded_instr = 0x00008067;

	decode(state);

	instri_t *instri = (instri_t *)state->instr;

	printf("%d\n", instri->format);
	printf("%2x\n", instri->opcode);
	printf("%d\n", instri->funct3);
	printf("%d\n", instri->rs1);
	printf("%d\n", instri->rd);
	printf("%d\n", instri->imm);

	state_deinit(&state);

	return 0;
}
