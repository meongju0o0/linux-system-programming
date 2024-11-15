#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]: mymv
[Description]: argv[1]의 파일명의 argv[2]로 변경
[Input]: 기존 파일명, 바꿀 파일명
[Output]: 바꾸어진 파일명
[Calls]:
	fprintf(FILE* fp, char* format, ...)
	perror(char* str)
	exit(int EXIT_CODE)
*/

int main(int argc, char *argv[]) {
	if(argc != 3) { // argument가 3개가 아니라면
		fprintf(stderr, "Usage: %s old new\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(!rename(argv[1], argv[2])) { // argv[1]의 이름을 argv[2]로 바꾸지 못한다면
		// argv[1]를 argv[2]로 변경
		perror("rename"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
