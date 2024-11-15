#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	char* src = argv[1];
	char* dst = argv[2];

	char buf[256];

	FILE *src_fp, *dst_fp;

	if(!(src_fp = fopen(src, "rt"))) {
		perror("fopen");
		exit(1);
	}

	if(!(dst_fp = fopen(dst, "wt"))) {
		perror("fopen");
		exit(1);
	}

	while(fgets(buf, sizeof(buf), src_fp)) {
		for(int i = 0; buf[i] != '\0'; i++) {
			fputc(toupper(buf[i]), dst_fp);
		}
	}

	fclose(src_fp);
	fclose(dst_fp);

	return 0;
}
