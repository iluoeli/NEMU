#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM

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
	{"\\+", '+'},					// plus
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"==", EQ},						// equal
	{"[0-9]+", NUM}


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
	 	for(i = 0; i < NR_REGEX; i ++) {
	 		if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				++nr_token;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case EQ:
						tokens[i].type = EQ;
						break;
					case '+':
						tokens[i].type = '+';
						break;
					case '-':
						tokens[i].type = '-';
						break;
					case '*':
						tokens[i].type = '*';
						break;
					case '/':
						tokens[i].type = '/';
						break;
					case '(':
						tokens[i].type = '(';
						break;
					case ')':
						tokens[i].type = ')';
						break;
					case NUM:
						tokens[i].type = NUM;
						// WARNING: substr_len no more than 32;
						strncpy(substr_start, tokens[i].str, substr_len);
						break;
					default: panic("please implement me");
							--nr_token;
	 			}
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
bool check_parentheses(int p, int q)
{
	int buff=0;
	int i=p;
	for (; i <= q; ++i) {
		if(tokens[i].type == '(')
			++ buff;
		else if(tokens[i].type == ')') {
			if(buff < 1) {	Assert(0, "Error: bra not match\n");	return false;}
			else
				-- buff;
		}
	}
	if(buff != 0) {
		Assert(0, "Error; bra not match\n");
		return false;
	}

	if(tokens[p].type != '(' || tokens[q].type != ')')
		return false;
	for (i=p; i <= q; ++i) {
		if(tokens[i].type == '(' || tokens[i].type == ')')
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
			case '+':
				op = i;
				break;
			case '-':
				op = i;
				break;	
			case '*':
				if(tokens[op].type != '+' && tokens[op].type != '-')
					op = i;
				break;
			case '/':
				if(tokens[op].type != '+' && tokens[op].type != '-')
					op = i;
				break;	
			case '(':
				for (; tokens[i].type != ')'; ++i);	
				break;
		}
	}
	return op;
}

int eval(int p, int q)
{
	Log("p = %d, q = %d\n", p, q);
	if(p > q) {
		Assert(0, "Error: error expression , p = %d, q = %d \n",p , q);		
		return -1;
	}
	else if(p == q) {
		int n = 0;
		int i;
		for (i=0; tokens[p].str[i] != '\0'; ++i)
			n = n*10 + tokens[p].str[i]-'0';
		Log("value = %d\n", n);
		return n;
	}
	else if(check_parentheses(p, q) == true) {
		return eval(p+1, q-1);	
	}
	else {
		int op = dot_ope(p, q);	
		Log("op = %d\n", op);
		int val1 = eval(p, op-1);
		int val2 = eval(op+1, q);

		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: Assert(0,"Error: when eval tokens[op]\n");
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

//	panic("please implement me");
	Log("nr_token = %d\n", nr_token);
	return eval(0,nr_token-1);
}

