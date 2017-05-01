%{
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "factype.h"
#include "facmath.h"
#include "facerr.h"
#include "AST.h"
#include "symbol_table.h"

extern FILE * yyin;

entry * symbol_table = NULL; // declaration of the variable
int yylex ();
void yyerror(char * s);



AST_node * head = NULL;
AST_node * last;


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
	value_t value;
	AST_node * syntax_tree;
}
%type <syntax_tree> expr
%type <syntax_tree> var_assignment;
%type <syntax_tree> print_var;
%type <syntax_tree> declaration;


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


stmt : 
| stmt expr SEPARATOR {
	printf("EXPR"); 
	if(head == NULL){
		head = $2;
		last = head;
		last->next = NULL;
	} else {
		last->next = $2;
		last = $2;
	}
}
| stmt declaration SEPARATOR { 
	printf("DECLARATION\n");
	if(head == NULL){
		head = $2;
		last = head;
		last->next = NULL;
	} else {
		last->next = $2;
		last = last->next;
	}	
}
| stmt var_assignment SEPARATOR { 
	printf("Var assignment\n"); 
	if(head == NULL){
		head = $2;
		last = head;
		last->next = NULL;
	} else {
		last->next = $2;
		last = last->next;
	}
}
| stmt print_var SEPARATOR { 
	printf("Print Var");
	if(head == NULL){
		head = $2;
		last = head;
		last->next = NULL;
	} else {
		last->next = $2;
		last = last->next;
	}
}
| stmt '\n'
;

expr : 
expr AOP0 expr { 
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AOP0;
	node->data->op.aop0 = $2;
	$$ = node;
}
| expr AOP1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AOP1;
	node->data->op.aop1 = $2;
	$$ = node;
}
| AOP0 expr %prec USIGN { 
	AST_node * node = newASTNode(1, $2);
	node->data->token = AOP0;
	node->data->op.aop0 = $1;
	$$ = node;
}
| L_DEL_EXPR expr R_DEL_EXPR { $$ = $2; }
| FRACT {
	AST_node * node = newASTNode(0);
	node->data->token = FRACT;
	node->data->type = FRACT_T;
	node->data->value = malloc(sizeof(fract_t));
	*(fract_t*)(node->data->value) = $1;
	$$ = node;
	}
| expr BOP2 expr { 
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = BOP2;
	node->data->op.bop2 = $2;
	$$ = node;
}
| expr RELOP expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = RELOP;
	node->data->op.relop = $2;
	$$ = node;
}
| BOP1 expr %prec UBOP1{
	AST_node * node = newASTNode(1, $2);
	node->data->token = BOP1;
	node->data->op.bop1 = $1;
	$$ = node;
}
| BOOL	{ 
	AST_node * node = newASTNode(0);	
	node->data->token = BOOL;
	node->data->type = BOOL_T;
	node->data->value = malloc(sizeof(bool));
	*(bool*)(node->data->value) = $1;
	$$ = node;
}
| ID	{ 
	AST_node * node = newASTNode(0);	
	node->data->token = ID;
	node->data->value = strdup($1);
	
	$$ = node;
}
;

declaration : 
TYPE ID { 
	AST_node * id_node = newASTNode(0);
	id_node->data->token = ID;
	id_node->data->value = strdup($2);

	AST_node * node = newASTNode(1, id_node);
	node->data->token = TYPE;
	node->data->type = $1;
	$$ = node;
}	


var_assignment : 
ID ASSIGNMENT expr { 
	AST_node * id_node = newASTNode(0);
	id_node->data->token = ID;
	id_node->data->value = strdup($1);
	
	AST_node * node = newASTNode(2, id_node, $3);	
	node->data->token = ASSIGNMENT;
	$$ = node;
}

print_var : 
PRINT L_DEL_EXPR ID R_DEL_EXPR {
	AST_node * id_node = newASTNode(0);
	id_node->data->token = ID;
	id_node->data->value = strdup($3);
	
	AST_node * node = newASTNode(1);
	node->data->token = PRINT;
	node->children[0] = id_node;
	
	$$ = node;
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
	
	
	printf("\n--- The syntax Tree ---\n");
	printASTNode(head);
	type_checking(head);
	freeASTNode(head);
	return EXIT_SUCCESS;
}

void yyerror(char * s) {
	fprintf(stderr, "%s\n", s);
	exit(EXIT_FAILURE);
}
