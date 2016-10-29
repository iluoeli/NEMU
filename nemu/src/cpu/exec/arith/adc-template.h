#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define RET_DATA_TYPE int8_t
#elif DATA_BYTE == 2
#define RET_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define RET_DATA_TYPE int32_t
#endif

#define instr adc

static void do_execute()
{
	uint64_t result = (uint64_t)op_dest->val + (uint64_t)op_src->val + cpu.EFLAGES.CF;
	OPERAND_W(op_dest, result);
	cpu.EFLAGES.OF =  ( MSB(op_src->val) == MSB(op_dest->val) && (MSB(op_dest->val) != MSB(result)));
	cpu.EFLAGES.SF = MSB(result);
	cpu.EFLAGES.ZF = ((DATA_TYPE)result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (result >> (DATA_BYTE*8)) & 1;

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
