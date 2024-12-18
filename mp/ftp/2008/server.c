#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
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

int main() {
	pid_t pid;
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

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

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);

	while(1) {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); 
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}
		if ((pid = fork()) < 0) {
			perror("fork");
			close(newSockfd);
			exit(1);
		}
		else if(pid == 0) {
			close(Sockfd);
			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
				perror("read");
				exit(1);
			}
			
			char *args[64];
			int i = 0;
			args[i] = strtok(msg.data, " ");
			while (args[i] != NULL && i < 63) {
				i++;
				args[i] = strtok(NULL, " ");
			}
			args[i] = NULL;

			pid_t exec_pid = fork();
			if (exec_pid < 0) {
				perror("fork");
				exit(1);
			}
			else if (exec_pid == 0) {
				if (execvp(args[0], args) < 0) {
					perror("execvp");
					exit(1);
				}
			}
			else {
				waitpid(exec_pid, NULL, 0);
			}

			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
				perror("write");
				exit(1);
			}
			printf("Replied.\n");

			usleep(10000);
			close(newSockfd);
			exit(0);
		}
		else {
			close(newSockfd);
		}
	}

	return 0;
}
