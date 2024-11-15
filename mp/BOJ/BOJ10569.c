#include <stdio.h>

int main() {
	int t;
	scanf("%d", &t);

	int v, e;
	while(t--) {
		scanf("%d %d", &v, &e);
		printf("%d\n", 2 + e - v);
	}

	return 0;
}
