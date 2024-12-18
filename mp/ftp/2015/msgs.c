#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int serverSock;

void closeServer() {
	close(serverSock);	
	exit(0);
}

void main() {
	int clientSock, clientAddrLen;
	struct sockaddr_in serverAddr, clientAddr;
	pid_t pid;

	signal(SIGINT, closeServer);

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
			int n;
			FILE* fp;
			char id[32], path[64], msg[256];
			
			memset(id, 0, 32);
			memset(path, 0, 64);
			n = recv(clientSock, id, 32, 0);
			id[n] = '\0';
			sprintf(path, "./%s", id);
		
			fp = fopen(path, "wt");

			while (1) {
				memset(msg, 0, 256);
				if ((n = recv(clientSock, msg, sizeof(msg), 0)) <= 0) 
					break;
				
				printf("%s> %s", id, msg);
				fflush(stdout);
				fwrite(msg, 1, n, fp);
				fflush(fp);

				if (!strcmp(msg, "quit\n"))
					break;
			}

			close(clientSock);
			fclose(fp);
		}
	}

	close(serverSock);
}
