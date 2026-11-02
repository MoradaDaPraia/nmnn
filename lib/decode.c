#include "decode.h"
#include "state.h"
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

static instrformat_t instrformat_from_opcode(opcode_t opcode) {
	instrformat_t instrformat = 0;
	switch (opcode) {
	case OPCODEI_LOAD:
	case OPCODEI_FENCE:
	case OPCODEI_IMM:
	case OPCODEI_JALR:
	case OPCODEI_SYSTEM:
		instrformat = INSTRFORMAT_I;
		break;
	case OPCODES_STORE:
		instrformat = INSTRFORMAT_S;
		break;
	case OPCODEB_BRANCH:
		instrformat = INSTRFORMAT_B;
		break;
	case OPCODER_REG:
		instrformat = INSTRFORMAT_R;
		break;
	case OPCODEU_AUIPC:
	case OPCODEU_LUI:
		instrformat = INSTRFORMAT_U;
		break;
	case OPCODEJ_JAL:
		instrformat = INSTRFORMAT_J;
		break;
	default:
		break;
	}
	return instrformat;
}

static opcode_t opcode_from_encoded(uint32_t encoded) {
	return (opcode_t)util_extract(encoded, 6, 0);
}

static instri_t *instri_decode(opcode_t opcode, uint32_t encoded) {
	instri_t *instri = malloc(sizeof(instri_t));

	instri->format = INSTRFORMAT_I;
	instri->opcode = opcode;
	instri->funct3 = (funct_t)util_extract(encoded, 14, 12);
	instri->rs1 = (reg_t)util_extract(encoded, 19, 15);
	instri->rd = (reg_t)util_extract(encoded, 11, 7);
	instri->imm = (imm_t)util_extract(encoded, 31, 20);

	return instri;
}

static instrs_t *instrs_decode(opcode_t opcode, uint32_t encoded) {

	instrs_t *instrs = malloc(sizeof(instrs_t));

	instrs->format = INSTRFORMAT_I;
	instrs->opcode = opcode;
	instrs->funct3 = (funct_t)util_extract(encoded, 14, 12);
	instrs->rs1 = (reg_t)util_extract(encoded, 19, 15);
	instrs->rs2 = (reg_t)util_extract(encoded, 24, 20);
	imm_t imm1 = (imm_t)util_extract(encoded, 11, 7);
	imm_t imm2 = (imm_t)util_extract(encoded, 31, 25);
	instrs->imm = (imm2 << 5) | imm1;

	return instrs;
}

static instrb_t *instrb_decode(opcode_t opcode, uint32_t encoded) {
	instrb_t *instrb = malloc(sizeof(instrb_t));

	instrb->format = INSTRFORMAT_I;
	instrb->opcode = opcode;
	instrb->funct3 = (funct_t)util_extract(encoded, 14, 12);
	instrb->rs1 = (reg_t)util_extract(encoded, 19, 15);
	instrb->rs2 = (reg_t)util_extract(encoded, 24, 20);
	imm_t imm1 = (imm_t)util_extract(encoded, 11, 8);
	imm_t imm2 = (imm_t)util_extract(encoded, 30, 25);
	imm_t imm3 = (imm_t)util_extract(encoded, 7, 7);
	imm_t imm4 = (imm_t)util_extract(encoded, 31, 31);
	instrb->imm = imm4 << 12 | (imm3 << 11) | (imm2 << 5) | (imm1 << 1);

	return instrb;
}

static instrr_t *instrr_decode(opcode_t opcode, uint32_t encoded) {
	instrr_t *instrr = malloc(sizeof(instrr_t));

	instrr->format = INSTRFORMAT_I;
	instrr->opcode = opcode;
	instrr->funct3 = (funct_t)util_extract(encoded, 14, 12);
	instrr->funct7 = (funct_t)util_extract(encoded, 31, 25);
	instrr->rs1 = (reg_t)util_extract(encoded, 19, 15);
	instrr->rs2 = (reg_t)util_extract(encoded, 24, 20);
	instrr->rd = (reg_t)util_extract(encoded, 11, 7);

	return instrr;
}

static instru_t *instru_decode(opcode_t opcode, uint32_t encoded) {
	instru_t *instru = malloc(sizeof(instru_t));

	instru->format = INSTRFORMAT_I;
	instru->opcode = opcode;
	instru->rd = (reg_t)util_extract(encoded, 11, 7);
	instru->imm = (imm_t)util_extract(encoded, 31, 12) << 11;

	return instru;
}

static instrj_t *instrj_decode(opcode_t opcode, uint32_t encoded) {
	instrj_t *instrj = malloc(sizeof(instrj_t));

	instrj->format = INSTRFORMAT_I;
	instrj->opcode = opcode;
	instrj->rd = (reg_t)util_extract(encoded, 11, 7);
	imm_t imm1 = (imm_t)util_extract(encoded, 30, 21);
	imm_t imm2 = (imm_t)util_extract(encoded, 20, 20);
	imm_t imm3 = (imm_t)util_extract(encoded, 19, 12);
	imm_t imm4 = (imm_t)util_extract(encoded, 31, 31);
	instrj->imm = imm4 << 20 | (imm3 << 12) | (imm2 << 11) | (imm1 << 1);

	return instrj;
}

static void instr_deinit(instr_t **instr_ptr) {
	if (instr_ptr == NULL || *instr_ptr == NULL) {
		return;
	}

	free(*instr_ptr);
	*instr_ptr = NULL;
}

void instr_decode(state_t *state) {
	opcode_t opcode = opcode_from_encoded(state->encoded_instr);
	instrformat_t instrformat = instrformat_from_opcode(opcode);

	instr_t *instr = NULL;
	switch (instrformat) {
	case INSTRFORMAT_I:
		instr = (instr_t *)instri_decode(opcode, state->encoded_instr);
		break;
	case INSTRFORMAT_S:
		instr = (instr_t *)instrs_decode(opcode, state->encoded_instr);
		break;
	case INSTRFORMAT_B:
		instr = (instr_t *)instrb_decode(opcode, state->encoded_instr);
		break;
	case INSTRFORMAT_R:
		instr = (instr_t *)instrr_decode(opcode, state->encoded_instr);
		break;
	case INSTRFORMAT_U:
		instr = (instr_t *)instru_decode(opcode, state->encoded_instr);
		break;
	case INSTRFORMAT_J:
		instr = (instr_t *)instrj_decode(opcode, state->encoded_instr);
		break;
	default:
		break;
	}

	instr_deinit(&(state->instr));

	state->instr = instr;
}

instrish_t instri_to_instrish(const instri_t *instri) {
	instrish_t instrish;

	instrish.format = instri->format;
	instrish.opcode = instri->opcode;
	instrish.funct3 = instri->funct3;
	instrish.rs1 = instri->rs1;
	instrish.rd = instri->rd;
	instrish.shamt = (uint8_t)util_extract((uint32_t)instri->imm, 4, 0);
	instrish.imm = (imm_t)util_extract((uint32_t)instri->imm, 11, 5) << 4;

	return instrish;
}

void instr_select(state_t *state) {
	switch (state->instr->format) {
	case INSTRFORMAT_I: {
		instri_t *instri = (instri_t *)state->instr;

		switch (instri->opcode) {
		case OPCODEI_LOAD:
			switch (instri->funct3) {
			case FUNCT3I_LB:
				// ...
				break;
			case FUNCT3I_LH:
				// ...
				break;
			case FUNCT3I_LW:
				// ...
				break;
			case FUNCT3I_LBU:
				// ...
				break;
			case FUNCT3I_LHU:
				// ...
				break;
			default:
				break;
			}
			break;
		case OPCODEI_FENCE:
			// ...
			break;
		case OPCODEI_IMM:
			switch (instri->funct3) {
			case FUNCT3I_ADD:
				// ...
				break;
			case FUNCT3I_SLL: {
				instrish_t instrish = instri_to_instrish(instri);
				switch (instrish.imm) {
				case IMMI_LOGICAL:
					// ...
					break;
				default:
					break;
				}
				break;
			}
			case FUNCT3I_SLT:
				// ...
				break;
			case FUNCT3I_SLTU:
				// ...
				break;
			case FUNCT3I_XOR:
				// ...
				break;
			case FUNCT3I_SR: {
				instrish_t instrish = instri_to_instrish(instri);
				switch (instrish.imm) {
				case IMMI_LOGICAL:
					// ...
					break;
				case IMMI_ARITHMETIC:
					// ...
					break;
				}
				break;
			}
			case FUNCT3I_OR:
				// ...
				break;
			case FUNCT3I_AND:
				// ...
				break;
			default:
				break;
			}
			break;
		case OPCODEI_JALR: {
			switch (instri->funct3) {
			case FUNCT3I_JALR:
				// ...
				break;
			default:
				break;
			}
			break;
		}
		case OPCODEI_SYSTEM:
			switch (instri->imm) {
			case IMMI_ECALL:
				// ...
				break;
			case IMMI_EBREAK:
				// ...
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		break;
	}
	case INSTRFORMAT_S: {
		instrs_t *instrs = (instrs_t *)state->instr;

		switch (instrs->opcode) {
		case OPCODES_STORE:
			switch (instrs->funct3) {
			case FUNCT3S_SB:
				// ...
				break;
			case FUNCT3S_SH:
				// ...
				break;
			case FUNCT3S_SW:
				// ...
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		break;
	}
	case INSTRFORMAT_B: {
		instrb_t *instrb = (instrb_t *)state->instr;

		switch (instrb->opcode) {
		case OPCODEB_BRANCH:
			switch (instrb->funct3) {
			case FUNCT3B_BEQ:
				// ...
				break;
			case FUNCT3B_BNE:
				// ...
				break;
			case FUNCT3B_BLT:
				// ...
				break;
			case FUNCT3B_BGE:
				// ...
				break;
			case FUNCT3B_BLTU:
				// ...
				break;
			case FUNCT3B_BGEU:
				// ...
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		break;
	}
	case INSTRFORMAT_R: {
		instrr_t *instrr = (instrr_t *)state->instr;

		switch (instrr->opcode) {
		case OPCODER_REG:
			switch (instrr->funct3) {
			case FUNCT3R_ADD_SUB:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				case FUNCT7R_ALTERNATE:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_SLL:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_SLT:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_SLTU:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_XOR:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_SRL_SRA:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				case FUNCT7R_ALTERNATE:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_OR:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			case FUNCT3R_AND:
				switch (instrr->funct7) {
				case FUNCT7R_NORMAL:
					// ...
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		break;
	}
	case INSTRFORMAT_U: {
		instru_t *instru = (instru_t *)state->instr;

		switch (instru->opcode) {
		case OPCODEU_AUIPC:
			// ...
			break;
		case OPCODEU_LUI:
			// ...
			break;
		default:
			break;
		}

		break;
	}
	case INSTRFORMAT_J: {
		instrj_t *instrj = (instrj_t *)state->instr;

		switch (instrj->opcode) {
		case OPCODEJ_JAL:
			// ...
			break;
		default:
			break;
		}

		break;
	}
	default:
		break;
	}
}

void decode(state_t *state) {
	instr_decode(state);
	instr_select(state);
}
