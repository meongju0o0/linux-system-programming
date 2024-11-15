#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
[Program Name]: stat
[Description]: argument로 입력받은 경로의 정보를 출력한다.
[Input]: 파일이나 디렉토리의 경로
[Output]: 해당 파일이나 디렉터리의 정보를 콘솔에 출력
[Calls]:
	printf(char* str, ...)
	lstat(char* pathname, struct stat *buf)
	perror(char* err_msg)
	S_ISREG()
	S_ISDIR()
	S_ISCHR()
	S_ISBLK()
	S_ISFIFO()
	S_ISLNK()
	S_ISSOCK()
*/

int main(int argc, char *argv[]) {
	struct stat statbuf; // stat을 저장할 버퍼 구조체 선언
	char *mode; // 해당 경로의 정보를 저장할 문자열 선언
	
	for(int i = 1; i < argc; i++) { // 모든 argument에 대해
		printf("%s: ", argv[i]); // 현재 argument의 정보를 출력
		if(lstat(argv[i], &statbuf) == -1) { // argument가 잘못 주어진 경우 -> -1 반환
			perror("lstat"); // "lstat" 에러 출력
		}
		else { // 주어진 경로의 정보를 읽을 수 있는 경우
			if(S_ISREG(statbuf.st_mode)) { // regular file 이면
				mode = "regular"; // mode에 "regular" 저장
			}
			else if(S_ISDIR(statbuf.st_mode)) { // directory 이면
				mode = "directory"; // mode에 "directory" 저장
			}
			else if(S_ISCHR(statbuf.st_mode)) { // device character special file 이면
				mode = "character special"; // mode에 "character special" 저장
			}
			else if(S_ISBLK(statbuf.st_mode)) { // disk block file 이면
				mode = "block special"; // mode에 "blcok special" 저장
			}
			else if(S_ISFIFO(statbuf.st_mode)) { // FIFO(IPC, pipe) 이면
				mode = "FIFO"; // mode에 "FIFO" 저장
			}
			else if(S_ISLNK(statbuf.st_mode)) { // symbolic link 이면
				mode = "symbolic link"; // mode에 "symbolic link" 저장
			}
			else if(S_ISSOCK(statbuf.st_mode)) { // socket 이면
				mode = "socket"; // mode에 "socket" 저장
			}
			// mode와 기타 정보 출력
			printf("%s\n", mode);
			printf("\tst_mode = %d\n", statbuf.st_mode);
			printf("\tst_ino = %ld\n", statbuf.st_ino);
			printf("\tst_dev = %ld\n", statbuf.st_dev);
			printf("\tst_rdev = %ld\n", statbuf.st_rdev);
			printf("\tst_nlink = %ld\n", statbuf.st_nlink);
			printf("\tst_uid = %d\n", statbuf.st_uid);
			printf("\tst_gid = %d\n", statbuf.st_gid);
			printf("\tst_size = %ld\n", statbuf.st_size);
			printf("\tst_atime = %ld\n", statbuf.st_atime);
			printf("\tst_mtime = %ld\n", statbuf.st_mtime);
			printf("\tst_ctime = %ld\n", statbuf.st_ctime);
			printf("\tst_blksize = %ld\n", statbuf.st_blksize);
			printf("\tst_blocks = %ld\n", statbuf.st_blocks);
		}
	}

	return 0;
}
