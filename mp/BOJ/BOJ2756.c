#include <stdio.h>

int calc_score(long double dist) {
	if(dist > 225) {
		return 0;
	}
	else if(dist > 144) {
		return 20;
	}
	else if(dist > 81) {
		return 40;
	}
	else if(dist > 36) {
		return 60;
	}
	else if(dist > 9) {
		return 80;
	}
	else {
		return 100;
	}
}

int main() {
	int t;
	scanf("%d", &t);
	
	while(t--) {
		int n = 0, m = 0;
		double dist;
		for(int i = 0; i < 3; i++) {
			double x, y;
			scanf("%lf %lf", &x, &y);
			dist = x * x + y * y;
			n += calc_score(dist);
		}
		for(int i = 0; i < 3; i++) {
			double x, y;
			scanf("%lf %lf", &x, &y);
			dist = x * x + y * y;
			m += calc_score(dist);
		}
		if(n == m) {
			printf("SCORE: %d to %d, TIE.\n", n, m);
		}
		else if(n > m) {
			printf("SCORE: %d to %d, PLAYER 1 WINS.\n", n, m);
		}
		else if(n < m) {
			printf("SCORE: %d to %d, PLAYER 2 WINS.\n", n, m);
		}
	}

	return 0;
}
