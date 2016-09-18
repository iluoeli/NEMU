#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

int valid = true;	//to detect eval() valid

enum {
	 NOTYPE=256, OR, AND, NEQ, EQ, SUB, ADD, DIV, MUL, NOT, DEREF, NEG, LBR, RBR, NUM, HEX, REG
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", ADD},					// plus
	{"\\-", SUB},
	{"\\*", MUL},
	{"\\/", DIV},
	{"\\(", LBR},
	{"\\)", RBR},
	{"==", EQ},						// equal
	{"0(x | X)[0-9a-fA-F]+", HEX},
	{"[0-9]+", NUM},
	{"&&", AND},
	{"\\|{2}", OR},
	{"!=", NEQ},
	{"\\!", NOT},
	{"\\$[a-z]{2,3}", REG},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) { 
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
 		}
 	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
 	 	for(i = 0; i < NR_REGEX;  i ++) {
 	 		if(regexec(&re[i], e  + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;



 				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
 		 		 */

  				switch(rules[i].token_type) {
					case EQ:
						tokens[nr_token].type = EQ;
						break;
					case NEQ:
						tokens[nr_token].type = NEQ;
						break;
					case AND:
						tokens[nr_token].type = AND;
						break;
					case OR:
						tokens[nr_token].type = OR;
						break;
					case NOT:
						tokens[nr_token].type = NOT;
						break;
					case ADD:
						tokens[nr_token].type = ADD;
						break;
					case SUB:
						tokens[nr_token].type = SUB;
						break;
					case MUL:
						tokens[nr_token].type = MUL;
						break;
					case DIV:
						tokens[nr_token].type = DIV;
						break;
					case LBR:
						tokens[nr_token].type = LBR;
						break;
					case RBR:
						tokens[nr_token].type = RBR;
						break;
					case NUM:
						tokens[nr_token].type = NUM;
						// WARNING: substr_len no more than 32;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						break;
					case HEX:
						tokens[nr_token].type = HEX;
						// WARNING: substr_len no more than 32;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						break;
					case REG:
						tokens[nr_token].type = REG;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						break;
					case NOTYPE:
						--nr_token;
						break;
					default:// panic("please implement me");
						printf("Error: not match type\n");
  	 	 		}
				++nr_token;
				break;

  		 	}
  		} 

  		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
 	}

	return true; 
}


//check bra is valid
int check_parentheses(int p, int q)
{
	int buff=0;
	int i=p;
 	for (; i <= q; ++i) {
		if(tokens[i].type == LBR)
 			++ buff;
		else  if(tokens[i ].type == RBR) {
			if(buff < 1) {	/*Assert(0, "Error: bra not match\n")*/;
				printf("Error: right bra not match");
				return -1;
			 	}
			else
				-- buff;
		}
	}
	if(buff != 0) {
	//	Assert(0, "Error; bra not match\n");
		printf("Error: left bra not match\n");
		return -1;
 	}
	Log("bra match\n");

	if(tokens[p].type != LBR || tokens[q].type != RBR)
		return false;
	for (i=p+1; i < q; ++i) { 
		if(tokens[i].type == LBR || tokens[i].type == RBR)
			return false;	
  	}
	return true;
}

int dot_ope(int p, int q) 
{
	int op = p;
	int i = p;

  	for (; i <= q; ++i) {
  		switch(tokens[i].type) {
			case OR:
				op = i;
				break;
			case AND:
				op = (tokens[op].type > AND) ? i : op;
				break;
			case NEQ:
				op = (tokens[op].type > NEQ) ? i : op;
				break;
			case EQ:
				op = (tokens[op].type > EQ) ? i : op;
				break;
			case ADD:
				op = (tokens[op].type > ADD) ? i : op;
				break;
			case SUB:
				op = (tokens[op].type > SUB) ? i : op;
				break;	
			case MUL:
				op = (tokens[op].type > MUL) ? i : op;
				break;
			case DIV:
				op = (tokens[op].type > DIV) ? i : op;
				break;	
			case DEREF: 
				op = (tokens[op].type > DEREF) ? i : op;
				break;	
			case NEG: 
				op = (tokens[op].type > NEG) ? i : op;
				break;	
			case NOT: 
				op = (tokens[op].type > NOT) ? i : op;
				break;	
			case LBR:
				for (; tokens[i].type != RBR; ++i);	
				break;
	 	}
	}
	return op;
}

uint32_t eval(int p, int q)
{
	Log("p = %d, q = %d\n", p, q);
	if(p > q) {
	//	Assert(0, "Error: error expression , p = %d, q = %d \n",p , q);		
		printf("Error: bad expression\n");
		valid = false;
		return 0;
 	}
	//p == q: number ; p = q-1: a neg+number or DEREF+number
  	else if(p == q) {
		int n = 0;
		int i;
		Log("str %s \n", tokens[p].str);
		//NUM
 		if(tokens[p].type == NUM) {
			for (i=0; tokens[p].str[i] != '\0'; ++i) 
				n = n*10 + tokens[p].str[i]-'0';
			//Log("value = %d\n", n);
			return n;
  	 	}
		//HEX
  		else if(tokens[p].type == HEX) {
  	 		for (i=2; tokens[p].str[i] != '\0'; ++i) {
				if(tokens[p].str[i] <= '9' && tokens[p].str[i] >= '0')	
					n = n*16 + tokens[p].str[i]-'0';	
				else if(tokens[p].str[i] <= 'f' && tokens[p].str[i] >= 'a')
					n = n*16 + tokens[p].str[i] - 'a' + 10;
				else 
					n = n*16 + tokens[p].str[i] - 'A' + 10;
			}
			return n;
 		}

		//REG
		else if(tokens[p].type == REG) {
			if(strcmp(tokens[p].str, "$eax") == 0) 
				return cpu.gpr[0]._32;
			else if(strcmp(tokens[p].str, "$ecx") == 0) 
				return cpu.gpr[1]._32;
			else if(strcmp(tokens[p].str, "$edx") == 0) 
				return cpu.gpr[2]._32;
			else if(strcmp(tokens[p].str, "$ebx") == 0) 
				return cpu.gpr[3]._32;
			else if(strcmp(tokens[p].str, "$esp") == 0) 
				return cpu.gpr[4]._32;
			else if(strcmp(tokens[p].str, "$ebp") == 0) 
				return cpu.gpr[5]._32;
			else if(strcmp(tokens[p].str, "$esi") == 0) 
				return cpu.gpr[6]._32;
			else if(strcmp(tokens[p].str, "$edi") == 0) 
				return cpu.gpr[7]._32;
			else if(strcmp(tokens[p].str, "$eip") == 0) 
				return cpu.eip;
			else if(strcmp(tokens[p].str, "$ax") == 0) 
				return cpu.gpr[0]._16;
			else if(strcmp(tokens[p].str, "$cx") == 0) 
				return cpu.gpr[1]._16;
			else if(strcmp(tokens[p].str, "$dx") == 0) 
				return cpu.gpr[2]._16;
			else if(strcmp(tokens[p].str, "$bx") == 0) 
				return cpu.gpr[3]._16;
			else if(strcmp(tokens[p].str, "$sp") == 0) 
				return cpu.gpr[4]._16;
			else if(strcmp(tokens[p].str, "$bp") == 0) 
				return cpu.gpr[5]._16;
			else if(strcmp(tokens[p].str, "$si") == 0) 
				return cpu.gpr[6]._16;
			else if(strcmp(tokens[p].str, "$di") == 0) 
				return cpu.gpr[7]._16;
			else if(strcmp(tokens[p].str, "$al") == 0) 
				return cpu.gpr[0]._8[0];
			else if(strcmp(tokens[p].str, "$cl") == 0) 
				return cpu.gpr[1]._8[0];
			else if(strcmp(tokens[p].str, "$dl") == 0) 
				return cpu.gpr[2]._8[0];
			else if(strcmp(tokens[p].str, "$bl") == 0) 
				return cpu.gpr[3]._8[0];
			else if(strcmp(tokens[p].str, "$ah") == 0) 
				return cpu.gpr[0]._8[1];
			else if(strcmp(tokens[p].str, "$ch") == 0) 
				return cpu.gpr[1]._8[1];
			else if(strcmp(tokens[p].str, "$dh") == 0) 
				return cpu.gpr[2]._8[1];
			else if(strcmp(tokens[p].str, "$bh") == 0) 
				return cpu.gpr[3]._8[1];
			else {
				printf("invalid regester name %s\n", tokens[p].str);
				valid = false;
				return 0;
			}
 	 	}
		
 	}
	// may be can be delete for it's useless.
/*	else if(p == q-1) {
		if(tokens[p].type == NEG) {
			return -eval(p+1, q);
 		}
 		else if(tokens[p].type == DEREF) {
			return swaddr_read(eval(p+1, q), 4);
		}
		else if(tokens[p].type == NOT) {
			return (eval(p+1, q) == 0) ? 1:0;
		} 
		else {
			//Assert(0, "Error: bad expression\n");
			printf("Error: bad expression\n");
			return 0;
		}
 	}
*/
	else if(check_parentheses(p, q) == true) {
		return eval(p+1, q-1);	
	}
	else if(check_parentheses(p, q) == -1) {
		valid =false;
		return 0;
	}
   	else {
		int op = dot_ope(p, q);	
		Log("op = %d\n", op);
	//	int val1 = eval(p, op-1);
	//	int val2 = eval(op+1, q);

 		switch(tokens[op].type) {
			case ADD: return eval(p, op-1) + eval(op+1, q);
			case SUB: return eval(p, op-1) - eval(op+1, q);
			case MUL: return eval(p, op-1) * eval(op+1, q);
			case DIV: return eval(p, op-1) / eval(op+1, q);
			case EQ:	return (eval(p,op-1) == eval(op+1, q));
			case NEQ:	return (eval(p, op-1) != eval(op+1, q));
			case AND:	return eval(p, op-1) && eval(op+1, q);
			case OR:	return eval(p, op-1) || eval(op+1, q);
			case NOT:	return !eval(p+1, q);
			case DEREF:	return swaddr_read(eval(p+1, q), 4);
			case NEG:	return -eval(p+1, q);
			default:// Assert(0,"Error: when eval tokens[op]\n");
				printf("Error: invalid expression at position %d ~ %d\n", p, q);
				valid = false;
				return 0;
  		}
	}
	return 0;
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
 	}

	/* TODO: Insert codes to evaluate the expression. */
	int i;
	for (i=0; i < nr_token; ++i) {
		if(tokens[i].type == MUL && (i == 0 || (tokens[i-1].type != HEX && tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != RBR)))	
				tokens[i].type = DEREF;
		else if(tokens[i].type == SUB && (i == 0 || (tokens[i-1].type != HEX && tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != RBR)))	
			tokens[i].type = NEG;
 	}
//	panic("please implement me");
	valid = true;
	uint32_t n = eval(0, nr_token-1);
	if(valid == true) {
		*success = true;
		Log("Last value: %d\n", n);
	}
	else
		*success = false;
		return n;
}

