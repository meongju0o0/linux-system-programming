#include <stdio.h>

/*
[Program Name]: output
[Description]: 여러 자료형의 값들을 출력
[Input]: None
[Output]: 콘솔에 출력된 여러 자료형의 값
[Calls]:
	- printf(char* foramt, ...)
	- putchar(char ch)
	- puts(char* str)
*/

int main() {
	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;

	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
	// 자료형에 적합한 foramt으로 콘솔에 출력
	putchar(c); // 문자 하나 출력
	puts(s); // 문자열 출력

	return 0;
}
