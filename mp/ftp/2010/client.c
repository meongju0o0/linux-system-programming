#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void main(int argc, char *argv[]) {
	int clientSock;
	struct sockaddr_in serverAddr;
	struct hostent *hp;

	clientSock = socket(AF_INET, SOCK_DGRAM, 0);

	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	
	if (isdigit(argv[1][0])) {
		serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		if (hp = gethostbyname(argv[1])) {
			memcpy(&serverAddr.sin_addr, hp->h_addr, hp->h_length);		
		}
		else {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7038);

	sendto(clientSock, argv[2], strlen(argv[2]), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	close(clientSock);
}
