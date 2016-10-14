#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define RET_DATA_TYPE int8_t
#elif DATA_BYTE == 2
#define RET_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define RET_DATA_TYPE int32_t
#endif

#define instr sbb

static void do_execute()
{
	DATA_TYPE result = op_dest->val - (op_src->val + cpu.EFLAGES.CF);
	OPERAND_W(op_dest, result);
	cpu.EFLAGES.OF =  ( !MSB(op_src->val) == MSB(op_dest->val) && (MSB(op_dest->val+cpu.EFLAGES.CF) != MSB(result) ));
	cpu.EFLAGES.SF = MSB(result);
	cpu.EFLAGES.ZF = (result == 0);
	uint32_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (op_dest->val < (op_src->val+cpu.EFLAGES.CF));
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2a)
make_instr_helper(i2rm)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#undef RET_DATA_TYPE

#include "cpu/exec/template-end.h"
