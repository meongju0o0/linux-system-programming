#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(int)

/*
[Program Name]: mipc
[Description]:
	- mmap을 통해 counter 변수를 공유
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
	- mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
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
	int fd, counter;
	// file descripter 저장 변수
	// counter 변수 선언(콘솔 출력용, 실제 업데이트는 area를 통해 이루어짐)
	pid_t pid; // process id를 저장할 변수 선언
	caddr_t area; // mmap을 통해 매핑한 메모리의 주소를 저장

	if((fd = open("/dev/zero", O_RDWR)) < 0) { // mmap을 위해 /dev/zero 파일을 열어서 사용
		perror("open"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t)-1) { // area와 fd 메모리 매핑
	/*
	0: 커널이 매핑할 메모리 주소를 선택
	SIZE: 파일 크기
	PROT_READ: 읽기 모드
	MAP_SHARED: 파일과 메모리 동기화
	fd: file descripter
	0: offset
	*/
		perror("mmap"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	close(fd); // file descripter 종료

	TELL_WAIT(); // 파이프 생성
	if((pid = fork()) < 0) { // 자식 프로세스 생성
		perror("fork"); // 자식 프로세스 생성에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(pid > 0) { // 부모 프로세스이면
		for(int i = 0; i < NLOOPS; i += 2) { // i를 2씩 증가 시키며(전체 반복 횟수의 절반)
			if((counter = update((int *)area)) != i) { // counter 변수 update
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
			if((counter = update((int *)area)) != i) { // counter 변수 update
				fprintf(stderr, "Counter mismatch\n"); // update에 실패한 경우 에러 메시지 출력
				// 예상되는 증가량인 2와 불일치하는 경우가 에러
				exit(1); // 비정상 종료
			}
			printf("Child: counter=%d\n", counter); // 카운터 변수 출력
			TELL_PARENT(); // 부모 프로세스에 메시지 전송
		}
	}

	return 0;
}
