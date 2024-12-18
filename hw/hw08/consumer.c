#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]: consumer
[Description]:
	- 버퍼에 아이템이 존재하는 경우 아이템을 꺼냄
	- 위 과정을 NLOOPS회 반복
[Input]: None
[Output]:
	- 콘솔에서 아이템을 꺼내고 버퍼에서 꺼냈다는 것을 출력
	- 반복문이 종료된 후, 버퍼의 상황 출력
*/

int main() {
	BoundedBufferType *pBuf; // 아이템을 저장하고 있는 버퍼의 포인터를 저장할 포인터 변수 선언
	int shmid, i, data;

	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 생성
		perror("shmget"); // 공유 메모리 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { // 공유 메모리 주소 할당
		perror("shmat"); // 공유 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	srand(0x9999); // 난수 시드값 설정

	for(i = 0; i < NLOOPS; i++) {
		if(pBuf->counter == 0) { // 버퍼가 가득 찬 경우
			printf("Consumer: Buffer empty. Waiting.....\n"); // 대기중이라는 것을 콘솔에 출력
			while(pBuf->counter == 0); // 콘솔에 빈 공간이 생길 때까지 대기 (Busy Waiting)
		}

		printf("Consumer: Consuming an item.....\n"); // 아이템을 소비중이라는 것을 출력
		data = pBuf->buf[pBuf->out].data; // 버퍼의 아이템을 복사
		pBuf->out = (pBuf->out + 1) % MAX_BUF; // 다음 버퍼 위치 갱신
		pBuf->counter--; // 버퍼 카운터 변수 1 감소

		usleep((rand() % 100) * 10000); // ((rand() % 100) * 10000) μs 만큼 대기
	}

	printf("Consumer: Consumed %d items.....\n", i); // 소비한 총 아이템 개수 출력
	printf("Consumer: %d items in buffer.....\n", pBuf->counter); // 버퍼에 남은 아이템 개수 출력

	return 0;
}
