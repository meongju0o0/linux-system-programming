#include <stdio.h>

/*
[Program Name]: env
[Description]: system environ variable들을 출력
[Input]: None
[Output]: 현재 시스템의 환경변수들을 출력
[Calls]:
	printf()
*/

int main(int argc, char *argv[], char *envp[]) {
	int i;
	char **p;
	extern char **environ; // 시스템 환경변수가 저장되어 있는 문자열 배열

	printf("List command-line arguments\n");
	for(i = 0; i < argc; i++) { // 모든 command-line argument를
		printf("%s\n", argv[i]); // 출력
	}
	printf("\n");

	printf("List environment variables from environ variable\n");
	for(i = 0; environ[i] != NULL; i++) { // environ에 저장된 시스템의 모든 환경변수를
		printf("%s\n", environ[i]); // 출력
	}	
	printf("\n");

	printf("List environment variables from envp variable\n");
	for(i = 0; envp[i] != NULL; i++) { // envp에 저장된 시스템의 모든 환경변수를
		printf("%s\n", envp[i]); // 출력
	}

	return 0;
}
