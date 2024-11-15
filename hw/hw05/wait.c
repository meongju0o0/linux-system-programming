#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
[Program Name]: wait
[Description]: 자식 프로세스에서 콘솔을 먼저 사용한 후, 부모 프로세스는 자식 프로세스가 종료되면 콘솔 사용
[Input]: None
[Output]: 자식 프로세스와 부모 프로세스에서 콘솔에 출력한 문자열
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	fork()
	sleep(unsigned int seconds)
	wait(int *status)
*/

int main() {
	pid_t pid;
	int status;

	if((pid = fork()) < 0) { // 자식 프로세스를 생성하지 못한 경우
		perror("fork");
		exit(1);
	}
	else if(pid == 0) { // 자식 프로세스인 경우
		printf("I'm a child\n");
		sleep(2);
	}
	else { // 부모 프로세스인 경우
		wait(&status); // 자식 프로세스가 종료되기까지 대기
		printf("A child killed with %d status\n", status);
	}

	return 0;
}
