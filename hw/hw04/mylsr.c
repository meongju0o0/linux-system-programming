#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PATH 256

/*
[Program Name]: mylsr
[Description]: 현재 디렉토리의 파일 및 디렉토리, 하위 디렉토리의 파일 및 디렉토리를 재귀적으로 출력
[Input]: None
[Output]: 현재 디렉토리의 파일 및 디렉토리, 하위 디렉토리의 파일 및 디렉토리가 재귀적으로 콘솔에 출력됨
[Calls]:
	opendir(char* pathname)
	readdir(DIR* dp)
	closedir(DIR* dp)
	rewinddir(DIR* dp)
	lstat(char* pathname, struct stat* buf)
	S_ISDIR()
	strcmp(char* str1, char* str2)
	strcat(char* src, char* dst)
	perror(char* str)
*/

void recursive_ls(char* path) {
	/*
	[Function Name]: void recursive_ls(char* path)
	[Description]:
		- 현재 디렉토리 내의 파일이나 디렉토리의 목록을 출력
		- 하위 디렉토리가 존재하는 경우 하위 디렉토리 내 파일이나 디렉토리의 목록을 출력
		- 위 과정을 재귀적으로 반복
			- 탈출 조건: 하위 디렉토리가 존재하지 않는 경우
	[Input]: 경로명
	[Output]: 현재 디렉토리, 하위 디렉토리 내 파일을 콘솔에 출력
	[Calls]:
		recursive_ls(char* path)
	*/
	DIR *dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];

	if(!(dp = opendir(path))) { // 디렉토리를 열지 못하면
		perror("opendir"); // 에러 출력
		exit(1); // 비정상 종료
	}

	printf("\n%s:\n", path); // 현재 경로 출력
	while(dep = readdir(dp)) { // 현재 경로 내 파일을 입력받음
		if(strcmp(".", dep->d_name) != 0 && strcmp("..", dep->d_name) != 0) { // 입력받은 파일명이 "."과 ".."이 아니면
			printf("%s\n", dep->d_name); // 파일명 출력
		}
	}

	rewinddir(dp); // directory descripter 초기화

	while(dep = readdir(dp)) { // 하위 디렉토리가 존재하는지 검사하기 위한 반복문
		if(strcmp(".", dep->d_name) != 0 && strcmp("..", dep->d_name) != 0) {
			strcpy(fullpath, path); // 하위 경로 생성을 위한 문자열 연산
			strcat(fullpath, "/"); // 하위 경로 생성을 위한 문자열 연산
			strcat(fullpath, dep->d_name); // 하위 경로 생성을 위한 문자열 연산
			if(lstat(fullpath, &statbuf) == -1) { // 현재 파일의 메타데이터를 얻지 못하면
				perror("lstat"); // 에러 출력
				continue; // 반복문의 시작으로 이동
			}
			if(S_ISDIR(statbuf.st_mode)) { // 현재 파일이 디렉토리이면
				recursive_ls(fullpath); // recursive_ls 재귀호출
			}
		}
	}

	closedir(dp); // directory descripter pointer 삭제
}

int main() {
	recursive_ls("."); // recursive_ls 함수 호출
	return 0;
}
