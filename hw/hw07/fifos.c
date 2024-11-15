#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifo.h"

#define TRUE 1
#define FALSE 0

/*
[Program Name]: fifos
[Description]:
	- fifo를 활용하여 클라이언트 프로세스로부터의 메시지 도착을 반복적으로 대기
[Input]: None
[Output]: 콘솔에 클라이언트로부터 받은 메시지를 출력
[Calls]:
	- sprintf(char *dst, char *format, ...)
	- printf(char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- mkfifo(const char *pathname, mode_t mode)
	- signal(int signum, sighandler_t handler)
	- open(const char *pathname, int flags)
	- write(int fd, const void *buf, size_t count)
	- close(int fd)
	- remove(const char *pathname)
*/

void sig_int_handler(int signum) {
	/*
	[Function Name]: void sig_int_handler(int signum)
	[Description]: SIGINT 시그널 발생 시, SERV_FIFO 파일 삭제 후 프로세스 종료
	[Input]: signum
	[Output]: None
	[Calls]:
		- remove(const char *pathname)
	*/
	if(remove(SERV_FIFO) < 0) { // SERV_FIFO 파일 삭제
		perror("remove"); // 삭제 실패 시 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	exit(0); // 프로세스 종료
}

int main() {
	int fd, cfd, n; // fd(server file descripter), cfd(client file descripter), n(number of objects) 변수 선언
	MsgType msg; // 메시지를 저장할 구조체

	if(signal(SIGINT, sig_int_handler) == SIG_ERR) { // SIGINT 발생 시 sig_int_handler 실행 등록
		perror("signal"); // 등록 실패 시 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(mkfifo(SERV_FIFO, 0600) < 0) { // .fifo 파일 생성(메시지 입출력, mode: prw-------)
		/*
		errno: 마지막 System Call의 오류 원인을 저장하는 전역 변수
		EEXIST: 이미 해당 이름의 파일이 존재한다는 에러 코드
		*/
		if(errno != EEXIST) { // 이미 .fifo 파일이 존재한다는 에러를 제외한 에러인 경우
			perror("mkfifo"); // 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}

	if((fd = open(SERV_FIFO, O_RDWR)) < 0) { // 서버에서 읽기로 사용할 .fifo 파일 열기
		perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	while(TRUE) { // 반복적으로 클라이언트의 요청 처리
		if((n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 읽기
			if(errno == EINTR) { // 읽기 작업이 SIGINT 등의 시그널로 종료된 경우
				continue; // 반복문의 시작으로 이동
			}
			else {
				perror("read"); // 에러 메시지 출력
				exit(1); // 비정상 종료
			}
		}
		printf("Received request: %s.....", msg.data); // 메시지("This is a request from {getpid()}")를 콘솔에 출력

		if((cfd = open(msg.returnFifo, O_WRONLY)) < 0) { // 클라이언트에서 생성한 .fifo{getpid()} 파일 열기
			perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		sprintf(msg.data, "This is a reply from %d.", getpid()); // 클라이언트로 보낼 메시지를 msg.data에 복사
		write(cfd, (char *)&msg, sizeof(msg)); // 생성된 응답 메시지를 클라이언트의 FIFO로 씀
		close(cfd); // 클라이언트의 FIFO 파일 닫기
		printf("Replied.\n"); // 응답을 완료했음을 콘솔에 출력
	}

	return 0;
}
