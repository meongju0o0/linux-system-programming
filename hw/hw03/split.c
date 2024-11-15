#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUF 1024

/*
[Program Name]: split
[Description]: src로부터 읽은 파일을 dst1과 dst2에 절반씩 나누어 저장
[Input]: char* src.txt, dst1.txt, dst2.txt
[Output]: dst1과 dst2에 나누어진 src의 텍스트
[Calls]:
	- fprintf(FILE* fp, char* str, ...)
	- perror(char* str)
	- exit(int EXIT_CODE)
	- fread(void* buffer, size_t size, size_t count, FILE* fp)
	- fwrite(const void* buffer, size_t size, size_t count, FILE* fp)
	- fseek(FILE* fp, long offset, int origin)
		- origin: {SEEK_SET, SEEK_CUR, SEEK_END}
	- ftell(FILE* fp)
	- fclose(FILE* fp)
*/

long filesize(FILE *fp) {
	/*
	[Function Name]: long filesize(FILE* fp)
	[Description]:
		- 주어진 file descripter pointer가 가르키는 파일의 크기를 반환
	[Input]: FILE *fp
	[Output]: 현재 cursor로붜 남은 byte(혹은 text) 수
	[Call By]:
		- main()
	[Calls]:
		- ftell(FILE* fp)
		- fseek(FILE* fp, long offset, int origin)
	[Given]:
		- file descripter
	[Returns]:
		- file descripter가 지정하고 있는 파일의 크기를 반환
	*/
	long cur, size; // 현재 위치와 파일 끝 위치를 저장할 변수 선언

	cur = ftell(fp); // ftell(FILE* fp)를 통해 cur에는 현재 커서의 위치를 저장 
	fseek(fp, 0L, SEEK_END); // fseek(FILE* fp, long offset, int origin)을 통해 커서를 맨 마지막으로 옮김
	size = ftell(fp); // 맨 마지막 위치를 size에 저장
	fseek(fp, cur, SEEK_SET); // fseek(FILE* fp, long offset, int origin)을 통해 커서를 초기화
	return size;
}

int main(int argc, char *argv[]) {
	FILE *src, *dst1, *dst2; // file descripter pointer 선언
	char buf[MAX_BUF]; // 읽은 내용을 저장할 버퍼 선언
	int cnt, size; // cnt, size 변수 선언 

	if(argc != 4) { // argument의 개수가 4개가 아닌 경우
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src = fopen(argv[1], "rb"))) { // argv[1]을 열 수 없는 경우
		perror("fopen, src"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(dst1 = fopen(argv[2], "wb"))) {
		perror("fopen, dst1");
		exit(1);
	}

	if(!(dst2 = fopen(argv[3], "wb"))) {
		perror("fopen, dst2");
		exit(1);
	}

	size = filesize(src) / 2; // dst1의 크기를 계산

	while(size > 0) { // size가 0보다 큰 동안
		cnt = (size > MAX_BUF) ? MAX_BUF : size; // size가 더 큰 경우 size만큼만 출력, 아닌 경우 MAX_BUF만큼 출력
		fread(buf, sizeof(char), cnt, src); // src로부터 cnt만큼 읽음
		fwrite(buf, sizeof(char), cnt, dst1); // src로부터 읽은 데이터를 dst1에 저장
		size -= cnt; // 수행된 파일의 길이(cnt)만큼 size에서 차감
	}

	while((cnt = fread(buf, sizeof(char), MAX_BUF, src)) > 0) { // 현재 src의 남온 데이터 만큼 반복
		fwrite(buf, sizeof(char), cnt, dst2); // dst2에 저장
	}

	fclose(src); // src file descripter 종료
	fclose(dst1); // dst1 file descripter 종료
	fclose(dst2); // dst2 file descripter 종료

	return 0;
}
