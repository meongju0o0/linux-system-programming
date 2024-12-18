#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

/*
[Program Name]: prodcons_s
[Description]:
	- 세마포어를 활용하여 생산자, 소비자 스레드 간 동기화
	- 예시 시나리오
		- 소비자 스레드는 현재 EmptySem 상태이므로 소비 불가
		- 생산자 스레드는 현재 FullSem 상태가 아니므로 생산
		- 생산자 스레드에서 우선 MutexSem 획득
		- 생산자 스레드에서 item 생산
		- 생산자 스레드에서 item 생산 완료, 버퍼 삽입 후 MutexSem 반납
		- FullSem 값 1 증가
		- 소비자 스레드는 item 소비 가능
[Input]: None
[Output]: 콘솔에 출력되는 생산자, 소비자, 버퍼의 상태
[Calls]:
	- perror(char *err_msg)
	- printf(char *format, ...)
	- pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
	- pthread_exit(void *retval)
	- pthread_join(pthread_t thread, void **retval)
	- sem_init(sem_t *sem, int pshared, int value)
	- sem_wait(sem_t *sem)
	- sem_post(sem_t *sem)
	- sem_destroy(sem_t *sem)
*/

BoundedBufferType Buf;
sem_t EmptySem, FullSem, MutexSem;

void Producer(void *dummy) {
	/*
	[Function Name]: void Producer(void *dummy)
	[Description]:
		- 버퍼에 빈 공간이 생길 때까지 세마포어를 활용하여 대기
		- 버퍼에 빈 공간이 생기면 뮤텍스를 획득하고 아이템 생성 후 버퍼에 삽입
	[Input]: NULL
	[Output]: 콘솔에 출력되는 생산자 스레드의 상황
	[Calls]:
		- perror(char *err_msg)
		- printf(char *format, ...)
		- pthread_exit(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
	*/
	int i, data;

	printf("Producer: Start.....\n");

	// NLOOPS 회 반복
	for (i = 0; i < NLOOPS; i++) {
		// 버퍼에 빈 공간이 있을 때까지 대기
		if (sem_wait(&EmptySem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// 뮤텍스를 획득할 때까지 대기
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		// 뮤텍스 반납
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		// 버퍼에 아이템을 채웠다는 것을 세마포어에 기록
		if (sem_post(&FullSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		usleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

void Consumer(void *dummy) {
	/*
	[Function Name]: void Consumer(void *dummy)
	[Description]:
		- 버퍼에 아이템이 생길 때까지 세마포어를 활용하여 대기
		- 버퍼에 아이템이 존재하면 뮤텍스를 획득하고 아이템을 버퍼에서 삭제
	[Input]: NULL
	[Output]: 콘솔에 출력되는 소비자 스레드의 상황
	[Calls]:
		- perror(char *err_msg)
		- printf(char *format, ...)
		- pthread_exit(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
	*/
	int i, data;

	printf("Consumer: Start.....\n");

	// NLOOPS회 반복
	for(i = 0; i < NLOOPS; i++) {
		// 버퍼에 아이템이 없으면 대기
		if (sem_wait(&FullSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// 뮤텍스를 획득할 때까지 대기
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;
		
		// 뮤텍스 반납
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		// 버퍼에서 아이템을 꺼냈다는 것을 세마포어에 기록
		if (sem_post(&EmptySem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		usleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

int main() {
	pthread_t tid1, tid2;

	srand(0x8888);

	// 세마포어 초기화
	if (sem_init(&EmptySem, 0, MAX_BUF) < 0) {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	// 스레드 생성
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	// 스레드 완료 대기, 종료
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	printf("Main : %d itmes in buffer.....\n", Buf.counter);

	// 세마포어 삭제
	if (sem_destroy(&EmptySem) < 0) {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0) {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0) {
		perror("sem_destroy");
	}

	return 0;
}
