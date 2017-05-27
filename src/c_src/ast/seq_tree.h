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


typedef struct seq_node {
	struct seq_node * left;
	ast_node * right;
} seq_node;

seq_node * newSeqNode(seq_node * left, ast_node * right);
void freeSeqNode(seq_node * this);
int printSeqNode(seq_node * this);
int printSeqNodeRec(seq_node * this, int instruction, int tab);
#endif /* __SEQ_TREE__H__ */
