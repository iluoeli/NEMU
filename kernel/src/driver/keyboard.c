#include "common.h"
#include "x86.h"


#define KBD_PORT_BASE 0x60

void add_irq_handle(int , void  (*) (void));

void kbd_intr()
{
	in_byte(KBD_PORT_BASE);	
}

void init_kbd(void) {
	add_irq_handle(1, kbd_intr);	
}
