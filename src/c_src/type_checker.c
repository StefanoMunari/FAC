#include "factype.h"
#include "factype_ast.h"
#include "type_checker.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include <stdio.h>

extern entry * symbol_table;
extern void yyerror(char *);

static bool recursive_type_check_fract(AST_node * node);
static bool recursive_type_check_bool(AST_node * node);
static bool recursive_type_check(AST_node * AST, type_t type);
static bool type_check_AST_node(AST_node * AST);

bool type_check(seq_node * seqTree){
	if(seqTree == NULL)
		return true;
	bool result = type_check(seqTree->left);
	result &= type_check_AST_node(seqTree->right);
	return result;
}
/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
bool recursive_type_check_fract(AST_node * node){
	switch(node->data->token){
		case AST_FRACT:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == FRACT_T;
		case AST_AOP:
			if(node->number_of_children == 1)
				return recursive_type_check_fract(node->children[0]);
			else
				return recursive_type_check_fract(node->children[0]) &&
						recursive_type_check_fract(node->children[1]);
		default:
			return false;
	}
}

static
bool recursive_type_check_bool(AST_node * node){
	switch(node->data->token){
		case AST_BOOL:
			return true;
		case AST_ID:
			return getType((char*) node->data->value) == BOOL_T;
		case AST_BOP1:
			return recursive_type_check_bool(node->children[0]);
		case AST_BOP2:
			return recursive_type_check_bool(node->children[0]) &&
				recursive_type_check_bool(node->children[1]);
		case AST_RELOP:
			return recursive_type_check_fract(node->children[0]) &&
				recursive_type_check_fract(node->children[1]);
		default:
			return false;

	}
}
/**
 * Check if the given expr has the right type
 */
static
bool recursive_type_check(AST_node * AST, type_t type){
	bool result;
	switch(type){
		case FRACT_T:
			result = recursive_type_check_fract(AST);
			break;
		case BOOL_T:
			result = recursive_type_check_bool(AST);
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

static
bool type_check_AST_node(AST_node * AST) {
	AST_node * node = AST;
	bool result = true;
	switch(node->data->token){
		case AST_DECLARATION:
			/* Perform a declaration */
			installID((char*) node->children[0]->data->value, node->data->type);
			break;
		case AST_ASSIGNMENT:
		{ 	/* Perform an assignment */
			type_t expected_type = getType(node->children[0]->data->value);
			result &= recursive_type_check(node->children[1], expected_type);
			break;
		}
		case AST_PRINT:
		{ /* Check only if the ID is installed in the symbol table
			 It is equivalent to checking it the type is already defined*/
			getType(node->children[0]->data->value);
			break;
		}
		default:
		{
			char* err_message="%d token not recognized by type_checker";
			int size_token= sizeof(node->data->token);
			int size_message= (int)strlen(err_message);
			char err_buffer[size_message+size_token];
			sprintf(err_buffer, err_message, node->data->token);
			yyerror(err_buffer);
		}
	}
	// DEBUG : %TODO - remove
	if(result == true)
		printf("Type checking successful!\n");
	return result;
}
