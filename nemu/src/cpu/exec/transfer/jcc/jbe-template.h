#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute()
{
	if(cpu.EFLAGES.ZF || cpu.EFLAGES.CF)
		cpu.eip = (int8_t)op_src->imm;	
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
