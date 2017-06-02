/**
 * @file
 * @brief parser - processes and convert a token stream into an ast
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
%{
#include "types/factype.h"
#include "types/factype_ast.h"

#include "symbol_table/symbol_table.h"

#include "ast/ast.h"
#include "ast/seq_tree.h"

#include "type_check/type_checker.h"

#include "tac/tac.h"
#include "tac/tac_list.h"
#include "tac/printer/tac_printer.h"

#include "utils/facmath.h"
#include "err/facerr.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
/********************************************
		GLOBAL SCOPE DECLARATIONS
*********************************************/
/* NULL OPerator macro */
#ifndef NULL_OP
#define NULL_OP -1
#endif
/** External vars **/
extern FILE * yyin;
/** Shared vars */
symbol_table_entry * symbol_table = NULL;
/*********************
	File-scoped vars
**********************/
static
seq_node * head = NULL;
static
tac_list * tlist = NULL;
static
void finalize();
/**********************
	BISON declarations
***********************/
/**
* Lexical scanner
*
* @brief recognize tokens from the input stream and returns them to the Parser
*/
int yylex ();



seq_node * head = NULL;

/**
 * Error handler of BISON
 * @brief handle errors detected by the parser
 * Exit the program with failure status code
 * @param error message
 */
void yyerror(const char *, ...);
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
	op_t op;
	seq_node * seq_tree;
	ast_node * syntax_tree;
}

/* Non-Terminal symbols */
%type <seq_tree> stmt;
%type <syntax_tree> declaration;
%type <syntax_tree> expr
%type <syntax_tree> var_assignment;
%type <syntax_tree> print_var;
%type <syntax_tree> ifrule;
%type <syntax_tree> whilerule;

/* Tokens */
%token <bval> BOOL		/* Token for true and false literals */
%token <fract> FRACT	/* Token for fract */
%token SEPARATOR		/* Separator of statement */
%token L_DEL_SCOPE		/* Scope left delimiter */
%token R_DEL_SCOPE		/* Scope right delimiter */
%token L_DEL_EXPR		/* Expression left delimiter */
%token R_DEL_EXPR		/* Expression right delimiter */
%token <op> AOP_1	   	/* Arithmetic operation: + and - */
%token <op> AOP_0	   	/* Arithmetic operation: * and / */
%token <op> TYPE		/* Token for types: fract and bool */
%token <op> BOP1		/* Boolean operation with arity 1 */
%token <op> BOP2_0		/* Boolean operation with arity 2
							(highest precedence): "&&"*/
%token <op> BOP2_1		/* Boolean operation with arity 2: "||"*/
%token <op> BOP2_2		/* Boolean operation with arity 2: "->"*/
%token <op> BOP2_3		/* Boolean operation with arity 2
							(lowest precedence): "<->","XOR"*/
%token <op> RELOP_0		/* Relation operation "<",">=",...: highest precedence*/
%token <op> RELOP_1		/* Relation operation "==","!=": lowest precedence*/
%token WHILE			/* token for the while symbol */
%token IF				/* token for "if" symbol */
%token ELSE 			/* token for "else" symbol */
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

%left AOP_1
%left AOP_0

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
/* empty */ { $$ = NULL; }
| stmt declaration SEPARATOR {

	seq_node * subtree = newSeqNode($1, $2->ast_children[0]);
	$$ = newSeqNode(subtree, $2->ast_children[1]);
	free($2);
}
| stmt var_assignment SEPARATOR {
	$$=newSeqNode($1, $2);

}
| stmt print_var SEPARATOR {
	$$=newSeqNode($1, $2);
}
| stmt ifrule {
	$$ = newSeqNode($1, $2);
}
| stmt whilerule {
	$$ = newSeqNode($1, $2);
}
;

whilerule:
WHILE L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE {
	if($6 == NULL){
		yyerror("Lines %d-%d: The while body cannot be empty  \n",
			@5.first_line, @7.first_line);
	}
	op_t operator = NULL_OP;
	$$ = astNode(AST_WHILE, @1.first_line, operator, NULL, 1, 1, $3, $6);
}
;


ifrule:
IF L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE ELSE L_DEL_SCOPE
stmt R_DEL_SCOPE {
	if($6 == NULL){
		yyerror("Lines %d-%d: The if body cannot be empty  \n", @5.first_line,
			@7.first_line);
	}
	if($10 == NULL){
		yyerror("Lines %d-%d: The else body cannot be empty  \n", @9.first_line,
			@11.first_line);
	}
	op_t operator = NULL_OP;
	$$ = astNode(AST_IF, @1.first_line, operator, NULL, 1, 2, $3, $6, $10);
}
|
IF L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE {
	if($6 == NULL){
		yyerror("Lines %d-%d: The if body cannot be empty  \n", @5.first_line,
			@7.first_line);
	}
	op_t operator = NULL_OP;
	$$ = astNode(AST_IF, @1.first_line, operator, NULL, 1, 1, $3, $6);
}
;

expr :
expr AOP_1 expr {
	op_t operator = $2;
	$$ = astNode(AST_AOP2, @2.first_line, operator, NULL , 2, 0,  $1, $3);
}
| expr AOP_0 expr {
	op_t operator = $2;
	$$ = astNode(AST_AOP2, @2.first_line, operator, NULL , 2, 0,  $1, $3);
}
| AOP_1 expr %prec USIGN {
	op_t operator = ($1==SUM?PLUS:MINUS);
	$$ = astNode(AST_AOP1, @1.first_line, operator, NULL,  1, 0, $2);
}
| L_DEL_EXPR expr R_DEL_EXPR {
	$$ = $2;
}
| FRACT {
	fract_t * f = malloc(sizeof(fract_t));
	*f = $1;
	op_t operator = NULL_OP;
	$$ = astNode(AST_FRACT, @1.first_line, operator, f, 0, 0);
}
| expr BOP2_0 expr {
	op_t operator = $2;
	$$ = astNode(AST_BOP2, @2.first_line, operator, NULL, 2, 0, $1, $3);
}
| expr BOP2_1 expr {
	op_t operator = $2;
	$$ = astNode(AST_BOP2, @2.first_line, operator, NULL,  2, 0, $1, $3);
}
| expr BOP2_2 expr {
	/* Applying desugaring of "->" operator:
	   "A imply B"  can be rewritten as "not A or B" */
	ast_node * notA = astNode(AST_BOP1, @2.first_line, NOT, NULL, 1, 0, $1);
	$$ = astNode(AST_BOP2, @2.first_line, OR, NULL,  2, 0, notA, $3);
}
| expr BOP2_3 expr {
	op_t operator = $2;
	$$ = astNode(AST_BOP_RELOP, @2.first_line, operator, NULL, 2, 0, $1, $3);
}
| expr RELOP_0 expr {
	op_t operator = $2;
	$$ = astNode(AST_RELOP, @2.first_line, operator, NULL, 2, 0, $1, $3);
}
| expr RELOP_1 expr {
	op_t operator = $2;
	$$ = astNode(AST_RELOP, @2.first_line, operator, NULL, 2, 0, $1, $3);
}
| BOP1 expr %prec UBOP1{
	op_t operator = $1;
	$$ = astNode(AST_BOP1, @1.first_line, operator, NULL, 1, 0, $2);
}
| BOOL	{
	bool * b = malloc(sizeof(bool));
	*b = $1;
	op_t operator = NULL_OP;
	$$ = astNode(AST_BOOL, @1.first_line, operator, b, 0, 0);
}
| ID	{
	char * identifier = strdup($1);
	op_t operator = NULL_OP;
	$$ = astNode(AST_ID, @1.first_line, operator, identifier, 0, 0);
}
;

declaration :
TYPE ID ASSIGNMENT expr {
	/* Desugare this stmt in a declaration and an assignment */
	op_t operator = NULL_OP;
	/* Construct the node for the declaration */
	ast_node * id_node =
		astNode(AST_ID, @2.first_line, operator, strdup($2), 0, 0);
	ast_node * declaration =
		astNode(AST_DECLARATION, @1.first_line, operator, NULL,
		1, 0, id_node);
	declaration->data->type = $1;

	/* Construct the node for the assignment */
	ast_node * id_node_assignment =
		astNode(AST_ID, @2.first_line, operator, strdup($2), 0, 0);
	ast_node * assignment =
		astNode(AST_ASSIGNMENT, @2.first_line, operator, NULL,
		2, 0, id_node_assignment, $4);

	/* Encode the information in a single ast node */
	$$ = astNode(AST_DECLARATION, @2.first_line, operator, NULL, 2, 0,
		declaration, assignment);
}
;

var_assignment :
ID ASSIGNMENT expr {
	op_t operator = NULL_OP;
	ast_node * id_node =
		astNode(AST_ID, @1.first_line, operator, strdup($1), 0, 0);
	$$ = astNode(AST_ASSIGNMENT, @2.first_line, operator, NULL, 2, 0, id_node,
		$3);
}

print_var :
PRINT L_DEL_EXPR ID R_DEL_EXPR {
	op_t operator = NULL_OP;
	ast_node * id_node =
		astNode(AST_ID, @3.first_line, operator, strdup($3), 0, 0);
	$$ = astNode(AST_PRINT, @1.first_line, operator, NULL, 1, 0, id_node);
}

%%
/* Entrypoint of the program */
int main(int argc, char * argv[]) {
	tprinter printer;
	FILE * fp = NULL;

	if(argc < 3){
		char * err_msg =
			"Usage: %s <file-to-compile> <printer>\n Arguments: \n\t \
			<printer> \t IR - Intermediate Representation\n \t\t\t\t C - \
			C representation\n";
		fprintf(stderr, err_msg, argv[0]);
		return EXIT_FAILURE;
	}

	if (strcmp(argv[2], "C") == 0)
		printer = (tprinter) { C };
	else if (strcmp(argv[2], "IR") == 0)
		printer = (tprinter) { IR };
	else{
	  yyerror("main: invalid printer");
	  return EXIT_FAILURE;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		yyerror("parser.y::main: could not open file %s", fp);
	}

	yyin = fp;

	switch(yyparse()){
		case 1:
			yyerror("Input contains a syntax error\n");
			return EXIT_FAILURE;
			break;
		case 2:
			yyerror("Memory exhausted\n");
			return EXIT_FAILURE;
			break;
		case 0: /* successful */ break;
	}

	int err_code = fclose(yyin);
	if(err_code == EOF)
		err_handler(argv[1], FAC_STANDARD_ERROR);

	if(!type_check(head)){
		fprintf(stderr, "Error, type checking failed. Exiting \n");
		return EXIT_FAILURE;
	}


	tlist=generate_tac(head);

	
	tdynamic_dispatch(&printer, tlist);
	finalize();
	return EXIT_SUCCESS;
}

void yyerror(const char * err_msg, ...) {
	va_list ap;
	va_start(ap, err_msg);
	fprintf(stderr, "(Around line %d):\t", yylineno);
	vfprintf(stderr, err_msg, ap);
	fputc('\n', stderr);
	finalize();
	exit(EXIT_FAILURE);
}

void finalize(){
	printf("Clean up..\n");
	freeSeqNode(head);
	freeTable();
	free_tac(tlist);
}
