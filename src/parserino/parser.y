%{
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
%}

%union {
	char c; 
	bool bval; 
	struct fract_t{int num; int den; } fract; 
	enum type_t{BOOL, FRACT} type;
	enum relop_t{LT, LEQ, EQ, GEQ, GT, NEQ} relop;
	enum bop2_t{IFF, AND, OR, IMPLY, XOR} bop2;
}
%token <fract> aexpr
%token <bval> bexpr
%token <fval> FRACT	   /* Token for the fract */
%token SEPARATOR   /* Separator of statement */
%token L_DEL_SCOPE /* Scope left delimiter */
%token R_DEL_SCOPE /* Scope right delimiter */
%token L_DEL_EXPR  /* Scope left delimiter */
%token R_DEL_EXPR  /* Scope right delimiter */
%token <fval> AOP0	   /* Arithmetic operation: + and - */
%token <fval> AOP1	   /* Arithmetic operation */	
%token TYPE
%token <bval> BOP1		   /* Boolean operation with arity 1 */
%token <bval> BOP2		   /* Boolean operation with arity 2 */
%token RELOP	   /* Relation operation LT, LQ, ... */
%token WHILE	   /* token for the while symbol */
%token IF		   /* token for the if symbol */
%token ELSE 	   /* token for the else symbol */
%token SKIP		   /* token for the skip symbol */
%token PRINT		
%token ASSIGNMENT	
%token ID			

%left AOP0
%left AOP1
%
%right USIGN
%left BOP2 
%right UBOP1

%%
lines : lines aexpr '\n' { printf("%d\n", $2); }
| lines '\n'
| /* empty */
;
aexpr : aexpr AOP0 aexpr { 
	switch(yylval.c){
		case '+': $$.fval = $1.fval + $3.fval; break;
		case '-': $$ = $1 - $3; break;
	}
}
| aexpr AOP1 aexpr {
	switch(yylval.c){
		case '*': $$ = $1 * $3; break;
		case '/': $$ = $1 / $3; break;
	}
}
| AOP0 aexpr %prec USIGN { 
	switch(yyval.c) {
		case '+': $$ = +$2; break;
		case '-': $$ = -$2; break;
	}
}
| FRACT
;

bexpr : bexpr  BOP2 bexpr { 
	switch(yylval.c){
		case IFF: $$ = (!$1 || $3) && ($1 || !$3); break;
		case AND: $$ = $1 && $3; break;
		case OR: $$ = $1 || $3; break;
		case IMPLY: $$ = !$1 || $3; break;
		case XOR: $$ = ($1 || $3) && !($1 && $3); break;
	}
}
| aexpr RELOP aexpr {
	switch(yylval.relop){
		case EQ: $$ = $1 == $2; break;
		case LT: $$ = $1 < $2; break;
		case LEQ: $$ = $1 <= $2; break;
		case GT: $$ = $1 > $2; break;
		case GE: $$ = $1 >= $2; break;
		case NEQ: $$ = $1 != $2; break;
	}
}
| BOP1 bexpr %prec UBOP1{
	$$ = !$2;
}
| BOOL
;


%%
int main() { 
	if(yyparse() != 0)
		fprintf(stderr, "Abnormal exit\n");
	return 0;
}
