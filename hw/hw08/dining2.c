#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/*
[Program Name]: dining2
[Description]: 식사하는 철학자 문제를 코드로 구현
	- 철학자의 상태는 아래와 같다
		- Thinking
		- Eating
		- Hungry
	- 철학자가 취하는 행동은 아래와 같다
		- Thinking
		- PickUp
		- Eating
		- PutDown
	- 젓가락이라는 자원을 점유하기 위한 race condition 발생
	- 해당 자원에 대한 동시성 제어 필요
		- 젓가락을 집는 행위에 대한 바이너리 세마포어 필요
		- 철학자의 식사 상태를 관리하기 위한 state 배열과 세마포어 필요
	- 식사 가능한 상태를 다루기 위한 해당 철학자의 상태 관리 -> sem_t Philosopher[NUM_MEN]
	- 식사 가능한 상태이면 Philosopher[NUM_MEN]이 1인 상태
	- 식사 불가능한 상태이면 Philosopher[NUM_MEN]이 0인 상태

	- 예시 시나리오
		- 0번 철학자가 우선 PickUp과 Test 함수를 통해 식사 가능 상태로 변경됨
		- 0번 철학자는 Eating 상태
		- 그 동안에 1번 철학자와 4번 철학자는 0번 철학자로 인해 식사가 불가능한 상태
			- 0번 철학자의 식사 행위로 인해 1번 철학자는 State[LEFT(1)]이 EATING인 상태
			- 0번 철학자의 식사 행위로 인해 4번 철학자는 State[RIGHT(4)]가 EATING인 상태
			- 이에 따라, sem_post(&Philosopher[1])과 sem_post(&Philosopher[4])를 수행하지 못한 채 1번 철학자와 4번 철학자는 Test 함수를 종료
		- 0번 철학자가 식사를 마치면 (즉, Eating 함수가 종료되면)
		- 0번 철학자가 PutDown 함수를 호출하여 본인의 상태를 THINKING으로 변경
		- 0번 철학자는 PutDown 함수에서 Test(LEFT(id))와 Test(RIGHT(id)) 함수를 호출하여 왼쪽 철학자와 오른쪽 철학자가 식사 가능한 상태인지 다시 확인하고 식사 가능 상태이면 sem_post(&Philosopher[id])를 통해 1번 철학자와 4번 철학자에게 식사 가능 상태임을 알림
		- 1번 철학자와 4번 철학자는 PickUp 함수의 sem_wait(&Philosopher[id]) 탈출 후 Eating 시작
		- 위 과정을 5명의 철학자가 비동기적으로 5회 반복
[Input]: None
[Output]: 콘솔에 출력되는 철학자 5명의 상태
[Calls]:
	- printf(char *format, ...)
	- perror(char *err_msg)
	- usleep(useconds_t usec)
	- pthread_create(pthread_t *thread, const phtread_attr_t, void *(*start_routine) (void *), void *arg)
	- pthread_join(pthread_t thread, void **ret_val)
	- pthread_exit(void *retval)
	- sem_init(sem_t *sem, int pshared, unsigned int value)
	- sem_wait(sem_t *sem)
	- sem_post(sem_t *sem)
	- sem_destroy(sem_t *sem)
*/

#define NUM_MEN 5
#define NLOOPS 5

// C++ inline function과 동일한 역할을 위한 전처리 매크로 정의
#define LEFT(i) ((i + NUM_MEN - 1) % NUM_MEN)
#define RIGHT(i) ((i + 1) % NUM_MEN)

enum {THINKING, EATING, HUNGRY};

sem_t Philosopher[NUM_MEN]; // 각 철학자의 상태를 관리하는 세마포어 선언
sem_t Mutex; // PickUp 함수에 대한 동시성 제어를 위한 바이너리 세마포어 선언
int State[NUM_MEN]; // 각 철학자의 상태를 관리

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
		- printf(char *format)
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

void Test(int id) {
	/*
	[Function Name]: void Test(int id)
	[Description]:
		- 철학자가 식사 가능 상태인지 확인
		- 식사 가능 상태이면 State를 EATING으로 변경
		- sem_post(&Philosopher[id])를 통해 PickUp 함수는 sem_wait(&Philosopher[id])를 탈출
		- 혹은 PickDown 함수의 Test 함수 호출로 양 옆 철학자의 상태를 EATING으로 변경
	[Input]: 철학자 아이디
	[Output]: 변경되는 철학자의 상태
	[Call by]:
		- PickUp(int id)
		- PutDown(int id)
	[Calls]:
		- sem_post(sem_t *sem)
		- perror(char *err_msg)
	*/
	if(State[id] == HUNGRY && State[LEFT(id)] != EATING && State[RIGHT(id)] != EATING) {
		State[id] = EATING;

		if(sem_post(&Philosopher[id])) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void PickUp(int id) {
	/*
	[Function Name]: void PickUp(int id)
	[Description]:
		- 바이너리 세마포어(Mutex)를 통해 서로 다른 철학자가 같은 젓가락을 드는 상황을 방지
		- Test 함수를 호출하여 현재 스레드(철학자)가 식사 가능한 상태인지 확인
			- 식사 가능 상태이면 Test함수에서 Philosopher[id]를 1로 변경
			- sem_wait(&Philosopher[id]) 탈출
			- 식사 불가능 상태이면 Test함수 종료
			- sem_wait(&Philosopher[id])에서 스레드 대기 상태로 변경
	[Input]: 철학자 아이디
	[Output]: 변경되는 철학자의 상태
	[Call By]:
		- DiningPhilosopher(void *arg)
	[Calls]:
		- perror(char *err_msg)
		- pthread_exit(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
		- Test(int id)
	*/
	if(sem_wait(&Mutex)) { // 바이너리 세마포어 대기
		perror("sem_wait"); // 대기 중 에러 발생시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 종료
	}

	State[id] = HUNGRY; // 철학자 상태 HUNGRY로 설정

	Test(id); // 현재 철학자가 식사 가능하면 EATING으로 상태 변경

	if(sem_post(&Mutex)) { // 바이너리 세마포어 반납
		perror("sem_post"); // 반납에 실패하면 에러 메시지 출력
		pthread_exit(NULL); // 스레드 종료
	}

	if(sem_wait(&Philosopher[id])) { // 세마포어 대기 (먹을수 있는 상태 대기)
		perror("sem_wait"); // 대기 중 에러 발생시 에러 메시지 출력
		pthread_exit(NULL); // 스레드 종료
	}
}

void PutDown(int id) {
	/*
	[Function Name]: void PutDown(int id)
	[Description]:
		- 바이너리 세마포어를 통해, 두 철학자가 동시에 같은 젓가락을 내려 놓는 상황 방지
		- 식사 완료 후, 오른쪽 철학자와 왼쪽 철학자가 식사 가능한지 확인하고 식사를 할 수 있도로 설정
		- 현재 철학자의 세마포어 반납
	[Input]: 철학자 아이디
	[Output]: 현재 철학자의 상태를 THINKING으로 변경, 좌우 철학자의 상태 변경
	[Call By]: DiningPhilosopher(void *arg)
	[Calls]:
		- perror(char *err_msg)
		- pthread_exit(void *retval)
		- sem_wait(sem_t *sem)
		- sem_post(sem_t *sem)
		- Test(int id)
	*/
	if(sem_wait(&Mutex)) { // 바이너리 세마포어 대기
		perror("sem_wait"); // 대기 중 에러 발생시 에러 메시지 출력
		pthread_exit(NULL); // 비정상 종료
	}

	State[id] = THINKING; // 철학자 상태 THINKING으로 설정

	Test(LEFT(id)); // 왼쪽 철학자가 EATING 가능하면 왼쪽 철학자에게 세마포어 제공
	Test(RIGHT(id)); // 오른쪽 철학자가 EATING 가능하면 오른쪽 철학자에게 세마포어 제공

	if(sem_post(&Mutex)) { // 바이너레 세마포어 반납
		perror("sem_post"); // 반납에 실패하면 에러 메시지 출력
		pthread_exit(NULL); // 비정상 종료
	}
}

void DiningPhilosopher(void *arg) {
	/*
	[Function Name]: DiningPhilosopher(void *arg)
	[Description]: 개별 철학자 스레드의 행동 정의
	[Input]: 철학자 아이디
	[Output]: 철학자가 생각과 식사를 수행한 횟수를 콘솔에 출력
	[Call by]: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
	[Calls]:
		- Thinking(int id)
		- PickUp(int id)
		- Eating(int id)
		- PutDown(int id)
		- printf(char *format, ...)
		- pthread_exit(void *retval)
	*/
	int i; // 인덱스 저장을 위한 변수
	int id = *(int *)arg; // 철학자 id 저장

	// NLOOPS 만큼 반복하며 식사
	for(i = 0; i < NLOOPS; i++) {
		Thinking(id);
		PickUp(id);
		Eating(id);
		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);
	pthread_exit(NULL);
}

int main() {
	pthread_t tid[NUM_MEN];
	int id[NUM_MEN];

	srand(0x8888);

	// 각 철학자의 식사권에 대한 세마포어 생성, 초기화
	for(int i = 0; i < NUM_MEN; i++) {
		if(sem_init(&Philosopher[i], 0, 0)) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	// 철학자가 젓가락을 들고 놓는 행동에 대한 세마포어 생성, 초기화
	if(sem_init(&Mutex, 0, 1)) {
		perror("sem_init");
		exit(1);
	}

	// 철학자 스레드 생성
	for(int i = 0; i < NUM_MEN; i++) {
		if(pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, (void *)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	// 철학자 스레드 완료 대기, 완료 시 스레드 제거
	for(int i = 0; i < NUM_MEN; i++) {
		if(pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	// 철학자 식사권 제거
	for(int i = 0; i < NUM_MEN; i++) {
		if(sem_destroy(&Philosopher[i])) {
			perror("sem_destroy");
		}
	}

	// 젓가락을 들고 놓는 행동 제어에 대한 세마포어 제거
	if(sem_destroy(&Mutex)) {
		perror("sem_destroy");
		exit(1);
	}

	return 0;
}
