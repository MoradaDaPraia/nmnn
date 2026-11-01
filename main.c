#include "lib/decode.h"
#include <stdio.h>

int main(void) {
	uint8_t buf[64];

	instr_t *instr = instr_decode(0x00008067, buf, sizeof(buf));

	printf("%d\n", instr->format);

	return 0;
}
