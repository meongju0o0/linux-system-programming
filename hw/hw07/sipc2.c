#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

/*
[Program Name]: sipc2
[Description]:
	- shared memory를 통해 sipc1과 통신
	- sipc1에 요청을 보냄
		- *p_data에 요청 메시지를 복사
		- 복사 완료 후에 *p_int flag를 1로 수정하여 sipc1에 요청 완료 했음을 알림
	- sipc2는 sipc1의 응답을 대기
		- ```while(*p_int == 1) usleep(100);```을 통해 100μsec마다 응답 확인(busy waiting)
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
*/

int main() {
	int shm_id; // 공유 메모리 id를 저장할 변수 선언
	char *ptr, *p_data;
	// ptr: 공유 메모리 시작 주소를 저장할 변수
	// p_data: 공유 메모리에서 데이터 영역의 시작 주소
	int *p_int;
	// p_int: 공유 메모리에서 flag 영역의 시작 주소

	if((shm_id = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // 공유 메모리 할당, 공유 메모리 id 저장
		perror("shmget"); // 공유 메모리 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((ptr = shmat(shm_id, 0, 0)) == (void *)-1) { // 할당된 공유 메모리에 접근할 가상 메모리 주소 할당
		fprintf(stdout, "shmat, errno: %d", errno); // 가상 메모리 주소 할당에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	p_int = (int *)ptr; // 가상 메모리 시작 주소로부터 4바이트 만큼 요청/응답 flag로 사용
	
	p_data = (char *)ptr + sizeof(int); // flag 뒤부터 데이터 영역으로 사용
	sprintf(p_data, "This is a request from %d.", getpid()); // p_data에 전송할 데이터 복사
	*p_int = 1; // flag를 1로 수정
	printf("Sent a request.....");

	while(*p_int == 1) usleep(100); // flag가 1인 상태에서 반복 대기
	// sipc1에서 응답을 다 보내고 나면 flag를 0으로 수정하여 반복문 탈출

	printf("Received reply: %s\n", p_data); // sipc1으로부터 전송된 데이터 출력

	if(shmdt(ptr) < 0) { // 현재 프로세스에서 shmptr과 공유 메모리 연결 해제
		perror("shmdt"); // 해제에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	return 0;
}
