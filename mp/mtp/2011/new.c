#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	DIR *dp;
	struct dirent *dept;
	struct stat statbuf;

	int timeinfos[100];
	char *filenames[100];
	int num_dirs = 0;
	
	if(!(dp = opendir("."))) {
		perror("opendir");
		exit(1);
	}

	while(dept = readdir(dp)) {
		if(lstat(dept->d_name, &statbuf) == -1) {
			perror("lstat");
		}
		else {
			timeinfos[num_dirs] = statbuf.st_mtime;
			filenames[num_dirs] = dept->d_name;
			num_dirs++;
		}
	}

	int max_date_idx = 0;
	unsigned long long max_st_mtime = 0;

	for(int i = 0; i < num_dirs; i++) {
		if(timeinfos[i] > max_st_mtime) {
			max_st_mtime = timeinfos[i];
			max_date_idx = i;
		}
	}

	printf("%s\n", filenames[max_date_idx]);

	closedir(dp);

	return 0;
}
