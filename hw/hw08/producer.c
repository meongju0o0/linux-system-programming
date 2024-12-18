#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]: producer
[Description]:
	- 버퍼에 남는 공간이 있는 경우 아이템을 생성 후 삽입
	- 위 과정을 NLOOPS회 반복
[Input]: None
[Output]:
	- 콘솔에 아이템을 생성하고 버퍼에 삽입한다는 것을 출력
	- 반복문이 종료된 후, 버퍼의 상황 출력
[Calls]:
	- shmget(key_t key, size_t size, int shmflg)
	- shmat(int shmid, const void *shmaddr, int shmflg)
	- srand(unsigned int seed)
	- usleep(useconds_t usec)
	- perror(char *err_msg)
	- printf(char *format, ...)
*/

int main() {
	BoundedBufferType *pBuf; // 아이템을 저장하고 있는 버퍼의 포인터를 저장할 포인터 변수 선언
	int i, shmid, data;

	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성
		perror("shmget"); // 공유 메모리 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1) { // 공유 메모리 주소 할당
		perror("shmat"); // 공유 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	srand(0x8888); // 난수 시드값 설정

	for(i = 0; i < NLOOPS; i++) {
		if(pBuf->counter == MAX_BUF) { // 버퍼가 가득 찬 경우
			printf("Producer: Buffer full. Waiting.....\n"); // 대기중이라는 것을 콘솔에 출력 
			while(pBuf->counter == MAX_BUF); // 콘솔에 빈 공간이 생길 때까지 대기 (Busy Waiting)
		}

		printf("Producer: Producing an item.....\n"); // 아이템 생산중이라는 것을 콘솔에 출력
		data = (rand() % 100) * 10000; // 난수 데이터 생성
		pBuf->buf[pBuf->in].data = data; // 버퍼에 데이터 삽입
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // 다음 버퍼 위치 갱신
		pBuf->counter++; // 버퍼 카운터 변수 1 증가

		usleep(data); // data μs 만큼 대기
	}

	printf("Producer: Produced %d items.....\n", i); // 생산한 총 아이템 개수 출력

	sleep(2); // 프로세스 2초간 대기
	printf("Producer: %d items in buffer.....\n", pBuf->counter); // 버퍼에 남은 아이템 개수 출력

	if(shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 할당 해제
		perror("shmctl"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
