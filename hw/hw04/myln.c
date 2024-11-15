#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]: myln
[Description]: argument로 주어진 파일의 hard link 생성
[Input]: original path name, hard link's file name
[Output]: hard link 파일
[Calls]:
	fprintf(FILE* fp, char* str, ...)
	perror(char* str)
	exit(int EXIT_CODE)
	link(char* src_path_name, char* dst_path_name)
*/

int main(int argc, char *argv[]) {
	if(argc != 3) { // argument가 잘못 주어진 경우
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(!link(argv[1], argv[2])) { // argv[1]의 링크 argv[2]를 생성하지 못한 경우
		perror("link"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
