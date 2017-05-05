#include "seq_tree.h"


seq_node * newSeqNode(seq_node * left, AST_node * right){
	seq_node * this=malloc(sizeof(seq_node));
	this->left=left;
	this->right=right;
	return this;
}

void freeSeqNode(seq_node * this){
	if(this->left != NULL)
		freeSeqNode(this->left);
	if(this->right != NULL)
		freeASTNode(this->right);
	free(this);
}