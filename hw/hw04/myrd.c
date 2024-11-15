#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]: myrd
[Description]: 주어진 경로의 디렉토리 삭제
[Input]: 디렉토리명
[Output]: 삭제된 디렉토리
[Calls]:
	rmdir(char* pathname)
	fprintf(FILE* fp, char* format, ...)
	perror(char* err_msg)
	exit(int EXIT_CODE)
*/

int main(int argc, char *argv[]) {
	if(argc != 2) { // argument가 2개가 아닌 경우
		fprintf(stderr, "Usage: %s dirname\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(rmdir(argv[1])) { // argv[1]이라는 디렉토리를 삭제하지 못한 경우
		// argv[1] 디렉토리 삭제
		perror("rmdir"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
