#include "type_checker.h"
#include "type_checker_ast_expr.h"
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../types/factype_ast.h"

extern void yyerror(char *, ...);

bool type_check_ast_node(ast_node * ast) {
	ast_node * node = ast;
	bool result = true;
	switch(node->data->token){
		case AST_DECLARATION:
			/* Perform a declaration */
			install_ID((char*) node->ast_children[0]->data->value,
				node->data->type
			);
			break;
		case AST_ASSIGNMENT:
		{ 	/* Perform an assignment */
			type_t expected_type = get_type(node->ast_children[0]->data->value);
			result &= type_check_ast_expr(node->ast_children[1], expected_type);
			break;
		}
		case AST_PRINT:
		{ /* Check only if the ID is installed in the symbol table
			 It is equivalent to checking it the type is already defined*/
			get_type(node->ast_children[0]->data->value);
			break;
		}
		case AST_IF:
		{
			result &= type_check_ast_expr(node->ast_children[0], BOOL_T);
			{
				int i = 0;
				for(i = 0; i < node->number_of_seq_children; ++i){
					result &= type_check(node->seq_children[i]);
				}
			}
			break;
		}
		case AST_WHILE:
		{
			result &= type_check_ast_expr(node->ast_children[0], BOOL_T);
			result &= type_check(node->seq_children[0]);
			break;
		}
		default:
		{
			yyerror("tyype_checker_ast::type_check_ast_node \
					%s token not recognized by type_checker on line %d",
					token_string(node->data->token), node->data->line
			);
		}
	}
	return result;
}
