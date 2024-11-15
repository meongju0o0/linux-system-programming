#include <stdio.h>
#include <stdbool.h>

bool is_valid(int a, int b, int c) {
	if(a >= b && a >= c) {
		return (a < b + c);
	}
	else if(b >= a && b >= c) {
		return (b < a + c);
	}
	else if(c >= b && c >= a) {
		return (c < a + b);
	}
}

int main() {
	int a, b, c;
	while(true) {
		scanf("%d %d %d", &a, &b, &c);
		if(a == 0 && b == 0 && c == 0) {
			break;
		}
		else if(is_valid(a, b, c)) {
			if(a == b && b == c && a == c) {
				printf("Equilateral\n");
			}
			else if(a == b || a == c || b == c) {
				printf("Isosceles\n");
			}
			else {
				printf("Scalene\n");
			}
		}
		else {
			printf("Invalid\n");
		}
	}

	return 0;
}
