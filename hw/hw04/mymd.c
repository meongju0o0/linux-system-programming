#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
[Program Name]: mymd1
[Description]: 주어진 이름의 디렉토리 생성, 권한: 0755
[Input]: 디렉토리명
[Output]: 생성된 디렉토리
[Calls]:
	mkdir(char* pathname, mode_t mode)
	fprintf(FILE* fp, char* format, ...)
	perror(char* err_msg)
	exit(EXIT_CODE)
*/

int main(int argc, char *argv[]) {
	if(argc != 2) { // argument가 2개가 아닌 경우
		fprintf(stderr, "Usage: %s dirname\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(mkdir(argv[1], 0755)) { // 0755 mode로 argv[1] 디렉토리를 생성하지 못한 경우
		// argv[1] 이름의 디렉토리를 0755 mode로 생성
		perror("mkdir"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
