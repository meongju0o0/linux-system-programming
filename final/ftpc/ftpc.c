#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

int main(int argc, char *argv[]) {
	int fd;

	int sockfd, n;
	struct sockaddr_in servAddr;
	struct hostent *hp;
	PacketType pkt;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <server_address> <file_name>\n", argv[0]);
		exit(1);
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0])) {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		if ((hp = gethostbyname(argv[1])) == NULL) {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}

	strcpy(pkt.buf, argv[2]);

	if (write(sockfd, (char *)&pkt, sizeof(pkt)) < 0) {
		perror("write");
		exit(1);
	}

	fd = open(argv[2], O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	while (1) {
		pkt.count = read(fd, pkt.buf, sizeof(pkt.buf));

		if (pkt.count < 0) {
			perror("read");
			exit(1);
		}

		if (write(sockfd, (char *)&pkt, sizeof(pkt)) < 0) {
			perror("write");
			exit(1);
		}

		if (pkt.count < sizeof(pkt.buf)) {
			break;
		}
	}

	return 0;
}
