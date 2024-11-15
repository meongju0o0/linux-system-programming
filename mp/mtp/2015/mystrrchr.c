#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <limits.h>

char *mystrrchr(const char *str, char c) {
	int cnt = 0;
	int last_idx = 0;
	
	while(*(str + cnt) != '\0') {
		if(tolower(*(str + cnt)) == tolower(c)) {
			last_idx = cnt;
		}
		cnt++;
	}

	char *return_str = malloc(sizeof(char) * 100);
	if(return_str == NULL) {
		perror("malloc");
		exit(1);
	}

	int idx = 0;
	while(*(str + last_idx) != '\0') {
		return_str[idx] = *(str + last_idx);
		idx++; last_idx++;
	}
	return_str[idx] = '\0';

	return return_str;
}

int main() {
	char str[] = "Hello hello HELLO";
	char *str2 = mystrrchr(str, 'l');
	printf("Last %c in %s: %s\n", 'l', str, str2);
	free(str2);
	return 0;
}
