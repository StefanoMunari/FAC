#include "type_checker_ast_expr.h"
#include "../types/factype_ast.h"
#include "../symbol_table/symbol_table.h"

extern char yyerror(char *, ...);

/**
 * Checks if node is an expression of type fract
 * @param node a node to check
 * @return true if node is of type fract, false otherwise.
 */
static bool _type_check_fract(ast_node *);
/**
 * Checks if node is an expression of type bool
 * @param node a node to check
 * @return true if node is of type bool, false otherwise.
 */
static bool _type_check_bool(ast_node *);

bool type_check_ast_expr(ast_node * ast, type_t type){
	bool result;
	switch(type){
		case FRACT_T:
			result = _type_check_fract(ast);
			break;
		case BOOL_T:
			result = _type_check_bool(ast);
			break;
		default:
			yyerror("Line %d: Type not recognized by type_checker\n",
					ast->data->line
			);
	}
	if(!result){
		print_ast_node(ast);
		yyerror("Type Mismatch on line %d: Expected type %s!\n",
				ast->data->line, type==FRACT_T?"FRACT":"BOOL"
		);
	}
	return result;
}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
bool _type_check_fract(ast_node * node){
	switch(node->data->token){
		case AST_FRACT:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == FRACT_T;
		case AST_AOP1:
			return _type_check_fract(node->ast_children[0]);
		case AST_AOP2:
			return _type_check_fract(node->ast_children[0]) &&
					_type_check_fract(node->ast_children[1]);
		default:
			return false;
	}
}

static
bool _type_check_bool(ast_node * node){
	switch(node->data->token){
		case AST_BOOL:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == BOOL_T;
		case AST_BOP1:
			return _type_check_bool(node->ast_children[0]);
		case AST_BOP2:
			return _type_check_bool(node->ast_children[0]) &&
				_type_check_bool(node->ast_children[1]);
		case AST_BOP_RELOP:
		{
			bool ret = _type_check_bool(node->ast_children[0]) &&
					   _type_check_bool(node->ast_children[1]);
			if(!ret)
				yyerror("type_checker_ast_expr::_type_check_bool:Line %d: \
					operator %s can be used only for boolean expressions",
					node->data->line,
					node->data->op==IFF?"<->":"XOR"
				);
			return ret;
		}
		case AST_RELOP:
		{
			bool ret = _type_check_fract(node->ast_children[0]) &&
				_type_check_fract(node->ast_children[1]);
			if(!ret){
				switch(node->data->op){
					case EQ:
					case NEQ:
						yyerror("type_checker_ast_expr::_type_check_bool:\
							Line %d: %s can only be used for fract variables.\
							If lhs and rhs are bool use %s.\n",
							node->data->line,
							node->data->op==EQ?"==":"<>",
							node->data->op==EQ?"<->":"XOR"
						);
						break;
					default:
						yyerror("type_checker_ast_expr::_type_check_bool: \
							Line %d: relop can be used only for fract \
							variables.",
							node->data->line
						);

				}
			}
			return ret;
		}
		default:
			return false;
	}
}


