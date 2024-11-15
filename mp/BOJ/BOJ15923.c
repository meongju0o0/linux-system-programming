#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Pos {
    int x, y;
};

int main() {
    int n;
    scanf("%d", &n);

    struct Pos pos[n];

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &(pos[i].x), &(pos[i].y));
    }

    int total_len = 0;

    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        total_len += abs(pos[next].x - pos[i].x) + abs(pos[next].y - pos[i].y);
    }

    printf("%d\n", total_len);

    return 0;
}
