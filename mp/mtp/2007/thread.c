#include <stdio.h>
#include <pthread.h>

void *dummy_func(void *arg) {
	pthread_t *tid = (pthread_t *)arg;
	*tid = pthread_self();
	return NULL;
}

int main() {
	pthread_t self_tid1, self_tid2, self_tid3;
	pthread_t tid1, tid2, tid3;

	pthread_create(&tid1, NULL, dummy_func, &self_tid1);
	pthread_create(&tid2, NULL, dummy_func, &self_tid2);
	pthread_create(&tid3, NULL, dummy_func, &self_tid3);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

	printf("%ld\n", self_tid1 + self_tid2 + self_tid3);

	return 0;
	
}
