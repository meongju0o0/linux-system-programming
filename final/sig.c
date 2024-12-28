#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;

void sigusr_handler(int signum) {
	exit(0);
}

void sigchld_handler(int signum) {
	exit(0);
}

int main() {
	signal(SIGUSR1, sigusr_handler);
	signal(SIGCHLD, sigchld_handler);

	pid_t pid = fork();

	if (pid == 0) {
		while(1) count++;
	}
	else {
		while(1) {
			sleep(1);
			kill(pid, SIGUSR1);
		}
	}

	return 0;
}
