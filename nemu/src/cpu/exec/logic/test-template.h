#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
	DATA_TYPE tmp = op_src->val & op_dest->val;
	cpu.EFLAGES.CF = 0;
	cpu.EFLAGES.OF = 0;
	//will DATA_BYTE eflect??
	cpu.EFLAGES.SF = MSB(tmp);	
	cpu.EFLAGES.ZF = (tmp == 0);	
	//PF
	uint32_t ret = tmp;
	ret = ((uint32_t)tmp>>4) ^ tmp;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
