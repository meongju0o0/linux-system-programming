#include <stdio.h>

/*
[Program Name]: string
[Description]: stl과 동일한 strlen(char* str), strcpy(char* str), strcat(char* str1, char* str2), strcmp(char* str1, char* str2) 기능을 제공
[Input]: None
[Output]: None
[Calls]: printf(char* format, ...)
*/

int strlen_p(char *str) {
	/*
	strlen_p: 
		return type: integer
		function name: strlen_p
		parameter: 문자열
	Description:
		문자열의 길이를 확인하는 함수
		포인터를 활용해 문자열의 길이를 확인
	Input:
		문자열 1개
	Call By:
		main()
	Calls:
		None
	Example:
		int hello_len = strlen_p("Hello");
	*/

	int len = 0;

	while(str[len] != '\0') {
		len++;
	} // NUL을 만날 때까지 len에 1씩 더함

	return len; // 길이 반환
}

int strlen_a(char str[]) {
	/*
	strlen_a:
		return type: integer
		function name: strlen_a
		parameter: 문자열
	Description:
		문자열의 길이를 확인하는 함수
		배열 idx를 통한 접근을 활용해 문자열의 길이를 확인
	Input:
		문자열 1개
	Call By:
		main()
	Calls:
		None
	Example:
		int hello_len = strlen_a("Hello");
	*/

	int i; // 변수 선언
	for(i = 0; str[i] != '\0'; i++); // NUL을 만날 때까지 i에 1을 더함
	return i; // 길이 반환
}

void strcpy_p(char *dst, char *src) {
	/*
	strcpy_p:
		return type: void
		function name: strcpy_p
		parameter: 복사될 문자열, 복사할 문자열
	Description:
		dst에 src의 문자열을 복사하는 함수
		포인터를 이용해 dst에 src의 내용을 복사
	Input:
		dst: 복사될 문자열
		src: 복사할 문자열
	Call By:
		main()
	Calls:
		None
	Example:
		char *str1, *str2;
		str2 = "안녕 세계";
		strcpy_p(str1, str2);
	*/

	while(*src) {
		*dst++ = *src++;
	} // 포인터의 주소를 한 칸씩 옮기면서 문자 하나씩 복사
	*dst = *src; // 마지막 NUL 복사
}

void strcpy_a(char dst[], char src[]) {
	/*
	strcpy_a:
		return type: void
		function name: strcpy_a
		parameter: 복사될 문자열, 복사할 문자열
	Description:
		dst에 src의 문자열을 복사하는 함수
		배열 idx를 활용해 dst에 src의 내용을 복사
	Input:
		dst: 복사될 문자열
		src: 복사할 문자열
	Call By:
		main()
	Calls:
		None
	Example:
		char *str1, *str2;
		str2 = "안녕 세계";
		strcpy_a(str1, str2);
	*/

	int i; // 변수 i 선언
	for(i = 0; src[i] != '\0'; i++) {
		dst[i] = src[i];
	} // idx를 하나씩 증가 시키면서 문자 하나씩 복사
	dst[i] = src[i]; // 마지막 NUL 복사
}

void strcat_p(char *dst, char *src) {
	/*
	strcat_p:
		return type: void
		function name: strcat_p
		parameter: 합쳐질 문자열, 합칠 문자열
	Description:
		포인터를 활용해 dst의 맨 오른쪽 뒤에 src를 합침
	Input:
		dst: 합쳐질 문자열
		src: 합칠 문자열
	Call By:
		main()
	Calls:
		None
	Example:
		char *str1, *str2;
		str1 = "안녕";
		str2 = " 세계";
		strcat_p(str1, str2);
	*/

	while(*dst++); // dst의 마지막까지 이동 -> dst 포인터가 가리키는 위치는 NUL 다음의 메모리 주소
	dst--; //dst가 NUL을 가리키도록 하기 위함

	while(*src) { // NUL을 만나지 않는 동안
		*dst++ = *src++; // dst의 end부터, src의 begin부터, 하나씩 복사
	}

	*dst = *src; // 마지막 NUL을 붙임
}

void strcat_a(char dst[], char src[]) {
	/*
	strcat_a:
		return type: void
		function name: strcat_a
		parameter: 합쳐질 문자열, 합칠 문자열
	Description:
		배열 idx를 활용해 dst의 맨 오른쪽 뒤에 src를 합침
	Input:
		dst: 합쳐질 문자열
		src: 합칠 문자열
	Call By:
		main()
	Calls:
		None
	Example:
		char *str1, *str2;
		str1 = "안녕";
		str2 = " 세계";
		strcat_a(str1, str2);
	*/

	int i, j; // 변수 i, j 선언
	
	for(i = 0; dst[i] != '\0'; i++); // i가 dst의 NUL을 가리키도록함
	
	for(j = 0; src[j] != '\0'; j++) {
		dst[i+j] = src[j]; //dst의 end부터, src의 begin부터, 하나씩 복사
	}

	dst[i+j] = src[j]; // 마지막 NUL을 붙임
}

int strcmp_p(char *dst, char *src) {
	/*
	strcmp_p:
		return type: integer
		function name: strcmp_p
		parameter: 비교할 문자열 2개
	Description:
		포인터를 활용해 문자열을 비교
		두 문자열이 같은 경우 0을 반환
		두 문자열이 다른 경우 dst - src를 반환
	Call By:
		main()
	Calls:
		None
	Example:
		strcmp_p("사과", "수박");
	*/

	while(*dst || *src) { // 두 문자열 모두 NUL이 아닌 동안
		if(*dst != *src) { // 만약 문자가 다르면
			return *dst - *src; // 현재 두 문자의 차이를 반환
		}
		dst++; // 포인터 이동
		src++; // 포인터 이동
	}
	return 0; // 문자열이 같으면 0 반환
}

int strcmp_a(char dst[], char src[]) {
	/*
	strcmp_a:
		return type: integer
		function name: strcmp_a
		parameter: 비교할 문자열 2개
	Description:
		배열 idx를 활용해 문자열을 비교
		두 문자열이 같은 경우 0을 반환
		두 문자열이 다른 경우 dst - src를 반환
	Call By:
		main()
	Calls:
		None
	Example:
		strcmp_a("사과", "수박");
	*/

	for(int i = 0; dst[i] || src[i]; i++) { // 두 문자열 모두 NUL이 아닌 동안
		if(dst[i] != src[i]) { // 만약 문자가 다르면
			return dst[i] - src[i]; // 현재 두 문자의 차이를 반환
		}
	}
	return 0; // 문자열이 같으면 0 반환
}

int main() {
	int len1, len2;
	char str1[20], str2[20];
	
	len1 = strlen_p("Hello"); // 포인터를 활용하여 문자열의 길이 확인
	len2 = strlen_a("Hello"); // idx를 활용하여 문자열의 길이 확인
	printf("strlen: p=%d, a=%d\n", len1, len2); // 문자열의 길이 출력

	strcpy_p(str1, "Hello"); // str1에 "Hello"라는 문자열 복사
	strcpy_a(str2, "Hello"); // str2에 "Hello"라는 문자열 복사
	printf("strcpy: p=%s, a=%s\n", str1, str2); // 복사된 str1, str2 출력

	strcat_p(str1, ", World!"); // str1 뒤에 ", World!"라는 문자열을 붙임
	strcat_a(str2, ", World!"); // str2 뒤에 ", World!"라는 문자열을 붙임
	printf("strcat: p=%s, a=%s\n", str1, str2); // 합쳐진 str1, str2 출력
	
	printf("strcmp: p=%d, a=%d\n", strcmp_p(str1, str2), strcmp_a(str1, str2));
	// str1과 str2가 같은지 비교한 후, 결과 출력
	printf("strcmp: p=%d, a=%d\n", strcmp_p("apple", "banana"), strcmp_a("apple", "banana"));
	// "apple"과 "banana"가 같은지 비교한 후, 결과 출력

	return 0;
}
