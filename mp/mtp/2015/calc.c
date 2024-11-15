#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
	FILE* fp = fopen(argv[1], "rt");
	char buf[256];
	int items[256];
	int num_items = 0;

	while(fgets(buf, 256, fp)) {
		char* token = strtok(buf, " ");
		while(token != NULL) {
			items[num_items++] = atoi(token);
			token = strtok(NULL, " ");
		}
	}

	int sum = 0, min = items[0], max = items[0];
	for(int i = 0; i < num_items; i++) {
		sum += items[i];
		if(items[i] < min) {
			min = items[i];
		}
		if(items[i] > max) {
			max = items[i];
		}
	}

	printf("%d\n", sum / num_items);
	printf("%d\n", min);
	printf("%d\n", max);

	fclose(fp);

	return 0;
}
