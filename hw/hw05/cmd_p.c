#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD 256
#define FALSE 0
#define TRUE 1

/*
[Program Name]: cmd_p
[Description]: 자식 프로세스를 생성하여 do_cmd를 수행
[Input]: None
[Output]: 콘솔에 "Doing {cmd}"와 "Done\n"을 출력
[Calls]:
	printf(char* format, ...)
	fgets(char* str, size_t str_size, FILE* fp)
	perror(char* err_msg)
	exit(int EXIT_CODE)
	fork()
	wait(int *statloc)
*/

void do_cmd(char *cmd) {
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main() {
	char cmd[MAX_CMD];
	int pid;

	while(TRUE) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q') break; // cmd에 'q'가 입력된 경우 반복문 종료

		if((pid = fork()) < 0) { // 자식 프로세스 생성
			perror("fork"); // 자식 프로세스 생성에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		else if(pid == 0) { // 자식 프로세스 이면
			do_cmd(cmd); // do_cmd(char* str) 수행
			exit(0); // 정상 종료
		}
		else { // 부모 프로세스 이면
			wait(NULL); // 자식 프로세스 종료까지 대기
		}
	}

	return 0;
}
