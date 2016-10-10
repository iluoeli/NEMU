#include "cpu/exec/helper.h"

make_helper(cwtl)
{
	int32_t tmp = reg_w(R_EAX);
	reg_l(R_EAX) = tmp;
	print_asm("cwtl");	
	return 1;
}


make_helper(cltd)
{
	int64_t tmp = reg_l(R_EAX);
	reg_l(R_EDX) = (tmp >> 32) & 0xffffffff;
	print_asm("cltd");	
	return 1;
}

