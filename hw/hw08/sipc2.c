#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "sipc.h"
#include "semlib.h"

/*
[Program Name]: sipc2
[Description]:
	- shared memory를 통해 sipc1과 통신
	- sipc1에 요청을 보냄
		- *p_data에 요청 메시지를 복사
		- 복사 완료 후에 *p_int flag를 1로 수정하여 sipc1에 요청 완료 했음을 알림
	- sipc2는 sipc1의 응답을 대기
		- ```semWait(replySemid)```를 통해 동기화 처리
	- sipc1에서 응답 완료 후에 *p_int flag를 0으로 수정
	- shmptr과 공유 메모리 연결 해제
	- 프로세스 종료
[Input]: None
[Output]: 콘솔에 출력되는 sipc1의 응답 메시지
[Calls]:
	- printf(char *format, ...)
	- fprintf(FILE *stream, const char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- usleep(useconds_t usec)
	- sprintf(cha *str, const char *format, ...)
	- shmdt(const void *shmaddr)
	- semInit(key_t key)
	- semWait(int semid)
	- semPost(int semid)
*/

int main() {
	int shm_id; // 공유 메모리 id를 저장할 변수 선언
	char *ptr, *p_data;
	// ptr: 공유 메모리 시작 주소를 저장할 변수
	// p_data: 공유 메모리에서 데이터 영역의 시작 주소

	// 세마포어 ID 저장 변수 선언
	int requestSemid;
	int replySemid;

	// 공유 메모리 할당, 공유 메모리 ID 저장
	if((shm_id = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget"); // 공유 메모리 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	// 할당된 공유 메모리에 접근할 가상 메모리 주소 할당
	if((ptr = shmat(shm_id, 0, 0)) == (void *)-1) {
		fprintf(stdout, "shmat, errno: %d", errno); // 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	// 세마포어 ID 저장
	if((requestSemid = semInit(REQUEST_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if((replySemid = semInit(REPLY_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n"); // 초기화에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	p_data = (char *)ptr; // char형으로 데이터 형식 캐스팅

	sprintf(p_data, "This is a request from %d.", getpid()); // p_data에 전송할 데이터 복사
	printf("Sent a request.....\n");

	// 요청 완료 후 세마포어에 값 1 추가
	if(semPost(requestSemid) < 0) {
		fprintf(stderr, "semPost (request) failure\n");
		exit(1);
	}

	// sipc1으로 부터의 응답 대기
	if(semWait(replySemid) < 0) {
		fprintf(stderr, "semWait (reply) failure\n");
		exit(1);
	}

	printf("Received reply: %s\n", p_data); // 응답 받으면 응답 메시지 출력

	if(shmdt(ptr) < 0) { // 현재 프로세스에서 shmptr과 공유 메모리 연결 해제
		perror("shmdt"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
