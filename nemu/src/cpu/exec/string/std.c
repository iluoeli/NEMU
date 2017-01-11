#include "cpu/exec/helper.h"

make_helper(std)
{
	cpu.EFLAGES.DF = 1;	
	print_asm("std");	
	return 1;
}


make_helper(cld)
{
	cpu.EFLAGES.DF = 0;	
	print_asm("cld");	
	return 1;
}
