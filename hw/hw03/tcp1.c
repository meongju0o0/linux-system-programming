#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]: tcp1
[Description]: src의 내용을 dst에 복사
[Input]: 복사할 파일인 src, 복사될 파일인 dst
[Output]: 복사된 파일인 dst
[Calls]:
	- fprintf(FILE* fp, char* format, ...)
	- exit(int EXIT_CODE)
	- fgetc(FILE* fp)
	- fputc(int ch, FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *src, *dst; // file descripter pointer 선언
	int ch; // 문자를 저장할 변수 선언

	if(argc != 3) { // argument가 3개가 아닌 경우
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src = fopen(argv[1], "rt"))) { // argv[1]을 "rt"모드로 열지 못한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(dst = fopen(argv[2], "wt"))) { // argv[2]를 "wt"모드로 열지 못한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	while((ch = fgetc(src)) > 0) { // src로 부터 읽은 문자가 0 보다 많은 동안
		fputc(ch, dst); // 해당 문자 출력
	}

	fclose(src); // src file descripter 종료
	fclose(dst); // dst file descripter 종료

	return 0;
}
