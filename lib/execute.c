#include "decode.h"
#include <stddef.h>

void execute(uint32_t raw) {
	uint8_t __buf[32];
	uint8_t *buf = __buf;
	size_t buf_size = sizeof(__buf) / sizeof(uint8_t);

	instr_t *instr = instr_decode(raw, buf, buf_size);

	switch (instr->format) {
	case INSTRFORMAT_I: {
		instri_t *instri = (instri_t *)instr;

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
		instrs_t *instrs = (instrs_t *)instr;

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
		instrb_t *instrb = (instrb_t *)instr;

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
		instrr_t *instrr = (instrr_t *)instr;

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
		instru_t *instru = (instru_t *)instr;

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
		instrj_t *instrj = (instrj_t *)instr;

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
