#include "type_checker_ast_expr.h"
#include "factype.h"
#include "factype_ast.h"
#include "symbol_table.h"
#include <stdio.h>


/**
 * Checks if node is an expression of type fract
 * @param node a node to check
 * @return true if node is of type fract, false otherwise.
 */
static bool type_check_fract(ast_node *);
/**
 * Checks if node is an expression of type bool
 * @param node a node to check
 * @return true if node is of type bool, false otherwise.
 */
static bool type_check_bool(ast_node *);

/** Internal struct to perform type inference */
typedef struct type_inference_struct {
	type_t type;
	bool success;
} type_inference_struct;

/**
 * Help function for type_check_bool used to implement overloading
 * of EQ and NEQ operator that can be used both for mathematical 
 * and boolean expression. 
 * 
 * Functionality: 
 * 1) it infers the types of lhs and rhs. 
 * 2) if both have the same type then it is well typed, otherwise it
 * is not.
 */
static type_inference_struct type_inference(ast_node * node);




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
			fprintf(stderr, "Line %d: Type not recognized by type_checker\n", ast->data->line);
			result = false;
		}
	}
	if(!result){
		fprintf(stderr, "Type Mismatch on line %d: Expected type %s!\n", ast->data->line, type==FRACT_T?"FRACT":"BOOL");
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
		case AST_FRACT:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == FRACT_T;
		case AST_AOP:
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
		case AST_BOOL:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == BOOL_T;
		case AST_BOP1:
			return type_check_bool(node->ast_children[0]);
		case AST_BOP2:
			return type_check_bool(node->ast_children[0]) &&
				type_check_bool(node->ast_children[1]);
		case ast_RELOP1: 
			{
				type_inference_struct tis = type_inference(node);
				return tis.success;
			}	
		case AST_RELOP:
			return type_check_fract(node->ast_children[0]) &&
				type_check_fract(node->ast_children[1]);
		default:
			return false;
	}
}





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
				tis.type = BOOL_T;
			}
			else {
				tis.type = false;
			}
			break;
		}
		case ast_RELOP:
			tis.success = type_check_fract(node->ast_children[0]) && type_check_fract(node->ast_children[1]);
			tis.type = BOOL_T;
			break;
		default: /* other ast_types are not expression */
			tis.success = false;
			break;
			
	}
	return tis;
}
