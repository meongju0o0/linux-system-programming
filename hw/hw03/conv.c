#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

/*
[Program Name]: conv
[Description]: test.in에 기록된 레코드를 읽어 file.out으로 변환. file.out은 access가 읽을 수 있는 형태이다.
[Input]: char* src, char* dst
[Output]: access가 읽을 수 있는 형태로 레코드를 수정
[Calls]:
	fprintf(FILE* fp, char* str),
	exit(EXIT_CODE),
	perror(char* error_msg),
	fgets(char* str, int count, FILE* fp),
	strchr(const char* str, int ch),
	fwrite(const void* buffer, size_t size, size_t count, FILE* fp),
	fclose(FILE* fp)
*/

int main(int argc, char *argv[]) {
	FILE *src, *dst; // file descripter pointer 선언
	Record rec; // 레코드 구조체 자료형 선언

	if (argc != 3) { // test.in과 test.out가 주어지지 않은 경우
		fprintf(stderr, "Usage: %s source dest\n", argv[0]); // 에로 출력 
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(src = fopen(argv[1], "rt"))) { // test.in이 "rt" 모드로 열리지 않는 경우
		perror("fopen, src"); // 에러 출력
		exit(1); // 강제종료, EXIT_CODE: 1
	}

	if(!(dst = fopen(argv[2], "wb"))) { // test.out이 "wb" 모드로 열리지 않는 경우
		perror("fopen, dst"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}
	
	while(fgets(rec.stud, SMAX, src)) { // 파일로부터 문자열을 한줄 씩 읽음, rec.stud
		*strchr(rec.stud, '\n') = '\0';
		// rec.stud에서 '\n'이 위치한 곳의 포인터를 반환
		// 해당 포인터가 위치한 곳의 값을 '\n'(LF)에서 '\0'(NUL)로 변환
		fgets(rec.num, NMAX, src); // 파일로부터 문자열을 한줄 씩 읽음, rec.num
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src); // 파일로붜 문자열을 한줄 씩 읽음, rec.dept
		*strchr(rec.dept, '\n') = '\0';
		
		fwrite(rec.stud, sizeof(rec.stud), 1, dst); //binary 출력 형식으로 rec.stud를 rec.stud의 크기만큼 file.out에 입력, 남은 빈 공간은 padding byte로 채워짐
		fwrite(rec.num, sizeof(rec.num), 1, dst);
		fwrite(rec.dept, sizeof(rec.dept), 1, dst);
	}
	
	fclose(src); // src file descripter 제거
	fclose(dst); // dst file descripter 제거
	
	return 0;
}
