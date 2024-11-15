#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]: system
[Description]: system 환경변수에 등록된 프로그램을 실행
[Input]: None
[Output]: 프로그램 실행 시 콘솔에 출력되는 내용이 있으면 같은 File Descripter를 통해 콘솔에 출력
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	exit(int EXIT_CODE)
	system(char* cmd)
*/

int main() {
	int status;

	if((status = system("date")) < 0) { // date 명령어 실행에 실패하면
		perror("system"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	printf("exit status = %d\n", status); // exit code 출력

	if((status = system("nosuchcommand")) < 0) { // nosuchcommand 명령어 실행에 실패하면
		perror("system"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	printf("exit status = %d\n", status); // exit code 출력

	if((status = system("who; exit 44")) < 0) { // who; exit 44 명령어 실행에 실패하면
		perror("system"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	printf("exit status = %d\n", status); // exit code 출력

	return 0;
}
