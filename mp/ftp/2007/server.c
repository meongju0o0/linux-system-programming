#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

int serverSock;

void closeServer() {
	close(serverSock);
	exit(0);
}

void main() {
	int clientSock, clientAddrLen, one = 1;
	struct sockaddr_in serverAddr, clientAddr;
	pid_t pid;

	signal(SIGINT, closeServer);

	serverSock = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(7038);

	bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	listen(serverSock, 5);
	
	while (1) {
		clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &clientAddrLen);

		pid = fork();
		
		if (pid == 0) {
			int n;
			FILE *fp;
			char filename[32] = {0}, buf[256] = {0};

			recv(clientSock, filename, 32, 0);
			sprintf(buf, "./%s", filename);
			fp = fopen(buf, "w");

			while (1) {
				memset(buf, 0, 256);
				if ((n = recv(clientSock, buf, 256, 0)) <= 0)
					break;
				fwrite(buf, 1, n, fp);
			}

			fclose(fp);
			close(clientSock);
		}
	}
}
