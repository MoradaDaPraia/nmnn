#include "lib/decode.h"
#include "lib/state.h"
#include "lib/util.h"
#include <stdio.h>

int main(void) {
	state_t *state = state_init();
	state->encoded_instr = 0x01010413;

	decode(state);

	instri_t *instri = (instri_t *)state->instr;

	printf("Instrução: ADDI x%u, x%u, %d\n", (unsigned)instri->rd,
	       (unsigned)instri->rs1, (int)util_extend_signed(instri->imm, 12));
	printf("Tipo: I (INSTRFORMAT_I = %u)\n", (unsigned)instri->format);
	printf("Formato decodificado: opcode=0x%02X, rd=x%u, funct3=%u, rs1=x%u, ",
	       (unsigned)instri->opcode, (unsigned)instri->rd,
	       (unsigned)instri->funct3, (unsigned)instri->rs1);
	printf("imm=%d\n", (int)util_extend_signed(instri->imm, 12));

	state_deinit(&state);

	return 0;
}
