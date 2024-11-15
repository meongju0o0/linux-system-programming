#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_FILES 100
#define MAX_PW_LEN 20
#define MAX_GR_LEN 20
#define MAX_DIR_LEN 256
#define MAX_TIME_LEN 20

/*
[Program Name]: mylsl
[Description]: ls -al과 유사하게 작동
[Input]: None
[Output]: 현재 디렉토리내 파일들과 그 정보를 콘솔에 출력
[Calls]:
	perror(char* err_msg)
	printf(char* format, ...)
	putchar(char ch)
	opendir(char* pathname)
	readdir(DIR* dp)
	closedir(DIR* dp)
	lstat(char* pathname, struct stat* buf)
	S_ISDIR()
	S_ISLNK()
	S_ISREG()
	strcmp(char* str1, char* str2)
	strcpy(char* dst, char* src)
	strncpy(char* dst, char* src, size_t count)
*/

typedef struct { // 파일 메타데이터를 저장할 구조체
	int _st_mode;
	long _n_link;
	char _pw_name[MAX_PW_LEN];
	char _gr_name[MAX_GR_LEN];
	long _st_size;
	char _st_mtime[MAX_TIME_LEN];
	char _d_name[MAX_DIR_LEN];
} file_meta;

char _strcmp(char* str1, char* str2) {
	/*
	[Function Name]: char _strcmp(char* str1, char* str2)
	[Description]:
		- str1과 str2를 비교하여 서로 다른 경우, 현재 위치 문자의 차를 반환
		- 이때, 대소문자를 구분하지 않고 모두 소문자인 상태로 비교
	[Input]: 비교할 두 문자열
	[Output]: 두 문자열 크기의 차
	[Calls]:
		tolower(int ch)
	*/
	int idx = 0;
	while(*(str1 + idx) != '\0' && *(str2 + idx) != '\0') {
		if(tolower(*(str1 + idx)) != tolower(*(str2 + idx))) {
			return tolower(*(str1 + idx)) - tolower(*(str2 + idx));
		}
		idx++;
	}
}

void sort_file_meta(file_meta* file_meta_arr, const int ar_size) {
	/*
	[Function Name]: void sort_file_meta(file_meta* file_meta_arr, const int ar_size)
	[Description]:
		- 불러온 파일 메타데이터를 파일명 순으로 사전식 정렬
		- Selection Sort
	[Input]: 파일메타데이터가 저장된 어레이, 어레이의 크기
	[Output]: 정렬된 어레이
	[Calls]:
		strcmp(char* str1, char* str2)
	*/

	// Selection Sort 수행
	for(int i = 0; i < ar_size - 1; i++) {
		for(int j = i + 1; j < ar_size; j++) {
			if(_strcmp(file_meta_arr[i]._d_name, file_meta_arr[j]._d_name) > 0) {
				file_meta temp = file_meta_arr[i];
				file_meta_arr[i] = file_meta_arr[j];
				file_meta_arr[j] = temp;
			}
		}
	}
}

void print_mode(const int mode) {
	/*
	[Function Name]: void print_mode(const int mode)
	[Description]:
		- execution, write, read 권한을 확인
		- 해당 권한을 가지고 있으면 해당 알파벳 출력
		- 해당 권한이 없으면 '-' 출력
	[Input]: 현재 모드가 저장된 정수
	[Output]: 현재 모드를 콘솔에 출력
	[Calls]:
		putchar(char* str)
	*/
	const int exec_mode = mode & 0b001; // execute permission을 가지고 있는지 확인 (bit masking)
	const int write_mode = mode & 0b010; // write permission을 가지고 있는지 확인 (bit masking)
	const int read_mode = mode & 0b100; // read permission을 가지고 있는지 확인 (bit masking)
	putchar(read_mode ? 'r' : '-'); // 읽기 권한을 가지고 있으면 'r', 아니면 '-'출력 
	putchar(write_mode ? 'w' : '-'); // 쓰기 권한을 가지고 있으면 'w', 아니면 '-' 출력
	putchar(exec_mode ? 'x' : '-'); // 실행 권한을 가지고 있으면 'x', 아니면 '-' 출력
}

void print_permission(const int st_mode) {
	/*
	[Function Name]: void print_permission(const int st_mode)
	[Description]:
		- user, owner, group 단위로 권한을 분해
		- 분해된 권한을 print_mode(const int mode) 함수에 넘겨 출력
	[Input]: 전체 모드가 저장된 정수
	[Output]: 전체 모드를 콘솔에 출력
	[Calls]:
		print_mode(const int mode)
		putchar(char* str)
	*/
	print_mode((st_mode & 0700) >> 6); // user의 permission 출력 (user bit masking 후 bit shift)
	print_mode((st_mode & 0070) >> 3); // group의 permission 출력 (group bit masking 후 bit shift)
	print_mode(st_mode & 0007); // others의 permission 출력 (others bit masking 후 bit shift)
	putchar(' ');
}

void print_file_type(const int st_mode) {
	/*
	[Function Name]: void print_file_type(const int st_mode)
	[Description]:
		- dir, lnk, reg 인지 확인 후 해당 사항을 콘솔에 출력
			- dir: 'd'
			- lnk: 'l'
			- reg: '-'
	[Input]: 모드가 저장된 정수
	[Output]: 파일 형식을 콘솔에 출력
	[Calls]:
		S_ISDIR(int st_mode)
		S_ISLNK(int st_mode)
		S_ISREG(int st_mode)
	*/
	if(S_ISDIR(st_mode)) { // 디렉토리이면
		putchar('d'); // 'd' 출력
	}
	else if(S_ISLNK(st_mode)) { // 심볼릭 링크이면
		putchar('l'); // 'l' 출력
	}
	else if(S_ISREG(st_mode)) { // 정규 파일이면
		putchar('-'); // '-' 출력
	}
	else { // 모두 해당되지 않으면
		putchar('?'); // '?' 출력
	}
}

int main() {
	DIR *dp;
	struct dirent *dept;
	struct stat statbuf;
	int num_files = 0;
	
	file_meta files[MAX_FILES]; // 파일 메타데이터 저장 배열 선언

	if(!(dp = opendir("."))) { // 현재 디렉토리를 열지 못하면
		perror("opendir"); // 에러 출력
		exit(1); // 비정상 종료
	}

	while(dept = readdir(dp)) { // 현재 디렉토리 내 파일 정보를 읽을 수 있는 동안
		// 현재 파일의 정보를 읽음
		if(lstat(dept->d_name, &statbuf) == -1) { // 현재 파일의 기타 메타데이터를 읽을 수 없으면
			// 현재 파일의 기타 메타데이터를 읽음
			perror("lstat"); // 에러 출력
		}
		else {
			files[num_files]._st_mode = statbuf.st_mode; // 구조체 배열에 현재 권한 저장
			files[num_files]._n_link = statbuf.st_nlink; // 구조체 배열에 현재 링크 수 저장
			strcpy(files[num_files]._pw_name, getpwuid(statbuf.st_uid)->pw_name); // 구조체 배열에 user_name 저장
			strcpy(files[num_files]._gr_name, getgrgid(statbuf.st_gid)->gr_name); // 구조체 배열에 group_name 저장
			files[num_files]._st_size = statbuf.st_size; // 구조체 배열에 파일 크기 저장
		}

		struct tm *timeinfo = localtime(&statbuf.st_mtime); // 마지막 수정 시간을 계산하기 위한 전처리
		strftime(files[num_files]._st_mtime, sizeof(files[num_files]._st_mtime), "%b %d %H:%M", timeinfo); // 구조체 배열에 마지막 수정 시간 저장

		strncpy(files[num_files]._d_name, dept->d_name, MAX_DIR_LEN - 1); // 구조체 배열에 파일명 저장
		files[num_files]._d_name[MAX_DIR_LEN - 1] = '\0';

		num_files++;
	}

	sort_file_meta(files, num_files); // 파일명 순으로 사전식 정렬
	for(int i = 0; i < num_files; i++) { // 정렬된 어레이의 정보를 출력
		print_file_type(files[i]._st_mode);
		print_permission(files[i]._st_mode);
		printf("%ld ", files[i]._n_link);
		printf("%s ", files[i]._pw_name);
		printf("%s ", files[i]._gr_name);
		printf("%5ld ", files[i]._st_size);
		printf("%s ", files[i]._st_mtime);
		printf("%s\n", files[i]._d_name);
	}

	closedir(dp); // directory descripter pointer 종료

	return 0;
}
