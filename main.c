#include "lib/shared/message.h"
#include <stdio.h>

int main(void) {
	const char *message = message_get();

	printf("%s\n", message);

	return 0;
}
