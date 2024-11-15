#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]: mymore
[Description]:
	- STDOUT_FILENO를 파이프를 위한 fd로 사용
	- ls -l의 출력 결과를 more로 넘겨서
	- 그 결과를 콘솔에 출력
[Input]: None
[Output]: 콘솔에 more을 통해 출력되는 "ls -l" 결과
[Calls]:
	- exit(int exit_code)
	- perror(char *err_msg)
	- close(int fd)
	- pipe(int fd[2])
	- dup2(int oldfd, int newfd)
	- execlp(const char *file, const char *arg, ..., (char *)NULL)
*/

int main() {
	int fd[2]; // file descripter id를 저장할 변수 선언
	pid_t child; // 자식 프로세스 id를 저장할 변수 선언

	if(pipe(fd)) { // 파이프 생성
		perror("pipe"); // 파이프 생성에 실패하면 에러 메시지 출력
		exit(1); // 비정상 종료
	}

	child = fork(); // 자식 프로세스 생성
	if(child == (pid_t)(-1)) { // 자식 프로세스 생성에 실패하면
		perror("fork"); // 에러 메시지 출력
		exit(1); // 비정상 종료
	}
	else if(child == (pid_t)(0)) { // 자식 프로세스 이면
		close(fd[0]); //fd[0](읽기) file descripter 종료

		if(dup2(fd[1], STDOUT_FILENO) < 0) { // STDOUT을 fd[1](파이프 쓰기)로 변경
			perror("execlp"); // 실패 시 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		close(fd[1]); // fd[1](쓰기) file descripter 종료

		if((execlp("ls", "ls" "-l", NULL)) == - 1) { // 자식 프로세스에서 "ls -l" 명령어 실행
			perror("execlp"); // 실행에 실패시 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}
	else { // 부모 프로세스 이면
		close(fd[1]); // fd[1](쓰기) file descripter 종료

		if(dup2(fd[0], STDIN_FILENO) < 0) { // STDIN을 fd[0](파이프 읽기)로 변경
			perror("dup"); // 실패 시 에러 메시지 출력
			exit(1); // 비정상 종료
		}
		close(fd[0]); // fd[0](읽기) file descripter 종료

		if((execlp("more", "more", NULL)) == -1) { // 부모 프로세스에서 "more" 명령어 실행
			perror("execlp"); // 실행 실패시 에러 메시지 출력
			exit(1); // 비정상 종료
		}
	}

	return 0;
}
