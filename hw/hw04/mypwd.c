#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUF 256

/*
[Program Name]: mypwd
[Description]: 현재 위치의 절대경로 출력
[Input]: 파일명위치
[Output]: 현재 위치의 절대경로를 콘솔에 출력
[Calls]:
	getcwd(char* buf, int buf_size)
	perror(char* err_msg)
	exit(int EXIT_CODE)
	printf(char* str, ...)
*/

int main() {
	char buf[MAX_BUF];

	if(!getcwd(buf, sizeof(buf))) { // 현재 디렉토리의 절대 경로를 얻어오지 못하면
		perror("getcwd"); // 오류 출력
		exit(1); // 비정상 종료
	}

	printf("%s\n", buf); // 현재 디렉토리의 절대 경로 출력

	return 0;
}
