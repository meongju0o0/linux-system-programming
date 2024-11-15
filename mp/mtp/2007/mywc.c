#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

typedef int bool;

int main(int argc, char* argv[]) {
	char *pathname = argv[1];
	char buf[256];
	FILE *fp;
	
	if(!(fp = fopen(pathname, "rt"))) {
		perror("fopen");
		exit(1);
	}
	
	int num_characters = 0, num_words = 0, num_lines = 0;
	bool in_word = TRUE;

	while(fgets(buf, sizeof(buf), fp)) {
		num_lines++;

		for(int i = 0; buf[i] != '\0'; i++) {
			num_characters++;

			if(isspace(buf[i])) {
				if(in_word) {
					num_words++;
					in_word = FALSE;
				}
			}
			else {
				in_word = TRUE;
			}
		}

		if(in_word) {
			num_words++;
			in_word = FALSE;
		}
	}

	printf("Number of characters: %d\n", num_characters);
	printf("Number of words: %d\n", num_words);
	printf("Number of lines: %d\n", num_lines);

	fclose(fp);

	return 0;
}
