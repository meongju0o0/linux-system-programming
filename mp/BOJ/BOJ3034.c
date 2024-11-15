#include <stdio.h>
#include <math.h>

int main() {
	int n, w, h;
	scanf("%d %d %d", &n, &w, &h);

	double box_max_len = sqrt(w * w + h * h);
	
	int match_len;
	for(int i = 0; i < n; i++) {
		scanf("%d", &match_len);
		if(match_len > box_max_len) {
			printf("NE\n");
		}
		else {
			printf("DA\n");
		}
	}

	return 0;
}
