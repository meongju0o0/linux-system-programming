#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

int mystrcasecmp(char* str1, char* str2) {
        int idx = 0;
        while(*(str1 + idx) != '\0' || *(str2 + idx) != '\0') {
                if(tolower(*(str1 + idx)) != tolower(*(str2 + idx))) {
                        return tolower(*(str1 + idx)) - tolower(*(str2 + idx));
                }
                idx++;
        }
}

int main(int argc, char *argv[]) {
	char* str1 = argv[1];
	char* str2 = argv[2];

	printf("%d\n", mystrcasecmp(str1, str2));
	
	return 0;
}
