/**
 * @author <mirko.bez@studenti.unipd.it>,<stefano.munari.1@studenti.unipd.it>
 * @file AST.h
 * @brief file defining the syntax tree data structure and its API
 */
#ifndef __ABSTRACT_SYNTAX_TREE__H__
#define __ABSTRACT_SYNTAX_TREE__H__
#include "factype.h"
#include "factype_ast.h"
#include <stdarg.h>


/**
 * union encoding all possible expr operations
 */
typedef union {
	aop_t aop;
	bop1_t bop1;
	bop2_t bop2;
	relop_t relop;
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
