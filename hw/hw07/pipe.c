#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 128

/*
[Program Name]: pipe
[Description]: 
	- 파이프를 사용하여 부모 프로세스와 자식 프로세스 간 통신
	- 자식 프로세스에서 "Hello, World!\n" 문자열을 자식프로세스에게 전송
	- 부모 프로세스는 "Hello, World!\n" 문자열을 파이프를 통해 받아서 콘솔에 출력
[Input]: None
[Output]: 콘솔에 출력되는 "Hello, World!\n"
[Calls]:
	- exit(int exit_code)
	- pipe(int fd[2])
	- perror(char* err_msg)
	- printf(char* format, ...)
	- fflush(FILE* stream)
	- write(int fd, char* buf, int count)
	- close(int fd)
	- strcpy(char* dst, char* src)
*/

int main() {
	int n, fd[2];
	pid_t pid;
	char buf[MAX_BUF];

	if(pipe(fd)) { // 파이프 생성
		perror("pipe"); // 파이프 생성에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패하면
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스이면
		close(fd[1]); // fd[1](쓰기) file descripter 종료
		printf("Child: Wait for parent to send data\n");
		if((n = read(fd[0], buf, MAX_BUF)) < 0) { // 부모 프로세스가 작성한 데이터를 읽음
			perror("read"); // 데이터 읽기에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		printf("Child: Received data from parent: ");
		fflush(stdout); // STDOUT의 버퍼를 비움
		write(STDOUT_FILENO, buf, n); // 자식 프로세스로부터 읽은 데이터를 콘솔에 출력
	}
	else { // 부모 프로세스이면
		close(fd[0]); // fd[0](읽기) file descripter 종료
		strcpy(buf, "Hello, World!\n"); // 버퍼에 "Hello, World!\n" 복사
		printf("Parent: Send data to child\n");
		write(fd[1], buf, strlen(buf) * sizeof(char)); // 파이프를 통해 "Hello, World\n" 전송
	}

	return 0;
}
