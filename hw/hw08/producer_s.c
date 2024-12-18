#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

/*
[Program Name]: producer_s
[Description]:
	- 세마포어를 사용하여 기존 producer 프로그램의 동기화 문제를 해결
	- 공유 메모리를 선언하여 프로세스 간 공유해서 사용하는 버퍼 선언
	- 세마포어를 사용하여 공유 메모리 접근 시, 동시성 문제가 발생하는 경우를 해결
[Input]: None
[Output]:
	- producer에서 생산하고 있는 상황을 콘솔에 출력
[Calls]:
	- perror(char *err_msg)
	- fprintf(FILE *stream, const char *foramt, ...)
	- shmget(key_t key, size_t size, int shmflg)
	- shmat(int shmid, const void *shmaddr, int shmflg)
	- semInit(key_t key)
	- semWait(int semid)
	- semPost(int semid)
*/

int main() {
	BoundedBufferType *pBuf; // 아이템을 저장하고 있는 버퍼의 포인터를 저장할 포인터 변수 선언
	int shmid, i, data; // 공유 메모리 ID, 인덱스, 데이터를 저장할 변수 선언
	int emptySemid, fullSemid, mutexSemid; // 세마포어 ID를 관리할 변수 선언

	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유메모리 생성
		perror("shmget"); // 공유메모리 생성에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 주소 할당
		perror("shmat"); // 공유 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) { // 버퍼의 빈공간을 관리할 세마포어 변수 초기화
		fprintf(stderr, "semInit failure\n"); // 초기화에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((fullSemid = semInit(FULL_SEM_KEY)) < 0) { // 버퍼에서 사용중인 공간을 관리할 세마포어 변수 초기화
		fprintf(stderr, "semInit failure\n"); // 초기화에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) { // 버퍼로의 동시 접근을 막기 위한 세마포어 변수 초기화
		fprintf(stderr, "semInit failure\n"); // 초기화에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	srand(0x8888); // 난수 생성 시드값 설정

	for(i = 0; i < NLOOPS; i++) {
		if(semWait(emptySemid) < 0) {
			// 버퍼가 가득 차 있는 동안 대기
			// 버퍼가 사용가능한 상태로 변경되어 버퍼 내 값을 변경할 수 있는 경우 세마포어 값 1 감소
			fprintf(stderr, "semWait failure\n"); // semWait에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		if(semWait(mutexSemid) < 0) { // 버퍼를 사용 중인 동안 대기
			fprintf(stderr, "semWait failure\n"); // semWait에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}

		printf("Producer: Producing an item.....\n"); // 아이템 생산중이라는 것을 콘솔에 출력
		data = (rand() % 100) * 10000; // 난수 데이터 생성
		pBuf->buf[pBuf->in].data = data; // 버퍼에 데이터 삽입
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // 다음 버퍼 위치 갱신
		pBuf->counter++; // 버퍼 카운터 변수 1 증가

		if(semPost(mutexSemid) < 0) { // 버퍼를 더이상 사용하지 않는다는 것을 세마포어에 기록
			fprintf(stderr, "semPost failure\n"); // semPost에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		if(semPost(fullSemid) < 0) { // 버퍼에 사용가능한 공간이 줄어들었다는 것을 세마포어에 기록
			fprintf(stderr, "semPost failure\n"); // semPost에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		usleep(data); // data μs 만큼 대기
	}

	printf("Producer: Produced %d items.....\n", i); // 생산한 아이템 개수를 출력

	sleep(2); // 2초간 대기
	printf("Producer: %d items in buffer.....\n", pBuf->counter); // 버퍼에 남은 아이템 개수를 출력

	if(semDestroy(emptySemid) < 0) { // 세마포어 삭제에 실패하면
		fprintf(stderr, "semDestory failure\n"); // 에러 메시지 출력
	}
	if(semDestroy(fullSemid) < 0) { // 세마포어 삭제에 실패하면
		fprintf(stderr, "semDestroy failure\n"); // 에러 메시지 출력
	}
	if(semDestroy(mutexSemid) < 0) { // 세마포어 삭제에 실패하면
		fprintf(stderr, "semDestroy failure\n"); // 에러 메시지 출력
	}

	if(shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 삭제
		perror("shmctl"); // 삭제에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}