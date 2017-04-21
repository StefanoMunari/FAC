%{
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "factype.h"
#include "facmath.h"
#include "facerr.h"
#include "../../parser/c_src/symbol_table.h"

extern FILE * yyin;

entry * symbol_table = NULL; // declaration of the variable



int yylex ();

void yyerror(char * s);

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
%token L_DEL_EXPR  /* Expression left delimiter */
%token R_DEL_EXPR  /* Expression right delimiter */
%token <aop0> AOP0	   /* Arithmetic operation: + and - */
%token <aop1> AOP1	   /* Arithmetic operation */	
%token <type> TYPE
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
stmt : stmt aexpr SEPARATOR { printf("RESULT: [%d|%d]\n", $2.num, $2.den); }
| stmt bexpr SEPARATOR  { printf("%s\n", $2?"true":"false"); }
| stmt declaration SEPARATOR { printf("DECLARATION\n"); }
| stmt var_assignment SEPARATOR {printf("Assignment\n"); }
| stmt print_var SEPARATOR 
| stmt '\n'
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
| L_DEL_EXPR aexpr R_DEL_EXPR
| FRACT
| ID	{ $$ = *(fract_t*) lookupID($1, FRACT_T); }
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
| L_DEL_EXPR bexpr R_DEL_EXPR
| BOOL
| ID	{ $$ = *(bool*)lookupID($1, BOOL_T); }
;

declaration : TYPE ID { installID($2,$1); }	

var_assignment : ID ASSIGNMENT aexpr {
	fract_t f = $3; 
	setValue($1, FRACT_T, &f);
}
| ID ASSIGNMENT bexpr {
	bool t = $3;
	setValue($1, BOOL_T, &t);

}
;

print_var : PRINT L_DEL_EXPR ID R_DEL_EXPR {
	type_t type = getType($3);
	switch(type){
		case FRACT_T: 
		{
			fract_t f = *(fract_t*) lookupID($3, FRACT_T); 
			printf("VAR %s = [%d|%d]\n", $3, f.num, f.den);
			break;
		}
		case BOOL_T:
		{
			bool b = (bool) lookupID($3, BOOL_T);
			printf("VAR %s = %s\n", $3, b?"true":"false");
			break;
		}
	}
}

%%
int main(int argc, char * argv[]) {
	if(argc < 2){
		fprintf(stderr, "Usage: %s <file-to-interpret>", argv[0]);
		return EXIT_FAILURE;
	}
 
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
      err_handler(argv[1], FAC_STANDARD_ERROR);
      return EXIT_FAILURE;
    } 
    
    yyin = fp;
  
	yyparse();
	int err_code = fclose(yyin);
	if(err_code == EOF)
		err_handler(argv[1], FAC_STANDARD_ERROR);
	freeTable();
	return EXIT_SUCCESS;
}

void yyerror(char * s) {
	fprintf(stderr, "%s\n", s);
}
