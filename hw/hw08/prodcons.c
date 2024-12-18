#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

/*
[Program Name]: prodcons
[Description]:
	- 세마포어를 사용하여 기존 prodcons_t 프로그램의 동기화 문제를 해결
	- 전역 변수에 공용으로 사용하는 버퍼 선언
	- 세마포어를 사용하여 전역 변수 접근 시, 동시성 문제가 발생하는 경우를 해결
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

BoundedBufferType Buf;
sem_t EmptySem, FullSem, MutexSem;

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
	[Input]: None
	[Call by]: Producer(void *dummy)
	[Calls]:
		- printf(char *format, ...)
		- pthread_wait(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
	*/
	int i, data;

	printf("Producer: Start.....\n");

	for(i = 0; i < NLOOPS; i++) {
		if(sem_wait(&EmptySem) < 0) {
			// 버퍼가 가득 차 있는 동안 대기
			// 버퍼가 사용가능한 상태로 변경되어 버퍼 내 값을 변경할 수 있는 경우 세마포어 값 1 감소
			perror("sem_wait"); // sem_wait에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}
		if(sem_wait(&MutexSem) < 0) { // 버퍼를 사용 중인 동안 대기
			perror("sem_wait"); // sem_wait에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}

		printf("Producer: Producing an item.....\n"); // 아이템 생산중이라는 것을 콘솔에 출력
		data = (rand() % 100) * 10000; // 난수 데이터 생성
		Buf.buf[Buf.in].data = data; // 버퍼에 데이터 삽입
		Buf.in = (Buf.in + 1) % MAX_BUF; // 다음 버퍼 위치 갱신
		Buf.counter++; // 버퍼 카운터 변수 1 증가

		if(sem_post(&MutexSem) < 0) { // 버퍼를 더이상 사용하지 않는다는 것을 세마포어에 기록
			perror("sem_post"); // sem_post에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}
		if(sem_post(&FullSem) < 0) { // 버퍼에 사용가능한 공간이 줄어들었다는 것을 세마포어에 기록
			perror("sem_post"); // sem_post에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}

		ThreadUsleep(data); // 스레드 data μs 만큼 대기
	}

	printf("Producer: Produced %d items.....\n", i); // 생산한 아이템 개수를 출력
	printf("Producer: %d items in buffer.....\n", Buf.counter); // 버퍼에 남은 아이템 개수를 출력

	pthread_exit(NULL); // 스레드 종료
}

void Consumer(void *dummy) {
	/*
	[Function Name]: void Producer(void *dummy)
	[Description]:
		- 버퍼에 남는 공간이 있는 경우 아이템을 버퍼에 삽입
		- 버퍼에 남는 공간이 없는 경우 공간이 생길 때까지 waiting (Busy Waiting)
	[Input]: None
	[Call by]: Producer(void *dummy)
	[Calls]:
		- printf(char *format, ...)
		- pthread_wait(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
	*/
	int i, data;

	printf("Consumer: Start.....\n");

	for(i = 0; i < NLOOPS; i++) {
		if(sem_wait(&FullSem) < 0) {
			// 버퍼가 비어있는 동안 대기
			// 버퍼가 사용가능한 상태로 변경되어 버퍼 내 값을 변경할 수 있는 경우 세마포어 값 1 증가
			perror("sem_wait"); // sem_wait에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}
		if(sem_wait(&MutexSem) < 0) { // 버퍼를 사용 중인 동안 대기
			perror("sem_wait"); // sem_wait에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}

		printf("Consumer: Consuming an item.....\n"); // 아이템 소비 중이라는 것을 콘솔에 출력
		data = Buf.buf[Buf.out].data; // 버퍼로부터 데이터 복사
		Buf.out = (Buf.out + 1) % MAX_BUF; // 다음 버퍼 위치 갱신
		Buf.counter--; // 버퍼 카운터 변수 1 감소

		if(sem_post(&MutexSem) < 0) { // 버퍼를 더이상 사용하지 않는다는 것을 세마포어에 기록
			perror("sem_post"); // sem_post에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}
		if(sem_post(&EmptySem) < 0) { // 버퍼에 사용가능한 공간이 증가했다는 것을 세마포어에 기록
			perror("sem_post"); // sem_post에 실패한 경우 에러 메시지 출력
			pthread_exit(NULL); // 스레드 비정상 종료
		}

		ThreadUsleep((rand() % 100) * 10000); // (임의의 난수 값 * 10000) μs 만큼 대기
	}

	printf("Consumer: Consumed %d items.....\n", i); // 소비한 아이템 개수를 출력
	printf("Consumer: %d items in buffer.....\n", Buf.counter); // 버퍼에 남은 아이템 개수 출력

	pthread_exit(NULL); // 스레드 종료
}

int main() {
	pthread_t tid1, tid2;

	srand(0x9999);

	// 세마포어 초기화
	if(sem_init(&EmptySem, 0, MAX_BUF) < 0) {
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&FullSem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&MutexSem, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	// 스레드 생성
	if(pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	// 스레드 종료 대기
	if(pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	// 버퍼 상태 출력
	printf("Main: %d items in buffer.....\n", Buf.counter);
	
	// 세마포어 삭제
	if(sem_destroy(&EmptySem) < 0) {
		perror("sem_destroy");
	}
	if(sem_destroy(&FullSem) < 0) {
		perror("sem_destroy");
	}
	if(sem_destroy(&MutexSem) < 0) {
		perror("sem_destroy");
	}

	return 0;
}
