#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	// memory expression
	char expr[32];
	int oldValue; //value old;

} WP;

WP* new_wp();
void free_wp(WP*);
void detect_wp(bool *change);
WP *h_wp();
WP *nr_wp(int num);


#endif
