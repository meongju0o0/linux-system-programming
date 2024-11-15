#include <stdio.h>

/*
[Program Name]: input
[Description]: 다양한 자료형의 값을 콘솔로부터 입력받아 콘솔에 출력
[Input]:
	- char c, s[80]
	- int i, long l
	- float f, double d
[Output]:
	- 입력받은 값을 콘솔에 다시 출력
[Calls]:
	- scanf(char* format, ...)
	- printf(char* format, ...)
*/

int main() {
	char c ,s[80];
	int i; long l;
	float f; double d;
	// 자료형 선언

	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); // 콘솔로부터 입력받음
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d); // 콘솔에 출력

	c = getchar(); // 문자 하나 입력받음
	putchar(c); // 입력받은 문자 출력

	fgets(s, sizeof(s), stdin); // 문자열 입력받음
	puts(s); // 입력받은 문자열 출력

	return 0;
}
