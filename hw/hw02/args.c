#include <stdio.h>

/*
[Program Name]: args
[Description]: argument에 명시된 문자열을 한 줄 씩 출력
[Input]: char* argv[]
[Output]: arguemnt에 명시된 문자열들이 출력됨
[Calls]: printf(char* format, ...)
*/

int main(int argc, char *argv[]) {
	for(int i = 0; i < argc; i++) { // argc 크기 만큼 반복
		printf("argv[%d]=%s\n", i, argv[i]); // 현재 인덱스의 argument 출력 및 줄바꿈
	}

	return 0;
}
