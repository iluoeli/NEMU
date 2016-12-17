#include "cpu/exec/helper.h"

make_helper(bt_r2rm_v)
{
	int len = decode_r2rm_l(cpu.eip+2);
		
	cpu.EFLAGES.CF = (op_src->val >> (op_dest->val-1)) & 1;	

	return len+2;
}
