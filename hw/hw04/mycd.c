#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]: mycd
[Description]: 현재 프로세스 내에서 디렉토리 이동
[Input]: 디렉토리명
[Output]: None
[Calls]:
	fprintf(FILE* fp, char* format, ...)
	perror(char* str)
	exit(int EXIT_CODE)
	chdir(char* pathname)
*/

int main(int argc, char *argv[]) {
	if(argc != 2) { // argument가 2개가 아니면
		fprintf(stderr, "Usage: %s dirname\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}
	if(!chdir(argv[1])) { // argv[1]으로 디렉토리 이동에 실패하면
		perror("chdir"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
