/**
 * @author <mirko.bez@studenti.unipd.it>,<stefano.munari.1@studenti.unipd.it>
 * @file AST.h
 * @brief file defining the syntax tree data structure and its API
 */
#ifndef __ABSTRACT_SYNTAX_TREE__H__
#define __ABSTRACT_SYNTAX_TREE__H__
#include "factype.h"
#include <stdarg.h>


/**
 * union encoding all possible expr operations
 */
typedef union {
	aop0_t aop0;
	aop1_t aop1;
	bop1_t bop1;
	bop2_0_t bop2_0;
	bop2_1_t bop2_1;
	bop2_2_t bop2_2;
	bop2_3_t bop2_3;
	relop0_t relop0;
	relop1_t relop1;
} op_t;


/**
 * struct defining the data part of the AST node
 */
typedef struct record_t{
	int token;
	op_t op;
	type_t type;
	void * value;
} record;

/**
 * struct defining the Abstract syntax tree of fac
 */
typedef struct AST_node_t {
	record * data;
	unsigned char number_of_children;
	struct AST_node_t ** children;
	struct AST_node_t * next; /* Pointer to the next stmt */
} AST_node;

/** Create a new AST node
 * @param number_of_children nodes
 * @param list of children
 * @return a new initialized node with the children given in the list
 */
AST_node * newASTNode(int number_of_children, ...);
/**
 * Free (recursively) the syntax three given in input
 * @param root the root of the syntax tree
 */
void freeASTNode(AST_node * root);

/**
 * Print a syntax tree on the screen (only the tokens)
 * @param root the root of the syntax tree
 */
void printASTNode(AST_node * root);


#endif /* __ABSTRACT_SYNTAX_TREE__H__ */
