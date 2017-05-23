#include "type_checker.h"
#include "type_checker_ast.h"
#include "type_checker_ast_expr.h"
#include "symbol_table.h"
#include "factype.h"
#include "factype_ast.h"
#include <string.h>
#include <stdio.h>

extern void yyerror(char *, ...);

bool type_check_ast_node(ast_node * ast) {
	ast_node * node = ast;
	bool result = true;
	switch(node->data->token){
		case AST_DECLARATION:
			/* Perform a declaration */
			installID((char*) node->ast_children[0]->data->value, node->data->type);
			break;
		case AST_ASSIGNMENT:
		{ 	/* Perform an assignment */
			type_t expected_type = getType(node->ast_children[0]->data->value);
			result &= type_check_ast_expr(node->ast_children[1], expected_type);
			break;
		}
		case AST_PRINT:
		{ /* Check only if the ID is installed in the symbol table
			 It is equivalent to checking it the type is already defined*/
			getType(node->ast_children[0]->data->value);
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
			yyerror("%s token not recognized by type_checker on line %d", 
					tokenString(node->data->token), node->data->line);
		}
	}
	// DEBUG : %TODO - remove
	if(result == true)
		printf("Type checking successful!\n");
	return result;
}
