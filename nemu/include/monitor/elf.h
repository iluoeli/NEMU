#ifndef __MYELF_H__
#define __MYELF_H__

#include "common.h"

typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
} PartOfStackFrame;

uint32_t search_elf_obj(char *, bool *);
void load_elf_tables(int argc, char *argv[]); 


#endif
