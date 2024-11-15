#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
[Program Name]: fork
[Description]: 각 프로세스 별로 모든 변수가 복사되어 관리가 된다는 것을 확인하기 위한 프로그램
[Input]: None
[Output]: 콘소렝 각 프로세스 별 변수 출력
[Calls];
	write(FILE* fp, char* str, size_t str_size)
	printf(char* format, ...)
	perror(char* err_msg)
	fork()
	sleep(unsigned int seconds)
*/

int Var = 6; // 전역변수 선언
char Buf[] = "a write to stdout\n"; // 전역변수 선언

int main() {
	int var; // main() 함수 지역변수 선언
	pid_t pid; // process id를 저장할 변수 선언

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1); // 콘솔에 Buf 출력
	printf("Before fork\n"); // fork 전, "Before fork\n"는 한번만 출력됨

	if((pid = fork()) < 0) { // 프로세스 생성에 실패하면
		perror("fork");
		exit(1);
	}
	else if(pid == 0) { // 자식 프로세스이면
		Var++; // 자식 프로세스의 Var 변수는 7이 됨
		var++; // 자식 프로세스의 var 변수는 89가 됨
	}
	else { // 부모 프로세스이면
		// 부모 프로세스의 Var, var 변수 수정 없음
		sleep(2); // stdout의 race condition 생성 방지
	}

	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var); // 자식 프로세스 1번, 부모 프로세스 1번 출력

	return 0;
}
