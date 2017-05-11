#include "ast.h"
#include "factype_ast.h"
#include "seq_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

char * tokenString(AST_category token);

AST_node * ASTNode(unsigned int token, const int number_of_AST_children, const int number_of_SEQ_children, ...) {
	AST_node * node = malloc(sizeof(AST_node));
	node->data = (record *) ASTRecord(token, -1, NULL);
	node->number_of_AST_children = number_of_AST_children;
	node->number_of_SEQ_children = number_of_SEQ_children;
	
	
	
	node->AST_children = (AST_node**)malloc(sizeof(AST_node*) * number_of_AST_children);
	node->SEQ_children = (seq_node**)malloc(sizeof(seq_node*) * number_of_SEQ_children);
	
	va_list args_iterator;
	va_start(args_iterator, number_of_SEQ_children);
	{
		int i;
		for(i = 0; i < number_of_AST_children; ++i) {
			
			node->AST_children[i] = va_arg(args_iterator, AST_node*);
			
		}
		for(i = 0; i < number_of_SEQ_children; ++i){
			node->SEQ_children[i] = va_arg(args_iterator, seq_node*);
		}
	}
	va_end(args_iterator);
	
	return node;
}

void freeASTNode(AST_node * node){
	if(node == NULL)
		return;
	{
		int i;
		/* Free children */
		for(i = 0; i < node->number_of_AST_children; ++i)
			freeASTNode(node->AST_children[i]);
		for(i = 0; i < node->number_of_SEQ_children; ++i)
			freeSeqNode(node->SEQ_children[i]);
	}
	/* Free resources */
	freeASTRecord(node->data);
	free(node);
}

void printASTNodeRec(AST_node * node, int tab){
	if(node == NULL)
		return;
	int i;
	for(i = 0; i < tab; i++){
		putchar('\t');
	}
	printf("Token : %s\n", tokenString(node->data->token));


	for(i = 0; i < node->number_of_AST_children; i++){
		printASTNodeRec(node->AST_children[i], tab+1);
	}
	for(i = 0; i < node->number_of_SEQ_children; ++i){
		printSeqNode(node->SEQ_children[i]);
	}

}

void printASTNode(AST_node * node) {
	if(node == NULL)
		return;
	putchar('\n');
	printASTNodeRec(node, 0);

}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/

char * tokenString(AST_category token){
	switch(token){
		case AST_BOOL: return "BOOL"; break;
		case AST_FRACT: return "FRACT"; break;
		case AST_AOP: return "AOP"; break;
		case AST_DECLARATION: return "TYPE"; break;
		case AST_BOP1: return "BOP1"; break;
		case AST_BOP2: return "BOP2"; break;
		case AST_RELOP: return "RELOP"; break;
		case AST_WHILE: return "WHILE"; break;
		case AST_IF: return "IF"; break;
		case AST_ELSE: return "ELSE"; break;
		case AST_SKIP: return "SKIP"; break;
		case AST_PRINT: return "PRINT"; break;
		case AST_ASSIGNMENT: return "ASSIGNMENT"; break;
		case AST_ID : return "ID"; break;
	}
	return "";
}

