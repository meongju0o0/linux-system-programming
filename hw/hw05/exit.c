#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]: exit
[Description]: atexit() 함수에 exit function 추가
[Input]: None
[Output]: 각 exit_function에서 문자열을 콘솔에 출력
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	atexit(void (*func)(void))
	exit(int EXIT_CODE)
*/

void myexit1() {
	printf("first exit handler\n");
}

void myexit2() {
	printf("second exit handler\n");
}

int main() {
	if(atexit(myexit2)) {
		perror("atexit");
		exit(1);
	}

	if(atexit(myexit1)) {
		perror("atexit");
		exit(1);
	}
	
	if(atexit(myexit1)) {
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");

	return 0;
}
