#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#define NLOOPS 5

sem_t ParentSem, ChildSem;

void parent_thread(void *dummy) {
	for(int i = 0; i < NLOOPS; i++) {
		sem_post(&ChildSem);
		printf("PARENT: Tell to child\n");
		printf("PARENT: Wait for child to tell\n");
		sem_wait(&ParentSem);
	}

	pthread_exit(NULL);
}

void child_thread(void *dummy) {
	for(int i = 0; i < NLOOPS; i++) {
		sem_wait(&ChildSem);
		printf("CHILD: Wait for parent to tell\n");
		printf("CHILD: Tell to parent\n");
		sem_post(&ParentSem);
	}

	pthread_exit(NULL);
}

int main() {
	pthread_t tid1, tid2;

	sem_init(&ParentSem, 0, 0);
	sem_init(&ChildSem, 0, 0);

	pthread_create(&tid1, NULL, (void *)parent_thread, (void *)NULL);
	pthread_create(&tid2, NULL, (void *)child_thread, (void *)NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	sem_destroy(&ParentSem);
	sem_destroy(&ChildSem);

	return 0;
}
