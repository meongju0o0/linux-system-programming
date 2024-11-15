#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]: myrm 
[Description]: arguemnt로 주어진 경로 상의 파일을 삭제
[Input]: 경로명
[Output]: 삭제된 파일
[Calls]:
	fprintf(FILE* fp, char* format, ...)
	perror(char* str)
	remove(char* pathname)
*/

int main(int argc, char *argv[]) {
	if(argc == 1) { // argument의 수가 1인 경우
		fprintf(stderr, "Usage: %s filename\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	for(int i = 1; i < argc; i++) {
		if(!remove(argv[i])) { // argv[i]를 삭제하지 못한 경우
			perror("remove"); // 오류 출력
			exit(1); // 비정상 종료
		}
	}

	return 0;
}
