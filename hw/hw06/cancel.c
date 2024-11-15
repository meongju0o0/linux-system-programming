#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

/*
[Program Name]: sig_int_handler
[Description]: SIGINT 시그널 발생 시 프로세스에서 생성한 스레드를 안전하게 종료 후 프로세스를 종료
[Input]: None
[Output]: None
[Calls]:
	- printf(char *format, ...)
	- fprintf(FILE *fp, char* format, ...)
	- perror(char* err_msg)
	- exit(int EXIT_CODE)
	- signal(int signum, sighandler_t handler)
	- pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine), (void *), void *arg)
	- pthread_setcancelstate(int state, int *oldstate)
	- pthread_cancel(pthread_t tid)
	- pthraed_join(pthread_t tid, void **retval)
*/

pthread_t ThreadID[2];

void sig_int_handler(int signum) {
	/*
	[Program Name]: sig_int_handler
	[Description]: SIGINT 시그널 발생 시 프로세스에서 생성한 2개 스레드를 안전하게 종료한 후 프로세스 종료
	[Input]: None
	[Output]: None
	[Call by]: signal(int signum, sighandler_t handler)
	[Calls]:
		- printf(char *format, ...)
		- fprintf(FILE *fp, char *format, ...)
		- exit(int EXIT_CODE)
		- pthread_cancel(pthread_t tid)
		- pthread_join(pthread_t tid, void **retval)
	*/

	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadID[0], ThreadID[1]);

	for(int i = 0; i < 2; i++) {
		int thread_err_no;
		if(thread_err_no = pthread_cancel(ThreadID[i])) { // 스레드 강제 종료
			fprintf(stderr, "pthread_cancel error occured, err no: %d\n", thread_err_no);
			exit(1);
		}
	}

	for(int i = 0; i < 2; i++) {
		int thread_err_no;
		if(thread_err_no = pthread_join(ThreadID[i], NULL)) { // 스레드가 완전히 종료될 때까지 대기 후 스레드 삭제
			fprintf(stderr, "pthread_join error occured, err no: %d\n", thread_err_no);
			exit(1);
		}
	}

	printf("Threads terminated: tid=%ld, %ld\n", ThreadID[0], ThreadID[1]);

	exit(0);
}

void *thread1(void *dummy) {
	/*
	[Function Name]: void *thread1(void *dummy)
	[Description]: 현재 스레드를 비동기적으로 취소 가능한 상태로 설정
	[Input]: NULL
	[Output]: None
	[Call by]: pthread_create(pthread_t *thread, const pthead_attr_t *attr, void *(*start_routine), (void *), void *arg)
	[Calls]:
		- printf(char* str, ...)
		- perror(char* err_msg)
		- pthread_setcancelstate(int state, int *oldstate)
		- pthread_exit(void *retval)
		- pthread_setcanceltype(int type, int *oldtype)
	*/
	printf("Thread %ld created.....\n", pthread_self());

	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) { // 현재 스레드를 취소 가능 상태로 설정
		perror("pthread_setcancelstate"); // 실패한 경우 에러 출력
		pthread_exit(NULL); // 스레드 종료
	}

	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) { // 현재 스레드를 비동기적으로 취소 가능하도록 설정
		perror("pthread_setcanceltype"); // 실패한 경우 에러 출력
		pthread_exit(NULL); // 스레드 종료
	}

	while(TRUE); // 무한 반복
}

void *thread2(void *dummy) {
	/*
	[Function Name]: void *thread2(void *dummy)
	[Description]: 현재 스레드를 비동기적으로 취소 가능한 상태로 설정
	[Input]: NULL
	[Output]: None
	[Call by]: pthread_create(pthread_t *thread, const pthead_attr_t *attr, void *(*start_routine), (void *), void *arg)
	[Calls]:
		- printf(char* str, ...)
		- perror(char* err_msg)
		- pthread_setcancelstate(int state, int *oldstate)
		- pthread_exit(void *retval)
		- pthread_setcanceltype(int type, int *oldtype)
	*/
	printf("Thread %ld created.....\n", pthread_self());

	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) { // 현재 스레드를 취소 가능 상태로 설정
		perror("pthread_setcancelstate"); // 실패한 경우 에러 출력
		pthread_exit(NULL); // 스레드 종료
	}

	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) { // 현재 스레드를 비동기적으로 취소 가능하도록 설정
		perror("pthread_setcanceltype"); // 실패한 경우 에러 출력
		pthread_exit(NULL); // 스레드 종료
	}

	while(TRUE); // 무한 반복
}

int main() {
	if(pthread_create(&ThreadID[0], NULL, thread1, NULL) < 0) { // 스레드 생성
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&ThreadID[1], NULL, thread2, NULL) < 0) { // 스레드 생성
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, sig_int_handler); // SIGINT 시그널 발생 시, sig_int_handler를 호출하도록 등록

	printf("Press ^C to quit\n");

	while(TRUE) pause();
	return 0;
}
