#include <stdio.h>
#include "mystrcpy.c"

int main()
{
	char str[80];

	mystrcpy(str, "Hello, World!");
	puts(str);
}

