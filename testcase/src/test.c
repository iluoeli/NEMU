#include "trap.h"
#include <stdio.h>
#include <string.h>

int main()
{
	char s[10];
	sprintf(s, "%d", -1);	
	nemu_assert(strcmp(s, "-1") == 0);
	return 0;
}
