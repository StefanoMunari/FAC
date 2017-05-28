#include "type_checker_ast_expr.h"
#include "../types/factype.h"
#include "../types/factype_ast.h"
#include "../symbol_table/symbol_table.h"
#include <stdio.h>

extern char yyerror(char *, ...);

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
		case AST_AOP1:
			return type_check_fract(node->ast_children[0]);
		case AST_AOP2:
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
		case AST_BOP2_TO_CHANGE:
		{
			bool ret = type_check_bool(node->ast_children[0]) &&
					   type_check_bool(node->ast_children[1]);
			if(!ret){
				yyerror("Line %d: operator %s can be used only on bool expression", 
					node->data->line,
					node->data->op==IFF?"<->":"XOR");
			}
			
			
			//	type_check_bool(node->ast_children[1]);
			switch(node->data->op){ 
				/* After type-checking change the operation of the node:
				 * a IFF b corresponds to check if a equals b
				 * a XOR b corresponds to check if a is different from b
				 */
				case IFF: node->data->token = AST_RELOP; node->data->op = EQ; break;
				case XOR: node->data->token = AST_RELOP; node->data->op = NEQ; break;
				default: yyerror("Wrong operation at line %d", node->data->line); return false;
			}
			return ret;
		}
		case AST_RELOP:
		{
			bool ret = type_check_fract(node->ast_children[0]) &&
				type_check_fract(node->ast_children[1]);
			
			if(!ret){
				switch(node->data->op){ 
					case EQ:
					case NEQ:
						yyerror("Line %d: %s can only be used on fract variables. If lhs and rhs are bool use %s.\n", 
							node->data->line, 
							node->data->op==EQ?"==":"<>",
							node->data->op==EQ?"<->":"XOR"
							);
						 break; 
					default:yyerror("Line %d: relop can be used only on fract variables.", 
					node->data->line);
				
				}
			}
				
			return ret;
		}
		default:
			return false;
	}
}


