#include "../../lib/shared/message.h"
#include <assert.h>
#include <string.h>

void test_message_get(void) {
	const char *message = message_get();
	assert(strcmp(message, "Hello, World!") == 0);
}

int main(void) {
	test_message_get();

	return 0;
}
