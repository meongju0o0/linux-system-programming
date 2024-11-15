#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int time = atoi(argv[1]);
	char *msg = argv[2];

	pid_t pid;
	
	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		sleep(time);
		int status;
		char command[100];
		sprintf(command, "banner %s", msg);
		if((status = system(command)) < 0) {
			perror("system");
			exit(1);
		}
	}
	else {
		wait(NULL);
	}

	return 0;
}
