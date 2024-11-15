#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CMD 256

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

	pthread_exit(NULL);
}

int main() {
	char cmd[MAX_CMD];
	pthread_t tid;

	while(1) {
		printf("CMD>");
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q') break;

		if(pthread_create(&tid, NULL, (void *)do_cmd, (void *)cmd)) {
			perror("pthread_create");
			exit(1);
		}

		pthread_join(tid, NULL);
	}

	return 0;
}
