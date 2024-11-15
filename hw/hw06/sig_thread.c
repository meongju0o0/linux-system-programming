#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define THREAD_MAIN
#define THREAD_1
#define THREAD_2

#define TRUE 1
#define FALSE 0

/*
[Program Name]: sig_thread
[Description]: 멀티스레드 환경에서 SIGINT 시그널 처리 예제
[Input]: None
[Output]: None
[Calls]:
	- printf(char* format, ...)
	- perror(char* err_msg)
	- exit(int EXIT_CODE)
	- signal(int signum, sighandler_t handler)
	- pthread_create(pthread_t *thread, const pthread_attr_t *attr, void*(*start_routine), (void *), void *arg)
*/

void sig_int_handler(int signum) {
	/*
	[Function Name]: void sig_int_handler(int signum)
	[Description]:
		- SIGINT 시그널이 발생하면 호출되는 함수
		- SIGINT 시그널 발생 시, 문자열 출려 후 프로세스 종료
	[Input]: int signum(시그널 번호)
	[Output]: None
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- printf(char* format, ...)
		- exit(int EXIT_CODE)
	*/
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate this process\n");

	exit(0);
}

void *thread1(void *dummy) {
	/*
	[Function Name]: void *thread1(void *dummy)
	[Description]:
		- THREAD_1 매크로가 정의되어 있으면 thread1에서 SIGINT 시그널 핸들러를 sig_int_handler로 설정
		- THREAD_1 매크로가 정의되어 있지 않으면 시그널 핸들러 미등록
		- 이후 무한 반복
	[Input]: None
	[Output]: None
	[Call by]: pthread_create(pthread_t *thred, const pthread_attr_t *attr, void*(*start_routine), (void *), void *arg)
	[Calls]:
		- signal(int signum, sighandler_t handler)
	*/
#ifdef THREAD_1
	signal(SIGINT, sig_int_handler); // Signal Handler에 sig_int_handler 등록
#endif
	while(TRUE); // 무한 반복
	
	return NULL;
}

void *thread2(void *dummy) {
	/*
	[Function Name]: void *thread2(void *dummy)
	[Description]:
		- THREAD_2 매크로가 정의되어 있으면 thread1에서 SIGINT 시그널 핸들러를 sig_int_handler로 설정
		- THREAD_2 매크로가 정의되어 있지 않으면 시그널 핸들러 미등록
		- 이후 무한 반복
	[Input]: None
	[Output]: None
	[Call by]: pthread_create(pthread_t *thred, const pthread_attr_t *attr, void*(*start_routine), (void *), void *arg)
	[Calls]:
		- signal(int signum, sighandler_t handler)
	*/
#ifdef THREAD_2
	signal(SIGINT, sig_int_handler); // Signal Handler에 sig_int_handler 등록
#endif
	while(TRUE); // 무한 반복

	return NULL;
}

int main() {
	pthread_t tid1, tid2; // 스레드 id를 저장할 변수 선언

	if(pthread_create(&tid1, NULL, thread1, NULL) < 0) { // 스레드 1 생성
		perror("pthread_create"); // 스레드 1 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(pthread_create(&tid2, NULL, thread2, NULL) < 0) { // 스레드 2 생성
		perror("pthread_create"); // 스레드 2 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Create two threads: tid1=%ld, tid2=%ld\n", tid1, tid2);
	printf("Main thread: tid=%ld\n", pthread_self());

#ifdef THREAD_MAIN
	signal(SIGINT, sig_int_handler); // SIGINT 시그널에 대해 sig_int_handler 등록
#endif
	
	printf("Press ^c to quit\n");

	while(TRUE) pause(); // SIGINT 시그널을 반복적으로 대기

	return 0;
}
