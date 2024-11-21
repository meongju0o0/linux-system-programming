#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARR_SIZE 40000
#define MALLOC_SIZE 10000
#define SHM_SIZE 10000
#define SHM_MODE (SHM_R | SHM_W)

char Array[ARR_SIZE];

/*
[Program Name]: shm
[Description]: 전역 메모리, 스택(정적 할당) 메모리, 힙(동적 할당) 메모리, 공유 메모리를 할당받은 후 시작 주소를 출력
[Input]: None
[Output]: 콘솔에 각 메모리의 시작 주소를 출력
[Calls]:
	- printf(char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- malloc(size_t size)
	- shmget(key_t key, size_t size, int shmflg)
	- shmat(int shmid, const void *shmaddr, int shmflg)
	- shmdt(const void *shmaddr)
	- shmctl(int shmid, int cmd, struct shmid_ds *buf)
*/

int main() {
	int shmid; // 공유 메모리 id를 저장할 변수 선언
	char *ptr, *shmptr;
	// ptr: 동적 메모리 시작 주소를 저장할 변수
	// shmptr: 공유 메모리 시작 주소를 저장할 변수

	if((ptr = (char *)malloc(MALLOC_SIZE)) == NULL) { // 동적 할당에 실패한 경우
		perror("malloc"); // 동적 메모리 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 할당, 공유 메모리 id 저장
		perror("shmget"); // 공유 메모리 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((shmptr = shmat(shmid, 0, 0)) == (void *)-1) { // 할당된 공유 메모리에 접근할 가상 메모리 주소 할당
		perror("shmat"); // 가상 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	printf("Array[] from %p to %p\n", &Array[0], &Array[ARR_SIZE]); // 전역 메모리 시작 주소와 끝 주소 출력
	printf("Stack around %p\n", &shmid); // 스태(정적 할당) 메모리 시작 주소 출력
	printf("Malloced from %p to %p\n", ptr, ptr + MALLOC_SIZE); // 힙(동적 할당) 메모리 시작 주소와 끝 주소 출력
	printf("Shared Memory attached from %p to %p\n", shmptr, shmptr + SHM_SIZE); // 공유 메모리 시작 주소와 끝 주소 출력

	if(shmdt(shmptr) < 0) { // 현재 프로세스에서 shmptr과 공유 메모리 연결 해제
		perror("shmdt"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(shmctl(shmid, IPC_RMID, 0) < 0) { // 공유 메모리 할당 해제
		perror("shmctl"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
