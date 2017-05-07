#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "seq_tree.h"
#include "parser.tab.h"
#include "factypetoken_ast.h"

AST_node * newASTNode(int number_of_children, ...) {
	AST_node * node = malloc(sizeof(AST_node));
	node->data = malloc(sizeof(record));
	node->number_of_children = number_of_children;
	if(node->number_of_children == 0)
		node->children = NULL;
	else {
		node->children = (AST_node**)malloc(sizeof(AST_node*) * number_of_children);
		assert(node->children != NULL);
		va_list ap;
		va_start(ap, number_of_children);
		int i;
		for(i = 0; i < number_of_children; i++){
			node->children[i] = va_arg(ap, AST_node*);
		}
		va_end(ap);
	}
	node->data->value = NULL;
	return node;
}


void freeASTNode(AST_node * node){
	if(node == NULL)
		return;
	int i;
	/* Free children */
	for(i = 0; i < node->number_of_children; i++){
		freeASTNode(node->children[i]);
	}


	if(node->data->value != NULL)
		free(node->data->value);
	/* Free data */
	free(node->data);
	free(node);
}



char * tokenString(int token){
	switch(token){
		case BOOL: return "BOOL"; break;
		case FRACT: return "FRACT"; break;
		case AST_AOP: return "AOP"; break;
		case TYPE: return "TYPE"; break;
		case BOP1: return "BOP1"; break;
		case AST_BOP2: return "BOP2"; break;
		case AST_RELOP: return "RELOP"; break;
		case WHILE: return "WHILE"; break;
		case IF: return "IF"; break;
		case ELSE: return "ELSE"; break;
		case SKIP: return "SKIP"; break;
		case PRINT: return "PRINT"; break;
		case ASSIGNMENT: return "ASSIGNMENT"; break;
		case ID : return "ID"; break;
	}
	return "";
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
