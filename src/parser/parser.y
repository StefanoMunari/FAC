/**
 * @file
 * @brief parser - processes and convert a token stream into an AST
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "AST.h"
#include "seq_tree.h"

#include "factype.h"
#include "factype_ast.h"
#include "facmath.h"
#include "facerr.h"
#include "symbol_table.h"
//#include "type_checker.h"
/********************************************
		GLOBAL SCOPE DECLARATIONS
*********************************************/
/** External vars **/
extern FILE * yyin;

/** Declaring the symbol table */
entry * symbol_table = NULL;

/** File-scoped vars **/
static seq_node * head = NULL;
/** BISON declarations **/
/**
* Lexical scanner
*
* @brief recognize tokens from the input stream and returns them to the Parser
*/
int yylex ();
/**
 * Error handler of BISON
 * @brief handle errors detected by the parser
 * Exit the program with failure status code
 * @param error message
 */
void yyerror(const char *);
%}
/********************************************
		TRANSLATION RULES
*********************************************/
/* Types of grammar symbols */
%union {
	char * id;
	bool bval;
	fract_t fract;
	type_t type;
	aop_0_t aop_0;
	aop_1_t aop_1;
	bop1_t bop1;
	bop2_0_t bop2_0;
	bop2_1_t bop2_1;
	bop2_2_t bop2_2;
	bop2_3_t bop2_3;
	relop_0_t relop_0;
	relop_1_t relop_1;
	value_t value;
	seq_node * seq_tree;
	AST_node * syntax_tree;
}

/* Non-Terminal symbols */
%type <seq_tree> stmt;
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
%token <aop_0> AOP_0	   	/* Arithmetic operation: + and - */
%token <aop_1> AOP_1	   	/* Arithmetic operation */
%token <type> TYPE		/* Token for types: fract and bool */
%token <bop1> BOP1		/* Boolean operation with arity 1 */
%token <bop2_0> BOP2_0	/* Boolean operation with arity 2: "&&"*/
%token <bop2_1> BOP2_1	/* Boolean operation with arity 2: "||"*/
%token <bop2_2> BOP2_2	/* Boolean operation with arity 2: "->"*/
%token <bop2_3> BOP2_3	/* Boolean operation with arity 2: "<->","X"*/
%token <relop_0> RELOP_0	/* Relation operation "<",">=",...: highest precedence*/
%token <relop_1> RELOP_1	/* Relation operation "==","!=": lowest precedence*/
%token WHILE			/* token for the while symbol */
%token IF				/* token for "if" symbol */
%token ELSE 			/* token for "else" symbol */
%token SKIP 			/* token for "skip" symbol */
%token PRINT 			/* token for "print" symbol */
%token ASSIGNMENT 		/* token for "=" symbol */
%token <id> ID 			/* token for variable identifier symbol */

/*
* Associativity and Precedence rules (enforcing)
* Precedence defined in ascending order
*/
%right ASSIGNMENT
%right BOP2_3
%right BOP2_2
%left BOP2_1
%left BOP2_0
%left RELOP_1
%left RELOP_0

%left AOP_0
%left AOP_1

%right BOP1
%right USIGN
%right UBOP1
%%
/*
* Context-Free-Grammar productions
* and the related semantic actions
*/

program :
stmt { head=$1; }

stmt :
| stmt expr SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("EXPR");
}
| stmt declaration SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("DECLARATION\n");
}
| stmt var_assignment SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("Var assignment\n");
}
| stmt print_var SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("Print Var");
}
| stmt '\n'
;

expr :
expr AOP_0 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_AOP;
	node->data->op.aop = aop(AOP_0, $2);
	$$ = node;
}
| expr AOP_1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_AOP;
	node->data->op.aop = aop(AOP_1, $2);
	$$ = node;
}
| AOP_0 expr %prec USIGN {
	AST_node * node = newASTNode(1, $2);
	node->data->token = AST_AOP;
	node->data->op.aop = aop(AOP_0, $1);
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
	node->data->token = AST_BOP2;
	node->data->op.bop2 = bop2(BOP2_0, $2);
	$$ = node;
}
| expr BOP2_1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_BOP2;
	node->data->op.bop2 = bop2(BOP2_1, $2);
	$$ = node;
}
| expr BOP2_2 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_BOP2;
	node->data->op.bop2 = bop2(BOP2_2, $2);
	$$ = node;
}
| expr BOP2_3 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_BOP2;
	node->data->op.bop2 = bop2(BOP2_3, $2);
	$$ = node;
}
| expr RELOP_0 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_RELOP;
	node->data->op.relop = relop(RELOP_0, $2);
	$$ = node;
}
| expr RELOP_1 expr {
	AST_node * node = newASTNode(2, $1, $3);
	node->data->token = AST_RELOP;
	node->data->op.relop = relop(RELOP_1, $2);
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
/* Entrypoint of the program */
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
	//printASTNode(head);
	type_checking(head);
	
	/* generate code ??? */
	freeSeqNode(head);
	//freeASTNode(head);
	return EXIT_SUCCESS;
}

void yyerror(const char * err_msg) {
	fprintf(stderr, "%s\n", err_msg);
	exit(EXIT_FAILURE);
}
