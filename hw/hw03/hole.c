#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/*
[Program name]: hole
[Description]:
	- 1024 byte짜리 파일(file.hole)을 생성
	- 1024번째 위치에는 NUL을 입력
[Input]:
	None
[Output]:
	- file.hole
[Calls]:
	- perror(char* error_msg)
	- exit(int EXIT_CODE)
	- lseek(int fd, off_t offset, int whence)
	- write(int fd, void* buf, size_t nbytes)
	- close(int fd)
*/

int main() {
	int fd; // file descripter 변수 선언
	char ch = '\0'; // "file.hole" 1024번지에 입력할 character

	if((fd = creat("file.hole", 0400)) < 0) { // file.hole을 -r-------- mode로 생성 및 접근
		perror("creat"); // file descripter 생성 실패 시 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(lseek(fd, 1023, SEEK_SET) < 0) { // file descripter가 가르키는 문자의 위치를 SEEK_SET으로부터 1024번째로 옮김 
		perror("lseek"); // 실패 시, 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	write(fd, &ch, 1); // file descripter가 가르키는 위치에 '\0' 입력

	close(fd); // file descripter 종료

	return 0;
}
