#include "trap.h"
#include <stdio.h>
#include <string.h>

int main()
{
	char s[10];
	sprintf(s, "%d", -12);	
	nemu_assert(strncmp(s, "-12", 3) == 0);
	return 0;
}
