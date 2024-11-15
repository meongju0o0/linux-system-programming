#include <stdio.h>
#include <stdbool.h>

bool is_valid(const int a, const int b, const int c) {
	if(a >= b && a >= c) {
		return (a < b + c);
	}
	else if(b >= a && b >= c) {
		return (b < a + c);
	}
	else if(c >= a && c >= b) {
		return (c < a + b);
	}
	else {
		return false;
	}
}

bool is_equilateral(const int a, const int b, const int c) {
	return (a == b && b == c && c == a);
}

bool is_isosceles(const int a, const int b, const int c) {
	return (a == b || b == c || c == a);
}

int main() {
	const int t;
	scanf("%d", &t);

	for(int i = 1; i <= t; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		printf("Case #%d: ", i);

		if(is_valid(a, b, c)) {
			if(is_equilateral(a, b, c)) {
				printf("equilateral\n");
			}
			else if(is_isosceles(a, b, c)) {
				printf("isosceles\n");
			}
			else {
				printf("scalene\n");
			}
		}
		else {
			printf("invalid!\n");
		}
	}

	return 0;
}
