#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

void myusleep(int secs, int usecs) {
	struct timeval tm;

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0) {
		perror("select");
		return;
	}
}

int main() {
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}
