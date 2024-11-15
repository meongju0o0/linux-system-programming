#include <stdio.h>

int main() {
	int n;
	scanf("%d", &n);

	for(int i = 1; i <= n; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);
		
		printf("Scenario #%d:\n", i);
		if (a >= b && a >= c) {
			if(a * a == b * b + c * c) {
				printf("yes\n\n");
			}
			else {
				printf("no\n\n");
			}
		}
		else if(b >= a && b >= c) {
			if(b * b == a * a + c * c) {
				printf("yes\n\n");
			}
			else {
				printf("no\n\n");
			}
		}
		else if(c >= a && c >= b) {
			if(c * c == a * a + b * b) {
				printf("yes\n\n");
			}
			else {
				printf("no\n\n");
			}
		}
	}

	return 0;
}
