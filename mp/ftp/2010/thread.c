#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>

sem_t r, w;
char Msg[80];

void in() {
	while (1) {
		sem_wait(&r);
		printf("In> ");
		fflush(stdout);
		scanf("%s", Msg);
		sem_post(&w);
		
		if (!strcmp(Msg, "quit"))
			break;
	}
}

void out() {
	while (1) {
		sem_wait(&w);
		printf("Out> ");
		for (int i = 0; i < strlen(Msg); i++)
			printf("%c", toupper(Msg[i]));
		printf("\n");
		sem_post(&r);

		if (!strcmp(Msg, "quit"))
			break;
	}
}

void main() {
	pthread_t tid[2];
	
	sem_init(&r, 0, 1);
	sem_init(&w, 0, 0);

	pthread_create(&tid[0], NULL, (void *)in, NULL);
	pthread_create(&tid[1], NULL, (void *)out, NULL);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	sem_destroy(&r);
	sem_destroy(&w);
}
