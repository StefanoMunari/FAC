#include "ast.h"
#include "factype_ast.h"
#include "seq_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

char * tokenString(ast_category token);

ast_node * astNode(unsigned int token, int line, op_t op, void * value, 
	const int number_of_ast_children, const int number_of_SEQ_children, ...) {


	assert(number_of_ast_children >= 0 && number_of_SEQ_children >= 0);
	
	ast_node * node = calloc(1, sizeof(ast_node));

	

	node->number_of_ast_children = number_of_ast_children;
	node->number_of_SEQ_children = number_of_SEQ_children;

	node->ast_children = (ast_node**)calloc(number_of_ast_children, sizeof(ast_node*));
	node->SEQ_children = (seq_node**)calloc(number_of_SEQ_children, sizeof(seq_node*));

	va_list args_iterator;
	va_start(args_iterator, number_of_SEQ_children);
	{
		int i;
		for(i = 0; i < number_of_ast_children; ++i) {

			node->ast_children[i] = va_arg(args_iterator, ast_node*);

		}
		for(i = 0; i < number_of_SEQ_children; ++i){
			node->SEQ_children[i] = va_arg(args_iterator, seq_node*);
		}
	}
	va_end(args_iterator);


	if(node->number_of_ast_children == 0){
		node->ast_children = NULL;
	}
	if(node->number_of_SEQ_children == 0){
		node->SEQ_children = NULL;
	}
	
	
	node->data = (record *) astRecord(token, line, op, value);


	return node;
}




void freeastNode(ast_node * node){
	if(node == NULL)
		return;
	{
		int i;
		/* Free children */
		for(i = 0; i < node->number_of_ast_children; ++i) {
			freeastNode(node->ast_children[i]);
		}

		for(i = 0; i < node->number_of_SEQ_children; ++i) {
			freeSeqNode(node->SEQ_children[i]);
		}
	}
	/* Free resources */
	freeastRecord(node->data);
	free(node);
}

int printastNodeRec(ast_node * node, int instruction, int tab){
	if(node == NULL)
		return instruction;
	int i;
	for(i = 0; i < tab; i++){
		putchar('\t');
	}
	printf("Token : %s", tokenString(node->data->token));
	if(node->data->token == AST_ID){
		printf(" %s", (char*)node->data->value);
	}
	putchar('\n');

	for(i = 0; i < node->number_of_ast_children; i++){
		instruction = printastNodeRec(node->ast_children[i], instruction, tab+1);
	}
	for(i = 0; i < node->number_of_SEQ_children; ++i){
		instruction = printSeqNodeRec(node->SEQ_children[i], instruction, tab+1);
	}
	return instruction;

}

int printastNode(ast_node * node) {
	if(node == NULL)
		return 0;
	putchar('\n');
	return printastNodeRec(node, 0, 0);

}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/

char * tokenString(ast_category token){
	switch(token){
		case AST_BOOL: return "BOOL"; break;
		case AST_FRACT: return "FRACT"; break;
		case AST_AOP1: return "AOP1"; break;
		case AST_AOP2: return "AOP2"; break;
		case AST_DECLARATION: return "TYPE"; break;
		case AST_BOP1: return "BOP1"; break;
		case AST_BOP2: return "BOP2"; break;
		case AST_RELOP1: return "RELOP1"; break;
		case AST_RELOP: return "RELOP"; break;
		case AST_WHILE: return "WHILE"; break;
		case AST_IF: return "IF"; break;
		case AST_ELSE: return "ELSE"; break;
		case AST_PRINT: return "PRINT"; break;
		case AST_ASSIGNMENT: return "ASSIGNMENT"; break;
		case AST_ID : return "ID"; break;
		default: return "";
	}
}

