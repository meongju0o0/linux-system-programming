#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 256

long filesize(FILE *fp) {
	long cur, size;

	cur = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, cur, SEEK_SET);

	return size;
}

int main(int argc, char *argv[]) {
	const char* src_path = argv[1];
	const int num_split = atoi(argv[2]);

	FILE *src, *dst;

	char buf[MAX_BUF];
	
	if(!(src = fopen(src_path, "rb"))) {
		perror("fopen, src");
		exit(1);
	}

	const int split_size = filesize(src) / num_split;

	for(int i = 0; i < num_split - 1; i++) {
		char cp_no[MAX_BUF];
		char *dst_path = strdup(src_path);
		sprintf(cp_no, ".%d", i);
		strcat(dst_path, cp_no);

		if(!(dst = fopen(dst_path, "wb"))) {
			perror("fopen, dst");
			exit(1);
		}

		
		int rest_size = split_size;
		int cnt;
		while(rest_size > 0) {
			cnt = (rest_size > MAX_BUF) ? MAX_BUF : rest_size;
			fread(buf, sizeof(char), cnt, src);
			fwrite(buf, sizeof(char), cnt, dst);
			rest_size -= cnt;
		}

		fclose(dst);
	}

	char cp_no[MAX_BUF];
	char* dst_path = strdup(src_path);
	sprintf(cp_no, ".%d", num_split - 1);
	strcat(dst_path, cp_no);

	if(!(dst = fopen(dst_path, "wb"))) {
		perror("fopen, dst");
		exit(1);
	}

	int cnt;
	while((cnt = fread(buf, sizeof(char), MAX_BUF, src)) > 0) {
		fwrite(buf, sizeof(char), cnt, dst);
	}

	fclose(dst);
	fclose(src);

	return 0;
}
