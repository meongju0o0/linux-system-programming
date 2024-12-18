#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>

void main(int argc, char *argv[]) {
	int clientSock, n;
	struct sockaddr_in serverAddr;
	struct hostent *hp;
	FILE *fp;
	char buf[256] = {0};

	clientSock = socket(AF_INET, SOCK_STREAM, 0);

	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	if (isdigit(argv[1][0])) {
		serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		if (hp = gethostbyname(argv[1])) {
			memcpy(&serverAddr.sin_addr, hp->h_addr, hp->h_length);
		}
		else {
			fprintf(stderr, "Unknown Host: %s\n", argv[1]);
			exit(1);
		}
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7038);

	connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	send(clientSock, argv[2], 32, 0);
	sprintf(buf, "./%s", argv[2]);
	fp = fopen(buf, "r");

	while ((n = fread(buf, 1, 256, fp)) > 0) 
		send(clientSock, buf, n, 0);
	
	fclose(fp);
	close(clientSock);
}
