#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

sem_t r, w;
char msg[100];

void threadA() {
	while (1) {
		sem_wait(&r);
		printf("Input: ");
		fflush(stdout);
		fgets(msg, 100, stdin);
		sem_post(&w);

		if (!strcmp(msg, "quit\n"))
			break;
	}
}

void threadB() {
	while (1) {
		sem_wait(&w);
		printf("Output: %s", msg);
		sem_post(&r);

		if (!strcmp(msg, "quit\n"))
			break;
	}
}

void main() {
	pthread_t tid[2];

	sem_init(&r, 0, 1);
	sem_init(&w, 0, 0);

	pthread_create(&tid[0], NULL, (void *)threadA, NULL);
	pthread_create(&tid[1], NULL, (void *)threadB, NULL);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	sem_destroy(&r);
	sem_destroy(&w);
}
