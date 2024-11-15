#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>

#define TRUE 1
#define FALSE 0

/*
[Program Name]: nonreent
[Description]:
	- SignalHandler와 main() 함수의 while(TRUE)에서 non-thread 안전 함수 사용 시 발생하는 문제를 테스트
	- getpwname(const char* name)은 non-reentrant function
	- alarm(1)과 my_alarm_handler(int signum) 호출
		- my_alarm_handler(int signum)은 getpwname("root")를 통해 root의 정보 조회
	- 한편, main() 함수에서는 지속적으로 getpwname("lsp30")을 통해 lsp30의 정보 조회
	- 따라서, main() 함수에서 lsp30의 정보가 유효한지 검사할 때, 손상된 정보로 판단될 수 있음
[Input]: None
[Output]: None
[Calls]:
	- printf(char* format, ...)
	- perror(char* err_msg)
	- exit(int EXIT_CODE)
	- signal(int signum, sighandler_t handler)
	- alarm(unsigned int seconds)
	- getpwnam(const char* name)
*/

void my_alarm_handler(int signum) {
	/*
	[Function Name]: my_alarm_handler(int signum)
	[Description]:
		- SIGALRM 시그널이 발생하면 호출되는 함수
		- SIGALRM 발생 시 핸들러를 재등록하여 반복적인 alarm(1) 함수 호출을 처리
		- alarm(1) 함수를 통해 1초 뒤 SIGALRM 시그널이 발생하도록 재설정
		- getpwnam("root")를 호출하여 root의 정보 조회
			- getpwnam(const char* name)은 비스레드 안전 함수
			- 동시 호출 시 메모리 손상 가능성 존재
	[Input]: int signum(시그널 번호)
	[Output]: None
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- printf(char* format, ...)
		- perror(char* err_msg)
		- exit(int EXIT_CODE)
		- alarm(unsigned int seconds)
		- getpwnam(const char* name)
	*/
	struct passwd *rootptr; // root 정보 조회를 위한 passwd 구조체 포인터 선언

	signal(SIGALRM, my_alarm_handler); // Signal Handler에 my_alarm_handler 등록
	alarm(1); // 1초 후에 SIGALRM 시그널을 발생시킴

	printf("in signal handler\n");

	if((rootptr = getpwnam("root")) == NULL) { // root의 정보를 얻지 못한 경우
		perror("getpwnam"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return;
}

/*
linux에서는 static 메모리 공간 보호를 위해 자체적으로 ___lll_lock_wait_private() 사용
- getpwnam(const char* name)
	- getpwnam(const char* name) 함수는 유저 정보를 관리
	- 유저 정보가 관리되고 있는 메모리 공간은 static
- ___lll_lock_wait_private()
	- glibc의 스레드 관련 기능에서 사용하는 저수준 락 대기 함수
	- 뮤텍스와 같은 락이 다른 스레드에서 사용 중일 때, 현재 스레드가 대기하면서 락이 해제되기를 대기
*/
/*
nonreent 프로그램이 교착상태에 빠지는 매커니즘
1. 메인 루프에서 getpwnam("lsp30")을 호출하는 도중 SIGALRM 시그널 발생
2. 시그널 핸들러 실행
3. 시그널 핸들러는 getpwnam("root") 호출
4. 메인 루프의 getpwnam("lsp30")이 락을 소유하고 있는 상태에서 시그널 핸들러가 실행된다면
5. 시그널 핸들러의 getpwnam("root") 호출은 getpwnam("lsp30")이 소유한 락이 해제될 때까지 대기
6. 메인 루프는 my_alarm_handler()라는 시그널 핸들러가 종료되기까지 대기

결론적으로, 5과 6의 순환 대기로 인해 교착상태가 발생
*/

int main() {
	struct passwd *ptr; // lsp30 사용자 정보를 저장할 passwd 구조체 포인터 선언

	signal(SIGALRM, my_alarm_handler); // Signal Handler에 SIGALRM에 대해 my_alarm_handler 등록
	alarm(1); // 1초 후에 SIGALRM 시그널을 발생시킴

	while(TRUE) {
		if((ptr = getpwnam("lsp30")) == NULL) { // lsp30의 정보를 얻지 못한 경우
			perror("getpwnam"); // 에러 메시지 출력
			exit(1); // 비정상 종료
		}

		if(strcmp(ptr->pw_name, "lsp30") != 0) { // lsp30의 정보가 아닌 다른 정보를 얻은 경우
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name); // 해당 정보의 pw_name 출력
			exit(0); // 프로세스 종료
		}
	}

	return 0;
}
