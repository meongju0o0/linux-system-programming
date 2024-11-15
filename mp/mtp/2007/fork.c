#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		if(execl("./mywc", "mywc", "mywc.c", NULL) < 0) {
			perror("execl");
			exit(1);
		}
	}
	else {
		wait(NULL);
	}

	return 0;
}
