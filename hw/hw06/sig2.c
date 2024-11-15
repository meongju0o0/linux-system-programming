#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

void sig_usr_handler(int signum) {
	if(signum == SIGUSR1) {
		printf("Received a SIGUSR1 signal\n");
	}
	else if(signum == SIGUSR2) {
		printf("Received a SIGUSR2 signal\n");
	}
	else {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main() {
	if(signal(SIGUSR1, sig_usr_handler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	if(signal(SIGUSR2, sig_usr_handler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	while(TRUE) {
		pause();
	}
	
	return 0;
}
