#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "synclib.h"

#define MAX_BUF 128
#define NLOOPS 5

/*
[Program Name]: sync
[Description]:
	- 파이프를 사용하여 부모 스레드와 자식 프로세스 간 통신
	- 부모 프로세스에서 자식 프로세스에 메시지 전송
		- 자식 프로세스는 부모 프로세스가 메시지를 모두 전송할 때까지 대기
	- 자식 프로세스에서 부모 프로세스에 메시지 전송
		- 부모 프로세스는 자식 프로세스가 메시지를 모두 전송할 때까지 대기
[Input]: None
[Output]:
	- 부모 프로세스는 자식 프로세스에게 메시지를 전송한다는 것을 콘솔에 출력
	- 메시지 전송 완료 후, 자식 프로세스의 메시지를 대기한다는 것을 콘솔에 출력

	- 자식 프로세스는 부모 프로세스에게 메시지를 전송한다는 것을 콘솔에 출력
	- 메시지 전송 완료 후, 부모 프로세스의 메시지를 대기한다는 것을 콘솔에 출력
[Calls]:
	- exit(int exit_code)
	- perror(char *err_msg)
	- printf(char *format, ...)
	- TELL_WAIT()
	- TELL_CHILD()
	- WAIT_CHILD()
	- WAIT_PARENT()
	- TELL_PARENT()
*/

int main() {
	pid_t pid;

	TELL_WAIT(); // 파이프 생성
	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid > 0) { // 부모 프로세스 이면
		for(int i = 0; i < NLOOPS; i++) { // 5회 반복
			TELL_CHILD(); // 자식 프로세스에게 메시지 "p" 전송
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); // 자식 프로세스로부터 메시지 대기
		}
	}
	else {
		FILE* fp = fopen("./.fifo", "wt+");
		for(int i = 0; i < NLOOPS; i++) {
			WAIT_PARENT(); // 부모 프로세스로부터 메시지 대기
			printf("Child: Wait for parent to tell\n");
			printf("Child: Tell to parent\n");
			TELL_PARENT(); // 부모 프로세스에게 메시지 "c" 출력
		}
	}

	return 0;
}
