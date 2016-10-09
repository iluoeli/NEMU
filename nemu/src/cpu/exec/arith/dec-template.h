#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	uint64_t result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.EFLAGES.OF =  ( MSB(op_src->val) == 0 && (MSB(op_src->val) != MSB(result) ));
    cpu.EFLAGES.SF = MSB(result);
	cpu.EFLAGES.ZF = ((DATA_TYPE)result == 0);
    uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (result >> (DATA_BYTE*8 - 1)) & 1;	 

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
