#include <stdio.h>

void main() {
	for (int i = 0; i < 9; i++) {
		for (int j = 1; j < 10 - i; j++) {
			printf("%d ", j);
		}
		printf("\n");
	}
}
