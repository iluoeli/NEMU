#include "cpu/exec/template-start.h"

#define instr jne
make_helper(concat(jne_i_, SUFFIX))
{
	uint32_t len = concat(decode_i_, SUFFIX)(eip);
	if(cpu.EFLAGES.ZF == 0)
		cpu.eip = cpu.eip +  (DATA_TYPE_S)op_src->simm;	
	print_asm_template1();
	return 1+len;
}

#include "cpu/exec/template-end.h"
