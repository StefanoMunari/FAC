#include "ast.h"
#include "factype_ast.h"
//#include "parser.tab.h"
#include "seq_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static char * tokenString(AST_category token);

AST_node * ASTNode(unsigned int token, int number_of_children, ...) {
	AST_node * node = malloc(sizeof(AST_node));
	node->data = (record *) ASTRecord(token, -1, NULL);
	node->number_of_children = number_of_children;
	if(node->number_of_children == 0)
		node->children = NULL;
	else {
		node->children = (AST_node**)malloc(sizeof(AST_node*) * number_of_children);
		va_list args_iterator;
		va_start(args_iterator, number_of_children);
		{
			int i;
			for(i = 0; i < number_of_children; ++i)
				node->children[i] = va_arg(args_iterator, AST_node*);
		}
		va_end(args_iterator);
	}
	return node;
}

void freeASTNode(AST_node * node){
	if(node == NULL)
		return;
	{
		int i;
		/* Free children */
		for(i = 0; i < node->number_of_children; ++i)
			freeASTNode(node->children[i]);
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


	for(i = 0; i < node->number_of_children; i++){
		printASTNodeRec(node->children[i], tab+1);
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

static char * tokenString(AST_category token){
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

