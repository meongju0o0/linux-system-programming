#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define BUF_SIZE 1024

char shared_data[BUF_SIZE];

int data_ready = 0;

pthread_cond_t MsgCond;
pthread_mutex_t Mutex;

void input_thread(void *dummy) {
	if (pthread_mutex_lock(&Mutex)) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	
	printf("Input: ");
	scanf("%s", shared_data);

	if (strcmp(shared_data, "quit") == 0) {
		exit(0);
	}

	data_ready = 1;

	if (pthread_cond_signal(&MsgCond)) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	if (pthread_mutex_unlock(&Mutex)) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
}

void output_thread(void *dummy) {
	if (pthread_mutex_lock(&Mutex)) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	while(!data_ready) {
		if (pthread_cond_wait(&MsgCond, &Mutex)) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}

	printf("Output: %s\n", shared_data);

	if (pthread_mutex_unlock(&Mutex)) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
}

int main() {
	pthread_t tid1, tid2;

	if(pthread_mutex_init(&Mutex, NULL)) {
		perror("pthread_mutex_init");
		exit(1);
	}

	if(pthread_cond_init(&MsgCond, NULL)) {
		perror("pthread_cond_init");
		exit(1);
	}
	
	while(1) {
		if(pthread_create(&tid1, NULL, (void *)input_thread, (void *)NULL) < 0) {
			perror("pthread_create");
			exit(1);
		}
		if(pthread_create(&tid2, NULL, (void *)output_thread, (void *)NULL) < 0) {
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

		data_ready = 0;
	}

	if(pthread_cond_destroy(&MsgCond)) {
		perror("pthread_cond_destroy");
	}
	if(!pthread_mutex_destroy(&Mutex)) {
		perror("pthread_mutex_destroy");
	}

	return 0;
}
