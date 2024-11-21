#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifo.h"

/*
[Program Name]: fifoc
[Description]:
	- printf(char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- mkfifo(const char *pathname, mode_t mode)
	- sprintf(char *dst, char *format, ...)
	- open(const char *pathname, int flags)
	- write(int fd, const void *buf, size_t count)
	- close(int fd)
	- remove(const char *pathname)
*/

int main() {
	char fname[MAX_FIFO_NAME]; // returnFifo 파일 이름 저장 변수 선언
	int fd, sfd, n; // fd(client file descripter), sfd(server file descripter). n(number of objects) 변수 선언
	MsgType msg; // 메시지를 저장할 구조체

	sprintf(fname, ".fifo%d", getpid()); // 클라이언트에서 쓰기로 사용할 .fifo 파일명 지정
	if(mkfifo(fname, 0600) < 0) { // 클라이언트에서 쓰기로 사용할 .fifo{getpid()} 파일 생성(메시지 입출력, mode: prw-------)
		perror("mkfifo"); // .fifo{(getpid())} 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((fd = open(fname, O_RDWR)) < 0) { // .fifo{getpid()} 파일 열기
		perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((sfd = open(SERV_FIFO, O_RDWR)) < 0) { // 클라이언트에서 읽기로 사용할 .fifo 파일 열기
		perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	strcpy(msg.returnFifo, fname); // msg.returnFifo에 .fifo{getpid()} 복사
	sprintf(msg.data, "This is a request from %d.", getpid()); // msg.data에 문자열 복사
	write(sfd, &msg, sizeof(msg)); // 서버에 메시지 전송
	printf("Sent a request..."); // 콘솔에 메시지를 서버에 전송했다는 것을 출력

	if((n = read(fd, &msg, sizeof(msg))) < 0) { // 서버로부터의 메시지를 읽음
		perror("read"); // 메시지 읽기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Received reply: %s\n", msg.data); // 서버로부터의 메시지를 콘솔에 출력

	close(fd); // fd 종료
	close(sfd); // sfd 종료

	if(remove(fname) < 0) { // .fifo{getpid()} 파일 삭제
		perror("remove"); // 삭제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
