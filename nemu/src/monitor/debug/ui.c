#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

char *reg_name[9] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eip"} ;

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
 	}

	line_read = readline("(nemu) ");

  	if (line_read && *line_read) {
		add_history(line_read);
 	}

	return line_read;
}

//TODO: Add more functions here

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args)
{
	char *arg=strtok(NULL, " ");
	if(NULL == arg)
		cpu_exec(1);
	else {
		int n=0;
		int i;
		for (i=0; arg[i] != '\0'; ++i) {
			if(arg[i] <= '9' && arg[i] >= '0')
				n = arg[i]-'0' + n*10;
			else {	
				printf("Error: no match arguments %s \n",arg);
				return 0;
			}
		}
//		Log("n = %d\n", n);
		cpu_exec(n);
	}
	return 0;
}

static int cmd_info(char *args)
{
	char *arg = strtok(NULL, " ");
	if(NULL == arg) {
		printf("Error: there must be a subcmd\n");
	}
 	else if(0 == strcmp("r", arg)) {
		int i;
 		for (i=0; i < 8; ++i) {
			printf("%s\t\t0x%x", reg_name[i], cpu.gpr[i]._32);
			if(i != R_ESP && i != R_EBP)
				printf("\t%d\n", cpu.gpr[i]._32);
			else
				printf("\n");
	 	}
		printf("%s\t\t0x%x\n", reg_name[i],cpu.eip);
	} 
	else if(0 == strcmp("w", arg)) {
	//TODO: add	cmd info w	
	}
 	else{
		printf("Error: no match cmd as %s \n", arg);
	}
	return 0;
}

static int cmd_x(char *args)
{
	char *arg1 = strtok(NULL, " ");
/*	char *arg2 = strtok(NULL, " ");*/
	if(NULL == arg1 || NULL == args) {
		printf("Error: there must be 2 subcmds\n");
 	}
  	else {
		int n;
		uint32_t addr;
		bool success = false;
		addr = expr(args, &success);
   		if( (n = atoi(arg1)) && success) {
			int i;
  			for (i=0; i < n; ++i) {
				printf("0x%x\n", swaddr_read(addr+i*4, 4));
 			}			
		}
	}
	return 0;
}

static int cmd_p(char *args)
{
/*	char *arg = strtok(NULL, " ");*/
	if(NULL == args) {
		printf("Error: there must follow a subcmd\n");
		return 0;
	}

	bool success = false;
	uint32_t value = expr(args, &success);
	if(!success) {
		printf("Error: bad expression\n");		
	}
	else {
		printf("%d\n", value);	
	}
	return 0;
}

static int cmd_w(char *args)
{
	char *arg = strtok(NULL, " ");
	if(NULL == arg) {
		printf("Error: there must follow a subcmd\n");
		return 0;
	}

	bool success = false;
	WP *new = new_wp();
	strcpy(new->expr, arg);
	new->expr[strlen(arg)] = '\0';
	Log("new->expr = %s\n", new->expr);
	// how to value a val??
	new->oldValue = expr(new->expr, &success);
	if(success == true) {
		printf("new  watchpoint %d success\n", new->NO);	
 	}
	else {
		printf("new watchpoint failed\n");	
 	}
	
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Step one instruction exactly.Use si[N] to step N times.",cmd_si },
	{ "info", "Generic command for showing things about the program being debuffed."
	"info r -- Print the information of registers"
	"info w -- Print the value of watchpoints", cmd_info },
	{ "p", "Print value of expression EXP", cmd_p },
		 { "w", "Set a watchpoint for an expression."
	"Usage: w EXPR"
	"A watchpoint stops execution of your program whenever the value of an expression changes", cmd_w },
/*	{ "d", "delete watchpoints", cmd_d },*/
	{ "x", "Usage: x N EXPR", cmd_x},

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

 	if(arg == NULL) {
		/* no argument given */
 		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
  		}
 	}
  	else {
 		for(i = 0; i < NR_CMD; i ++) {
 			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
  			}
  		}
		printf("Unknown command '%s'\n", arg);
 	}
	return 0;
}

void ui_mainloop() {
 	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
  		 */
		char *args = cmd + strlen(cmd) + 1;
  		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
  			}
   		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
 	}
	
}
