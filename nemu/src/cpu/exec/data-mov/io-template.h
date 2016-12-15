#include "cpu/exec/template-start.h"

//#define instr in
uint32_t pio_read(ioaddr_t addr, size_t len);
void pio_write(ioaddr_t addr, size_t len, uint32_t data);

make_helper(concat(in_i2a_, SUFFIX))
{
	int len = decode_i_b(cpu.eip+1);
	//do_execute();
	REG(R_EAX) = pio_read(op_src->val, DATA_BYTE);	

	print_asm("in" str(SUFFIX) "0x%x, %%%s", op_src->val, REG_NAME(R_EAX));

	return len+1;
}

make_helper(concat(in_d2a_, SUFFIX))
{
	REG(R_EAX) = pio_read(reg_w(R_EDX), DATA_BYTE);	

	print_asm("in" str(SUFFIX) "%%DX, %%%s", REG_NAME(R_EAX));

	return 1;
}

//------instr out-----------
make_helper(concat(out_a2i_, SUFFIX))
{
	int len = decode_i_b(cpu.eip+1);
	//do_execute();
	pio_write(op_src->val, DATA_BYTE, REG(R_EAX));	

	print_asm("out" str(SUFFIX) "%%%s, 0x%x", REG_NAME(R_EAX), op_src->val);

	return len+1;
}

make_helper(concat(out_a2d_, SUFFIX))
{
	pio_write(reg_w(R_EDX), DATA_BYTE, REG(R_EAX));	

	print_asm("out" str(SUFFIX) "%%%s, %%DX",  REG_NAME(R_EAX));

	return 1;
}


#include "cpu/exec/template-end.h"
