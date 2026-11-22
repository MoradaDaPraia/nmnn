#include "execute.h"
#include "decode.h"
#include "memory.h"
#include "shared.h"
#include "util.h"

static void nop(void) {}

static void executei(state_t *state) {
	instri_t *instri = (instri_t *)state->instr;
	instrish_t instrish;
	uint32_t simm = util_extend_signed(instri->imm, 12);
	uint32_t rs1 = state->regs[instri->rs1];
	uint32_t result = state->regs[instri->rd];

	switch (state->exec) {
	case EXEC_LB:
		result = util_extend_signed(load_byte(state->memory, rs1 + simm), 8);
		break;
	case EXEC_LH:
		result = util_extend_signed(load_half(state->memory, rs1 + simm), 16);
		break;
	case EXEC_LW:
		result = load_word(state->memory, rs1 + simm);
		break;
	case EXEC_LBU:
		result = load_byte(state->memory, rs1 + simm);
		break;
	case EXEC_LHU:
		result = load_half(state->memory, rs1 + simm);
		break;
	case EXEC_FENCE:
		nop();
		break;
	case EXEC_ADDI:
		result = rs1 + simm;
		break;
	case EXEC_SLLI:
		instrish = instri_to_instrish(instri);
		result = rs1 << instrish.shamt;
		break;
	case EXEC_SLTI:
		result = (int32_t)rs1 < (int32_t)simm;
		break;
	case EXEC_SLTIU:
		result = rs1 < simm;
		break;
	case EXEC_XORI:
		result = rs1 ^ simm;
		break;
	case EXEC_SRLI:
		instrish = instri_to_instrish(instri);
		result = rs1 >> instrish.shamt;
		break;
	case EXEC_SRAI:
		instrish = instri_to_instrish(instri);
		result = util_extend_signed(rs1 >> instrish.shamt, 32 - instrish.shamt);
		break;
	case EXEC_ORI:
		result = rs1 | simm;
		break;
	case EXEC_ANDI:
		result = rs1 & simm;
		break;
	case EXEC_JALR:
		result = state->pc + 4;
		state->next_pc = util_clear_bits(rs1 + simm, 1);
		break;
	case EXEC_ECALL:
		nop();
		break;
	case EXEC_EBREAK:
		nop();
		break;
	default:
		break;
	}

	state->regs[instri->rd] = result;
}

static void executes(state_t *state) {
	instrs_t *instrs = (instrs_t *)state->instr;
	uint32_t simm = util_extend_signed(instrs->imm, 12);
	uint32_t address = state->regs[instrs->rs1] + simm;
	uint32_t value = state->regs[instrs->rs2];

	switch (state->exec) {
	case EXEC_SB:
		store_byte(state->memory, address, (uint8_t)value);
		break;
	case EXEC_SH:
		store_half(state->memory, address, (uint16_t)value);
		break;
	case EXEC_SW:
		store_word(state->memory, address, value);
		break;
	default:
		break;
	}
}

static void executeb(state_t *state) {
	instrb_t *instrb = (instrb_t *)state->instr;
	uint32_t simm = util_extend_signed(instrb->imm, 13);
	uint32_t rs1 = state->regs[instrb->rs1];
	uint32_t rs2 = state->regs[instrb->rs2];
	int taken = 0;

	switch (state->exec) {
	case EXEC_BEQ:
		taken = rs1 == rs2;
		break;
	case EXEC_BNE:
		taken = rs1 != rs2;
		break;
	case EXEC_BLT:
		taken = (int32_t)rs1 < (int32_t)rs2;
		break;
	case EXEC_BGE:
		taken = (int32_t)rs1 >= (int32_t)rs2;
		break;
	case EXEC_BLTU:
		taken = rs1 < rs2;
		break;
	case EXEC_BGEU:
		taken = rs1 >= rs2;
		break;
	default:
		break;
	}

	if (taken) {
		state->next_pc = state->pc + simm;
	}
}

static void executer(state_t *state) {
	instrr_t *instrr = (instrr_t *)state->instr;
	uint32_t rs1 = state->regs[instrr->rs1];
	uint32_t rs2 = state->regs[instrr->rs2];
	uint8_t shamt = (uint8_t)(rs2 & 0x1f);
	uint32_t result = state->regs[instrr->rd];

	switch (state->exec) {
	case EXEC_CMOV:
		result = rs2 != 0 ? rs1 : result;
		break;
	case EXEC_ADD:
		result = rs1 + rs2;
		break;
	case EXEC_SUB:
		result = rs1 - rs2;
		break;
	case EXEC_SLL:
		result = rs1 << shamt;
		break;
	case EXEC_SLT:
		result = (int32_t)rs1 < (int32_t)rs2;
		break;
	case EXEC_SLTU:
		result = rs1 < rs2;
		break;
	case EXEC_XOR:
		result = rs1 ^ rs2;
		break;
	case EXEC_SRL:
		result = rs1 >> shamt;
		break;
	case EXEC_SRA:
		result = util_extend_signed(rs1 >> shamt, 32 - shamt);
		break;
	case EXEC_OR:
		result = rs1 | rs2;
		break;
	case EXEC_AND:
		result = rs1 & rs2;
		break;
	default:
		break;
	}

	state->regs[instrr->rd] = result;
}

static void executeu(state_t *state) {
	instru_t *instru = (instru_t *)state->instr;
	uint32_t result = state->regs[instru->rd];

	switch (state->exec) {
	case EXEC_LUI:
		result = instru->imm;
		break;
	case EXEC_AUIPC:
		result = state->pc + instru->imm;
		break;
	default:
		break;
	}

	state->regs[instru->rd] = result;
}

static void executej(state_t *state) {
	instrj_t *instrj = (instrj_t *)state->instr;
	uint32_t simm = util_extend_signed(instrj->imm, 21);
	uint32_t result = state->regs[instrj->rd];

	switch (state->exec) {
	case EXEC_JAL:
		result = state->pc + 4;
		state->next_pc = state->pc + simm;
		break;
	default:
		break;
	}

	state->regs[instrj->rd] = result;
}

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

	state->regs[REG_ZERO] = 0;
}
