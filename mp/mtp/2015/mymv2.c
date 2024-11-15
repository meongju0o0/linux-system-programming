#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	const int num_files = argc - 2;

	for(int i = 1; i <= num_files; i++) {
		char pathname[100];
		sprintf(pathname, "%s/%s", argv[argc - 1], argv[i]);
		printf("%s\n", pathname);
		
		if(rename(argv[i], pathname)) {
			perror("rename");
			exit(1);
		}
	}

	return 0;
}
