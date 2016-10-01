#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute()
{
	uint32_t tmp = op_src->simm - op_src->simm;

	cpu.EFLAGES.OF = !(((op_src->imm>>(8*DATA_BYTE-1) & 0x1) ^ (op_src2->imm>>(8*DATA_BYTE-1) & 0x1)) & ((op_src->imm>>(8*DATA_BYTE-1) & 0x1) ^ (tmp>>(8*DATA_BYTE-1) & 0x1)) & 0x1);		
	cpu.EFLAGES.SF = (tmp>>(DATA_BYTE-1) & 1);
	cpu.EFLAGES.ZF = (tmp == 0);
	uint32_t ret = (tmp>>4) ^ tmp;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;

	print_asm_template2();
}

make_instr_helper(i2r)

#include "cpu/exec/template-end.h"
