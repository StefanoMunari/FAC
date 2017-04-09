%{
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "factype.h"
#include "facmath.h"

int yylex ();

void yyerror(char * s) {
	fprintf(stderr, "%s\n", s);
}



%}

%union {
	char * id;
	bool bval;
	fract_t fract; 
	type_t type;
	relop_t relop;
	aop0_t aop0;
	aop1_t aop1;
	bop1_t bop1;
	bop2_t bop2;
}
%type <fract> aexpr
%type <bval> bexpr
%token <bval> BOOL		/* Token for true and false literals */
%token <fract> FRACT	   /* Token for the fract */
%token SEPARATOR   /* Separator of statement */
%token L_DEL_SCOPE /* Scope left delimiter */
%token R_DEL_SCOPE /* Scope right delimiter */
%token L_DEL_EXPR  /* Scope left delimiter */
%token R_DEL_EXPR  /* Scope right delimiter */
%token <aop0> AOP0	   /* Arithmetic operation: + and - */
%token <aop1> AOP1	   /* Arithmetic operation */	
%token TYPE
%token <bop1> BOP1		   /* Boolean operation with arity 1 */
%token <bop2> BOP2		   /* Boolean operation with arity 2 */
%token <relop> RELOP	   /* Relation operation LT, LQ, ... */
%token WHILE	   /* token for the while symbol */
%token IF		   /* token for the if symbol */
%token ELSE 	   /* token for the else symbol */
%token SKIP		   /* token for the skip symbol */
%token PRINT		
%token ASSIGNMENT	
%token <id> ID			

%left AOP0
%left AOP1

%right USIGN
%left BOP2 
%right UBOP1

%%
lines : lines aexpr SEPARATOR { printf("RESULT: [%d|%d]\n", $2.num, $2.den); }
| lines bexpr SEPARATOR  { printf("%s\n", $2?"true":"false"); }
| lines '\n'
| /* empty */
;
aexpr : aexpr AOP0 aexpr { 
	switch($2){
		case SUM: $$ = sum($1, $3); break;
		case DIFF: $$ = sum($1, minus($3)); break;
	}
}
| aexpr AOP1 aexpr {
	switch($2){
		case MULT: $$ = mult($1, $3); break;
		case DIV: $$ = mult($1, reciprocal($3)); break;
	}
}
| AOP0 aexpr %prec USIGN { 
	switch($1) {
		case SUM: $$ = $2; break;
		case DIFF: $$ = minus($2); break;
	}
}
| FRACT
;

bexpr : bexpr BOP2 bexpr { 
		switch($2){
			case IFF: $$ = (!$1 || $3) && ($1 || !$3); break;
			case AND: $$ = $1 && $3; break;
			case OR: $$ = $1 || $3; break;
			case IMPLY: $$ = !$1 || $3; break;
			case XOR: $$ = ($1 || $3) && !($1 && $3); break;
		}
	}	
| aexpr RELOP aexpr {
	switch($2){
		case EQ:  $$ = $1.num*$3.den == $3.num*$1.den; break;
		case LT:  $$ = $1.num*$3.den <  $3.num*$1.den; break;
		case LEQ: $$ = $1.num*$3.den <= $3.num*$1.den; break;
		case GT:  $$ = $1.num*$3.den >  $3.num*$1.den; break;
		case GEQ: $$ = $1.num*$3.den >= $3.num*$1.den; break;
		case NEQ: $$ = $1.num*$3.den != $3.num*$1.den; break;
	}
}
| BOP1 bexpr %prec UBOP1{
	$$ = !$2;
 }
| BOOL
;
%%
int main(void){
	yyparse();
	return 0;
}
