#include "../lib/decode.h"
#include "../lib/shared.h"
#include "../lib/state.h"
#include <assert.h>

typedef struct {
	uint32_t encoded;
	exec_t expected_exec;
} decode_case_t;

static void check_decode(state_t *state, const decode_case_t *test) {
	state->encoded_instr = test->encoded;
	decode(state);
	assert(state->exec == test->expected_exec);
}

static void test_decoded_instruction_fields(void) {
	state_t *state = state_init();

	/* I-type: addi x3, x1, 5 */
	state->encoded_instr = 0x00508193;
	decode(state);
	const instri_t *i = (const instri_t *)state->instr;
	assert(i->format == INSTRFORMAT_I);
	assert(i->opcode == 0x13);
	assert(i->funct3 == 0);
	assert(i->rs1 == 1);
	assert(i->rd == 3);
	assert(i->imm == 5);

	/* S-type: sw x2, 5(x1) */
	state->encoded_instr = 0x0020a2a3;
	decode(state);
	const instrs_t *s = (const instrs_t *)state->instr;
	assert(s->format == INSTRFORMAT_S);
	assert(s->opcode == 0x23);
	assert(s->funct3 == 2);
	assert(s->rs1 == 1);
	assert(s->rs2 == 2);
	assert(s->imm == 5);

	/* B-type: beq x1, x2, 8 */
	state->encoded_instr = 0x00208463;
	decode(state);
	const instrb_t *b = (const instrb_t *)state->instr;
	assert(b->format == INSTRFORMAT_B);
	assert(b->opcode == 0x63);
	assert(b->funct3 == 0);
	assert(b->rs1 == 1);
	assert(b->rs2 == 2);
	assert(b->imm == 8);

	/* R-type: sub x3, x1, x2 */
	state->encoded_instr = 0x402081b3;
	decode(state);
	const instrr_t *r = (const instrr_t *)state->instr;
	assert(r->format == INSTRFORMAT_R);
	assert(r->opcode == 0x33);
	assert(r->funct3 == 0);
	assert(r->funct7 == 0x20);
	assert(r->rs1 == 1);
	assert(r->rs2 == 2);
	assert(r->rd == 3);

	/* Custom R-type: cmov x3, x1, x2 */
	state->encoded_instr = 0x0020818b;
	decode(state);
	r = (const instrr_t *)state->instr;
	assert(r->format == INSTRFORMAT_R);
	assert(r->opcode == 0x0b);
	assert(r->funct3 == 0);
	assert(r->funct7 == 0);
	assert(r->rs1 == 1);
	assert(r->rs2 == 2);
	assert(r->rd == 3);

	/* U-type: lui x3, 0x12345 */
	state->encoded_instr = 0x123451b7;
	decode(state);
	const instru_t *u = (const instru_t *)state->instr;
	assert(u->format == INSTRFORMAT_U);
	assert(u->opcode == 0x37);
	assert(u->rd == 3);
	assert(u->imm == 0x12345000);

	/* J-type: jal x3, 8 */
	state->encoded_instr = 0x008001ef;
	decode(state);
	const instrj_t *j = (const instrj_t *)state->instr;
	assert(j->format == INSTRFORMAT_J);
	assert(j->opcode == 0x6f);
	assert(j->rd == 3);
	assert(j->imm == 8);

	state_deinit(&state);
}

static void test_instri_to_instrish(void) {
	const instri_t instri = {
	    .format = INSTRFORMAT_I,
	    .opcode = 0x13,
	    .funct3 = 5,
	    .rs1 = 1,
	    .rd = 3,
	    .imm = 0x405,
	};
	instrish_t instrish = instri_to_instrish(&instri);

	assert(instrish.format == INSTRFORMAT_I);
	assert(instrish.opcode == 0x13);
	assert(instrish.funct3 == 5);
	assert(instrish.rs1 == 1);
	assert(instrish.rd == 3);
	assert(instrish.shamt == 5);
	assert(instrish.imm == 0x20);
}

static void test_decode_exec_selection(void) {
	state_t *state = state_init();
	const decode_case_t cases[] = {
	    {0x00508183, EXEC_LB},     /* lb x3, 5(x1) */
	    {0x00509183, EXEC_LH},     /* lh x3, 5(x1) */
	    {0x0050a183, EXEC_LW},     /* lw x3, 5(x1) */
	    {0x0050c183, EXEC_LBU},    /* lbu x3, 5(x1) */
	    {0x0050d183, EXEC_LHU},    /* lhu x3, 5(x1) */
	    {0x0000818f, EXEC_FENCE},  /* fence */
	    {0x00508193, EXEC_ADDI},   /* addi x3, x1, 5 */
	    {0x00509193, EXEC_SLLI},   /* slli x3, x1, 5 */
	    {0x0050a193, EXEC_SLTI},   /* slti x3, x1, 5 */
	    {0x0050b193, EXEC_SLTIU},  /* sltiu x3, x1, 5 */
	    {0x0050c193, EXEC_XORI},   /* xori x3, x1, 5 */
	    {0x0050d193, EXEC_SRLI},   /* srli x3, x1, 5 */
	    {0x4050d193, EXEC_SRAI},   /* srai x3, x1, 5 */
	    {0x0050e193, EXEC_ORI},    /* ori x3, x1, 5 */
	    {0x0050f193, EXEC_ANDI},   /* andi x3, x1, 5 */
	    {0x005081e7, EXEC_JALR},   /* jalr x3, 5(x1) */
	    {0x000081f3, EXEC_ECALL},  /* ecall */
	    {0x001081f3, EXEC_EBREAK}, /* ebreak */
	    {0x002082a3, EXEC_SB},     /* sb x2, 5(x1) */
	    {0x002092a3, EXEC_SH},     /* sh x2, 5(x1) */
	    {0x0020a2a3, EXEC_SW},     /* sw x2, 5(x1) */
	    {0x00208463, EXEC_BEQ},    /* beq x1, x2, 8 */
	    {0x00209463, EXEC_BNE},    /* bne x1, x2, 8 */
	    {0x0020c463, EXEC_BLT},    /* blt x1, x2, 8 */
	    {0x0020d463, EXEC_BGE},    /* bge x1, x2, 8 */
	    {0x0020e463, EXEC_BLTU},   /* bltu x1, x2, 8 */
	    {0x0020f463, EXEC_BGEU},   /* bgeu x1, x2, 8 */
	    {0x002081b3, EXEC_ADD},    /* add x3, x1, x2 */
	    {0x402081b3, EXEC_SUB},    /* sub x3, x1, x2 */
	    {0x002091b3, EXEC_SLL},    /* sll x3, x1, x2 */
	    {0x0020a1b3, EXEC_SLT},    /* slt x3, x1, x2 */
	    {0x0020b1b3, EXEC_SLTU},   /* sltu x3, x1, x2 */
	    {0x0020c1b3, EXEC_XOR},    /* xor x3, x1, x2 */
	    {0x0020d1b3, EXEC_SRL},    /* srl x3, x1, x2 */
	    {0x4020d1b3, EXEC_SRA},    /* sra x3, x1, x2 */
	    {0x0020e1b3, EXEC_OR},     /* or x3, x1, x2 */
	    {0x0020f1b3, EXEC_AND},    /* and x3, x1, x2 */
	    {0x0020818b, EXEC_CMOV},   /* custom cmov x3, x1, x2 */
	    {0x123451b7, EXEC_LUI},    /* lui x3, 0x12345 */
	    {0x12345197, EXEC_AUIPC},  /* auipc x3, 0x12345 */
	    {0x008001ef, EXEC_JAL},    /* jal x3, 8 */
	    {0x0050b183, 0},           /* reserved load funct3 */
	    {0x20509193, 0},           /* invalid slli funct7 */
	    {0x6050d193, 0},           /* invalid srli/srai funct7 */
	    {0x005091e7, 0},           /* invalid jalr funct3 */
	    {0x0020b2a3, 0},           /* reserved store funct3 */
	    {0x0020a463, 0},           /* reserved branch funct3 */
	    {0x022091b3, 0},           /* invalid sll funct7 */
	};

	for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
		check_decode(state, &cases[i]);
	}
	state_deinit(&state);
}

int main(void) {
	test_decode_exec_selection();
	test_decoded_instruction_fields();
	test_instri_to_instrish();
	return 0;
}
