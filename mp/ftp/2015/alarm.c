#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char msg[100];

void handler(int signo) {
	execlp("banner", "banner", msg, NULL);
	
	exit(1);
}

void main(int argc, char* argv[]) {
	int count = atoi(argv[1]);
	strcpy(msg, argv[2]);

	signal(SIGALRM, handler);

	alarm(count);

	while (1) 
		pause();
}
