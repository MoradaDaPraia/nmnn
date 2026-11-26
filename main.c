#include "lib/cycle.h"
#include "lib/memory.h"
#include "lib/state.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VRAM_START 0x80000u
#define VRAM_SIZE 0x10000u
#define BENCHMARK_INTERVAL 32u

static const uint32_t benchmark_program[] = {
    0x00300513, 0x000805b7, 0xf8058613, 0x00a62023, 0x00460613, 0x00450513,
    0xfeb61ae3, 0x00080537, 0xf8052703, 0x00100593, 0x01700693, 0x00100613,
    0x1ad70a63, 0xf8452703, 0x00200613, 0x1ad70463, 0xf8852703, 0x01700693,
    0x00300613, 0x18d70c63, 0xf8c52703, 0x00400613, 0x18d70663, 0xf9052703,
    0x01700693, 0x00500613, 0x16d70e63, 0xf9452703, 0x00600613, 0x16d70863,
    0xf9852703, 0x01700693, 0x00700613, 0x16d70063, 0xf9c52703, 0x00800613,
    0x14d70a63, 0xfa052703, 0x01700693, 0x00900613, 0x14d70263, 0xfa452703,
    0x00a00613, 0x12d70c63, 0xfa852703, 0x01700693, 0x00b00613, 0x12d70463,
    0xfac52703, 0x00c00613, 0x10d70e63, 0xfb052703, 0x01700693, 0x00d00613,
    0x10d70663, 0xfb452703, 0x00e00613, 0x10d70063, 0xfb852703, 0x01700693,
    0x00f00613, 0x0ed70863, 0xfbc52703, 0x01000613, 0x0ed70263, 0xfc052703,
    0x01700693, 0x01100613, 0x0cd70a63, 0xfc452703, 0x01200613, 0x0cd70463,
    0xfc852703, 0x01700693, 0x01300613, 0x0ad70c63, 0xfcc52703, 0x01400613,
    0x0ad70663, 0xfd052703, 0x01700693, 0x01500613, 0x08d70e63, 0xfd452703,
    0x01600613, 0x08d70863, 0xfd852683, 0x01700613, 0x08c68263, 0xfdc52703,
    0x01700693, 0x01800613, 0x06d70a63, 0xfe052703, 0x01900613, 0x06d70463,
    0xfe452703, 0x01700693, 0x01a00613, 0x04d70c63, 0xfe852703, 0x01b00613,
    0x04d70663, 0xfec52703, 0x01700693, 0x01c00613, 0x02d70e63, 0xff052703,
    0x01d00613, 0x02d70863, 0xff452703, 0x01700693, 0x01e00613, 0x02d70063,
    0xff852703, 0x01f00613, 0x00d70a63, 0xffc52683, 0x01700713, 0x02000613,
    0x06e69e63, 0x00000813, 0x000046b7, 0x02a00713, 0x00a00793, 0x00000293,
    0x00058893, 0x410685b3, 0x00b6b333, 0xfff30313, 0x00b375b3, 0x00a80333,
    0x04558663, 0x005303b3, 0x00128293, 0x00e38023, 0xfe5898e3, 0x005802b3,
    0x02d28a63, 0x00128813, 0x00a285b3, 0x00f58023, 0x00188593, 0xfac89ce3,
    0xfff28513, 0x000045b7, 0xffd58593, 0x00a5e863, 0x00080537, 0x00a28533,
    0x000500a3, 0x00000513, 0x00008067,
};

static void print_vram(const state_t *state, uint32_t instructions_executed) {
	printf("\n===== Instrução %08u =====\n", instructions_executed);
	for (uint32_t offset = 0; offset < VRAM_SIZE; offset++) {
		uint8_t character = load_byte(state->memory, VRAM_START + offset);
		if (character == '\0') {
			break;
		}
		putchar(character);
	}
	puts("\n==============================");
}

static void run_program(state_t *state) {
	uint32_t instructions_executed = 0;
	uint32_t last_transmission = 0;
	do {
		cycle(state);
		instructions_executed++;
		if (instructions_executed % BENCHMARK_INTERVAL == 0) {
			print_vram(state, instructions_executed);
			last_transmission = instructions_executed;
		}
	} while (state->pc < state->text_end);

	if (last_transmission != instructions_executed) {
		print_vram(state, instructions_executed);
	}
}

static int run_benchmark(void) {
	state_t *state = state_init();
	if (state == NULL || state->memory == NULL) {
		fprintf(stderr, "erro: nao foi possivel inicializar o estado\n");
		state_deinit(&state);
		return 1;
	}

	const uint32_t program_words =
	    sizeof(benchmark_program) / sizeof(benchmark_program[0]);
	state_load_text(state, benchmark_program, program_words);
	run_program(state);
	state_deinit(&state);
	return 0;
}

static int run_program_file(const char *program_path) {
	state_t *state = state_init();
	if (state == NULL || state->memory == NULL) {
		fprintf(stderr, "erro: nao foi possivel inicializar o estado\n");
		state_deinit(&state);
		return 1;
	}

	FILE *program_file = fopen(program_path, "rb");
	if (program_file == NULL) {
		perror(program_path);
		state_deinit(&state);
		return 1;
	}

	if (fseek(program_file, 0, SEEK_END) != 0) {
		perror("fseek");
		fclose(program_file);
		state_deinit(&state);
		return 1;
	}
	long file_size = ftell(program_file);
	if (file_size <= 0 || file_size % (long)sizeof(uint32_t) != 0 ||
	    (unsigned long)file_size > 0x80000ul ||
	    fseek(program_file, 0, SEEK_SET) != 0) {
		fprintf(stderr, "erro: binario invalido (precisa ter tamanho positivo, "
		                "multiplo de 4 e caber na RAM)\n");
		fclose(program_file);
		state_deinit(&state);
		return 1;
	}

	uint8_t *program_bytes = malloc((size_t)file_size);
	if (program_bytes == NULL) {
		fprintf(stderr, "erro: sem memoria para ler o binario\n");
		fclose(program_file);
		state_deinit(&state);
		return 1;
	}
	size_t bytes_read =
	    fread(program_bytes, 1, (size_t)file_size, program_file);
	if (bytes_read != (size_t)file_size || ferror(program_file)) {
		fprintf(stderr, "erro: falha ao ler o binario\n");
		free(program_bytes);
		fclose(program_file);
		state_deinit(&state);
		return 1;
	}
	fclose(program_file);

	store_batch(state->memory, 0, program_bytes, (uint32_t)file_size);
	state->text_end = (uint32_t)file_size - sizeof(uint32_t);
	free(program_bytes);

	run_program(state);

	state_deinit(&state);
	return 0;
}

int main(int argc, char **argv) {
	if (argc > 2) {
		fprintf(stderr, "uso: %s [arquivo.bin]\n", argv[0]);
		return 1;
	}
	if (argc == 2) {
		return run_program_file(argv[1]);
	}
	return run_benchmark();
}
