/**
 * @file
 * @brief file defining the syntax tree data structure and its API
 *
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __SEQ_TREE__H__
#define __SEQ_TREE__H__
#include "ast.h"

/** A structure that represents the sequential composition of two statements */
typedef struct seq_node {
	/** pointer to the previous statement */
	struct seq_node * left;
	/** pointer to the real content of the statement */
	ast_node * right;
} seq_node;

/**
 * create a new seq tree (i.e. a statement)
 * @param left a pointer to the previous statement (the tree is built bottom up)
 * @param right a pointer to a ast node
 * @return the new created seq node with the given left and right children
 */
seq_node * newSeqNode(seq_node * left, ast_node * right);

/**
 * free recursively the seq node this
 * @param this the seq_node to free
 */
void freeSeqNode(seq_node * this);

/**
 * Print the given seq node (i.e. the whole abstract syntax tree)
 * @param this the seq node to print
 */
int printSeqNode(seq_node * this);
/**
 * Print the given seq node along with the instructions' ids. It receives as 
 * a parameter an id and print its children recursively. 
 * @param this the tree to print
 * @param instruction the starting instruction id
 * @param tab the number of tab characters to print before the statement
 * @return the number corresponding to the next instruction
 */
int printSeqNodeRec(seq_node * this, int instruction, int tab);
#endif /* __SEQ_TREE__H__ */
