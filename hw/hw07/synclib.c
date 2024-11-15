#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static int Pfd1[2], Pfd2[2];

void TELL_WAIT(void) {
	/*
	[Function Name]: void TELL_WAIT(void)
	[Description]: 파이프 생성
	[Input]: None
	[Output]: None
	[Call by]: main()
	[Calls]:
		- perror(char *err_msg)
		- exit(int exit_code)
		- pipe(int fd[2])
	*/
	if(pipe(Pfd1) < 0 || pipe(Pfd2) < 0) { // 파이프 생성에 실패하면
		perror("pipe"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
}

void TELL_PARENT(void) {
	/*
	[Function Name]: void TELL_PARENT(void)
	[Description]: 부모 프로세스에게 메시지 전송
	[Input]: None
	[Output]: None
	[Call by]: main()
	[Calls]:
		- write(int fd, char *buf, int count)
		- perror(char *err_msg)
		- exit(int exit_code)
	*/
	if(write(Pfd2[1], "c", 1) != 1) { // 메시지 전송에 실패하면
		perror("write"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
}

void WAIT_PARENT(void) {
	/*
	[Function Name]: void WAIT_PARENT(void)
	[Description]: 부모 프로세스의 메시지 대기
	[Input]: None
	[Output]: None
	[Call by]: main()
	[Calls]:
		- read(int fd, char *buf, int count)
		- perror(char *err_msg)
		- fprintf(FILE *fp, const char *format, ...)
		- exit(int exit_code)
	*/
	char c;

	if(read(Pfd1[0], &c, 1) != 1) { // 메시지 읽기에 실패하면
		perror("read"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if(c != 'p') { // 잘못된 메시지를 읽으면
		fprintf(stderr, "WAIT_PARENT: incorrect data"); // 비정상 메시지임을 콘솔에 출력
		exit(1); // 비정상 종료
	}
}

void TELL_CHILD(void) {
	/*
	[Function Name]: void TELL_CHILD(void)
	[Description]: 자식 프로세스에게 메시지 전송
	[Input]: None
	[Output]: None
	[Call by]: main()
	[Calls]:
		- write(int fd, char *buf, int count)
		- perror(char *err_msg)
		- exit(int exit_code)
	*/
	if(write(Pfd1[1], "p", 1) != 1) {
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void) {
	/*
	[Function Name]: void WAIT_CHILD(void)
	[Description]: 자식 프로세스의 메시지 대기
	[Input]: None
	[Output]: None
	[Call by]: main()
	[Calls]:
		- read(int fd, char *buf, int count)
		- perror(char *err_msg)
		- fprintf(FILE *fp, const char *format, ...)
		- exit(int exit_code)
	*/
	char c;

	if(read(Pfd2[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}
	if(c != 'c') {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
