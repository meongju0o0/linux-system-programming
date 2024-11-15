#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

/*
[Program Name]: sig1
[Description]: SignalHandler를 활용하여 SIGINT 시그널 발생 시 작성된 기능 수행 후 종료
[Input]: None
[Output]: SIGINT 발생시 콘솔에 문자열 출력
[Calls]:
	- printf(char* format, ...)
	- exit(int EXIT_CODE)
	- signal(int signum, sighandler_t handler)
*/

void SigIntHandler(int signum) {
	/*
	[Function Name]: void SigIntHandler(int signum)
	[Description]: SIGINT 시그널 발생시 작성된 문자열을 콘솔에 출력
	[Input]: signum
	[Output]: 콘솔에 출력되는 문자열
	[Calls]:
		- printf(char* format, ...)
		- exit(itn EXIT_CODE)
	*/
	printf("\nReceived a SIGINT signal\n"); // 작성된 문자열 출력
	printf("Terminate this process\n");

	exit(0); // 프로세스 종료
}

int main() {
	signal(SIGINT, SigIntHandler); // SignalHandler 등록
	
	while(TRUE) { // 무한 반복
		NULL; // 아무것도 안함
	}

	return 0;
}
