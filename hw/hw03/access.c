#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "record.h"

/*
[Program Name]: access
[Description]: test.out에 기록된 레코드를 읽어서 접근. 원하는 인덱스의 레코드에 무작위 접근도 가능하며 해당 위치에 기록된 정보를 수정할 수도 있다.
[Input]: test.out
[Output]: 해당 위치의 레코드에 기록된 정보를 콘솔에 출력
[Calls]:
	fprintf(FILE* fp, char* str),
	exit(int EXIT_CODE),
	perror(char* err_msg),
	fread(char* buffer, size_t buffer_size, size_t count, FILE* fp),
	fseek(FILE* fp, long offset, int origin),
		// origin types: {SEEK_SET, SEEK_CUR, SEEK_END}
	strcpy(char* dst, const char* src),
	rewind(FILE* fp),
	puts(char* str)
*/

void print_record(Record *rp) {
	/*
	[Function Name]: void print_record(Record *rp)
	[Description]: Record 구조체 포인터를 입력 파라미터로 받아 해당 레코드에 기록된 정보를 콘솔에 출력
	[Input]: Record 구조체
	[Output]: None
	[Call By]: main()
	[Calls]: puts(char* str)
	[Given]: Record *rp
	[Returns]: None
	*/
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

int main(int argc, char *argv[]) {
	FILE *fp; // file descripoter 포인터 선언
	Record rec; // 레코드 구조체 선언

	if(argc != 2) { // 레코드들이 기록된 파일 명이 주어지지 않은 경우
		fprintf(stderr, "Usage: %s file\n", argv[0]); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	if(!(fp = fopen(argv[1], "r+b"))) { // 주어진 파일명으로 file descripter를 생성하지 못하면
		perror("fopen"); // 에러 출력
		exit(1); // 강제 종료, EXIT_CODE: 1
	}

	printf("-----Ordered Record List-----\n"); 

	while(fread(&rec, sizeof(rec), 1, fp)) { // 반환된 Number of objects가 0이 아닌 동안
		print_record(&rec); // 파일로부터 읽은 레코드 출력
	}

	rewind(fp); // file descripter의 position을 파일의 첫 부분으로 옮김
	getchar(); // 사용자로부터 아무 입력이나 받을 때까지 대기

	printf("-----Shuffled Record List (3, 6, 2, 4, 1 ,5)-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // file descripter의 position을 3번째 레코드로 옮김
	fread(&rec, sizeof(rec), 1, fp); // 레코드 구조체의 크기 만큼 파일을 읽음
	print_record(&rec); // 읽은 레코드를 출력

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	getchar(); // 사용자로부터 아무 입력이나 받을 때까지 대기

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // file descripter의 position을 2번째 레코드로 옮김
	fread(&rec, sizeof(rec), 1, fp); // 레코드 구조체의 크기 만큼 파일을 읽음
	strcpy(rec.dept, "Movie"); // 레코드 구조체의 dept를 "Movie"로 수정
	fseek(fp, sizeof(rec)*2L, SEEK_SET); // file descripter의 position을 2번째 레코드로 옮김
	fwrite(&rec, sizeof(rec), 1, fp); // 레코드 파일의 정보를 수정된 레코드 구조체로 교체

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	rewind(fp); // file descripter의 position을 파일의 첫 부분으로 옮김

	while(fread(&rec, sizeof(rec), 1, fp)) {
		print_record(&rec); // 레코드 파일에 저장된 순서대로 레코드 출력
	}

	fclose(fp); // file descripter의 position을 파일의 첫 부분으로 옮김

	return 0;
}
