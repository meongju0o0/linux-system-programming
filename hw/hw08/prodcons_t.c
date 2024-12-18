#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

/*
[Program Name]: prodcons_t
[Description]:
	- 동시성 제어가 없는 상태에서 producer, consumer 스레드로 critical section에 접근함
	- 동기화 문제가 발생될 수 있는 프로그램
[Input]: None
[Output]:
	- producer와 consumer 스레드에서 생산하고 소비하는 상황을 콘솔에 출력
	- 프로세스 종료 시 버퍼의 상태 출력
[Calls]:
	- perror(char *err_msg)
	- printf(char *format, ...)
	- pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
	- pthread_cond_init(pthread_cond_t *restrict condition, pthread_condattr_t *restrict attr)
	- pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
	- pthread_mutex_lock(pthread_mutex_t *mutex)
	- pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
	- pthread_cond_destory(pthread_cond_t *cond)
	- pthread_cond_destroy(pthread_cond_t *mutex)
	- pthread_exit(void *retval)
	- gettimeofday(struct timeval *tv, struct timezone *tz)
*/

BoundedBufferType Buf; // 스레드가 공유 자원으로 사용할 버퍼 선언

void ThreadUsleep(int usecs) {
	/*
	[Function Name]: void ThreadUsleep(int usecs)
	[Description]: 현재 스레드를 주어진 usecs(마이크로초) 동안 대기
	[Input]: int usecs
	[Output]: None
	[Call by]:
		- Producer(void *dummy)
		- Consumer(void *dummy)
	[Calls]:
		- pthread_cond_init(pthread_cond_t *restrict condition, pthread_condattr_t *restric attr)
		- pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
		- pthread_mutex_lock(pthread_mutex_t *mutex)
		- pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *agstime)
		- pthread_cond_destroy(pthread_cond_t *cond)
		- pthread_cond_destroy(pthread_cond_t *mutex)
		- gettimeofday(struct timeval *tv, struct timezone *tz)
	*/
	pthread_cond_t cond; // 스레드 간 공유 데이터에 접근 시 조건 확인을 위한 조건 변수 선언
	pthread_mutex_t mutex; // mutex를 구현하기 위한 mutex 객체 선언
	struct timespec ts; // 고해상도 시간 계산을 위한 구조체 선언
	struct timeval tv; // 고행상도 시간 계산을 위한 구조체 선언

	if(pthread_cond_init(&cond, NULL) < 0) { // 스레드 조건 변수 기본 속성으로 초기화
		perror("pthread_cond_init"); // 초기화에 실패한 경우 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}
	if(pthread_mutex_init(&mutex, NULL) < 0) { // 뮤텍스 변수 기본 속성으로 초기화
		perror("pthread_mutex_init"); // 초기화에 실패한 경우 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}

	gettimeofday(&tv, NULL); // 현재 시간을 계산해 timeval 구조체에 저장
	ts.tv_sec = tv.tv_sec + usecs / 1000000; // 초 단위 시간을 계산하고 현재 시간에 더함
	ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000; // 나노초 단위 시간을 계산하고 현재 시간에 더함
	
	if(ts.tv_nsec >= 1000000000) { // 나노초 값이 1초를 초과한 경우
		ts.tv_nsec -= 1000000000; // 초과된 나노초 값 제거
		ts.tv_sec++; // 1초 증가
	}

	if(pthread_mutex_lock(&mutex) < 0) { // 뮤텍스 잠금
		perror("pthread_mutex_lock"); // 잠금 실패 시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}
	if(pthread_cond_timedwait(&cond, &mutex, &ts) < 0) { // 지정된 시간까지 조건 변수를 대기
		perror("pthread_cond_timedwait"); // 대기 실패 시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}

	if(pthread_cond_destroy(&cond) < 0) { // 조건 변수 제거
		perror("pthread_cond_destroy"); // 제거 실패 시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}
	if(pthread_mutex_destroy(&mutex) < 0) { // 뮤텍스 제거
		perror("pthread_mutex_destory"); // 제거 실패 시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 비정상 종료
	}
}

void Producer(void *dummy) {
	/*
	[Function Name]: void Producer(void *dummy)
	[Description]:
		- 버퍼에 남는 공간이 있는 경우 아이템을 버퍼에 삽입
		- 버퍼에 남는 공간이 없는 경우 공간이 생길 때까지 waiting (Busy Waiting)
	[Input]: None (NULL)
	[Output]: None
	[Call by]: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
	[Calls]:
		- printf(char *foramt, ...)
		- rand()
		- pthread_exit(void *retval)
		- ThreadUsleep(int usecs)
	*/
	int i, data;

	printf("Producer: Start...\n"); // Producer 스레드가 실행 시작했다는 것을 콘솔에 출력

	for(i = 0; i < NLOOPS; i++) {
		if(Buf.counter == MAX_BUF) { // 버퍼가 가득 찬 경우
			printf("Producer: Buffer full. Waiting.....\n"); // 버퍼가 가득 찬 경우 대기
			while(Buf.counter == MAX_BUF) usleep(100); // Busy Waiting
		}

		printf("Producer: Producing an item.....\n"); // 아이템을 생산 중이라는 것을 콘솔에 출력
		data = (rand() % 100) * 10000; // 무작위 아이템 생성
		Buf.buf[Buf.in].data = data; // 버퍼에 아이템 삽입
		Buf.in = (Buf.in + 1) % MAX_BUF; // 버퍼의 다음 위치 갱신
		Buf.counter++; // 현재 버퍼에 삽입된 아이템 개수 1 증가

		ThreadUsleep(data); // data 만큼 잠시 대기
	}

	printf("Producer: Produced %d items.....\n", i); // 반복이 끝난 수 생성한 아이템의 개수 출력
	printf("Producer: %d items in buffer.....\n", Buf.counter); // 버퍼에 있는 아이템의 개수 출력

	pthread_exit(NULL); // 스레드 종료
}

void Consumer(void *dummy) {
	/*
	[Function Name]: void Consumer(void *dummy)
	[Description]:
		- 버퍼에 아이템이 있는 경우 아이템을 버퍼에서 삭제
		- 버퍼에 아이템이 없는 경우 아이템이 버퍼에 삽입될 때까지 waiting (Busy Waiting)
	[Input]: None (NULL)
	[Output]: None
	[Call by]: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
	[Calls]:
		- printf(char *format, ...)
		- pthread_exit(void *retval)
		- ThreadUsleep(int usecs)
	*/
	int i, data;

	printf("Consumer: Start.....\n"); // Consumer 스레드가 실행 시작했다는 것을 콘솔에 출력

	for(i = 0; i < NLOOPS; i++) {
		if(Buf.counter == 0) { // 버퍼가 비어 있는 경우
			printf("Consumer: Buffer empty. Waiting.....\n"); // 버퍼가 비어있는 경우 대기
			while(Buf.counter == 0); // Busy Waiting
		}

		printf("Consumer: Consuming an item.....\n"); // 아이템을 소비 할 것이라고 콘솔에 출력
		data = Buf.buf[Buf.out].data; // 버퍼의 아이템 복사
		Buf.out = (Buf.out + 1) % MAX_BUF; // 버퍼의 다음 위치 갱신
		Buf.counter--; // 현재 버퍼에 삽입된 아이템 개수 1 감소

		ThreadUsleep((rand() % 100) * 10000); // data 만큼 잠시 대기
	}
	
	printf("Consumer: Consumed %d items.....\n", i); // 반복이 끝난 후 소비한 아이템의 개수 출력
	printf("Consumer: %d items in buffer.....\n", Buf.counter); // 버퍼에 있는 아이템의 개수 출력

	pthread_exit(NULL);
}

int main() {
	pthread_t tid1, tid2;

	srand(0x8888);

	if(pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0) { // Producer 스레드 생성
		perror("pthread_create"); // 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0) { // Consumer 스레드 생성
		perror("pthread_create"); // 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(pthread_join(tid1, NULL) < 0) { // tid1 스레드 종료 대기
		perror("pthread_join"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if(pthread_join(tid2, NULL) < 0) { // tid2 스레드 종료 대기
		perror("pthread_join"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Main: %d items in buffer.....\n", Buf.counter); // 버퍼에 남은 아이템의 개수 출력

	return 0;
}
