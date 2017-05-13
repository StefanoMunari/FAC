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

typedef struct _type_inference_struct {
	type_t type;
	bool success;
} type_inference_struct;

static 
type_inference_struct type_inference(ast_node * node){
	type_inference_struct tis;
	switch(node->data->token){
		case ast_BOOL:
			tis.type = BOOL_T;
			tis.success = true;
			break;
		case ast_FRACT:
			tis.type = FRACT_T;
			tis.success = true;
			break;
		case ast_ID:
			tis.type = getType((char*) node->data->value);
			tis.success = true;
			break;
		case ast_AOP:
			tis.type = FRACT_T;
			tis.success = type_check_fract(node->ast_children[0]) &&
				type_check_fract(node->ast_children[1]);
			break;
		case ast_BOP1:
		case ast_BOP2:
			tis.type = BOOL_T;
			tis.success = type_check_ast_expr(node, BOOL_T);
			break;
		case ast_RELOP1:
		{
			type_inference_struct tis1 = type_inference(node->ast_children[0]);
			type_inference_struct tis2 = type_inference(node->ast_children[1]);
			if(tis1.type == tis2.type && tis1.success && tis2.success){
				tis.success = true;
				tis.type = tis2.type;
				
			}
			else {
				tis.type = false;
			}
			break;
		}
		case ast_RELOP:
			tis.success = type_check_fract(node->ast_children[0]) && type_check_fract(node->ast_children[1]);
			break;
			
	}
	return tis;
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
		case ast_RELOP1: /* Perform type inference to know which type of equality do we need */
			{
				type_inference_struct tis = type_inference(node);
				return tis.success;
			}	
		case ast_RELOP:
			return type_check_fract(node->ast_children[0]) &&
				type_check_fract(node->ast_children[1]);
		default:
			return false;
	}
}
