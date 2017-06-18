#include "seq_tree.h"
#include <stdlib.h>
#include <stdio.h>

#define PRINT_TAB(tab) \
int i; \
for(i = 0; i < tab; i++) \
	putchar('\t'); \


seq_node * newSeqNode(seq_node * left, ast_node * right){
	seq_node * this=calloc(1, sizeof(seq_node));
	this->left=left;
	this->right=right;
	return this;
}

void freeSeqNode(seq_node * this){
	if(this == NULL)
		return;
	freeSeqNode(this->left);
	free_ast_node(this->right);
	free(this);
}

int printSeqNodeRec(seq_node * this, int instruction, int tab){
	if(this == NULL)
		return instruction;

	instruction = printSeqNodeRec(this->left, instruction, tab);
	PRINT_TAB(tab);
	printf("### Statement %d ###\n", instruction);
	instruction = print_ast_node_rec(this->right, instruction+1, tab);

	return instruction;
}

int printSeqNode(seq_node * this){
	return printSeqNodeRec(this, 1, 0);
}


