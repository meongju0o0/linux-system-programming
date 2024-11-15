#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 256

/*
[Program Name]: list
[Description]: 텍스트 파일을 읽은 후 한 줄씩 줄 번호와 함께 출력
[Input]:
	읽을 텍스트 파일
[Output]:
	읽은 텍스트파일을 줄 번호와 함께 콘솔에 출력
[Calls]:
	- perror(char* err_msg)
	- printf(const char* format, ...)
	- exit(int EXIT_CODE)
	- fgets(char* str, int count, FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *fp; // file descripter pointer 선언
	char buf[MAX_BUF]; // 파일을 읽기 위한 버퍼 선언
	int line; // 줄 번호를 출력하기 위한 line 변수 선언

	if(argc != 2) { // argument가 2개가 아닌 경우
		printf("Usage: %s filename\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(fp = fopen(argv[1], "rt"))) { // fopen() 함수가 NULL을 반환한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	line = 1; // 줄 번호 출력을 위한 line 변수 초기화
	while(fgets(buf, sizeof(buf), fp)) { // 파일로부터 한 줄 씩 읽으면서
		printf("%4d: %s", line++, buf); // 읽은 줄을 번호와 함께 출력
	}

	fclose(fp); // file descripter 종료

	return 0;
}
