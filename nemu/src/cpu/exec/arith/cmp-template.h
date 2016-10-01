#include "cpu/exec/template-start.h"

#define instr cmp

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute()
{
	uint32_t tmp = op_src->imm - op_dest->imm;

	cpu.EFLAGES.OF = !(((op_src->imm>>(8*DATA_BYTE-1) & 0x1) ^ (op_dest->imm>>(8*DATA_BYTE-1) & 0x1)) & ((op_src->imm>>(8*DATA_BYTE-1) & 0x1) ^ (tmp>>(8*DATA_BYTE-1) & 0x1)) & 0x1);		
	cpu.EFLAGES.SF = (tmp>>(DATA_BYTE-1) & 1);
	cpu.EFLAGES.ZF = (tmp == 0);
	uint32_t ret = (tmp>>4) ^ tmp;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;

	print_asm_template2();
}

make_helper(concat(cmp_ib2rm_, SUFFIX))
{
	concat(decode_rm_, SUFFIX)(eip);
	*op_dest = *op_src;
//	decode_i_b(eip+DATA_BYTE);
//	op_dest->type = OP_TYPE_MEM;
//	op_dest->imm = read(eip, );		
	do_execute();
	return 1+DATA_BYTE;
}

#endif

//make_instr_helper(i2rm)



#include "cpu/exec/template-end.h"
