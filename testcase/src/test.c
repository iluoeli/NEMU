#include "trap.h"
#include <stdio.h>
#include <string.h>

int main()
{
	char s[10];
	sprintf(s, "%d", -123456);	
	nemu_assert(strcmp(s, "-123456") == 0);
	return 0;
}
