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
[Program Name]: sipc1
[Description]:
	- shared memory를 통해 sipc2와 통신
	- sipc1은 sipc2의 요청을 대기
		- ```semWait(mutexSemid)```를 통해 동기화 처리
	- sipc2에서 요청 완료 후에 *p_int flag를 1로 수정
	- sipc2에 응답을 보냄
		- *p_data에 응답 메시지를 복사
		- 복사 완료 후에 *p_int flag를 0으로 수정하여 spic2에 응답 완료 했음을 알림
	- shmptr과 공유 메모리 연결 해제
	- sipc2에서도 더이상 shared memory를 사용하지 않으면 공유 메모리 할당 해제
	- 프로세스 종료
[Input]: None
[Output]: 콘솔에 출력되는 sipc2의 요청 메시지
[Calls]:
	- printf(char *format, ...)
	- fprintf(FILE *stream, const char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- sprintf(char *str, const char *format, ...)
	- shmdt(const void *shmaddr)
	- shmctl(int shmid, int cmd, struct shmid_ds *buf)
	- semInit(key_t key)
	- semInitValue(int semid, int value)
	- semWait(int semid)
	- semPost(int semid)
	- semDestroy(int semid)
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
		fprintf(stdout, "shmat, errno: %d", errno); // 가상 메모리 주소 할당에 실패한 경우 에러 메시지 출력
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

	// 세마포어 값 할당
	if(semInitValue(requestSemid, 0) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if(semInitValue(replySemid, 0) < 0) {
		fprintf(stderr, "SemInitValue failure\n"); // 값 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	// sipc2로 부터의 요청 대기
	if(semWait(requestSemid) < 0) {
		fprintf(stderr, "semWait failure\n"); // 요청 대기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	p_data = (char *)ptr; // char형으로 데이터 형식 캐스팅

	printf("Received request: %s.....\n", p_data); // sipc2로부터 전송된 데이터 출력
	sprintf(p_data, "This is a reply from %d.", getpid()); // p_data에 전송할 데이터 복사
	printf("Replied\n");

	if(semPost(replySemid) < 0) { // 세마포어에 값 1 추가
		fprintf(stderr, "semPost (replySemid) failure\n");
		exit(1);
	}

	if(shmdt(ptr) < 0) { // 현재 프로세스에서 shmptr과 공유 메모리 연결 해제
		perror("shmdt"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	struct shmid_ds shm_info;
	if(shmctl(shm_id, IPC_STAT, &shm_info) == 0 && shm_info.shm_nattch == 1) { // 공유 메모리에 attach된 프로세스가 없는 경우
		if(shmctl(shm_id, IPC_RMID, NULL) < 0) { // 공유 메모리 할당 해제
			perror("shmctl"); // 해제에 실패한 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}

	// 세마포어 삭제
	if (semDestroy(requestSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(replySemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}

	return 0;
}
