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
#include "type_checker.h"

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
	aop0_t aop0;
	aop1_t aop1;
	bop1_t bop1;
	bop2_0_t bop2_0;
	bop2_1_t bop2_1;
	bop2_2_t bop2_2;
	bop2_3_t bop2_3;
	relop0_t relop0;
	relop1_t relop1;
	value_t value;
	AST_node * syntax_tree;
}
/* Non-Terminal symbols */
%type <syntax_tree> expr
%type <syntax_tree> var_assignment;
%type <syntax_tree> print_var;
%type <syntax_tree> declaration;

/* Tokens */
%token <bval> BOOL		/* Token for true and false literals */
%token <fract> FRACT	/* Token for fract */
%token SEPARATOR		/* Separator of statement */
%token L_DEL_SCOPE		/* Scope left delimiter */
%token R_DEL_SCOPE		/* Scope right delimiter */
%token L_DEL_EXPR		/* Expression left delimiter */
%token R_DEL_EXPR		/* Expression right delimiter */
%token <aop0> AOP0	   	/* Arithmetic operation: + and - */
%token <aop1> AOP1	   	/* Arithmetic operation */
%token <type> TYPE		/* Token for types: fract and bool */
%token <bop1> BOP1		/* Boolean operation with arity 1 */
%token <bop2_0> BOP2_0	/* Boolean operation with arity 2: "&&"*/
%token <bop2_1> BOP2_1	/* Boolean operation with arity 2: "||"*/
%token <bop2_2> BOP2_2	/* Boolean operation with arity 2: "->"*/
%token <bop2_3> BOP2_3	/* Boolean operation with arity 2: "<->","X"*/
%token <relop0> RELOP0	/* Relation operation "<",">=",...: highest precedence*/
%token <relop1> RELOP1	/* Relation operation "==","!=": lowest precedence*/
%token WHILE			/* token for the while symbol */
%token IF				/* token for "if" symbol */
%token ELSE 			/* token for "else" symbol */
%token SKIP 			/* token for "skip" symbol */
%token PRINT 			/* token for "print" symbol */
%token ASSIGNMENT 		/* token for "=" symbol */
%token <id> ID 			/* token for variable identifier symbol */

/*
*	Associativity and Precedence rules (enforcing)
*	Precedence defined in ascending order
*/
%right ASSIGNMENT
%right BOP2_3
%right BOP2_2
%left BOP2_1
%left BOP2_0
%left RELOP1
%left RELOP0

%left AOP0
%left AOP1

%right BOP1
%right USIGN
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
| expr BOP2_0 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = BOP2_0;
	node->data->op.bop2_0 = $2;
	$$ = node;
}
| expr BOP2_1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = BOP2_1;
	node->data->op.bop2_1 = $2;
	$$ = node;
}
| expr BOP2_2 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = BOP2_2;
	node->data->op.bop2_2 = $2;
	$$ = node;
}
| expr BOP2_3 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = BOP2_3;
	node->data->op.bop2_3 = $2;
	$$ = node;
}
| expr RELOP0 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = RELOP0;
	node->data->op.relop0 = $2;
	$$ = node;
}
| expr RELOP1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = RELOP1;
	node->data->op.relop1 = $2;
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
