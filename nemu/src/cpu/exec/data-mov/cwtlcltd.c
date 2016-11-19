#include "cpu/exec/helper.h"

make_helper(cwtl)
{
	reg_w(R_DX) = ((int16_t)reg_w(R_AX) < 0) ? 0xffff : 0x0;
	print_asm("cwtl");	
	return 1;
}


make_helper(cltd)
{
	reg_l(R_EDX) = ((int32_t)reg_l(R_EAX) < 0) ? 0xffffffff : 0x0;
	print_asm("cltd");	
	return 1;
}

make_helper(cwtl_cltd_v)
{
	return (ops_decoded.is_operand_size_16 ? cwtl:cltd)(eip);	
}
