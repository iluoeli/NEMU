#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	cpu.EFLAGES.CF = 0;
	cpu.EFLAGES.OF = 0;
	cpu.EFLAGES.OF =  ( MSB(op_src->val) == MSB(op_dest->val) && (MSB(op_dest->val) != MSB(result)));
	cpu.EFLAGES.SF = MSB(result);
	cpu.EFLAGES.ZF = (result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
//	panic("please implement me");

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
