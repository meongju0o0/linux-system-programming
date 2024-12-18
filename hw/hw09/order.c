#include <stdio.h>

int main() {
	int a = 0x12345678;
	char *p = (char *)&a;

	// Endian 별로 주소값과 저장된 값을 출력
	// Intel System: Little-Endian
	// SPARC, MIPS, Power-PC: Big-Endian
	// ARM: 선택 가능
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;

	return 0;
}
