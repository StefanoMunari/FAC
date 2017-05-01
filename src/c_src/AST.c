#include "AST.h"
#include <stdlib.h>
#include <stdio.h>

AST_node * newASTNode(int number_of_children) {
	AST_node * node = malloc(sizeof(AST_node));
	node->data = malloc(sizeof(record));
	node->number_of_children = number_of_children;
	node->children = (AST_node**)malloc(sizeof(AST_node*) * number_of_children);
	node->data->value = NULL;
	return node;
}


void freeASTNode(AST_node * node){
	int i;
	printf("%d - Number of children %d \n", node->data->token, node->number_of_children);
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
