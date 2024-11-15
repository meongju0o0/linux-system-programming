#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 256

/*
[Program Name]: tcp2
[Description]: src로부터 읽은 문자를 dst에 출력
[Input]: char* src, char* dst
[Output]: 복사된 파일인 dst
[Calls]:
	- fprintf(FILE* fp, char* format, ...)
	- exit(int EXIT_CODE)
	- fgets(char* buffer, int count, FILE* fp)
	- fputs(char* buffer, FILE* fp)
	- fclose(FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *src, *dst; // src, dst file descripter pointer 선언
	char buf[MAX_BUF]; // 읽은 파일의 텍스트를 저장할 버퍼 선언

	if(argc != 3) { // argument의 개수가 3개가 아닌 경우
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src = fopen(argv[1], "rt"))) { // argv[1]을 "rt"모드로 열지 못한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(dst = fopen(argv[2], "wt"))) {
		perror("fopen");
		exit(1);
	}

	while(fgets(buf, sizeof(buf), src)) { // src fp로부터 문자열 단위로 buf만큼 읽은
		fputs(buf, dst); // 읽은 텍스트를 dst에 출력
	}

	fclose(src); // src file descripter 종료
	fclose(dst); // dst file descripter 종료

	return 0;
}
