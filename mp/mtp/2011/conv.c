#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int my_stoi(char* str) {
	int num = 0;
	size_t str_size = 0;

	while(str[str_size] != '\0') {
		str_size++;
	}

	for(int i = 0; str[i] != '\0'; i++) {
		num = num * 10 + (str[i] - '0');
	}

	return num;
}

void print_binary(const int num) {
	int tmp_num = num;

	char binaries[32];
	size_t binaries_size = 0;

	if(num == 0) {
		putchar('0');
		putchar('\n');
		return;
	}

	while(tmp_num > 0) {
		binaries[binaries_size++] = (tmp_num % 2) + '0';
		tmp_num = tmp_num / 2;
	}

	for(size_t i = binaries_size; i > 0; i--) {
		putchar(binaries[i - 1]);
	}
	putchar('\n');
}

int main(int argc, char *argv[]) {
	int num = my_stoi(argv[1]);
	print_binary(num);

	return 0;
}
