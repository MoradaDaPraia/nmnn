#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

typedef uint8_t instrformat_t;

typedef uint8_t reg_t;

typedef uint8_t funct_t;

typedef uint8_t opcode_t;

typedef uint32_t imm_t;

#define INSTRFORMAT_I ((instrformat_t)1)
#define INSTRFORMAT_S ((instrformat_t)2)
#define INSTRFORMAT_B ((instrformat_t)3)
#define INSTRFORMAT_R ((instrformat_t)4)
#define INSTRFORMAT_U ((instrformat_t)5)
#define INSTRFORMAT_J ((instrformat_t)6)

#define REG_ZERO ((reg_t)0)
#define REG_RA ((reg_t)1)
#define REG_SP ((reg_t)2)
#define REG_GP ((reg_t)3)
#define REG_TP ((reg_t)4)
#define REG_T0 ((reg_t)5)
#define REG_T1 ((reg_t)6)
#define REG_T2 ((reg_t)7)
#define REG_FP ((reg_t)8)
#define REG_S0 ((reg_t)8)
#define REG_S1 ((reg_t)9)
#define REG_A0 ((reg_t)10)
#define REG_A1 ((reg_t)11)
#define REG_A2 ((reg_t)12)
#define REG_A3 ((reg_t)13)
#define REG_A4 ((reg_t)14)
#define REG_A5 ((reg_t)15)
#define REG_A6 ((reg_t)16)
#define REG_A7 ((reg_t)17)
#define REG_S2 ((reg_t)18)
#define REG_S3 ((reg_t)19)
#define REG_S4 ((reg_t)20)
#define REG_S5 ((reg_t)21)
#define REG_S6 ((reg_t)22)
#define REG_S7 ((reg_t)23)
#define REG_S8 ((reg_t)24)
#define REG_S9 ((reg_t)25)
#define REG_S10 ((reg_t)26)
#define REG_S11 ((reg_t)27)
#define REG_T3 ((reg_t)28)
#define REG_T4 ((reg_t)29)
#define REG_T5 ((reg_t)30)
#define REG_T6 ((reg_t)31)

#define OPCODEI_LOAD ((opcode_t)0x03)
#define OPCODEI_FENCE ((opcode_t)0x0F)
#define OPCODEI_IMM ((opcode_t)0x13)
#define OPCODEI_JALR ((opcode_t)0x67)
#define OPCODEI_SYSTEM ((opcode_t)0x73)
#define OPCODES_STORE ((opcode_t)0x23)
#define OPCODEB_BRANCH ((opcode_t)0x63)
#define OPCODER_REG ((opcode_t)0x33)
#define OPCODEU_AUIPC ((opcode_t)0x17)
#define OPCODEU_LUI ((opcode_t)0x37)
#define OPCODEJ_JAL ((opcode_t)0x6F)

#define FUNCT3I_LB ((funct_t)0x0)
#define FUNCT3I_LH ((funct_t)0x1)
#define FUNCT3I_LW ((funct_t)0x2)
#define FUNCT3I_LBU ((funct_t)0x4)
#define FUNCT3I_LHU ((funct_t)0x5)
#define FUNCT3I_ADD ((funct_t)0x0)
#define FUNCT3I_SLL ((funct_t)0x1)
#define FUNCT3I_SLT ((funct_t)0x2)
#define FUNCT3I_SLTU ((funct_t)0x3)
#define FUNCT3I_XOR ((funct_t)0x4)
#define FUNCT3I_SR ((funct_t)0x5)
#define FUNCT3I_OR ((funct_t)0x6)
#define FUNCT3I_AND ((funct_t)0x7)
#define FUNCT3I_JALR ((funct_t)0x0)
#define FUNCT3S_SB ((funct_t)0x0)
#define FUNCT3S_SH ((funct_t)0x1)
#define FUNCT3S_SW ((funct_t)0x2)
#define FUNCT3B_BEQ ((funct_t)0x0)
#define FUNCT3B_BNE ((funct_t)0x1)
#define FUNCT3B_BLT ((funct_t)0x4)
#define FUNCT3B_BGE ((funct_t)0x5)
#define FUNCT3B_BLTU ((funct_t)0x6)
#define FUNCT3B_BGEU ((funct_t)0x7)
#define FUNCT3R_ADD_SUB ((funct_t)0x0)
#define FUNCT3R_SLL ((funct_t)0x1)
#define FUNCT3R_SLT ((funct_t)0x2)
#define FUNCT3R_SLTU ((funct_t)0x3)
#define FUNCT3R_XOR ((funct_t)0x4)
#define FUNCT3R_SRL_SRA ((funct_t)0x5)
#define FUNCT3R_OR ((funct_t)0x6)
#define FUNCT3R_AND ((funct_t)0x7)

#define FUNCT7R_NORMAL ((funct_t)0x00)
#define FUNCT7R_ALTERNATE ((funct_t)0x20)

#define IMMI_LOGICAL ((imm_t)0x00)
#define IMMI_ARITHMETIC ((imm_t)0x20)
#define IMMI_ECALL ((imm_t)0x00)
#define IMMI_EBREAK ((imm_t)0x01)

#endif
