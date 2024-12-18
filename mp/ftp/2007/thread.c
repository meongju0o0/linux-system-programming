#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void sum() {
	int n = pthread_self();
	long *ret = malloc(sizeof(long));

	*ret = 0;
	for (int i = 0; i < n; i++) {
		*ret += (long)i;
	}
	
	pthread_exit((void *)ret);
}

void main() {
	pthread_t tid1, tid2, tid3;
	long *ret1, *ret2, *ret3;

	pthread_create(&tid1, NULL, (void *)sum, NULL);	
	pthread_create(&tid2, NULL, (void *)sum, NULL);
	pthread_create(&tid3, NULL, (void *)sum, NULL);

	pthread_join(tid1, (void **)&ret1);
	pthread_join(tid2, (void **)&ret2);
	pthread_join(tid3, (void **)&ret3);

	printf("%ld\n", *ret1 + *ret2 + *ret3);
}
