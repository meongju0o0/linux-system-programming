#include <stdio.h>

void del(char str[]) {
	while (*str) {
		*str = *(str + 1);
		str++;
	}
}

void main() {
	char str[20] = "Hello";
	del(str);
	puts(str);
}
