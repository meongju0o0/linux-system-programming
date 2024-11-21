#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(int)

/*
[Program Name]: mipc
[Description]:
	- shared memory를 통해 ptr 포인터가 가르키는 영역을 공유
	- 부모 자식 간에 번갈아가며 2씩 추가
	- 파이프를 사용하여 부모 자식간에 counter 변수 동기화
[Input]: None
[Output]: 부모, 자식 간 번갈아가며 출력되는 counter 변수
[Calls]:
	- printf(char* format, ...)
	- fprintf(FILE *stream, char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- close(int fd)
	- fork()
	- shmget(key_t key, size_t size, int shmflg)
	- shmat(int shmid, const void *shmaddr, int shmflg)
	- shmdt(const void *shmaddr)
	- shmctl(int shmid, int cmd, struct shmid_ds *buf)
	- TELL_WAIT()
	- TELL_CHILD()
	- WAIT_CHILD()
	- WAIT_PARENT()
	- TELL_PARENT()
	- update(long *ptr)
*/

int update(int *ptr) {
	/*
	[Function Name]: int update(int *ptr)
	[Description]: *ptr의 값에 1을 더하고 그 값을 반환
	[Input]: int *ptr
	[Output]: 1이 더해진 *ptr의 값
	[Call by]: main()
	[Calls]: None
	[Returns]: 1이 더해진 *ptr의 값을 반환
	*/
	return (*ptr)++;
}

int main() {
	int counter; // counter 변수 선언(콘솔 출력용, 실제 업데이트는 ptr을 통해 이루어짐)
	pid_t pid; // process id를 저장할 변수 선언

	int shmid; // 공유 메모리 id를 저장할 변수 선언
	int *ptr; // 공유 메모리 시작 주소를 저장할 변수

	if((shmid = shmget(SHM_KEY, SIZE, SHM_MODE)) < 0) { // 공유 메모리 할당, 공유 메모리 id 저장
		perror("shmget"); // 공유 메모리 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1) { // 할당된 공유 메모리에 접근할 가상 메모리 주소 할당
		perror("shmat"); // 가상 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	TELL_WAIT(); // 파이프 생성
	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid > 0) { // 부모 프로세스이면
		for(int i = 0; i < NLOOPS; i += 2) { // i를 2씩 증가 시키며(전체 반복 횟수의 절반)
			if((counter = update(ptr)) != i) { // counter 변수 update
				fprintf(stderr, "Counter mismatch\n"); // update에 실패한 경우 에러 메시지 출력
				// 예상되는 증가량인 2와 불일치하는 경우가 에러
				exit(1); // 비정상 종료
			}
			printf("Parent: counter=%d\n", counter); // 카운터 변수 출력
			TELL_CHILD(); // 자식 프로세스로 메시지 전송
			WAIT_CHILD(); // 자식 프로세스로부터의 메시지 대기
		}
	}
	else { // 자식 프로세스이면
		for(int i = 1; i < NLOOPS; i += 2) { // i를 2씩 증가 시키며(전체 반복 횟수의 절반)
			WAIT_PARENT(); // 부모 프로세스로부터의 메시지 대기
			if((counter = update(ptr)) != i) { // counter 변수 update
				fprintf(stderr, "Counter mismatch\n"); // update에 실패한 경우 에러 메시지 출력
				// 예상되는 증가량인 2와 불일치하는 경우가 에러
				exit(1); // 비정상 종료
			}
			printf("Child: counter=%d\n", counter); // 카운터 변수 출력
			TELL_PARENT(); // 부모 프로세스에 메시지 전송
		}
	}

	if(shmdt(ptr) < 0) { // shmptr과 공유 메모리 연결 해제(부모, 자식 프로세스 모두 실행)
		perror("shmdt"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	struct shmid_ds shm_info;
	if(shmctl(shmid, IPC_STAT, &shm_info) == 0 && shm_info.shm_nattch == 1) { // 공유 메모리에 attach된 프로세스가 없는 경우
		if(shmctl(shmid, IPC_RMID, NULL) < 0) { // 공유 메모리 할당 해제
			perror("shmctl"); // 해제에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}

	return 0;
}
