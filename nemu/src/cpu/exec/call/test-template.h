#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
	int32_t tmp = op_src->val && op_src2->val;
	cpu.EFLAGES.CF = 0;
	cpu.EFLAGES.OF = 0;
	//will DATA_BYTE eflect??
	cpu.EFLAGES.SF = ((tmp & 0x8000) == 0)? 0:1;	
	cpu.EFLAGES.ZF = (tmp == 0x0000)? 1:0;	
	uint8_t count = 0;
	int8_t i = 0;
	for(;i < 8; i++)
		count += ((tmp & (0x1 << i)) == 0)? 0:1;
	cpu.EFLAGES.PF = (count%2 == 0)? 0:1;
}

make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
