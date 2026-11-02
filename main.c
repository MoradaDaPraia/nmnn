#include "lib/decode.h"
#include <stdio.h>

int main(void) {
	uint8_t buf[64];

	instri_t *instri = (instri_t *)instr_decode(0x00008067, buf, sizeof(buf));

	printf("%d\n", instri->format);
	printf("%2x\n", instri->opcode);
	printf("%d\n", instri->funct3);
	printf("%d\n", instri->rs1);
	printf("%d\n", instri->rd);
	printf("%d\n", instri->imm);

	return 0;
}
