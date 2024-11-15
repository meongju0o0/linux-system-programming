#include <stdio.h> // standard input/output 함수 사용을 위한 라이브러리
#include <stdlib.h> // exit(int status) 함수 사용을 위한 라이브러리
#include <ctype.h> // to_upper(char ch) 함수 사용을 위한 라이브러리

#define MAX_BUF 1024

/*
upper.c:
	return type: integer(exit status)
	function name: main
	parameter: arguments(file name)
Description:
	argument로 입력 받은 파일의 문자들을 모두 대문자로 변환
Input:
	파일명
Call By:
	shell or interfaces
Calls:
	- printf(char* str, ...)
	- perror(char* str)
	- fgetc(FILE* fp)
	- toupper(char ch)
Example:
	$ ./upper upper_src
*/

int main(int argc, char *argv[]) {
	FILE *fp; // file descripter의 포인터 저장 변수

	int ch;

	if(argc != 2) { // argument가 주어지지 않은 경우(파일명이 주어지지 않은 경우)
		printf("Usage:%s filename\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(!(fp = fopen(argv[1], "rt"))) { // argument로 주어진 파일을 읽을 수 없는 경우
		perror("fopen"); // 오류 출력
		exit(1); // 비정상 종료
	}

	while((ch = fgetc(fp)) != EOF) { // 주어진 파일에서 문자를 하나씩 읽음 -> EOF 반환 시 중료
		putchar(toupper(ch)); // 대문자로 변환 후 문자 하나 출력
	}

	fclose(fp); // file descripter 종료

	return 0;
}
