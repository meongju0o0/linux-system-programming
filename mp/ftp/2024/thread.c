#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

char msg[100];
sem_t sem;

void writeThread() {
	strcpy(msg, "hello");
	sem_post(&sem);
}

void readThread() {
	sem_wait(&sem);
	printf("%s\n", msg);
}

void main() {
	pthread_t tid[2];

	sem_init(&sem, 0, 0);

	pthread_create(&tid[0], NULL, (void *)writeThread, NULL);	
	pthread_create(&tid[1], NULL, (void *)readThread, NULL);	

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	sem_destroy(&sem);
}
