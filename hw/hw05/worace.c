#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
[Program Name]: race
[Description]: wait()를 통해 STDOUT에 대한 race condition 방지. 콘솔에 출력되는 문자열을 확인
[Input]: None
[Output]: 자식 프로세스가 콘솔에 문자열을 출력한 후, 부모 프로세스가 콘솔에 문자열 출력
[Calls]:
	perror(char* err_msg)
	putc(char ch, FILE* fp)
	setbuf(FILE* fp, char* buffer)
	fork()
	wait(int *_Nullable wstatus)
*/

void char_at_a_time(char *str) {
	/*
	[Function Name]: void char_at_a_time(char *str)
	[Description]:
		- 문자열의 문자를 하나씩 출력
		- 이때, OS CPU 자원 할당의 time quantum을 뛰어넘기 위해 dummy operation을 수행
	[Input]: 문자열
	[Output]: 콘솔에 입력받은 문자열을 출력
	[Calls]:
		setbuf(FILE* fp, char* buffer)
		putc(char ch, FILE* fp)
	*/
	char *ptr;
	int c, i;

	setbuf(stdout, NULL); // stdout에 대한 버퍼링 해제, 버퍼링 없이 콘솔에 문자열 출력
	for(ptr = str; c = *ptr; ptr++) { // 문자열의 각 문자에 대해
		for(i = 0; i < 999999; i++); // OS CPU 자원 할당의 time quantum을 뛰어넘기 위한 dummy operation
		putc(c, stdout); // 문자를 콘솔에 출력
	}
}

int main() {
	pid_t pid; // 자식 process id를 저장하기 위한 변수

	if((pid = fork()) < 0) { // 프로세스 생성에 실패하면
		perror("fork"); // 에러 출력
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스 이면
		char_at_a_time("output from child\n"); // char_at_a_time()을 통해 콘솔에 문자열 출력
	}
	else { // 부모 프로세스 이면
		wait(NULL); // 자식 프로세스가 종료될 때까지 대기
		char_at_a_time("output from parent\n"); // char_at_a_time()을 통해 콘솔에 문자열 출력
	}

	return 0;
}
