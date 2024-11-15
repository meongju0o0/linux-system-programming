#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

/*
[Program Name]: mycp
[Description]: src.txt의 내용을 dst.txt에 복사
[Input]: char* src.txt, char* dst.txt
[Output]: src.txt에서 복사된 dst.txt 파일
[Calls]:
    - printf(char* format, ...)
    - exit(int EXIT_CODE)
    - perror(char* str)
	- open(char* file_name, int oflag)
	- read(int fd, char* buffer, size_t buf_size)
	- write(int fd, char* buffer, size_t count)
	- close(FILE* fp)
*/

int main(int argc, char *argv[]) {
	int fd1, fd2, cnt; // file descripter, count 변수 선언
	char buf[MAX_BUF]; // buffer 변수 선언

	if(argc != 3) { // arguement가 3개가 아닌 경우
		printf("Usage: %s source destination\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0) { // argv[1]을 열지 못한 경우(-1 반환)
		perror("open"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) { // argv[2]를 열지 못한 경우(-2 반환)
		perror("open"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	while(cnt = read(fd1, buf, sizeof(buf))) { // fd1에서 buf만큼 object가 읽히는 동안
		write(fd2, buf, cnt); // object의 수만큼 fd2가 지칭하는 파일에 쓰기
	}

	close(fd1); // fd1 file descripter 종료
	close(fd2); // fd2 file descripter 종료

	return 0;
}

