#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int i = 0;
int count = 0;
int stop_flag = 0;
int rand_num[3] = {0, 0, 0};

void sig_int_handler() {
	rand_num[i] = count;
	printf("\n%d\n", count);
	stop_flag = 1;
}

int main() {
	signal(SIGINT, sig_int_handler);

	for(i = 0; i < 3; i++) {
		while(!stop_flag) {
			for(count = 0; count < 100; count++);
		}
		stop_flag = 0;
	}
	
	printf("Random number: %d, %d, %d\n", rand_num[0], rand_num[1], rand_num[2]);

	return 0;
}
