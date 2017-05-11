#include "type_checker_ast.h"
#include "type_checker_ast_expr.h"
#include "symbol_table.h"
#include "factype.h"
#include "factype_ast.h"
#include <string.h>
#include <stdio.h>

extern void yyerror(char *);

bool type_check_AST_node(AST_node * AST) {
	AST_node * node = AST;
	bool result = true;
	switch(node->data->token){
		case AST_DECLARATION:
			/* Perform a declaration */
			installID((char*) node->AST_children[0]->data->value, node->data->type);
			break;
		case AST_ASSIGNMENT:
		{ 	/* Perform an assignment */
			type_t expected_type = getType(node->AST_children[0]->data->value);
			result &= type_check_ast_expr(node->AST_children[1], expected_type);
			break;
		}
		case AST_PRINT:
		{ /* Check only if the ID is installed in the symbol table
			 It is equivalent to checking it the type is already defined*/
			getType(node->AST_children[0]->data->value);
			break;
		}
		case AST_IF:
		{
			result &= type_check_ast_expr(node->AST_children[0], BOOL_T);
			result &= type_check(node->SEQ_children[0]);
			result &= type_check(node->SEQ_children[1]);
			break;
		}
		case AST_WHILE:
		{
			result &= type_check_ast_expr(node->AST_children[0], BOOL_T);
			//TODO test statemeent
			break;
		}
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
