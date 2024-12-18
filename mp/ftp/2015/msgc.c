#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char* argv[]) {
	int clientSock;
	struct sockaddr_in serverAddr;
	struct hostent *hp;
	char msg[256];

	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	
	memset((char *)&serverAddr, 0, sizeof(serverAddr));

	if (isdigit(argv[1][0])) {
		serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		if (hp = gethostbyname(argv[1])) {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		else {
			memcpy(&serverAddr.sin_addr, hp->h_addr, hp->h_length);
		}
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7038);

	connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); 
	send(clientSock, argv[2], 32, 0); // send id

	while (1) {
		printf("MSG> ");
		fflush(stdout);
		fgets(msg, 256, stdin);
		send(clientSock, msg, strlen(msg), 0);

		if (!strcmp(msg, "quit\n"))
			break;
	}
}
