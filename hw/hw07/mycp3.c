#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
[Program Name]: mycp3
[Description]: mmap을 활용하여 파일 복사 기능을 구현
	- argv[1] 파일을 argv[2]에 복사
		- ./mycp3 argv[1] argv[2]
	- argv[1] 파일을 RDONLY 모드로 엶(fdin)
	- argv[2] 파일을 RDWR, CREAT, TRUNC 모드로 엶, 접근 권한: 0644(fdout)
	- argv[2] 파일의 크기를 argv[1] 파일과 동일하게 설정
		- by using
			- lseek(int fd, off_t offset, int shence)
			- write(int fd, const void *buf, size_t count)
	- src를 argv[1]과 매핑하여 메모리와 파일 간 동기화
	- dst를 argv[2]와 매핑하여 메모리와 파일 간 동기화
	- dst에 src를 복사하여 argv[2]로 argv[1] 파일을 복사
[Input]: src 파일, dst 파일
[Output]: src를 복사한 dst 파일
[Calls]:
	- fprintf(FILE *stream, const char *format, ...)
	- perror(char *err_msg)
	- exit(int exit_code)
	- open(const char *pathname, int flags)
	- open(const char *pathname, int flags, mode_t mode)
	- fstat(int fd, struct stat *statbuf)
	- lseek(int fd, off_t offset, int whence)
	- write(int fd, const void *buf, size_t count)
	- mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
	- memcpy(void *dest, const void *src, size_t n)
	- close(int fd)
*/

int main(int argc, char *argv[]) {
	int fdin, fdout; // file descripter 번호를 저장할 변수 선언
	char *src, *dst; // mmap을 통해 매핑할 변수 선언
	// src와 argv[1] 매핑, dst와 argv[2] 매핑
	struct stat statbuf; // file의 stat을 저장할 구조체 변수 선언

	if(argc != 3) { // argument의 개수가 잘못된 경우 예외처리
		fprintf(stderr, "Usage: %s source destination\n", argv[0]); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if((fdin = open(argv[1], O_RDONLY)) < 0) { // argv[1]을 읽기 전용 모드로 엶
		perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) { // argv[1]을 주어진 모드에 따라 엶
	/*
	O_RDWR: 읽기, 쓰기
	O_CREAT: 파일이 없는 경우 새로 생성
	O_TRUNC: 파일이 있는 경우 기존 내용을 모두 삭제
	0644: -rw-r--r--
	*/
		perror("open"); // 파일 열기에 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	if(fstat(fdin, &statbuf) < 0) { // argv[1] 파일의 정보를 가져옴
		perror("fstat"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	if(lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0) { // argv[2] 파일의 오프셋을 argv[1] 파일 크기 -1로 이동
		perror("lseek"); // 실패한 경우 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	write(fdout, "", 1); // 바로 다음 오프셋에 빈 문자열 입력

	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1) { // src와 fdin 메모리 매핑
	/*
	0: 커널이 매핑할 메모리 주소를 선택
	statbuf.st_size: 파일 크기
	PROT_READ: 읽기 모드
	MAP_SHARED: 파일과 메모리 동기화
	fdin: file descripter
	0: offset
	*/
		perror("mmap");
		exit(1);
	}
	if((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1) { // dst와 fdout 메모리 매핑
	/*
	0: 커널이 매핑할 메모리 주소를 선택
	statbuf.st_size: 파일 크기
	PROT_WRITE: 쓰기 모드
	MAP_SHARED: 파일과 메모리 동기화
	fdout: file descripter
	0: offset
	*/
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); // dst에 src의 내용 복사
	// 메모리 복사와 함께 매핑된 파일에 dst의 내용이 argv[1]에 쓰임

	close(fdin); // fdin file descripter 종료
	close(fdout); // fdout file descripter 종료

	return 0;
}
