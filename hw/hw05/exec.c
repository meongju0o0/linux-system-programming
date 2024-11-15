#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
[Program Name]: exec
[Description]: exec() 함수를 통해 주어진 경로의 프로그램을 실행
[Input]: None
[Output]: 주어진 경로의 프로그램 실행 결과
[Calls]:
	perror(char* err_msg)
	exit(int EXIT_CODE)
	execl(char *pathname, char *arg0, ..., (char *)0)
	execv(char *pathname, char *argv[])
	execlp(char *pathname, char *arg0, ..., (char *)0)
*/

int main() {
	pid_t pid;
	char *argv[] = {"mycp", "exec.c", "exec2.c", NULL};
	
	if((pid = fork()) < 0) { // 자식 프로세스 생성에 실패하면
		perror("fork"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스이면
		if(execl("../hw03/mycp", "mycp", "exec.c", "exec2.c", NULL) < 0) { // execl을 통해서 mycp 실행
			perror("execl"); // 실행에 실패하면 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}
	if(waitpid(pid, NULL, 0) < 0) { // 자식 프로세스가 정상적으로 종료되지 못한 경우
		perror("waitpid"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((pid = fork()) < 0) { // 자식 프로세스 생성에 실패하면
		perror("fork"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스이면
		if(execv("../hw03/mycp", argv) < 0) { // execv를 통해서 mycp 실행
			perror("execv"); // 실행에 실패하면 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}
	if(waitpid(pid, NULL, 0) < 0) { // 자식 프로세스가 정상적으로 종료되지 못한 경우
		perror("fork"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid == 0) { // 자식 프로세스이면
		if(execlp("../hw03/mycp", "mycp", "exec.c", "exec2.c", NULL) < 0) { // execlp를 통해서 mycp 실행
			perror("execlp"); // 실행에 실패하면 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}
	if(waitpid(pid, NULL, 0) < 0) { // 자식 프로세스가 정상적으로 종료되지 못한 경우
		perror("waitpid"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
