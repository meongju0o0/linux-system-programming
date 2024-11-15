#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]: mysln
[Description]: argv[1]로 주어진 파일의 symbolic link를 argv[2] 이름으로 생성
*/

int main(int argc, char *argv[]) {
	if(argc != 3) { // 만약 argument가 3개가 아니라면
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(!symlink(argv[1], argv[2])) { // symbolic link를 생성하지 못한 경우
		// argv[1]의 symbolic link를 argv[2] 이름으로 생성
		perror("link"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
