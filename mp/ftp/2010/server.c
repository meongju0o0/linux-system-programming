#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int serverSock;

void closeServer() {
	close(serverSock);
	
	exit(0);
}

void main() {
	int n, clientAddrLen;
	struct sockaddr_in serverAddr, clientAddr;
	char msg[200], buf[256];
	pid_t pid;
	FILE *fp;

	signal(SIGINT, closeServer);

	serverSock = socket(AF_INET, SOCK_DGRAM, 0);

	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(7038);

	bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	while (1) {
		memset(msg, 0, sizeof(msg));
		if ((n = recvfrom(serverSock, (char *)msg, sizeof(msg), 0, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0)
			exit(1);

		pid = fork();

		if (pid == 0) {
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%s %s\n", inet_ntoa(clientAddr.sin_addr), msg);
			printf("%s", buf);

			fp = fopen("log", "at");
			fwrite(buf, 1, strlen(buf), fp);
			fclose(fp);
		}
	}
}
