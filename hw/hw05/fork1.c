#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
[Program Name]: fork1
[Description]: child process를 1개 실행 후, 각 프로세스에서 서로 다른 문자열을 콘솔에 출력
[Input]: None
[Output]: 각 프로세스에서 서로 다른 문자열을 각자 콘솔에 출력
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	exit(int EXIT_CODE)
	fork()
*/

int main() {
	int pid;

	if((pid = fork()) < 0) { // fork()가 음수를 반환하면 프로세스 생성에 실패한 상황
		perror("fork"); // 에러 출력
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 실행되고 있는 process id가 0이면 현재 프로세스는 child process
		printf("I am child.\tprocess id is %d.\tMy parent is %d.\n", getpid(), getppid());
	}
	else { // 실행 중인 process id가 0보다 크면(parent process id 이면) 현재 프로세스는 parent process
		printf("I am parent.\tprocess id is %d.\tMy child is %d.\n", getpid(), pid);
	} 

	return 0;
}
