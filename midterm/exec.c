#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	char* source_name = argv[1];
	char exec_name[100];
	int source_name_idx = 0;
	while(*(source_name + source_name_idx) != '.') {
		exec_name[source_name_idx] = *(source_name + source_name_idx);
		source_name_idx++;
	}
	exec_name[source_name_idx] = '\0';

	char *exec_argv[100];
	exec_argv[0] = exec_name;
	int argument_idx;
	for(argument_idx = 1; argument_idx < argc; argument_idx++) {
		exec_argv[argument_idx] = argv[argument_idx + 1];
	}
	exec_argv[argument_idx] = NULL;

	pid_t pid;
	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid == 0) {
		char *gcc_argv[7];
		gcc_argv[0] = "gcc";
		gcc_argv[1] = "-o";
		gcc_argv[2] = exec_name;
		gcc_argv[3] = source_name;
		gcc_argv[4] = "-lpthread";
		gcc_argv[5] = "-lm";
		gcc_argv[6] = NULL;

		if(execv("/usr/bin/gcc", gcc_argv) < 0) {
			perror("system");
			exit(1);
		}
	}
	else {
		wait(NULL);
		char exec_command[100];
		sprintf(exec_command, "./%s", exec_name);
		if(execv(exec_command, exec_argv) < 0) {
			perror("execv");
			exit(1);
		}
	}

	return 0;
}
