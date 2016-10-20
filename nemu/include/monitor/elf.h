#ifndef __MYELF_H__
#define __MYELF_H__

#include "common.h"


uint32_t search_elf_obj(char *, bool *);
void load_elf_tables(int argc, char *argv[]); 

#endif
