#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

int Sockfd;

void CloseServer() {
	close(Sockfd);
	printf("\nTCP Server exit.....\n");
	exit(0);
}

void SigChldHandler(int signo) {
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void cleanup_and_exit(int sockfd, int fd, int status) {
	if (sockfd >= 0) close(sockfd);
	if (fd >= 0) close(fd);
	exit(status);
}

int main() {
	char file_name[100];
	int fd = -1;
	pid_t pid;
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	PacketType pkt;

	signal(SIGINT, CloseServer);
	signal(SIGCHLD, SigChldHandler);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		cleanup_and_exit(Sockfd, -1, 1);
	}
	
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);

	while (1) {
		newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
		if (newSockfd < 0) {
			perror("accept");
			cleanup_and_exit(-1, -1, -1);
		}

		if ((pid = fork()) < 0) {
			perror("fork");
			close(newSockfd);
			cleanup_and_exit(-1, -1, -1);
		}
		else if (pid == 0) {
			close(Sockfd);

			if ((n = read(newSockfd, (char *)&pkt, sizeof(pkt))) < 0) {
				perror("read");
				cleanup_and_exit(newSockfd, fd ,1);
			}

			strcpy(file_name, pkt.buf);

			if (access(file_name, F_OK) == 0) {
				fprintf(stderr, "Error: File %s already exists\n", file_name);
				cleanup_and_exit(newSockfd, -1, 1);
			}

			if ((fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC), 0666) < 0) {
				perror("open");
				cleanup_and_exit(newSockfd, -1, 1);
			}

			while (1) {
				if ((n = read(newSockfd, (char *)&pkt, sizeof(pkt))) < 0) {
					perror("read");
					cleanup_and_exit(newSockfd, fd, 1);
				}

				if (write(fd, pkt.buf, pkt.count) < 0) {
					perror("write");
					cleanup_and_exit(newSockfd, fd, 1);
				}

				if (pkt.count < sizeof(pkt.buf)) {
					break;
				}
			}

			close(fd);
			close(newSockfd);
			printf("File received: %s\n", file_name);
			exit(0);
		}

		else {
			close(newSockfd);
		}
	}

	return 0;
}
