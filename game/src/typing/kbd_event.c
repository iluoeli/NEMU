#include "x86.h"

#define KBD_PORT 0x60

void add_irq_handle(int , void  (*) (void));

void keyboard_event();

void init_kbd() {
	add_irq_handle(1, keyboard_event);
}

