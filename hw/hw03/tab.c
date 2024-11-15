#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
[Program Name]: tab
[Description]: 파일의 tab을 4칸씩 띄우게끔 변환
[Input]: 4칸씩 띄우게끔 변환할 파일명들
[Output]: 4칸씪 띄워진 변환된 파일
[Calls]:
	- fopen(char* file_name, char* mode)
	- strcpy(char* dst_str, char* src_str)
	- strcat(char* added_str, char* adding_str)
	- perror(char* err_msg)
	- fputc(char ch, FILE* fp)
*/

void conv(char *fname) {
	/*
	[Function Name]: conv
	[Description]: fname의 파일의 tab을 띄어쓰기 4칸으로 변경
	[Input]: 변경할 파일명
	[Output]: 변경된 파일
	[Calls]:
		- fopen(char* file_name, char* mode)
		- strcpy(char* dst_str, char* src_str)
		- strcat(char* added_str, char* adding_str)
		- perror(char* err_msg)
		- fputc(char ch, FILE* fp)
	*/
	FILE *src, *dst; // src, dst file descripter pointer 선언
	char fdest[40]; // 변경된 파일 이름을 저장하는 변수
	int ch, first; // 문자를 저장할 변수와 현재 줄의 첫번째 위치인지 확인할 변수 선언

	if(!(src = fopen(fname, "rt"))) { // fname을 "rt" 모드로 열지 못한 경우 
		perror("fopen, src"); // 에러 출력
		return; // 함수 종료
	}

	strcpy(fdest, fname); // fdest에 fname을 복사
	strcat(fdest, ".t"); // fdest에 ".t" 문자열 붙임
	if(!(dst = fopen(fdest, "wt"))) { // dst를 "wt" 모드로 열지 못한 경우 
		perror("fopen, dst"); // 에러 출력
		return; // 함수 종료
	}

	first = true; // first 변수 true로 초기화(텍스트 파일의 첫 부분부터 읽기 때문)
	while((ch = fgetc(src)) != EOF) { // EOF(End of File)이 아닌 동안
		if(first && ch == '\t') { // first 이고 '\t'를 만나는 경우
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		} // 4칸 씩 띄워서 출력
		else { // 그렇지 않은 경우
			fputc(ch, dst); // 입력받은 문자열 그대로 출력
			if(first) { // first이면
				first = false; // first를 false로 변환
			}
			if(ch == '\n') { // '\n'을 만난 경우
				first = true; // first를 true로 변환
			}
		}
	}

	fclose(src);
	fclose(dst);
}

int main(int argc, char *argv[]) {
	while(--argc) {
		conv(argv[argc]); // argc의 수만큼 conv함수 반복해서 호출
	}

	return 0;
}
