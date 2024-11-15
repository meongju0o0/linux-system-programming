#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

/*
[Program name]: myls
[Description]: 현재 폴더의 파일 및 디렉토리 목록 출력
[Input]: None
[Output]: 현재 폴더의 파일 및 디렉토리 목록을 콘솔에 출력
[Calls]:
	opendir(char* pathname)
	closedir(DIR* dp)
	printf(char* str, ...)
	perror(char* err_msg)
	exit(int EXIT_CODE)
*/

int main() {
	DIR *dp; // directory descripter pointer 선언
	struct dirent *dept; // 디렉토리 및 파일 정보를 저장할 구조체 선언 

	if(!(dp = opendir("."))) { // 현재 디렉토리를 열지 못하면
		perror("opendir"); // 에러 출력
		exit(1); // 비정상 종료
	}

	while(dept = readdir(dp)) { // 현재 디렉토리(".")부터 디렉토리 내 파일 순회
		printf("%s\n", dept->d_name); // 현재 directory descripter pointer가 가르키는 파일의 이름 출력
	}

	closedir(dp); // directory pointer 종료

	return 0;
}
