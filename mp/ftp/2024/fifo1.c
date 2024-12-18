#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 256

void main() {
	int fd;
	char buf[MAX_BUF];

	mkfifo("./myfifo", 0600);

	fd = open("./myfifo", O_RDWR);

	read(fd, buf, MAX_BUF);
	printf("FIFO1: Received a message: %s\n", buf);

	strcpy(buf, "Hi");
	write(fd, buf, MAX_BUF);
	printf("FIFO1: Sent a message: %s\n", buf);

	close(fd);
	remove("./myfifo");
}
