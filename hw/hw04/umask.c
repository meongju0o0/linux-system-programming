#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
[Program Name]: umask
[Description]: 지정된 파일 모드로 "bar", "foo" 생성
[Input]: None
[Output]: 지정된 파일 모드로 생성된 파일 생성
[Calls]:
	umask(mode_t cmask)
	creat(char* pathname, mode_t mode)
	perror(char* err_msg)
	exit(int EXIT_CODE)
*/

int main() {
	umask(0); // masking 하는 권한 없음
	if(creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // 지정된 모드로 파일을 생성하지 못하면
		// 모드: User: RW, Group: RW, Others: RW
		// 즉, 0333
		perror("creat, bar"); // 오류 출력
		exit(1); // 비정상 종료
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // Group: RW 금지, Others: RW 금지
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // 지정된 모드로 파일을 생성하지 못하면
		// 모드: (User: RW 즉, 3), (Group: ~RW & RW 즉, 0), (Others: ~RW & RW 즉, 0)
		// 따라서, 0300
		perror("creat, foo"); // 오류 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
