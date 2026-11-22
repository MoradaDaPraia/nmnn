#include "../lib/decode.h"
#include "../lib/execute.h"
#include "../lib/shared.h"
#include "../lib/state.h"
#include <assert.h>
#include <string.h>

static void run_instruction(state_t *state, uint32_t encoded) {
	state->encoded_instr = encoded;
	decode(state);
	execute(state);
}

static void test_loads(void) {
	state_t *state = state_init();
	uint8_t bytes[] = {0x80, 0xFE, 0x34, 0x12};
	store_batch(state->memory, 0x105, bytes, sizeof(bytes));
	state->regs[1] = 0x100;

	run_instruction(state, 0x00508183); /* lb x3, 5(x1) */
	assert(state->regs[3] == 0xFFFFFF80);
	run_instruction(state, 0x0050c183); /* lbu x3, 5(x1) */
	assert(state->regs[3] == 0x80);
	run_instruction(state, 0x00509183); /* lh x3, 5(x1) */
	assert(state->regs[3] == 0xFFFFFE80);
	run_instruction(state, 0x0050d183); /* lhu x3, 5(x1) */
	assert(state->regs[3] == 0xFE80);
	run_instruction(state, 0x0050a183); /* lw x3, 5(x1) */
	assert(state->regs[3] == 0x1234FE80);
	state_deinit(&state);
}

static void test_immediate_arithmetic(void) {
	state_t *state = state_init();
	state->regs[1] = 0x80000001;
	run_instruction(state, 0x00508193); /* addi x3, x1, 5 */
	assert(state->regs[3] == 0x80000006);
	run_instruction(state, 0x00509193); /* slli x3, x1, 5 */
	assert(state->regs[3] == 0x00000020);
	run_instruction(state, 0x0050a193); /* slti x3, x1, 5 */
	assert(state->regs[3] == 1);
	run_instruction(state, 0x0050b193); /* sltiu x3, x1, 5 */
	assert(state->regs[3] == 0);
	run_instruction(state, 0x0050c193); /* xori x3, x1, 5 */
	assert(state->regs[3] == 0x80000004);
	run_instruction(state, 0x0050d193); /* srli x3, x1, 5 */
	assert(state->regs[3] == 0x04000000);
	run_instruction(state, 0x4050d193); /* srai x3, x1, 5 */
	assert(state->regs[3] == 0xFC000000);
	run_instruction(state, 0x0050e193); /* ori x3, x1, 5 */
	assert(state->regs[3] == 0x80000005);
	run_instruction(state, 0x0050f193); /* andi x3, x1, 5 */
	assert(state->regs[3] == 1);
	state_deinit(&state);
}

static void test_control_and_noops(void) {
	state_t *state = state_init();
	state->regs[1] = 0x100;
	state->pc = 0x200;
	state->next_pc = 0x204;
	run_instruction(state, 0x005081e7); /* jalr x3, 5(x1) */
	assert(state->regs[3] == 0x204);
	assert(state->pc == 0x200);
	assert(state->next_pc == 0x104);

	state->regs[3] = 0x12345678;
	run_instruction(state, 0x0000818f); /* fence */
	assert(state->regs[3] == 0x12345678);
	run_instruction(state, 0x000081f3); /* ecall */
	assert(state->regs[3] == 0x12345678);
	run_instruction(state, 0x001081f3); /* ebreak */
	assert(state->regs[3] == 0x12345678);
	assert(state->regs[REG_ZERO] == 0);
	state_deinit(&state);
}

static void test_stores(void) {
	state_t *state = state_init();
	state->regs[1] = 0x100;
	state->regs[2] = 0x12345678;

	run_instruction(state, 0x002082a3); /* sb x2, 5(x1) */
	assert(load_byte(state->memory, 0x105) == 0x78);

	state->regs[1] = 0x106;
	state->regs[2] = 0x000000AB;
	run_instruction(state, 0xfe208fa3); /* sb x2, -1(x1) */
	assert(load_byte(state->memory, 0x105) == 0xAB);
	state->regs[1] = 0x100;

	state->regs[2] = 0x0000A1B2;
	run_instruction(state, 0x002092a3); /* sh x2, 5(x1) */
	assert(load_half(state->memory, 0x105) == 0xA1B2);

	state->regs[2] = 0x12345678;
	run_instruction(state, 0x0020a2a3); /* sw x2, 5(x1) */
	assert(load_word(state->memory, 0x105) == 0x12345678);

	state_deinit(&state);
}

static void assert_branch(state_t *state, uint32_t encoded, uint32_t rs1,
	                  uint32_t rs2, uint32_t expected_next_pc) {
	state->regs[1] = rs1;
	state->regs[2] = rs2;
	state->pc = 0x1000;
	state->next_pc = state->pc + 4;
	run_instruction(state, encoded);
	assert(state->pc == 0x1000);
	assert(state->next_pc == expected_next_pc);
}

static void test_branches(void) {
	state_t *state = state_init();
	const uint32_t target = 0x1008;
	const uint32_t fallthrough = 0x1004;

	assert_branch(state, 0x00208463, 7, 7, target);         /* beq taken */
	assert_branch(state, 0x00208463, 7, 8, fallthrough);    /* beq not taken */
	assert_branch(state, 0x00209463, 7, 8, target);         /* bne taken */
	assert_branch(state, 0x00209463, 7, 7, fallthrough);    /* bne not taken */
	assert_branch(state, 0x0020c463, 0xfffffffe, 1, target); /* blt taken */
	assert_branch(state, 0x0020c463, 1, 0xfffffffe, fallthrough); /* blt not taken */
	assert_branch(state, 0x0020d463, 1, 0xfffffffe, target); /* bge taken */
	assert_branch(state, 0x0020d463, 0xfffffffe, 1, fallthrough); /* bge not taken */
	assert_branch(state, 0x0020e463, 1, 2, target);         /* bltu taken */
	assert_branch(state, 0x0020e463, 0xfffffffe, 1, fallthrough); /* bltu not taken */
	assert_branch(state, 0x0020f463, 0xfffffffe, 1, target); /* bgeu taken */
	assert_branch(state, 0x0020f463, 1, 2, fallthrough);    /* bgeu not taken */
	assert_branch(state, 0xfe209ee3, 1, 2, 0x0ffc);        /* bne backward */

	state_deinit(&state);
}

static void test_register_operations(void) {
	state_t *state = state_init();
	state->regs[1] = 0x80000001;
	state->regs[2] = 1;

	run_instruction(state, 0x002081b3); /* add x3, x1, x2 */
	assert(state->regs[3] == 0x80000002);
	run_instruction(state, 0x402081b3); /* sub x3, x1, x2 */
	assert(state->regs[3] == 0x80000000);
	run_instruction(state, 0x002091b3); /* sll x3, x1, x2 */
	assert(state->regs[3] == 0x00000002);
	state->regs[2] = 33;
	run_instruction(state, 0x002091b3); /* sll x3, x1, x2 (shamt is 1) */
	assert(state->regs[3] == 0x00000002);
	state->regs[2] = 1;
	run_instruction(state, 0x0020a1b3); /* slt x3, x1, x2 */
	assert(state->regs[3] == 1);
	run_instruction(state, 0x0020b1b3); /* sltu x3, x1, x2 */
	assert(state->regs[3] == 0);
	run_instruction(state, 0x0020c1b3); /* xor x3, x1, x2 */
	assert(state->regs[3] == 0x80000000);
	run_instruction(state, 0x0020d1b3); /* srl x3, x1, x2 */
	assert(state->regs[3] == 0x40000000);
	run_instruction(state, 0x4020d1b3); /* sra x3, x1, x2 */
	assert(state->regs[3] == 0xC0000000);
	run_instruction(state, 0x0020e1b3); /* or x3, x1, x2 */
	assert(state->regs[3] == 0x80000001);
	run_instruction(state, 0x0020f1b3); /* and x3, x1, x2 */
	assert(state->regs[3] == 1);
	assert(state->regs[REG_ZERO] == 0);

	state_deinit(&state);
}

static void test_upper_and_jump_operations(void) {
	state_t *state = state_init();
	state->pc = 0x1000;
	state->next_pc = 0x1004;

	run_instruction(state, 0x123451b7); /* lui x3, 0x12345 */
	assert(state->regs[3] == 0x12345000);
	assert(state->pc == 0x1000);
	assert(state->next_pc == 0x1004);

	run_instruction(state, 0x12345197); /* auipc x3, 0x12345 */
	assert(state->regs[3] == 0x12346000);
	assert(state->pc == 0x1000);
	assert(state->next_pc == 0x1004);

	run_instruction(state, 0x008001ef); /* jal x3, 8 */
	assert(state->regs[3] == 0x1004);
	assert(state->pc == 0x1000);
	assert(state->next_pc == 0x1008);

	state->pc = 0x2000;
	state->next_pc = 0x2004;
	run_instruction(state, 0xffdff1ef); /* jal x3, -4 */
	assert(state->regs[3] == 0x2004);
	assert(state->pc == 0x2000);
	assert(state->next_pc == 0x1ffc);

	state_deinit(&state);
}

int main(void) {
	test_loads();
	test_immediate_arithmetic();
	test_control_and_noops();
	test_stores();
	test_branches();
	test_register_operations();
	test_upper_and_jump_operations();
	return 0;
}
