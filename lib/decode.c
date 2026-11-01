#include "decode.h"
#include "util.h"
#include <stddef.h>

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

static opcode_t opcode_from_raw(uint32_t raw) {
	return (opcode_t)util_extract(raw, 6, 0);
}

static instri_t *instri_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instri_t) > buf_size) {
		return (instri_t *)buf;
	}

	instri_t *instri = (instri_t *)buf;

	instri->format = INSTRFORMAT_I;
	instri->opcode = opcode;
	instri->funct3 = (funct_t)util_extract(raw, 14, 12);
	instri->rs1 = (reg_t)util_extract(raw, 19, 15);
	instri->rd = (reg_t)util_extract(raw, 11, 7);
	instri->imm = (imm_t)util_extract(raw, 31, 20);

	return instri;
}

static instrs_t *instrs_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instrs_t) > buf_size) {
		return (instrs_t *)buf;
	}

	instrs_t *instrs = (instrs_t *)buf;

	instrs->format = INSTRFORMAT_I;
	instrs->opcode = opcode;
	instrs->funct3 = (funct_t)util_extract(raw, 14, 12);
	instrs->rs1 = (reg_t)util_extract(raw, 19, 15);
	instrs->rs2 = (reg_t)util_extract(raw, 24, 20);
	imm_t imm1 = (imm_t)util_extract(raw, 11, 7);
	imm_t imm2 = (imm_t)util_extract(raw, 31, 25);
	instrs->imm = (imm2 << 5) | imm1;

	return instrs;
}

static instrb_t *instrb_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instrb_t) > buf_size) {
		return (instrb_t *)buf;
	}

	instrb_t *instrb = (instrb_t *)buf;

	instrb->format = INSTRFORMAT_I;
	instrb->opcode = opcode;
	instrb->funct3 = (funct_t)util_extract(raw, 14, 12);
	instrb->rs1 = (reg_t)util_extract(raw, 19, 15);
	instrb->rs2 = (reg_t)util_extract(raw, 24, 20);
	imm_t imm1 = (imm_t)util_extract(raw, 11, 8);
	imm_t imm2 = (imm_t)util_extract(raw, 30, 25);
	imm_t imm3 = (imm_t)util_extract(raw, 7, 7);
	imm_t imm4 = (imm_t)util_extract(raw, 31, 31);
	instrb->imm = imm4 << 12 | (imm3 << 11) | (imm2 << 5) | (imm1 << 1);

	return instrb;
}

static instrr_t *instrr_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instrr_t) > buf_size) {
		return (instrr_t *)buf;
	}

	instrr_t *instrr = (instrr_t *)buf;

	instrr->format = INSTRFORMAT_I;
	instrr->opcode = opcode;
	instrr->funct3 = (funct_t)util_extract(raw, 14, 12);
	instrr->funct7 = (funct_t)util_extract(raw, 31, 25);
	instrr->rs1 = (reg_t)util_extract(raw, 19, 15);
	instrr->rs2 = (reg_t)util_extract(raw, 24, 20);
	instrr->rd = (reg_t)util_extract(raw, 11, 7);

	return instrr;
}

static instru_t *instru_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instru_t) > buf_size) {
		return (instru_t *)buf;
	}

	instru_t *instru = (instru_t *)buf;

	instru->format = INSTRFORMAT_I;
	instru->opcode = opcode;
	instru->rd = (reg_t)util_extract(raw, 11, 7);
	instru->imm = (imm_t)util_extract(raw, 31, 12) << 11;

	return instru;
}

static instrj_t *instrj_decode(opcode_t opcode, uint32_t raw, uint8_t *buf,
                               size_t buf_size) {
	if (sizeof(instrj_t) > buf_size) {
		return (instrj_t *)buf;
	}

	instrj_t *instrj = (instrj_t *)buf;

	instrj->format = INSTRFORMAT_I;
	instrj->opcode = opcode;
	instrj->rd = (reg_t)util_extract(raw, 11, 7);
	imm_t imm1 = (imm_t)util_extract(raw, 30, 21);
	imm_t imm2 = (imm_t)util_extract(raw, 20, 20);
	imm_t imm3 = (imm_t)util_extract(raw, 19, 12);
	imm_t imm4 = (imm_t)util_extract(raw, 31, 31);
	instrj->imm = imm4 << 20 | (imm3 << 12) | (imm2 << 11) | (imm1 << 1);

	return instrj;
}

instr_t *instr_decode(uint32_t raw, uint8_t *buf, size_t buf_size) {
	opcode_t opcode = opcode_from_raw(raw);
	instrformat_t instrformat = instrformat_from_opcode(opcode);

	instr_t *instr = NULL;
	switch (instrformat) {
	case INSTRFORMAT_I:
		instr = (instr_t *)instri_decode(opcode, raw, buf, buf_size);
		break;
	case INSTRFORMAT_S:
		instr = (instr_t *)instrs_decode(opcode, raw, buf, buf_size);
		break;
	case INSTRFORMAT_B:
		instr = (instr_t *)instrb_decode(opcode, raw, buf, buf_size);
		break;
	case INSTRFORMAT_R:
		instr = (instr_t *)instrr_decode(opcode, raw, buf, buf_size);
		break;
	case INSTRFORMAT_U:
		instr = (instr_t *)instru_decode(opcode, raw, buf, buf_size);
		break;
	case INSTRFORMAT_J:
		instr = (instr_t *)instrj_decode(opcode, raw, buf, buf_size);
		break;
	default:
		break;
	}

	return instr;
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
