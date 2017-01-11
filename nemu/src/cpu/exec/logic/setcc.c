#include "cpu/exec/helper.h"

#define DATA_BYTE 1

#define instr setne
#define instr_condition (cpu.EFLAGES.ZF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr seta
#define instr_condition (cpu.EFLAGES.CF == 0 && cpu.EFLAGES.ZF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setae
#define instr_condition (cpu.EFLAGES.CF == 0 )
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setb
#define instr_condition (cpu.EFLAGES.CF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setbe
#define instr_condition (cpu.EFLAGES.CF == 1 || cpu.EFLAGES.ZF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setc
#define instr_condition (cpu.EFLAGES.CF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr sete
#define instr_condition (cpu.EFLAGES.ZF == 1 )
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setg
#define instr_condition (cpu.EFLAGES.ZF == 0 && cpu.EFLAGES.SF == cpu.EFLAGES.OF)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setge
#define instr_condition (cpu.EFLAGES.SF == cpu.EFLAGES.OF)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setl
#define instr_condition (cpu.EFLAGES.SF != cpu.EFLAGES.OF)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setle
#define instr_condition (cpu.EFLAGES.ZF == 1 || cpu.EFLAGES.SF != cpu.EFLAGES.OF)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setno
#define instr_condition (cpu.EFLAGES.OF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setnp
#define instr_condition (cpu.EFLAGES.PF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setns
#define instr_condition (cpu.EFLAGES.SF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setnz
#define instr_condition (cpu.EFLAGES.SF == 0)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr seto
#define instr_condition (cpu.EFLAGES.OF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setp
#define instr_condition (cpu.EFLAGES.PF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr sets
#define instr_condition (cpu.EFLAGES.SF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition

#define instr setz
#define instr_condition (cpu.EFLAGES.SF == 1)
#include "setcc-template.h"
#undef instr
#undef instr_condition




