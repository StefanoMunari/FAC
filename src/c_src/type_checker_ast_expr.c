#include "type_checker_ast_expr.h"
#include "factype.h"
#include "factype_ast.h"
#include "symbol_table.h"
#include <stdio.h>

extern void yyerror(char *);

static bool type_check_fract(ast_node *);
static bool type_check_bool(ast_node *);

bool type_check_ast_expr(ast_node * ast, type_t type){
	bool result;
	switch(type){
		case FRACT_T:
			result = type_check_fract(ast);
			break;
		case BOOL_T:
			result = type_check_bool(ast);
			break;
		default:
		{
			char* err_message="Type not recognized by type_checker";
			yyerror(err_message);
		}
	}
	if(!result){
		fprintf(stderr, "Type Mismatch in instruction + %s!\n", type==FRACT_T?"FRACT":"BOOL");
		printastNode(ast);
	}
	return result;
}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
bool type_check_fract(ast_node * node){
	switch(node->data->token){
		case ast_FRACT:
			return true;
		case ast_ID:
			return getType((char*) node->data->value) == FRACT_T;
		case ast_AOP:
			if(node->number_of_ast_children == 1)
				return type_check_fract(node->ast_children[0]);
			else
				return type_check_fract(node->ast_children[0]) &&
						type_check_fract(node->ast_children[1]);
		default:
			return false;
	}
}

static
bool type_check_bool(ast_node * node){
	switch(node->data->token){
		case ast_BOOL:
			return true;
		case ast_ID:
			return getType((char*) node->data->value) == BOOL_T;
		case ast_BOP1:
			return type_check_bool(node->ast_children[0]);
		case ast_BOP2:
			return type_check_bool(node->ast_children[0]) &&
				type_check_bool(node->ast_children[1]);
		case ast_RELOP:
			return type_check_fract(node->ast_children[0]) &&
				type_check_fract(node->ast_children[1]);
		default:
			return false;
	}
}
