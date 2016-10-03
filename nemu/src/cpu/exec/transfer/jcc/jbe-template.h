#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define RET_DATA_TYPE int8_t
#elif DATA_BYTE == 2
#define RET_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define RET_DATA_TYPE int32_t
#endif

#define instr jbe

static void do_execute()
{
	if(cpu.EFLAGES.ZF || cpu.EFLAGES.CF)
		cpu.eip += (RET_DATA_TYPE)op_src->imm;	
	print_asm_template1();
}

make_instr_helper(i)

#undef RET_DATA_TYPE

#include "cpu/exec/template-end.h"
