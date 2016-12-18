#include "cpu/exec/helper.h"

make_helper(bt_r2rm_v)
{
	int len = decode_r2rm_l(cpu.eip+2);
		
	cpu.EFLAGES.CF = (op_dest->val >> (op_src->val)) & 1;	

	return len+1;
}
