#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024

/*
[Program Name]: merge
[Description]: 두 파일을 입력받아 두 파일을 합친 형태의 파일을 출력
[Input]: char* txt1_src, txt2_src, txt_dst
[Output]: txt_dst
[Calls]:
	- fprintf(FILE* fp, const char* format)
	- perror(char* err_msg)
	- exit(int EXIT_CODE)
	- fopen(char* file_name, char* mode)
	- fread(char* buffer, size_t size, size_t count, FILE* fp)
	- fwrite(char* buffer, size_t size, size_t count, FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *src1, *src2, *dst; // file descripter pointer 선언
	char buf[MAX_BUF]; // 파일로부터 읽은 문자열을 저장할 버퍼 선언
	int cnt; // 읽은 문자열의 길이를 저장할 변수 선언

	if(argc != 4) { // arguement가 4개가 아닌 경우
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src1 = fopen(argv[1], "rb"))) { // "rb" 모드로 argv[1]이 열리지 않는 경우
		perror("fopen, src1"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src2 = fopen(argv[2], "rb"))) {
		perror("fopen, src2");
		exit(1);
	}

	if(!(dst = fopen(argv[3], "wb"))) {
		perror("fopen, dst");
		exit(1);
	}

	while(cnt = fread(buf, sizeof(char), MAX_BUF, src1)) { // src1 fp로부터 텍스트를 BUF의 크기 만큼 읽음. 읽은 object의 개수는 cnt에 저장
		fwrite(buf, sizeof(char), cnt, dst); // buf의 크기 씩 cnt만큼 dst fp가 가르키는 파일에 저장
	} // 반복문이 종료된 상황에서 dst fp는 파일의 맨 마지막을 가르키고 있음

	while(cnt = fread(buf, sizeof(char), MAX_BUF, src2)) { // src2 fp로부터 텍스트를 BUF의 크기만큼 읽음. 읽은 object의 개수는 cnt에 저장
		fwrite(buf, sizeof(char), cnt, dst); // buf의 크기 씩 cnt만큼 dst fp가 가르키는 파일에 저장
	}

	fclose(src1); // src1 fp 종료
	fclose(src2); // src2 fp 종료
	fclose(dst); // dst fp 종료

	return 0;
}
