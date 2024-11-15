#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ARR_SIZE 1073741824
#define NUM_THREADS 8

typedef long long dtype;

typedef struct {
    const dtype *nums;
    size_t step;
    size_t start_idx;
    size_t end_idx;
    dtype sum;
} ThreadData;

void* parallel_sum(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->sum = 0;
    for(size_t i = data->start_idx; i < data->end_idx; i += data->step) {
        data->sum += data->nums[i];
    }
    return NULL;
}

dtype* get_nums(const size_t arr_size) {
    dtype *nums = malloc(arr_size * sizeof(dtype));
    if(nums == NULL) {
        perror("malloc");
        exit(-1);
    }

    for(size_t i = 0; i < arr_size; i++) {
        nums[i] = rand() % 10000;
    }

    return nums;
}

int main() {
    srand(time(NULL));
    dtype *nums = get_nums(ARR_SIZE);

    const size_t chunk_size = ARR_SIZE / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].nums = nums;
        thread_data[i].step = 1;
        thread_data[i].start_idx = i * chunk_size;
        thread_data[i].end_idx = (i == NUM_THREADS - 1) ? ARR_SIZE : (i + 1) * chunk_size;
        thread_data[i].sum = 0;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < NUM_THREADS; i++) {
        if(pthread_create(&threads[i], NULL, parallel_sum, &thread_data[i])) {
            perror("pthread_create");
            exit(-1);
        }
    }

    dtype total_sum = 0;
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].sum;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    const double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("total sum aggr result: %lld\n", total_sum);
    printf("sum aggr execution time: %lf seconds\n", time_spent);

    free(nums);

    return 0;
}