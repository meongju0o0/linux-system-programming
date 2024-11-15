#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

/*
[Program Name]: wait
[Description]:
	- 부모 프로세스에서 비동기적으로 SIGCHLD 시그널을 대기
	- 자식 프로세스가 종료되어 SIGCHLD 시그널이 발생되면 부모 프로세스는 콘솔에 문자열을 출력
[Input]: None
[Output]: 자식 프로세스와 부모 프로세스에서 콘솔에 출력한 문자열
[Calls]:
	- perror(char* err_msg)
	- printf(char* format, ...)
	- fork()
	- sleep(unsigned int seconds)
	- signal(int signum, sighandler_t handler)
*/

void sigchld_handler(int signum) {
	/*
	[Function Name]: void sigchld_handler(int signum)
	[Description]: 자식 프로세스 중료 시, SIGCHLD 시그널을 받아 비동기적으로 이 함수를 호출
	[Input]: int signum(시그널 번호)
	[Output]: 콘솔에 출력되는 문자열
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- printf(char *format, ...)
	*/
	printf("A child killed\n");
	printf("I'm a parent\n");
}

int main() {
	pid_t pid;

	signal(SIGCHLD, sigchld_handler);

	if((pid = fork()) < 0) { // 자식 프로세스를 생성하지 못한 경우
		perror("fork");
		exit(1);
	}
	else if(pid == 0) { // 자식 프로세스인 경우
		printf("I'm a child\n");
		sleep(2);
	}
	else { // 부모 프로세스인 경우
		while(TRUE);
	}

	return 0;
}
