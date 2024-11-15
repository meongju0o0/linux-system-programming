#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGV 100

/*
[Program Name]: mysystem
[Description]: system() system_call과 동일한 동작
	- fork()를 활용하여 별도의 프로그램을 실행할 프로세스 생성
	- execv()를 활용하여 "/bin" 경로에 존재하는 프로그램 실행
	- waitpid()를 활용하여 자식 프로세스 종료까지 대기
[Input]: None
[Calls]:
	perror(char* err_msg)
	exit(int EXIT_CODE)
	snprintf(char* dst, size_t str_size, char* format, ...)
	strdup(char* str)
	strtok(char* str, char* delimiters)
	free(void* ptr)
	fork()
	waitpid(pid_t pid, int *statloc, int options)
	WIFEXITED(int statloc)
*/

int mysystem(char *cmd) {
	/*
	[Function Name]: int mysystem(char *cmd)
	[Description]:
		- 자식 프로세스 생성
		- 자식 프로세스에서 주어진 경로의 프로그램을 실행
		- 자식 프로세스 종료까지 대기 후, 종료된 프로그램의 EXIT_CODE를 반환
	[Input]: 쉘 명령어
	[Output]: 자식 프로세스의 EXIT_CODE
	[Calls]:
		perror(char* err_msg)
		exit(int EXIT_CODE)
		snprintf(char* dst, size_t str_size, char* format, ...)
		strdup(char* str)
		strtok(char* str, char* delimiters)
		free(void *ptr)
		fork()
		waitpid(pid_t pid, int *statloc, int options)
		WIFEXITED(int statloc)
	*/
	pid_t pid; // 자식 프로세스 아이디를 저장하기 위한 변수
	int statloc; // 자식 프로세스 상태 저장을 위한 변수

	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패한 경우 에러 메시지 출력
		return -1; // -1 반환
	}
	else if(pid == 0) { // 자식 프로세스인 경우
		char* tmp_cmd = strdup(cmd); // tmp_cmd에 주어진 cmd 복사
		if(tmp_cmd == NULL) { // 복사에 실패한 경우
			perror("strdup"); // 에러 메시지 출력
			exit(-1); // 자식 프로세스 비정상 종료
		}

		int argc = 0; // argument counter 선언
		char* argv[MAX_ARGV]; // argument들을 저장할 배열 선언
		char* token = strtok(tmp_cmd, " "); // argument 토큰화를 위한 임시 문자열 선언

		while(token != NULL) { // argument가 추가로 존재하는 동안
			argv[argc++] = token; // argv[]에 argument 저장
			token = strtok(NULL, " "); // 이전 구분자 뒤에서 다시 구분자를 찾기 위함
		}
		argv[argc] = NULL; // argv 마지막에 NULL 저장

		char path[100]; // 프로그램의 경로를 저장하기 위한 변수
		snprintf(path, sizeof(path), "/bin/%s", argv[0]); // 프로그램을 실행할 수 있도록 경로 수정

		if(execv(path, argv) < 0) { // 프로그램 실행
			perror("execv"); // 실행에 실패한 경우 에러 메시지 출력
			free(tmp_cmd); // tmp_cmd 메모리 할당 해제
			exit(-1); // 자식 프로세스 비정상 종료
		}
		free(tmp_cmd); // tmp_cmd 메모리 할당 해제
		exit(0); // 자식 프로세스 정상 종료
	}
	else {
		if(waitpid(pid, &statloc, 0) < 0) { // 자식 프로세스가 정상적으로 종료되지 않은 경우
			perror("waitpid"); // 에러 메시지 출력
			return -1; // -1 반환
		}

		if(WIFEXITED(statloc)) { // 자식 프로세스의 EXIT_CODE 추출
			int exit_code = WEXITSTATUS(statloc);
			return exit_code; // 추출된 exit_code 반환
		}
		else {
			return -1; // -1 반환
		}
	}
}

int main() {
	char cmd[100];
	printf("CMD> ");
	fgets(cmd, sizeof(cmd), stdin);
	cmd[strcspn(cmd, "\n")] = '\0';

	if(!mysystem(cmd)) {
		printf("Command executed sucessfully\n");
	}
	else {
		printf("Command execution failed\n");
	}

	return 0;
}
