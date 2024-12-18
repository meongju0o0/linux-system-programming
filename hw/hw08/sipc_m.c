#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define BUF_SIZE 1024

char shared_data[BUF_SIZE];

pthread_cond_t Req;
pthread_cond_t Rep;
pthread_mutex_t Mutex;

void sipc1(void *dummy) {
	if(pthread_mutex_lock(&Mutex)) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	if(pthread_cond_wait(&Req, &Mutex)) {
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}

	printf("sipc1: Received request: %s\n", shared_data);
	strcpy(shared_data, "This is a reply from sipc1");
	printf("sipc1: Replied\n");

	if(pthread_cond_signal(&Rep)) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	if(pthread_mutex_unlock(&Mutex)) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
}

void sipc2(void *dummy) {
	if(pthread_mutex_lock(&Mutex)) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	strcpy(shared_data, "This is a request from sipc2");
	printf("sipc2: Sent a request.....\n");

	if(pthread_cond_signal(&Req)) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	if(pthread_cond_wait(&Rep, &Mutex)) {
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}

	if(pthread_mutex_unlock(&Mutex)) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	printf("sipc2: Received reply: %s\n", shared_data);
}

int main() {
	pthread_t tid1, tid2;

	if(pthread_mutex_init(&Mutex, NULL)) {
		perror("pthread_mutex_init");
		exit(1);
	}

	if(pthread_cond_init(&Req, NULL)) {
		perror("pthread_cond_init");
		exit(1);
	}
	if(pthread_cond_init(&Rep, NULL)) {
		perror("pthread_cond_init");
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

	if(pthread_join(tid1, NULL)) {
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL)) {
		perror("pthread_join");
		exit(1);
	}

	if(pthread_cond_destroy(&Req)) {
		perror("pthread_cond_destroy");
	}
	if(pthread_cond_destroy(&Rep)) {
		perror("pthread_cond_destroy");
	}

	if(!pthread_mutex_destroy(&Mutex)) {
		perror("pthread_mutex_destroy");
	}

	return 0;
}
