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

        fd = open("./myfifo", O_RDWR);

		strcpy(buf, "Hello");
        write(fd, buf, MAX_BUF);
        printf("FIFO2: Sent a message: %s\n", buf);

        read(fd, buf, MAX_BUF);
        printf("FIFO2: Received a message: %s\n", buf);

        close(fd);
}
