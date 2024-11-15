#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main() {
	double a, b, c;
	int cnt = 0;

	while(true) {
		cnt++;

		scanf("%lf %lf %lf", &a, &b, &c);
		
		if(a == 0 && b == 0 && c == 0) {
			break;
		}
		else {
			if(a == -1) {
				printf("Triangle #%d\n", cnt);
				a = sqrt(c * c - b * b);
				if(isnan(a) || a == 0) {
					printf("Impossible.\n\n");
				}
				else {
					printf("a = %.3lf\n\n", a);
				}
			}
			else if(b == -1) {
				printf("Triangle #%d\n", cnt);
				b = sqrt(c * c - a * a);
				if(isnan(b) || b == 0) {
					printf("Impossible.\n\n");
				}
				else {
					printf("b = %.3lf\n\n", b);
				}
			}
			else if(c == -1) {
				printf("Triangle #%d\n", cnt);
				c = sqrt(a * a + b * b);
				if(isnan(c) || c == 0) {
					printf("Impossible.\n\n");
				}
				else {
					printf("c = %.3lf\n\n", c);
				}
			}
		}
	}

	return 0;
}
