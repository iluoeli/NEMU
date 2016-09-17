#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;
WP* new_wp();
void free_wp(WP *wp);

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i].oldValue = 0;
 	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


//return a free wp
WP* new_wp()
{
 	if(free_ != NULL) {		
		WP *new = free_;
		free_ = free_->next;
		WP *last = head;
		new->next = NULL;
		if(head == NULL)
			head = new;
		else {
			while (last && last->next && last->next->NO < new->NO)
				last = last->next;
			new->next = last->next;
			last->next = new;
		} 
		return new;
	}
  	else {
		// no more free wp
		Assert(0, "Error: no more free wp");	
		//return NULL;
	}
}

//return wp to free list
void free_wp(WP *wp)
{
	//be care of the  order of wp_pool;
	if(wp == NULL)
		return;
	
	if(wp->NO == 1) {
		head = head->next;
	}
 	else {
		WP *previous = head;	
		while (previous->next && previous->next->NO != wp->NO)
			previous = previous->next;
		if(previous->next && previous->NO == wp->NO) {
			previous->next = wp->next;	
		}
	}
	
	wp->next = NULL;	
	if(wp->NO < free_->NO) {
		wp->next = free_;
		free_ = wp;	
	}
	else {
		WP *previous = free_;
		while (previous && previous->NO < wp->NO)
			previous = previous->next;
		wp->next = previous->next;
		previous->next = wp;
	}
}

void detect_wp(bool *change)
{
	WP *current = head;
	bool success = false;
 	for (; current; current = current->next) {
 		if(current->oldValue != expr(current->expr, &success)) {
			printf("detected watchpoint %d, expression %s changed\n", current->NO, current->expr);
			*change = true;	
		}	
	}		
}

WP *h_wp()
{
	return head;	
}

WP *nr_wp(int n)
{
	if(n < 32)
		return &wp_pool[n];	
	else
		return NULL;
}
