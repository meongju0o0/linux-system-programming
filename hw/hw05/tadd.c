#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ARR_SIZE 100
#define NUM_THREADS 2

typedef struct {
	const int *nums;
	size_t start_idx;
	size_t end_idx;
	int sum;
} ThreadData; // parallel_sum에 arguemnt를 넘기기 위한 구조체

/*
[Program Name]: ttad
[Description]: 스레드를 활용하여 병렬적으로 덧셈 연산 수행
[Input]: None
[Output]: 1~100을 더한 결과를 콘솔에 출력
[Calls]:
	malloc(size_t mem_size)
	perror(char* err_msg)
	exit(int EXIT_CODE)
	printf(char* format, ...)
	pthread_create(pthread_t *tid, pthread_attr_t *attr, void *(start_routine) (void *), void *arg)
	pthread_join(pthread_t tid, void **thread_return)
*/

void *parallel_sum(void *arg) {
	/*
	[Function Name]: void *parallel_sum(void *arg)
	[Description]: 주어진 idx 범위에 저장된 배열의 값을 모두 합한 결과를 반환
	[Input]: ThreadData
	[Output]: data->sum에 저장된 결과
	[Calls]: None
	*/
	ThreadData *data = (ThreadData *)arg;
	data->sum = 0;
	for(size_t i = data->start_idx; i < data->end_idx; i++) {
		data->sum += data->nums[i];
	}
	return NULL;
}

int main() {
	int *nums = (int *)malloc(ARR_SIZE * sizeof(int)); // 메모리 동적 할당, 스레드 간 공유 자원으로 사용
	if(nums == NULL) { // 동적 할당에 실패한 경우(malloc이 NULL을 반환)
		perror("malloc"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	for(int i = 0; i < ARR_SIZE; i++) { // 어레이의 각 아이템에 값을 저장
		nums[i] = i + 1; // nums[0:100]에 (1~100)을 저장함
	}

	pthread_t threads[NUM_THREADS]; // 스레드 아이디들을 저장할 배열 선언
	ThreadData thread_data[NUM_THREADS]; // parallel_sum이 필요한 argument들을 저장할 배열 선언

	size_t chunk_size = ARR_SIZE / NUM_THREADS; // 각 스레드가 계산할 배열 인덱스 지정
	for(int i = 0; i < NUM_THREADS; i++) { // 각 스레드에 대해
		thread_data[i].nums = nums; // thread_data[i].nums에 nums 배열 포인터 저장
		thread_data[i].start_idx = i * chunk_size; // thread_data[i].start_idx에 시작 인덱스 저장
		thread_data[i].end_idx = (i == NUM_THREADS - 1) ? ARR_SIZE : (i + 1) * chunk_size; // thread_data[i].end_idx에 종료 인덱스 저장
		thread_data[i].sum = 0; // thread_data[i].sum에 덧셈 결과 저장
		
		if(pthread_create(&threads[i], NULL, parallel_sum, &thread_data[i])) { // 덧셈 연산을 수행할 스레드 선언
			perror("pthread_create"); // 스레드 생성에 실패할 경우 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}

	int total_sum = 0; // 전체 덧셈 결과를 저장할 변수 선언
	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL); // 스레드 종료까지 대기
		printf("THREAD #%d SUM AGGR RESULT: %d\n", i, thread_data[i].sum); // 종료된 스레드의 덧셈 결과 출력
		total_sum += thread_data[i].sum; // 전체 덧셈 결과에 종료된 스레드의 덧셈 결과 더함
	}

	printf("TOTAL SUM AGGR RESULT: %d\n", total_sum); // 전체 덧셈 결과 출력

	free(nums); // nums 배열 동적 할당 해제

	return 0;
}
