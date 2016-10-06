#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute()
{
	DATA_TYPE tmp = op_dest->val - op_src->val;

	cpu.EFLAGES.OF = ( ( MSB(op_src->val) ^ MSB(op_dest->val) ) & (MSB(op_src->val) ^ MSB(tmp) ));		
	cpu.EFLAGES.SF = MSB(tmp);
	cpu.EFLAGES.ZF = (tmp == 0);
	uint32_t ret = (tmp>>4) ^ tmp;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (op_dest->val < op_src->val);

	print_asm_template2();
}
/*
make_helper(concat(cmp_ib2rm_, SUFFIX))
{
	//concat(decode_rm_, SUFFIX)(eip);
	op_dest->imm = 1;
//	decode_i_b(eip+DATA_BYTE);
//	op_dest->type = OP_TYPE_MEM;
//	op_dest->imm = read(eip, );		
	do_execute();
	return 1+DATA_BYTE;
}
*/

make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(i2a)
make_instr_helper(rm2r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
