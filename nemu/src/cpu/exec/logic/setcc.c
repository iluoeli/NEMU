#include "cpu/exec/helper.h"

#define DATA_BYTE 1

#define instr setne
#define instr_condition (cpu.EFLAGES.CF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr seta
#define instr_condition (cpu.EFLAGES.CF == 0 && cpu.EFLAGES.ZF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition



