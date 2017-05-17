/**
 * @file
 * @brief parser - processes and convert a token stream into an ast
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
%{
#include "ast.h"
#include "seq_tree.h"

#include "factype.h"
#include "factype_ast.h"
#include "facmath.h"
#include "facerr.h"
#include "symbol_table.h"
#include "type_checker.h"
#include "tac.h"
#include "test_tac.h"
#include "tac_printer.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
/********************************************
		GLOBAL SCOPE DECLARATIONS
*********************************************/
/** External vars **/
extern FILE * yyin;
/** Shared vars */
symbol_table_entry * symbol_table = NULL;
/** File-scoped vars **/
static
seq_node * head = NULL;
static
tac_node * tac_head = NULL;
static
void finalize();
/** BISON declarations **/
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

extern uint line_counter;

bool success = true;

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
	value_t value;
	seq_node * seq_tree;
	ast_node * syntax_tree;
}

/* Non-Terminal symbols */
%type <seq_tree> stmt;
%type <syntax_tree> expr
%type <syntax_tree> var_assignment;
%type <syntax_tree> print_var;
%type <syntax_tree> declaration;
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
%token <op> AOP_0	   	/* Arithmetic operation: + and - */
%token <op> AOP_1	   	/* Arithmetic operation */
%token <op> TYPE		/* Token for types: fract and bool */
%token <op> BOP1		/* Boolean operation with arity 1 */
%token <op> BOP2_0		/* Boolean operation with arity 2: "&&"*/
%token <op> BOP2_1		/* Boolean operation with arity 2: "||"*/
%token <op> BOP2_2		/* Boolean operation with arity 2: "->"*/
%token <op> BOP2_3		/* Boolean operation with arity 2: "<->","X"*/
%token <op> RELOP_0		/* Relation operation "<",">=",...: highest precedence*/
%token <op> RELOP_1		/* Relation operation "==","!=": lowest precedence*/
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
/* empty */ { $$ = NULL; }
| stmt expr SEPARATOR {
	$$=$1;
	fprintf(stderr, "Warning: statement expr with no effect\n");
}
| stmt declaration SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("DECLARATION\n");
}
| stmt var_assignment SEPARATOR {
	printf("Var assignment\n");
	$$=newSeqNode($1, $2);

}
| stmt print_var SEPARATOR {
	$$=newSeqNode($1, $2);
	printf("Print Var");
}
| stmt SKIP SEPARATOR {
	printf("Ignoring skip operation");
	$$ = $1;
}
| stmt ifrule {
	$$ = newSeqNode($1, $2);
}
| stmt whilerule {
	printf("Building the seq_node for WHILE\n");
	$$ = newSeqNode($1, $2);
}
;



whilerule:
WHILE L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE {
	$$ = astNode(ast_WHILE, @1.first_line, -1, NULL, 1, 1, $3, $6);
}
;


ifrule:
IF L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE ELSE L_DEL_SCOPE stmt R_DEL_SCOPE {
	$$ = astNode(ast_IF, @1.first_line, -1, NULL, 1, 2, $3, $6, $10);
}
|
IF L_DEL_EXPR expr R_DEL_EXPR L_DEL_SCOPE stmt R_DEL_SCOPE {
	$$ = astNode(ast_IF, @1.first_line, -1, NULL, 1, 1, $3, $6);
}
;

expr :
expr AOP_0 expr {
	$$ = astNode(ast_AOP, @2.first_line, $2, NULL , 2, 0,  $1, $3);
}
| expr AOP_1 expr {
	$$ = astNode(ast_AOP, @2.first_line, $2, NULL , 2, 0,  $1, $3);
}
| AOP_0 expr %prec USIGN {
	$$ = astNode(ast_AOP1, @1.first_line, $1, NULL,  1, 0, $2);
}
| L_DEL_EXPR expr R_DEL_EXPR { $$ = $2; }
| FRACT {
	fract_t * f = malloc(sizeof(fract_t));
	*f = $1;
	
	$$ = astNode(ast_FRACT, @1.first_line, -1, f, 0, 0);
}
| expr BOP2_0 expr {
	$$ = astNode(ast_BOP2, @2.first_line, $2, NULL, 2, 0, $1, $3);
}
| expr BOP2_1 expr {
	$$ = astNode(ast_BOP2, @2.first_line, $2, NULL,  2, 0, $1, $3);
}
| expr BOP2_2 expr {
	/* Applying desugaring of "->" operator:  
	 * "A imply B"  can be rewritten as "not A or B" */
	ast_node * notA = astNode(AST_BOP1, @2.first_line, NOT, NULL, 1, 0, $1);
	$$ = astNode(AST_BOP2, @2.first_line, OR, NULL,  2, 0, notA, $3);
}
| expr BOP2_3 expr {
	switch($2) {
		case IFF: /* De-Sugaring (!$1 || $3) && ($1 || !$3); */
		{
			ast_node * not1 	=	astNode(AST_BOP1, @2.first_line, NOT, NULL, 1, 0, $1);
			ast_node * not3 	=	astNode(AST_BOP1, @2.first_line, NOT, NULL, 1, 0, $3);
			ast_node * left 	=	astNode(AST_BOP2, @2.first_line, OR, NULL, 2, 0, not1, $3);
			ast_node * right 	=	astNode(AST_BOP2, @2.first_line, OR, NULL, 2, 0, $1, not3);
			$$ = astNode(AST_BOP2, @2.first_line, AND, NULL, 2, 0, left, right);
			break; 
		}
		case XOR: /* De-Sugaring ($1 || $3) && !($1 && $3); */
		{
			ast_node * subright =	astNode(AST_BOP2, @2.first_line, AND, NULL, 2, 0, $1, $3);
			ast_node * right	=	astNode(AST_BOP1, @2.first_line, NOT, NULL, 1, 0, subright);
			ast_node * left  	=	astNode(AST_BOP2, @2.first_line, OR, NULL, 2, 0, $1, $3); 
			$$ = astNode(AST_BOP2, @2.first_line AND, NULL, 2, 0, left, right);
			break;
		}
		default:
			yyerror("Operation not known. ");
	}
}
| expr RELOP_0 expr {
	$$ = astNode(ast_RELOP, @2.first_line, $2, NULL, 2, 0, $1, $3);
}
| expr RELOP_1 expr {
	$$ = astNode(ast_RELOP, @2.first_line, $2, NULL, 2, 0, $1, $3);
}
| BOP1 expr %prec UBOP1{
	$$ = astNode(ast_BOP1, @1.first_line, $1, NULL, 1, 0, $2);
}
| BOOL	{
	bool * b = malloc(sizeof(bool));
	*b = $1;
	$$ = astNode(ast_BOOL, @1.first_line, -1, b, 0, 0);
}
| ID	{
	char * identifier = strdup($1);
	$$ = astNode(ast_ID, @1.first_line, -1, identifier, 0, 0);
}
;

declaration :
TYPE ID {
	ast_node * id_node = astNode(ast_ID, @2.first_line, -1, strdup($2), 0, 0);
	$$ = astNode(ast_DECLARATION, @1.first_line, -1, NULL, 1, 0, id_node);
	$$->data->type = $1;
}


var_assignment :
ID ASSIGNMENT expr {
	ast_node * id_node = astNode(ast_ID, @1.first_line, -1, strdup($1), 0, 0);
	$$ = astNode(ast_ASSIGNMENT, @2.first_line, -1, NULL, 2, 0, id_node, $3);

}

print_var :
PRINT L_DEL_EXPR ID R_DEL_EXPR {
	ast_node * id_node = astNode(ast_ID, @3.first_line, -1, strdup($3), 0, 0);
	$$ = astNode(ast_PRINT, @1.first_line, -1, NULL, 1, 0, id_node);
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

	switch(yyparse()){
		case 1: yyerror("Input contains a synntax error\n"); return EXIT_FAILURE; break;
		case 2: yyerror("Memory exhausted\n"); return EXIT_FAILURE; break;
		case 0: /* successful */ break;
	}
	
	
	
	int err_code = fclose(yyin);
	if(err_code == EOF)
		err_handler(argv[1], FAC_STANDARD_ERROR);

	printf("\n--- The syntax Tree ---\n");
	if(!type_check(head)){
		fprintf(stderr, "Error, type checking failed. Exiting \n");
		return EXIT_FAILURE;
	}



	//tac(head);
	printSeqNode(head);

	tac(head);

	test_tac(&tac_head);

	print_tac(tac_head);

	/* generate code ??? */
	finalize();
	return EXIT_SUCCESS;
}

void yyerror(const char * err_msg, ...) {
	va_list ap;
	va_start(ap, err_msg);
	fprintf(stderr, "(Around line %d):\t", yylineno);
	vfprintf(stderr, err_msg, ap);
	fputc('\n', stderr);
	success = false;
}

void finalize(){
	freeSeqNode(head);
	free_tac(tac_head);
	freeTable();
}
