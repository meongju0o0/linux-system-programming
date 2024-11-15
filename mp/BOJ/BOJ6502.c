#include <stdio.h>
#include <stdbool.h>

int main() {
	int r, w, l;

	int t = 1;
	while(true) {
		scanf("%d", &r);
		if(!r) break;
		scanf("%d %d", &w, &l);
		
		if(w * w + l * l <= 4 * r * r) {
			printf("Pizza %d fits on the table.\n", t++);
		}
		else {
			printf("Pizza %d does not fit on the table.\n", t++);
		}
	}

	return 0;
}
