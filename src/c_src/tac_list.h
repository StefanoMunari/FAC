/**
 * @file
 * @brief file defining the tac_node and tac_list data structures used to
 * represent respectively a single three address code instruction and a whole
 * list of instructions.
 *
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TAC_LIST_H__
#define __TAC_LIST_H__
#include "tac.h"

/**
 * Struct definining a single node of the tac_list. It contains a value, the
 * actual three address code instruction, a link to the previous and a link to
 * the next instruction
 */
typedef struct tac_node {
	/** the actual content of the node */
	struct tac_entry * value;
	/** pointer to the previous element of the list */
	struct tac_node * prev;
	/** pointer to the next element of the list */
	struct tac_node * next;
} tac_node;

/**
 * Struct defining a doubly-linked list of tac_nodes. It has a pointer to
 * the first and to the last element.
 */
typedef struct tac_list{
	/** pointer to the first node of the list */
	struct tac_node * first;
	/** pointer to the last node of the list */
	struct tac_node * last;
} tac_list;

#endif /*__TAC_LIST_H__*/
