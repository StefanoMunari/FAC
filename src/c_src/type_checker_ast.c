#include "type_checker_ast.h"
#include "type_checker_ast_expr.h"
#include "symbol_table.h"
#include "factype.h"
#include "factype_ast.h"
#include <string.h>
#include <stdio.h>

extern void yyerror(char *);

bool type_check_ast_node(ast_node * ast) {
	ast_node * node = ast;
	bool result = true;
	switch(node->data->token){
		case ast_DECLARATION:
			/* Perform a declaration */
			installID((char*) node->ast_children[0]->data->value, node->data->type);
			break;
		case ast_ASSIGNMENT:
		{ 	/* Perform an assignment */
			type_t expected_type = getType(node->ast_children[0]->data->value);
			result &= type_check_ast_expr(node->ast_children[1], expected_type);
			break;
		}
		case ast_PRINT:
		{ /* Check only if the ID is installed in the symbol table
			 It is equivalent to checking it the type is already defined*/
			getType(node->ast_children[0]->data->value);
			break;
		}
		case ast_IF:
		{
			result &= type_check_ast_expr(node->ast_children[0], BOOL_T);
			{
				int i = 0;
				for(i = 0; i < node->number_of_SEQ_children; ++i){
					result &= type_check(node->SEQ_children[i]);
				}
			}
			break;
		}
		case ast_WHILE:
		{
			result &= type_check_ast_expr(node->ast_children[0], BOOL_T);
			result &= type_check(node->SEQ_children[0], BOOL_T);
			break;
		}
		case ast_SKIP:
			result = true;
			break;
		default:
		{
			char* err_message="%s token not recognized by type_checker";
			int size_token= sizeof(node->data->token);
			int size_message= (int)strlen(err_message);
			char err_buffer[size_message+size_token+20];
			sprintf(err_buffer, err_message, tokenString(node->data->token));
			yyerror(err_buffer);
		}
	}
	// DEBUG : %TODO - remove
	if(result == true)
		printf("Type checking successful!\n");
	return result;
}
