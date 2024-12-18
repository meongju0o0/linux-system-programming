#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "semlib2.h"

#define BUF_SIZE 1024

char shared_data[BUF_SIZE];

sem_t RepSem, ReqSem, MutexSem;

void sipc1(void *dummy) {
	if(sem_wait(&ReqSem)) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if(sem_wait(&MutexSem)) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("sipc1: Received request: %s\n", shared_data);
	strcpy(shared_data, "This is a reply from sipc1");
	printf("sipc1: Replied\n");

	if(sem_post(&MutexSem)) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_post(&RepSem)) {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void sipc2(void *dummy) {
	if(sem_wait(&MutexSem)) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	strcpy(shared_data, "This is a request from sipc2");
	printf("sipc2: Sent a request.....\n");

	if(sem_post(&MutexSem)) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_post(&ReqSem)) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	if(sem_wait(&RepSem)) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("sipc2: Received reply: %s\n", shared_data);
}

int main() {
	pthread_t tid1, tid2;

	if(sem_init(&RepSem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&ReqSem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&MutexSem, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	if(pthread_create(&tid1, NULL, (void *)sipc1, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)sipc2, (void *)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	if(sem_destroy(&RepSem) < 0) {
		perror("sem_destroy");
	}
	if(sem_destroy(&ReqSem) < 0) {
		perror("sem_destroy");
	}
	if(sem_destroy(&MutexSem) < 0) {
		perror("sem_destroy");
	}

	return 0;
}
