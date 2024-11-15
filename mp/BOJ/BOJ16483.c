#include <stdio.h>
#include <math.h>

int main() {
	double t;
	scanf("%lf", &t);

	printf("%ld\n", lround(t * t / 4));

	return 0;
}
