#include "AST.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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
