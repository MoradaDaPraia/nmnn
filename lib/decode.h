#ifndef DECODE_H
#define DECODE_H

#include "constants.h"
#include "state.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	funct_t funct3;
	reg_t rs1;
	reg_t rd;
	imm_t imm;
} instri_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	funct_t funct3;
	reg_t rs1;
	reg_t rd;
	uint8_t shamt;
	funct_t imm;
} instrish_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	funct_t funct3;
	reg_t rs1;
	reg_t rs2;
	imm_t imm;
} instrs_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	funct_t funct3;
	reg_t rs1;
	reg_t rs2;
	imm_t imm;
} instrb_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	funct_t funct3;
	funct_t funct7;
	reg_t rs1;
	reg_t rs2;
	reg_t rd;
} instrr_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	reg_t rd;
	imm_t imm;
} instru_t;

typedef struct {
	instrformat_t format;
	opcode_t opcode;
	reg_t rd;
	imm_t imm;
} instrj_t;

void instr_decode(state_t *state);

instrish_t instri_to_instrish(const instri_t *instri);

#endif
