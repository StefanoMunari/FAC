/**
 * @file
 * @brief Abstract Syntax Tree (AST) -
 *	provides a minimal interface to manipulate its internal data structures
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __ABSTRACT_SYNTAX_TREE__H__
#define __ABSTRACT_SYNTAX_TREE__H__
#include "ast_record.h"
#include <stdarg.h>

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
