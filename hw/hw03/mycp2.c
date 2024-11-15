#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024

/*
[Program Name]: mycp2
[Description]: src.txt의 내용을 dst.txt에 복사
[Input]: char* src.txt, char* dst.txt
[Output]: src.txt에서 복사된 dst.txt 파일
[Calls]:
	- fprintf(FILE* fp, char* str)
	- exit(int EXIT_CODE)
	- perror(char* str)
	- fread(void* buffer, size_t size, size_t count, FILE* fp)
	- fwrite(void* buffer, size_t size, size_t count, FILE* fp)
	- fclose(FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *src, *dst; // src, dst file descripter pointer 선언
	char buf[MAX_BUF]; // 파일의 내용을 저장할 버퍼 선언
	int cnt; // object의 개수를 저장할 cnt 변수 선언

	if(argc != 3) { // argument의 개수가 3개가 아닌 경우
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 에러 출력
		exit(1); // 프로그램 강제 종료, EXIT_CODE: 1
	}

	if(!(src = fopen(argv[1], "rb"))) { // argv[1]을 열지 못한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 프로그램 강제 종료, EXIT_CODE: 1
	}

	if(!(dst = fopen(argv[2], "wb"))) { // argv[2]를 열지 못한 경우
		perror("fopen"); // 에러 출력
		exit(1); // 프로그램 강제 종료, EXIT_CODE: 1
	}

	while(cnt = fread(buf, sizeof(char), MAX_BUF, src)) { // src로 부터 읽은 object가 있는 경우
		fwrite(buf, sizeof(char), cnt, dst); // 해당 object를 dst에 저장
	}

	fclose(src); // src file descripter 종료
	fclose(dst); // dst file descripter 종료

	return 0;
}
