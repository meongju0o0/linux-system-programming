#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
[Program Name]: chmod
[Description]: "bar"와 "foo"의 파일 모드를 변경
[Input]: "bar", "foo"
[Output]: 파일 모드가 변경된 "bar"와 "foo"
[Calls]:
	stat(char* pathname, struct stat *buf)
	chmod(char* pathname, mode_t mode)
	perror(char* err_msg)
	exit(int EXIT_CODE)
*/

int main() {
	struct stat statbuf;

	if(stat("bar", &statbuf) < 0) { // "bar" 파일의 stat을 불러오지 못한 경우
		perror("stat, bar"); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0) { // "bar" 파일의 권한을 변경하지 못하면
		// "bar" 파일의 IWGRP 권한 삭제, ISUID 권한 추가
		perror("chmod, bar"); // 오류 출력
		exit(1); // 비정상 종료
	}

	if(chmod("foo", (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) { // "foo" 파일의 권한을 변경하지 못하면
		// "foo" 파일에 IRUSR, IWUSR, IRGRP, IROTH 권환 추가
		perror("chmod, foo"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
