#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

/*
[Program Name]: prodcons_m
[Description]:
	- 생산자 스레드와 소비자 스레드 생성
	- 생산자 스레드와 소비자 스레드 간에 mutex와 condition variable을 사용하여 동기화
		- 생산자 스레드가 우선 mutex를 획득하고 아이템을 생성한 후 mutex 반납
		- 소비자 스레드는 생산자가 반납한 mutex를 획득한 후 아이템 소비, mutex 반납
[Input]: None
[Output]:
	- 콘솔에 출력되는 생산, 소비 상황
	- 생산과 소비를 끝낸 후 버퍼 상황 콘솔에 출력
[Calls]:
	- perror(char *err_msg)
	- printf(char *foramt, ...)
	- exit(int exit_code)
	- pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
	- pthread_exit(void *retval)
	- pthread_join(pthread_t thread, void **retval)
	- pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *mattr)
	- pthread_mutex_destroy(pthread_mutex_t *mutex)
	- pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cattr)
	- pthread_cond_destroy(pthread_cond_t *cond)
	- pthread_mutex_lock(pthread_mutex_t *mutex)
	- pthread_mutex_unlock(pthread_mutex_t *mutex)
	- pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
	- pthread_cond_signal(pthread_cond_t *cond)
*/

BoundedBufferType Buf;
pthread_cond_t NotFull;
pthread_cond_t NotEmpty;
pthread_mutex_t Mutex;

void Producer(void *dummy) {
	/*
	[Function Name]: void Producer(void *dummy)
	[Description]:
		- 생산자 스레드가 버퍼에 아이템을 임의의 시간에 따라 삽입
		- mutex를 활용해 버퍼에 대한 접근 제어
		- condition variable을 활용해 버퍼의 상황 파악
			- NotFull인 경우(버퍼가 가득 차 있지 않은 경우)에만, 버퍼에 아이템을 삽입
			- 버퍼에 아이템을 삽입 하면, NotEmpty 시그널을 condition varialbe에 보냄
	[Input]: NULL
	[Output]: None
	[Calls]:
		- perror(char *err_msg)
		- printf(char *format, ...)
		- pthread_exit(void *retval)
		- pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
		- pthread_cond_signal(pthread_cond_t *cond)
		- pthread_mutex_unlock(pthread_mutex_t *mutex)
	*/
	int i, data;

	printf("Producer: Start.....\n");

	// NLOOPS회 생산
	for(i = 0; i < NLOOPS; i++) {
		// mutex를 활용하여 버퍼 접근 제어
		if(pthread_mutex_lock(&Mutex)) { 
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		// 버퍼가 가득 차 있는데 접근을 시도하는 경우
		while(Buf.counter == MAX_BUF) {
			// 버퍼가 가득 차 있지 않을 때까지 대기
			if(pthread_cond_wait(&NotFull, &Mutex)) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		// 버퍼에 아이템 삽입
		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		// 버퍼가 비어있지 않다는 것을 세마포어에 기록
		if(pthread_cond_signal(&NotEmpty)) {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

		// 버퍼에 더이상 접근하지 않으므로 mutex 반납
		if(pthread_mutex_unlock(&Mutex)) {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		usleep(data); // (data)μs 만큼 스레드 대기
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

void Consumer(void *dummy) {
	/*
	[Function Name]: void Consumer(void *dummy)
	[Description]:
		- 소비자 스레드가 버퍼의 아이템을 임의의 시간에 따라 삭제
		- mutex를 활용하여 버퍼에 대한 접근 제어
		- condition variable을 활용해 버퍼의 상황 파악
			- NotEmpty인 경우(버퍼가 비어있지 않은 경우)에만, 버퍼에서 아이템을 삭제
			- 버퍼의 아이템을 삭제한 후, NotFull 시그널을 condition variable에 보냄
	[Input]: NULL
	[Output]: None
	[Calls]:
		- perror(char *err_msg)
		- printf(char *format, ...)
		- pthread_exit(void *retval)
		- pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
		- pthread_cond_signal(pthread_cond_t *cond)
		- pthread_mutex_unlock(pthread_mutex_t *mutex)
	*/
	int i, data;

	printf("Consumer: Start.....\n");

	// NLOOPS회 소비
	for(i = 0; i < NLOOPS; i++) {
		// mutex를 활용하여 버퍼 접근 제어
		if(pthread_mutex_lock(&Mutex)) {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		// 버퍼가 비어 있는데 접근을 시도하는 경우
		while(Buf.counter == 0) {
			// 버퍼가 비어있찌 않을 때까지 대기
			if(pthread_cond_wait(&NotEmpty, &Mutex)) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		// 버퍼에 아이템 삽입
		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		// 버퍼가 비어있지 않다는 것을 세마포어에 기록
		if(pthread_cond_signal(&NotFull)) {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

		// 버퍼에 더이상 접근하지 않으므로 mutex 반납
		if(pthread_mutex_unlock(&Mutex)) {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		usleep((rand() % 100) * 10000); // (data)μs 만큼 스레드 대기
	}
	
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

int main() {
	pthread_t tid1, tid2; // 스레드 아이디 저장 변수 선언

	srand(0x8888); // 난수 시드값 설정

	// condition variable 초기화
	if(pthread_cond_init(&NotFull, NULL)) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_cond_init(&NotEmpty, NULL)) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}

	// mutex 초기화
	if(pthread_mutex_init(&Mutex, NULL)) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
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

	// 스레드 완료 대기 후, 완료 시 종료
	if(pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	printf("Main: %d items in buffer.....\n", Buf.counter);

	// condition variable 삭제
	if(pthread_cond_destroy(&NotFull)) {
		perror("pthread_cond_destroy");
	}
	if(pthread_cond_destroy(&NotEmpty)) {
		perror("pthread_cond_destroy");
	}

	// mutex 초기화
	if(pthread_mutex_destroy(&Mutex)) {
		perror("pthread_cond_destroy");
	}

	return 0;
}
