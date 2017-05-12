/**
 * @file
 * @brief Abstract Syntax Tree (ast) -
 *	provides a minimal interface to manipulate its internal data structures
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __AST__H__
#define __AST__H__
#include "ast_record.h"
#include <stdarg.h>

/**
 * @brief Abstract Syntax Tree (ast) node
 */
typedef struct ast_node {
	record * data;
	unsigned int number_of_ast_children;
	unsigned int number_of_SEQ_children;
	struct ast_node ** ast_children;
	struct seq_node ** SEQ_children;
} ast_node;

/** Create a new ast node
 * @param number_of_children nodes
 * @param list of children
 * @return a new initialized node with the children given in the list
 */
ast_node * astNode(unsigned int token, const int number_of_children, const int number_of_SEQ_children, ...);
/**
 * Free (recursively) the syntax three given in input
 * @param root the root of the syntax tree
 */
void freeastNode(ast_node * root);

/**
 * Print a syntax tree on the screen (only the tokens)
 * @param root the root of the syntax tree
 */
void printastNode(ast_node * root);

char * tokenString(ast_category token);

#endif /* __AST__H__ */
