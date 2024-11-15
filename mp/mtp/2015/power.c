#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int power(int a, int b) {
	if(b == 1) {
		return a;
	}
	else {
		return a * power(a, b - 1);
	}
}

int main(int argc, char *argv[]) {
	int a = atoi(argv[1]), b = atoi(argv[2]);
	printf("%d\n", power(a, b));

	return 0;
}
