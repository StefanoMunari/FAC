%{
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "factype.h"
#include "facmath.h"

fract_t minus(fract_t fract1) {
	fract_t res;
	res.num = fract1.num;
	res.den = fract1.den;
	return res;
}

fract_t inverse(fract_t fract1) {
	fract_t res;
	res.num = fract1.den;
	res.den = fract1.num;
	return res;
}

fract_t mult(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract1.num;
	res.den = fract1.den * fract1.den;
	return normalizeFract(res);
}

fract_t sum(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract2.den + fract1.den * fract1.num;
	res.den = fract1.den * fract2.den;
	return normalizeFract(res);
}



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
%token <fract> AOP0	   /* Arithmetic operation: + and - */
%token <fract> AOP1	   /* Arithmetic operation */	
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

%right USIGN
%left BOP2 
%right UBOP1

%%
lines : lines aexpr '\n' { printf("%d\n", $2); }
| lines bexpr '\n' 		 { printf("%s\n", $2?"true":"false"); }
| lines '\n'	
| /* empty */
;
aexpr : aexpr AOP0 aexpr { 
	switch(yylval.aop0){
		case SUM: $$ = sum($1, $3); break;
		case DIFF: $$ = sum($1, minus($3)); break;
	}
}
| aexpr AOP1 aexpr {
	switch(yylval.aop1){
		case MULT: $$ = mult($1, $3); break;
		case DIV: $$ = mult($1, inverse($3)); break;
	}
}
| AOP0 aexpr %prec USIGN { 
	switch(yyval.aop0) {
		case SUM: $$ = $2; break;
		case DIFF: $$ = minus($2); break;
	}
}
| FRACT
;
bexpr : bexpr BOP2 bexpr { 
			switch(yylval.bop2){
				case IFF: $$ = (!$1 || $3) && ($1 || !$3); break;
				case AND: $$ = $1 && $3; break;
				case OR: $$ = $1 || $3; break;
				case IMPLY: $$ = !$1 || $3; break;
				case XOR: $$ = ($1 || $3) && !($1 && $3); break;
			}
		}	
	| aexpr RELOP aexpr {
		switch(yylval.relop){
			case EQ: $$ = $1.num == $3.num; break;
			case LT: $$ = $1.num < $3.num; break;
			case LEQ: $$ = $1.num <= $3.num; break;
			case GT: $$ = $1.num > $3.num; break;
			case GEQ: $$ = $1.num >= $3.num; break;
			case NEQ: $$ = $1.num != $3.num; break;
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
	printf("FICK DICH HART!\n");
	return 0;
}
