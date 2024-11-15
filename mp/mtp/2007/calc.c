#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if(argc != 4) {
		fprintf(stderr, "Usage: %s <number1> <operator> <number2>", argv[0]);
	}
	int a = atoi(argv[1]);
	char* operator = argv[2];
	int b = atoi(argv[3]);

	if(strcmp(operator, "+") == 0) {
		printf("%d\n", a + b);
	}
	else if(strcmp(operator, "-") == 0) {
		printf("%d\n", a - b);
	}
	else if(strcmp(operator, "*") == 0) {
		printf("%d\n", a * b);
	}
	else if(strcmp(operator, "*") == 0) {
		if(b == 0) {
			fprintf(stderr, "Error: Division by Zero\n");
			exit(1);
		}
		printf("%d\n", a / b);
	}
	else {
		fprintf(stderr, "Error: Invalid operator. Use +, -, *, or /.\n");
		exit(1);
	}

	return 0;
}
