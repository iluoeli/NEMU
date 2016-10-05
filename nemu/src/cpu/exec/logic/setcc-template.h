#include "cpu/exec/template-start.h"

#define instr setne
make_helper(setne_rm_b)
{
	int len = decode_rm_b(eip);
	DATA_TYPE result = (cpu.EFLAGES.ZF == 0);
	OPERAND_W(op_src, result);
	print_asm_template1();
	return len;	
}
	

#include "cpu/exec/template-end.h"
