#include <stdio.h>
#include <limits.h>

struct Pos {
	int x, y;
};

int get_min_x(const struct Pos* pos_ar, const int n) {
	int min_x = INT_MAX;
	
	for(int i = 0; i < n; i++) {
		if(pos_ar[i].x < min_x) {
			min_x = pos_ar[i].x;
		}
	}

	return min_x;
}

int get_max_x(const struct Pos* pos_ar, const int n) {
	int max_x = INT_MIN;

	for(int i = 0; i < n; i++) {
		if(pos_ar[i].x > max_x) {
			max_x = pos_ar[i].x;
		}
	}

	return max_x;
}

int get_min_y(const struct Pos* pos_ar, const int n) {
	int min_y = INT_MAX;

	for(int i = 0; i < n; i++) {
		if(pos_ar[i].y < min_y) {
			min_y = pos_ar[i].y;
		}
	}

	return min_y;
}

int get_max_y(const struct Pos* pos_ar, const int n) {
	int max_y = INT_MIN;

	for(int i = 0; i < n; i++) {
		if(pos_ar[i].y > max_y) {
			max_y = pos_ar[i].y;
		}
	}

	return max_y;
}

int main() {
	const int n;
	scanf("%d", &n);

	struct Pos pos_ar[n];
	
	for(int i = 0; i < n; i++) {
		scanf("%d %d", &(pos_ar[i].x), &(pos_ar[i].y));
	}

	printf("%d\n", (get_max_x(pos_ar, n) - get_min_x(pos_ar, n)) * (get_max_y(pos_ar, n) - get_min_y(pos_ar, n)));

	return 0;
}
