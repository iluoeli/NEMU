#include "cpu/exec/template-start.h"

#define instr jz

static void do_execute()
{
	if(cpu.EFLAGES.ZF == 0)
		cpu.eip += op_src->val & 0xff;	
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
