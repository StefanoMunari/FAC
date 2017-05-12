#include "seq_tree.h"
#include <stdlib.h>
#include <stdio.h>

seq_node * newSeqNode(seq_node * left, AST_node * right){
	seq_node * this=calloc(1, sizeof(seq_node));
	this->left=left;
	this->right=right;
	return this;
}

void freeSeqNode(seq_node * this){
	if(this == NULL)
		return;
	//printf("%s freeing \n\n\n", tokenString(this->right->data->token));
	freeSeqNode(this->left);
	freeASTNode(this->right);
	free(this);
}

int printSeqNode(seq_node * this){
	if(this == NULL)
		return 0;
	int instruction = printSeqNode(this->left);
	printf("### Instruction %d ###\n", instruction+1);
	printASTNode(this->right);
	printf("#######################\n");
	return instruction+1;
}

