#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_str_size(char* str) {
	size_t size = 0;
	while(str[size] != '\0') {
		size++;
	}
	return size;
}

void find_str(char* path, char* find) {
	size_t find_size = get_str_size(find);
	FILE* fp = fopen(path, "r");
	if(!fp) {
		perror("Failed to open file");
		exit(-1);
	}

	char buf[256];
	while(fgets(buf, sizeof(buf), fp)) {
		for(int i = 0; buf[i + find_size - 1] != '\0'; i++) {
			char sliced_str[find_size + 1];
			for(int j = 0; j < find_size; j++) {
				sliced_str[j] = buf[i + j];
			}
			sliced_str[find_size] = '\0';

			if(strcmp(find, sliced_str) == 0) {
				puts(buf);
				break;
			}
		}
	}

	fclose(fp);
}

int main(int argc, char *argv[]) {
	char* find = argv[1];
	char* path = argv[2];

	find_str(path, find);

	return 0;
}
