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
	/** Field containing the real data				*/
	record * data;
	/** number of children of type ast_node			*/
	unsigned int number_of_ast_children;
	/** number of children of type seq_node			*/
	unsigned int number_of_seq_children;
	/** pointers to the children of type ast_node	*/
	struct ast_node ** ast_children;
	/** pointers to the children of type seq_node	*/
	struct seq_node ** seq_children;
} ast_node;

/** Create a new ast node
 * @param number_of_children nodes
 * @param list of children
 * @return a new initialized node with the children given in the list
 */

ast_node * astNode(unsigned int token, int line, op_t op, void * value, 
const int number_of_ast_children, const int number_of_seq_children, ...);

/**
 * Free (recursively) the syntax three given in input
 * @param root the root of the syntax tree
 */
void freeastNode(ast_node * root);

/**
 * Print a syntax tree on the screen (only the tokens)
 * @param root the root of the syntax tree
 */
int printastNode(ast_node * root);

/**
 * Print a syntax tree on the screen
 * @param node the root of the ast tree to print
 * @param instruction the incoming instruction id
 * @param tab the indentation, i.e. the number of '\t' characters
 * to prepend to the real print
 * @return an integer that corresponds to the next instruction id
 */
int printastNodeRec(ast_node * node, int instruction, int tab);

/**
 * @param token the token from which we want the representation
 * @return the string representation of the token 
 */
char * tokenString(ast_category token);

#endif /* __AST__H__ */
