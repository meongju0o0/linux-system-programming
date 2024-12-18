#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/*
[Program Name]: dining
[Descripiton]: 식사하는 철학자 문제를 코드로 구현
	- 철학자의 상태는 아래와 같다
		- Thinking
		- Eating
		- Waiting
	- 철학자가 Eating 상태가 되기 위해서는 Eating을 위한 젓가락(세마포어)를 대기
	- 모든 i 철학자가 i 젓가락을 들고 있는 상태에서 i+1 젓가락을 들지 못함
	- 위와 같은 상황에 의해 교착상태 발생

	- 교착상태 필요조건
		- 상호 배제(Mutual Exclusive)
			- 모든 젓가락은 한번에 한 명의 철학자만 사용할 수 있다
			- 즉, 하나의 젓가락으로 두 입을 동시에 감당할 수는 없다
		- 점유 대기(Hold and Wait)
			- 모든 철학자는 i번 젓가락과 i+1 젓가락을 필요로 함
			- 모든 철학자가 i번 젓가락을 들고 있는 상황에서 i+1 젓가락을 위한 대기 상태 진입
		- 비선점(No preemption)
			- 모든 철학자는 다른 철학자의 젓가락을 강제로 뺏어올 수 없다
		- 순환 대기(Circular Wait)
			- 모든 철학자는 다른 철학자가 젓가락을 놓기를 순환대기 한다
[Input]: None
[Output]: 콘솔에 출력되는 철학자 5명의 상태
[Calls]:
	- printf(char *format, ...)
	- usleep(useconds_t usec)
	- pthread_create(pthread_t *thread, const phtread_attr_t, void *(*start_routine) (void *), void *arg)
	- pthread_join(pthread_t thread, void **retval)
	- pthread_exit(void *retval)
	- sem_init(sem_t *sem, int pshared, unsigned int value)
	- sem_wait(sem_t *sem)
	- sem_post(sem_t *sem)
	- sem_destroy(sem_t *sem)
*/

#define NUM_MEN 5
#define NLOOPS 5

sem_t ChopStick[NUM_MEN];

void Thinking(int id) {
	/*
	[Function Name]: void Thinking(int id)
	[Description]:
		- 철학자의 Thinking 상태를 구현
		- 즉, 젓가락(바이너리 세마포어)를 점유하고 있지 않은 상태
	[Input]: 철학자 아이디
	[Output]: 콘솔에 출력되는 철학자의 상태
	[Call By]: DiningPhilosopher(int *tid)
	[Calls]:
		- printf(char *format, ...)
		- usleep(useconds_t usec)
			- System V usleep: 프로세스 별 대기
			- Linux usleep: 스레드 별 대기
	*/
	printf("Philosopher%d: Thinking.....\n", id); // 현재 철학자의 상태를 콘솔에 출력
	usleep((rand() % 200) * 10000); // (생성된 난수) μs 만큼 대기
	printf("Philosopher%d: Want to eat.....\n", id); // 식사 대기 상태 돌입을 출력
}

void Eating(int id) {
	/*
	[Function Name]: void Eating(int id)
	[Description]:
		- 철학자의 Eating 상태를 표현
		- 즉, 젓가락(바이너리 세마포어)를 점유하고 있는 상태
	[Input]: 철학자 아이디
	[Output]: 콘솔에 출력되는 철학자의 상태
	[Call by]: DiningPhilosopher(int *tid)
	[Calls]:
		- printf(char *format, ...)
		- usleep(useconds_t usec)
			- System V usleep: 프로세스 별 대기
			- Linux usleep: 스레드 별 대기
	*/
	printf("Philosopher%d: Eating.....\n", id); // 현재 철학자의 상태를 콘솔에 출력
	usleep((rand() % 200) * 10000); // (생성된 난수) μs 만큼 대기
}

void DiningPhilosopher(int *philosopher_id) {
	/*
	[Function Name]: void DiningPhilosopher(int *philosopher_id)
	[Description]:
		- 각 개별 철학자(스레드)의 상태를 관리하는 함수
		- 왼쪽 젓가락(세마포어)을 점유할 수 있는지 확인 및 대기
		- 오른쪽 젓가락(세마포어)을 점유할 수 있는지 확인 및 대기
		- 모두 사용 가능하면 식사
		- 식사가 완료되면 젓가락(세마포어) 반납
	[Input]: 철학자 아이디
	[Output]: 철학자가 생각과 식사를 수행한 횟수를 콘솔에 출력
	[Call by]: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
	[Calls]:
		- Thinking(int id)
		- Eating(int id)
		- perror(char *err_msg)
		- printf(char *format, ...)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
		- pthread_exit(void *retval)
	*/
	int i; // 인덱스 저장을 위한 변수
	int id = *philosopher_id; // 철학자 id 저장

	// NLOOPS 만큼 반복하며 식사
	for(i = 0; i < NLOOPS; i++) {
		Thinking(id); // 철학자 Thinking 상태

		// 왼쪽, 오른쪽 젓가락을 점유할 수 있는 상태이면 Eating(id) 실행
		if(sem_wait(&ChopStick[id % NUM_MEN])) { // 왼쪽 젓가락 대기
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&ChopStick[(id + 1) % NUM_MEN])) { // 오른쪽 젓가락 대기
			perror("sem_wait");
			pthread_exit(NULL);
		}

		Eating(id); // 모든 젓가락을 사용할 수 있으면 식사 상태 진입

		// 식사 완료 후, 왼쪽 젓가락(세마포어) 반환
		if(sem_post(&ChopStick[id % NUM_MEN])) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		// 식사 완료 후, 오른쪽 젓가락(세마포어) 반환
		if(sem_post(&ChopStick[(id + 1) % NUM_MEN])) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i); // 식사 횟수 출력

	pthread_exit(NULL); // 스레드 종료
}

int main() {
	pthread_t tid[NUM_MEN]; // 스레드 아이디 저장 변수 선언
	int id[NUM_MEN];

	srand(0x8888);

	// 각 철학자별 젓가락(세마포어) 초기화
	for(int i = 0; i < NUM_MEN; i++) {
		if(sem_init(&ChopStick[i], 0, 1)) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	// 각 철학자(스레드) 생성
	for(int i = 0; i < NUM_MEN; i++) {
		if(pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, (void *)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	// 모든 철학자(스레드)의 식사가 완료되면 철학자(스레드) 종료 후 조인
	for(int i = 0; i < NUM_MEN; i++) {
		if(pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	// 모든 젓가락(세마포어) 삭제
	for(int i = 0; i < NUM_MEN; i++) {
		if(sem_destroy(&ChopStick[i])) {
			perror("sem_destroy");
		}
	}

	return 0;
}
