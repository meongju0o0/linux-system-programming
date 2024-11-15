#include <stdio.h>
#include <math.h>

int main() {
	double k, d1, d2;
	scanf("%lf %lf %lf", &k, &d1, &d2);

	printf("%lf\n", (k * k - ((d1 - d2) * (d1 - d2)) / 4));

	return 0;
}
