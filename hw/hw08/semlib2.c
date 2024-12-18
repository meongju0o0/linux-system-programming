#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

int sem_init(sem_t *sem, int pshared, int value) {
	/*
	[Function Name]: int sem_init(sem_t *sem, int pshared, int value)
	[Description]: 주어진 세마포어를 주어진 값에 따라 초기화
	[Input]: 세마포어, 프로세스 간 공유 유무(0만 가능), 세마포어 값
	[Output]:
		- 성공시 0 반한
		- 실패시 -1 반환
	[Calls]:
		- fprintf(FILE *stream, const char *format, ...)
		- pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *mattr)
		- pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cattr)
	*/

	// ```pshared == 0```: 세마포어를 스레드 간 공유를 위해 사용
	// ```pshared != 0```: 세마포어를 프로세스 간 공유를 위해 사용
	// 스레드 간 세마포어만 가능
	if (pshared) {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	// mutex로 사용할 semaphore 초기화
	if (pthread_mutex_init(&sem->mutex, NULL) < 0) {
		return -1;
	}

	// condition variable 초기화
	if (pthread_cond_init(&sem->cond, NULL) < 0) {
		return -1;
	}

	sem->sval = value;

	return 0;
}

int sem_wait(sem_t *sem) {
	/*
	[Function Name]: int sem_wait(sem_t *sem)
	[Description]:
		- 세마포어 값이 0보다 크면 바로 세마포어 값 감소
		
		- 세마포어 값이 0인 경우 잠시 뮤텍스를 반납 후, 다른 스레드가 sem_post를 수행하기를 대기
		- 세마포어 값이 0보다 커지면 세마포어 값 감소
	[Input]: 세마포어
	[Output]:
		- 성공시 0 반환
		- 실패시 -1 반환
	[Calls]:
		- pthread_mutex_lock(pthread_mutex_t *mutex)
		- pthread_mutex_unlock(pthread_mutex_t *mutex)
		- pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
	*/
	// mutex 획득
	if (pthread_mutex_lock(&sem->mutex) < 0) {
		return -1;
	}

	// semaphore value가 0인 경우
	if (sem->sval == 0) {
		while (sem->sval == 0) {
			// condition variable이 1 이상일 때까지 대기, 조건 충족 전까지 mutex 반납
			// 즉, 잠시 mutex를 반납하여 다른 스레드가 sem_post(sem_t *sem)을 통해 condition varaible을 만족시킬 때까지 대기
			// 조건 충족 시, mutex 획득
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0) {
				// mutex 반납
				if (pthread_mutex_unlock(&sem->mutex) < 0) {
					return -1;
				}
				return -1;
			}
		}
		sem->sval--; // 세마포어 값 1 감소
	}

	// semaphore value가 0이 아닌 경우
	else {
		sem->sval--; // 세마포어 값 1 감소
	}

	// 뮤텍스 반납
	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;
	}

	return 0;
}

int sem_trywait(sem_t *sem) {
	/*
	[Function Name]: int sem_trywait(sem_t *sem)
	[Description]:
		- 세마포어 값이 0보다 크면 바로 세마포어 값 감소
		
		- 세마포어 값이 0인 경우 뮤텍스 반납 후, 함수 종료
		- 세마포어 값을 감소시키지 않음
	[Input]: 세마포어
	[Output]:
		- 성공시 0 반환
		- 실패시 -1 반환
	[Calls]:
		- pthread_mutex_lock(pthread_mutex_t *mutex)
		- pthread_mutex_unlock(pthread_mutex_t *mutex)
	*/

	// mutex 획득
	if (pthread_mutex_lock(&sem->mutex) < 0) {
		return -1;
	}

	// semaphore value가 0인 경우
	if (sem->sval == 0) {
		// mutex 반납
		if (pthread_mutex_unlock(&sem->mutex) < 0) {
			return -1;
		}
		return -1; // 함수 종료
	}

	// semaphore value가 0보다 큰 경우
	else {
		sem->sval--; // 세마포어 값 감소
	}

	// mutex 반납
	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;
	}

	return 0;
}

int sem_post(sem_t *sem) {
	/*
	[Function Name]: int sem_post(sem_t *sem)
	[Description]:
		- 세마포어 값이 0인 경우
			- condition variable 값을 1로 설정해 sem_wait가 가능한 상태임을 설정
			- 뮤텍스 해제
			- 세마포어 값 1 증가
		- 세마포어 값이 0보다 큰 경우
			- 세마포어 값 1 증가
	[Input]: 세마포어
	[Output]:
		- 성공시 0 반환
		- 실패시 -1 반환
	[Calls]:
		- pthread_mutex_lock(pthread_mutex_t *mutex)
		- pthread_mutex_unlock(pthread_mutex_t *mutex)
		- pthread_cond_signal(pthread_cond_t *cond)
	*/

	// 뮤텍스 획득
	if (pthread_mutex_lock(&sem->mutex) < 0) {
		return -1;
	}
	
	// 세마포어 값이 0인 경우
	if (sem->sval == 0) {
		// condition variable 값을 1로 설정해 sem_wait가 가능한 상태임을 설정
		if (pthread_cond_signal(&sem->cond) < 0) {
			// 뮤텍스 해제
			if (pthread_mutex_unlock(&sem->mutex) < 0) {
				return -1;
			}
			return -1;
		}
	}

	sem->sval++; // 세마포어 값 1 증가

	// 뮤텍스 해제
	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;
	}

	return 0;
}

int sem_getvalue(sem_t *sem, int *sval) {
	/*
	[Function Name]: int sem_getvalue(sem_t *sem, int *sval)
	[Descripiton]: 현재 세마포어에 저장된 값을 반환
	[Input]: 세마포어
	[Output]: 세마포어에 저장된 값
	[Calls]: None
	*/
	
	*sval = sem->sval; // sval에 sem->sval 저장

	return 0;
}

int sem_destroy(sem_t *sem) {
	/*
	[Function Name]: int sem_destroy(sem_t *sem)
	[Description]: 세마포어 삭제
	[Input]: 세마포어
	[Output]: None
	[Calls]:
		- pthread_mutex_destroy(pthread_mutex_t *mutex)
		- pthread_cond_destroy(pthread_cond_t *cond)
	*/

	// mutex 삭제
	if (pthread_mutex_destroy(&sem->mutex) < 0) {
		return -1;
	}

	// condition varialbe 삭제
	if (pthread_cond_destroy(&sem->cond) < 0) {
		return -1;
	}

	return 0;
}
