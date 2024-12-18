#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char *argv[]) {
	int fd;
	
	fd = open("./myfifo", O_RDWR);

	write(fd, argv[1], strlen(argv[1]));

	close(fd);
}
