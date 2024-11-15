#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TRUE 1
#define FALSE 0

static unsigned int AlarmSecs;

/*
[Program Name]: alarm
[Description]: 1초 마다 콘솔에 '.'을 출력
[Input]: None
[Output]: None
[Calls]:
	- printf(char* format, ...)
	- putchar(char ch)
	- perror(char* err_msg)
	- fflush(FILE* fp)
	- exit(int EXIT_CODE)
	- signal(int signum, sighandler_t handler)
	- alarm(unsigned int seconds)
*/

void sig_alarm_handler(int signum) {
	/*
	[Function Name]: void sig_alarm_handler(int signum)
	[Description]:
		- SIGALRM 시그널이 발생하면 호출되는 함수
		- Signal Handler에 sig_alarm_handler를 재등록한 후에, alarm(unsigend int seconds)함수 호출
		- alarm(1)을 통해 1초 마다 SIGALRM 시그널을 발생시킴
		- main() 함수의 while(TRUE) pause;를 통해 1초마다 반복하며 콘솔에 '.'을 출력
	[Input]: int signum(시그널 번호)
	[Output]: None
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- putchar(char ch)
		- perror(char* err_msg)
		- fflush(FILE* fp)
	*/
	if(signal(signum, sig_alarm_handler) == SIG_ERR) { // Signal Handler에 sig_alarm_handler 등록
		perror("signal"); // 등록에 실패 시 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	alarm(AlarmSecs); // AlarmSecs 후에 SIGALRM 시그널을 발생시킴

	putchar('.'); // 콘솔에 '.' 출력

	return;
}

int set_periodic_alarm(unsigned int nsecs) {
	/*
	[Function Name]: int set_periodic_alarm(unsigned int nsecs)
	[Description]:
		- sig_alarm_handler 최초 등록
		- alarm(nsecs) 함수를 통해 SIGALRM 시그널 발생 시 sig_alarm_handler 호출
	[Input]: unsigned int nsecs(알람 대기 시간)
	[Output]:
		- 함수 비정상 종료 시 -1 반환
		- 정상적으로 수행된 경우 0을 반환
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- signal(int signum, sighandler_t handler)
		- alarm(unsigend int seconds)
	*/
	if(signal(SIGALRM, sig_alarm_handler) == SIG_ERR) { // Signal Handler에 sig_alarm_handler 등록
		return -1;
	}

	AlarmSecs = nsecs; // nsecs 전역 변수 AlarmSecs에 대입

	alarm(nsecs); // nsecs 후에 알람이 울리도록 OS 스케줄에 등록

	return 0;
}

int main() {
	printf("Doing something every one seconds");

	setbuf(stdout, NULL); // stdout에 대해 버퍼를 NULL로 지정, stdout을 비버퍼링 모드로 변환
	// fflush(FILE* fp) 없이 콘솔에 즉각적인 문자 혹은 문자열 출력 가능

	set_periodic_alarm(1); // 1초마다 알람을 울리며 정의된 기능 수행
	while(TRUE) pause(); // 무한 반복하며 SIGALRM 시그널이 올 때까지 프로세스 대기

	return 0;
}
