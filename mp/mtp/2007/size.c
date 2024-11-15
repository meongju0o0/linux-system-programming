#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	DIR *dp;
	struct dirent *dept;
	struct stat statbuf;

	int tineinfos[100];
	char *filenames[100];
	
	if(!(dp = opendir("."))) {
		perror("opendir");
		exit(1);
	}

	while(dept = readdir(dp)) {
		if(lstat(dept->d_name, &statbuf) == -1) {
			perror("lstat");
		}
		else {
			if(statbuf.st_size > atoi(argv[1])) {
				printf("%s\n", dept->d_name);
			}
		}
	}

	closedir(dp);
	return 0;
}

