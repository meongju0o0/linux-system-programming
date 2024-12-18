#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 256

void handler(int signo) {
	remove("./myfifo");
	
	exit(0);
}

void main() {
	int fd, n;
	char msg[MAX_LEN];

	signal(SIGINT, handler);

	mkfifo("./myfifo", 0600);

	fd = open("./myfifo", O_RDWR);

	while (1) {
		memset(msg, 0, MAX_LEN);
		if ((n = read(fd, msg, MAX_LEN)) < 0) {
			exit(1);
		}

		printf("%s\n", msg);
	}
}
