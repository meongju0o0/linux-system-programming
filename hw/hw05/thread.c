#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
[Program Name]: thread
[Description]: 각 스레드에서 파라미터로 주어진 문자열을 콘솔에 출력
[Input]: None
[Output]: 각 스레드에서 파라미터로 주어진 문자열을 콘솔에 출력
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	exit(int EXIT_CODE)
	pthread_create(pthread_t *tid, pthread_attr_t *attr, void *(start_routine) (void *), void *arg)
	pthread_join(pthread_t tid, void **thread_return)
*/

void print_msg(char *msg) { // 스레드에서 실행할 함수
	printf("%s", msg); // 주어진 문자열을 콘솔에 출력
	pthread_exit(NULL); // 스레드 종료
}

int main() {
	pthread_t tid1, tid2; // 스레드 id를 저장할 변수 선언
	char *msg1 = "Hello, "; // tid1에서 출력할 문자열
	char *msg2 = "World!\n"; // tid2에서 출력할 문자열

	/* Thread ID: tid1, Thread function: PrintMsg, Thread argument: msg1 */
	if(pthread_create(&tid1, NULL, (void *)print_msg, (void *)msg1)) { // 스레드 1에서 문자열 출력
		perror("pthread_create"); // 스레드 1 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(pthread_create(&tid2, NULL, (void *)print_msg, (void *)msg2)) { // 스레드 2에서 문자열 출력
		perror("pthread_create"); // 스레드 2 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Threads created: tid=%ld, %ld\n", tid1, tid2);


	/* Wait for tid1 to exit */
	if(pthread_join(tid1, NULL)) { // 스레드 1 종료 대기
		perror("pthread_join"); // 종료에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(pthread_join(tid2, NULL)) { // 스레드 2 종료 대기
		perror("pthread_join"); // 종료에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);

	return 0;
}
