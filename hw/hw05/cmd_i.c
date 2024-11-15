#include <stdio.h>
#include <unistd.h>

#define MAX_CMD 256
#define FALSE 0
#define TRUE 1

/*
[Program Name]: cmd_i
[Description]: "Doing {cmd}"를 출력 후 1초간 대기
[Input]: None
[Output]: 터미널에 출력되는 "Doing {cmd}"와 "Done\n"
[Calls]:
	printf(char* format, ...)
	fgets(char* str, size_t str_size, FILE* fp)
*/

void do_cmd(char *cmd) {
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main() {
	char cmd[MAX_CMD];

	while(TRUE) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q') break;

		do_cmd(cmd);
	}

	return 0;
}
