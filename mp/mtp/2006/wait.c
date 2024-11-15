#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int status;

	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		printf("I'm a child\n");
		sleep(2);
	}
	else {
		waitpid(pid, &status, 0);
		printf("A child killed with %d status\n", status);
	}

	return 0;
}
