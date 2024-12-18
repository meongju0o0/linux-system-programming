#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>

#define MAX_BUF 256

void main() {
	pid_t pid;
	int serverSock, clientSock, clientAddrLen;
	struct sockaddr_in serverAddr, clientAddr;
	
	serverSock = socket(AF_INET, SOCK_STREAM, 0);

	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(7038);

	bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	listen(serverSock, 5);
	
	while (1) {
		clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &clientAddrLen);
		
		pid = fork();

		if (pid == 0) {
			int fd;
			char id[32], buf[MAX_BUF];
			struct timeval tv;

			recv(clientSock, id, MAX_BUF, 0);
			sprintf(buf, "./%s", id);
		
			fd = open(buf, O_WRONLY | O_APPEND | O_CREAT, 0644);
		
			gettimeofday(&tv, NULL);
			sprintf(buf, "%lu\n", tv.tv_sec);			
			write(fd, buf, strlen(buf));

			close(fd);			
		}
	}
}
