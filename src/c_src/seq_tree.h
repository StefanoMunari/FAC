/**
 * @author <mirko.bez@studenti.unipd.it>,<stefano.munari.1@studenti.unipd.it>
 * @file .h
 * @brief file defining the syntax tree data structure and its API
 */
#ifndef __SEQ_TREE__H__
#define __SEQ_TREE__H__
#include "ast.h"

typedef struct seq_node {
	struct seq_node * left;
	AST_node * right;
} seq_node;


seq_node * newSeqNode(seq_node *, AST_node *);
void freeSeqNode(seq_node *);

#endif /* __SEQ_TREE__H__ */
