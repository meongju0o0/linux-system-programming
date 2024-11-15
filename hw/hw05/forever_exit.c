#include <stdio.h>
#include <stdlib.h>

void main() {
	printf("Hello World\n");

	if(atexit(main)) {
		perror("System can not add exit function");
		exit(1);
	}
}
