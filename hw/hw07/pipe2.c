#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 128

/*
[Program Name]: pipe2
[Description]: 
	- 파이프를 사용하여 부모 프로세스와 자식 프로세스 간 통신
	- 자식 프로세스에서 "Hello, Parent!\n" 문자열을 부모 프로세스에게 전송
	- 자식 프로세스는 부모 프로세스로 부터 받은 "Hello, Child!\n" 문자열을 콘솔에 출력
	- 부모 프로세스에서 "Hello, Child!\n" 문자열을 자식 프로세스에게 전송
	- 부모 프로세스는 자식 프로세스로부터 받은 "Hello, Parent!\n" 문자열을 콘솔에 출력
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
	int n, fd1[2], fd2[2];
	pid_t pid;
	char buf[MAX_BUF];

	if(pipe(fd1) || pipe(fd2)) { // 파이프 생성
		perror("pipe"); // 파이프 생성에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패하면
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스이면
		close(fd1[1]); // fd1[1](쓰기) file descripter 종료
		close(fd2[0]); // fd2[0](읽기) file descripter 종료
		
		strcpy(buf, "Hello, Parent!\n"); // 버퍼에 "Hello, Parent!\n" 복사
		printf("Child: Send data to parent\n");
		write(fd2[1], buf, strlen(buf) * sizeof(char)); // 파이프를 통해 "Hello, Parent!\n" 전송
		
		printf("Child: Wait for parent to send data\n");
		if((n = read(fd1[0], buf, MAX_BUF)) < 0) { // 부모 프로세스가 작성한 데이터를 읽음
			perror("Child, read"); // 데이터 읽기에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		printf("Child: Received data from parent: ");
		fflush(stdout); // STDOUT의 버퍼를 비움
		write(STDOUT_FILENO, buf, n); // 자식 프로세스로부터 읽은 데이터를 콘솔에 출력
		
		close(fd1[0]); // fd1[0](읽기) file descripter 종료
		close(fd2[1]); // fd2[1](쓰기) file descripter 종료
	}
	else { // 부모 프로세스이면
		close(fd1[0]); // fd1[0](읽기) file descripter 종료
		close(fd2[1]); // fd2[1](쓰기) file descripter 종료

		strcpy(buf, "Hello, Child!\n"); // 버퍼에 "Hello, Child!\n" 복사
		printf("Parent: Send data to child\n");
		write(fd1[1], buf, strlen(buf) * sizeof(char)); // 파이프를 통해 "Hello, Child!\n" 전송
	
		printf("Parent: Wait for child to send data\n");
		if((n = read(fd2[0], buf, MAX_BUF)) < 0) { // 자식 프로세스가 작성한 데이터를 읽음
			perror("Parent, read");
			exit(1);
		}
		printf("Parent: Received data from child: ");
		fflush(stdout); // STDOUT의 버퍼를 비움
		write(STDOUT_FILENO, buf, n); // 부모 프로세스로부터 받은 데이터를 콘솔에 출력
	
		close(fd1[1]); // fd1[1](쓰기) file descripter 종료
		close(fd2[0]); // fd2[0](읽기) file descripter 종료
	}

	return 0;
}
