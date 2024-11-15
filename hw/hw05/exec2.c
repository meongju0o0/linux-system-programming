#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	char *argv[] = {"mycp", "exec.c", "exec2.c", NULL};
	
	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		if(execl("../hw03/mycp", "mycp", "exec.c", "exec2.c", NULL) < 0) {
			perror("execl");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		if(execv("../hw03/mycp", argv) < 0) {
			perror("execv");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		if(execlp("../hw03/mycp", "mycp", "exec.c", "exec2.c", NULL) < 0) {
			perror("execlp");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	return 0;
}
