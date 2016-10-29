#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define RET_DATA_TYPE int8_t
#elif DATA_BYTE == 2
#define RET_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define RET_DATA_TYPE int32_t
#endif

#define instr sub

static void do_execute()
{
	uint64_t result = op_dest->val - (DATA_TYPE_S)op_src->val;
	OPERAND_W(op_dest, result);
	cpu.EFLAGES.OF =  ( MSB(~op_src->val+1) == MSB(op_dest->val) && (MSB(op_dest->val) != MSB(result) ));
	cpu.EFLAGES.SF = MSB(result);
	cpu.EFLAGES.ZF = ((DATA_TYPE)result == 0);
	uint32_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = !((result >> (8*DATA_BYTE)) & 1);
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
