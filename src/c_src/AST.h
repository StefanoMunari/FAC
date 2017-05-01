#ifndef __ABSTRACT_SYNTAX_TREE__H__
#define __ABSTRACT_SYNTAX_TREE__H__
#include "factype.h"


typedef struct record_t{
	int token;
	op_t op;
	type_t type;
	void * value;
} record;

typedef struct AST_node_t {
	record * data;
	unsigned char number_of_children;
	struct AST_node_t ** children; /* Encode the children of this node */
	struct AST_node_t * next; 
} AST_node;

AST_node * newASTNode(int number_of_children);
void freeASTNode(AST_node *);


#endif /* __ABSTRACT_SYNTAX_TREE__H__ */
