#include "execute.h"
#include "decode.h"
#include "shared.h"
#include "util.h"

static void executei(state_t *state) {
	instri_t *instri = (instri_t *)state->instr;
	uint32_t simm = util_extend_signed(instri->imm, 12);
	uint32_t uimm = instri->imm;

	switch (state->exec) {
	case EXEC_LB:

		break;
	default:
		break;
	}
}

static void executes(state_t *state) {}

static void executeb(state_t *state) {}

static void executer(state_t *state) {}

static void executeu(state_t *state) {}

static void executej(state_t *state) {}

void execute(state_t *state) {
	state->regs[REG_ZERO] = 0;

	switch (state->instr->format) {
	case INSTRFORMAT_I:
		executei(state);
		break;
	case INSTRFORMAT_S:
		executes(state);
		break;
	case INSTRFORMAT_B:
		executeb(state);
		break;
	case INSTRFORMAT_R:
		executer(state);
		break;
	case INSTRFORMAT_U:
		executeu(state);
		break;
	case INSTRFORMAT_J:
		executej(state);
		break;
	default:
		break;
	}
}
