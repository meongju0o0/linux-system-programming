#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
[Program Name]: mysleep
[Description]: 주어진 시간동안 대기 후 프로세스 종료
[Input]: None
[Output]: None
[Calls]:
	- printf(char* format, ...)
	- signal(int signum, sighandler_t handler)
	- alarm(unsigned int seconds)
	- pause()
*/

void sig_alarm_handler(int signum) {
	/*
	[Function Name]: void sig_alarm_handler(int signum)
	[Description]: SIGALRM 시그널이 발생하면 호출되는 dummy function
	[Input]: int signum(시그널 번호)
	[Output]: None
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]: None
	*/
	return;
}

unsigned int mysleep(const unsigned int nsecs) {
	/*
	[Function Name]: unsigned int mysleep(unsigned int nsecs)
	[Description]: 함수 호출 시 프로세스가 nsecs간 대기
	[Input]: unsigned int nsecs(대기할 시간)
	[Output]: 프로세스가 대기한 시간
	[Call by]: main()
	[Calls]:
		- signal(int signum, sighandler_t handler)
		- alarm(unsigned int seconds)
		- pause()
	[Returns]:
		- 함수가 대기하지 못한 시간
			- 대기 중 interrupt가 발생한 경우 남은 시간을 반환
			- 함수가 nsecs 만큼 대기한 경우 0을 반환
	*/
	if(nsecs == 0) {
		// nsecs가 0인 경우, alarm(0)을 호출하면 기존 알람을 해제, 새로운 알람을 등록하지 않음
		// pause() 함수에서 시그널을 별도로 받지 않으면 프로세스가 종료되지 않음
		// 별도의 예외처리를 위한 조건문
		return 0;
	}

	if(signal(SIGALRM, sig_alarm_handler) == SIG_ERR) { // SIGALRM에 대한 Handler를 SignalHandler에 등록
		return nsecs; // 등록에 실패한 경우 nsecs 반환
	}

	alarm(nsecs); // nsecs초 후에 프로세스가 SIGALRM 시그널을 받도록 운영체제 스케줄에 등록

	pause(); // SIGALRM을 받을 때까지 대기

	return alarm(0);
	// 기존에 설정된 알람을 취소(alarm(0): 기존에 설정된 알람을 취소)
	// 이전에 호출한 alarm(unsigned int secodns) 함수가 수행된 시간을 반환
}

int main() {
	const unsigned int nsecs = 5;

	printf("Wait for %d seconds...\n", nsecs);
	printf("Waited for %u seconds\n", nsecs - mysleep(nsecs));

	return 0;
}
