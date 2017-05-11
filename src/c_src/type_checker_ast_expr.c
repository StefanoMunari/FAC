#include "type_checker_ast_expr.h"
#include "factype.h"
#include "factype_ast.h"
#include "symbol_table.h"
#include <stdio.h>

extern void yyerror(char *);

static bool type_check_fract(AST_node *);
static bool type_check_bool(AST_node *);

bool type_check_ast_expr(AST_node * AST, type_t type){
	bool result;
	switch(type){
		case FRACT_T:
			result = type_check_fract(AST);
			break;
		case BOOL_T:
			result = type_check_bool(AST);
			break;
		default:
		{
			char* err_message="Type not recognized by type_checker";
			yyerror(err_message);
		}
	}
	if(!result){
		fprintf(stderr, "Type Mismatch in instruction + %s!\n", type==FRACT_T?"FRACT":"BOOL");
		printASTNode(AST);
	}
	return result;
}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
bool type_check_fract(AST_node * node){
	switch(node->data->token){
		case AST_FRACT:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == FRACT_T;
		case AST_AOP:
			if(node->number_of_AST_children == 1)
				return type_check_fract(node->AST_children[0]);
			else
				return type_check_fract(node->AST_children[0]) &&
						type_check_fract(node->AST_children[1]);
		default:
			return false;
	}
}

static
bool type_check_bool(AST_node * node){
	switch(node->data->token){
		case AST_BOOL:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == BOOL_T;
		case AST_BOP1:
			return type_check_bool(node->AST_children[0]);
		case AST_BOP2:
			return type_check_bool(node->AST_children[0]) &&
				type_check_bool(node->AST_children[1]);
		case AST_RELOP:
			return type_check_fract(node->AST_children[0]) &&
				type_check_fract(node->AST_children[1]);
		default:
			return false;
	}
}
