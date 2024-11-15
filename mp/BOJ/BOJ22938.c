#include <stdio.h>

int main() {
	long long x1, y1, r1;
	long long x2, y2, r2;

	scanf("%lld %lld %lld", &x1, &y1, &r1);
	scanf("%lld %lld %lld", &x2, &y2, &r2);

	long long dist = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	long long sum_r = (r1 + r2) * (r1 + r2);

	char* answer = (dist < sum_r) ? "YES" : "NO";
	printf("%s\n", answer);

	return 0;
}
