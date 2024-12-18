#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int sum, count;
pthread_mutex_t mutex;
pthread_cond_t cond;

void threadFunc(void* arg) {
	int threadid = *(int *)arg;
	free(arg);
	int threadsum = 0;

	for (int i = 0; i < 10; i++)
		threadsum += i + threadid * 10;

	pthread_mutex_lock(&mutex);
	
	sum += threadsum;
	count++;
	if (count == 5) {
		pthread_cond_broadcast(&cond);
		printf("%d\n", sum);
	}
	else pthread_cond_wait(&cond, &mutex);
	
	pthread_mutex_unlock(&mutex);
}

void main() {
	pthread_t tid[5];
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	for (int i = 0; i < 5; i++) {
		int* id = malloc(sizeof(int));
		*id = i;
		pthread_create(&tid[i], NULL, (void *)threadFunc, (void *)id);
	}

	for (int i = 0; i < 5; i++)
		pthread_join(tid[i], NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}
