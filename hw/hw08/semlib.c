#include <stdio.h>
#include <unistd.h>
#include "semlib.h"

int semInit(key_t key) {
	/*
	[Function Name]: int semInit(key_t key)
	[Description]: 주어진 key의 세마포어를 생성
	[Input]: 세마포어의 키
	[Output]: 생성한 세마포어의 id 반환
	[Calls]:
		- semget(key_t key, int nsems, int flag)
		- perror(char *err_msg)
	*/

	int semid;

	if((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0) { // 세마포어 생성, semid에 세마포어 ID 저장
		perror("semget"); // 세마포어 생성에 실패한 경우 에러 메시지 출력
		return -1; // 비정상 종료
	}

	return semid; // 생성한 세마포어 ID 반환
}

int semInitValue(int semid, int value) {
	/*
	[Function Name]: int semInitValue(int semid, int value)
	[Description]: 주어진 id의 세마포어 value를 저장
	[Input]: 세마포어 ID, 저장할 value
	[Output]: 값이 변경된 세마포어 ID를 반환
	[Calls]:
		- perror(char *err_msg)
		- semctl(int semid, int semum, int cmd, union semun arg)
	*/
	union semun {
		int val;
	} semun; // 세마포어 값을 저장할 공용체 선언

	semun.val = value; // 유니온에 값을 저장

	if(semctl(semid, 0, SETVAL, semun) < 0) { // 주어진 세마포어 id의 값을 semun으로 설정
		perror("semctl"); // 값 설정에 실패한 경우 에러 메시지 출력
		return -1; // 비정상 종료
	}

	return semid; // 값이 변경된 세마포어 ID 반환
}

int semWait(int semid) {
	/*
	[Function Name]: int semWait(int semid)
	[Description]: 주어진 id의 세마포어 값을 감소
	[Input]: 값을 감소시킬 세마포어 id
	[Output]:
		- 성공한 경우 0을 반환
		- 실패한 경우 -1을 반환
	[Calls]:
		- perror(char *err_msg)
		- semop(int semid, struct sembuf *sops, size_t nsops)
	*/
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	// 세마포어 집합 내에서 조작할 세마포어 번호를 지정 
	// 여기선 0으로 설정하여 첫 번째 세마포어만 조작
	semcmd.sem_op = -1;
	// 세마포어 연산 값을 지정
	// -1: 세마포어 값을 1 감소시키는 P 연산(wait)
	// 세마포어 값이 0보다 크면 값을 감소
	// 값이 0이면 자원이 사용 중이므로 프로세스를 대기 상태로 전환
	semcmd.sem_flg = SEM_UNDO;
	// 연산에 대한 동작 플래그
	// SEM_UNDO: 프로세스가 종료되면 연산이 자동으로 취소되며 세마포어 상태를 복구

	if(semop(semid, &semcmd, 1) < 0) { // 세바포어 상태 변경
		perror("semop"); // 상태 변경에 실패한 경우 에러 메시지 출력
		return -1; // 비정상 종료
	}

	return 0;
}

int semPost(int semid) {
	/*
	[Function Name]: int semPost(int semid)
	[Description]: 주어진 id의 세마포어 값을 증가
	[Input]: 값을 증가시킬 세마포어 id
	[Output]:
		- 성공한 경우 0을 반환
		- 실패한 경우 1을 반환
	[Calls]:
		- perror(char *err_msg)
		- semop(int semid, struct sembuf *sops, size_t nsops)
	*/
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	// 세마포어 집합 내에서 조작할 세마포어 번호를 지정
	// 여기선 0으로 설정하여 첫 번째 세마포어만 조작
	semcmd.sem_op = 1;
	// 세마포어 연산 값을 지정
	// 1: 세마포어 값을 1 증가시키는 V 연산(signal)
	// 세마포어 값이 증가하며 대기 중인 프로세스가 깨어날 수 있음
	semcmd.sem_flg = SEM_UNDO;
	// SEM_UNDO: 프로세스가 종료되면 연산이 자동으로 취소되어 세마포어 상태 복구

	if(semop(semid, &semcmd, 1) < 0) {
		perror("semop"); // 상태 변경에 실패한 경우 에러 메시지 출력
		return -1; // 비정상 종료
	}

	return 0;
}

int semGetValue(int semid) {
	/*
	[Function Name]: int semGetValue(int semid)
	[Description]: 주어진 ID의 세마포어 값을 반환
	[Input]: 값을 반환한 세마포어의 ID
	[Output]:
		- semctl(int semid, int semnum, int cmd, ...) 반환 결과를 반환
		- GETVAL을 통해 세마포어에 저장된 값을 반환
	[Calls]:
		- semctl(int semid, int semnum, int cmd, ...)
	*/
	union semun {
		int val;
	} dummy; // semctl 호출을 위한 dummy 공용체 선언

	return semctl(semid, 0, GETVAL, dummy); // 세마포어에 저장된 값을 반환
}

int semDestroy(int semid) {
	/*
	[Function Name]: int semDestory(int semid)
	[Description]: 주어진 ID의 세마포어를 삭제
	[Input]: 삭제할 세마포어의 ID
	[Output]:
		- 삭제에 성공한 경우 0을 반환
		- 삭제에 실패한 경우 -1을 반환
	*/
	union semun {
		int val;
	} dummy; // semctl 호출을 위한 dummy 공용체 선언

	if (semctl(semid, 0, IPC_RMID, dummy) < 0) { // 주어진 ID의 세마포어를 삭제
		perror("semctl"); // 삭제에 실패한 경우 에러 메시지 출력
		return -1; // 비정상 종료
	}

	close(semid); // 세마포어와 연결된 file descipter 종료

	return 0;
}
