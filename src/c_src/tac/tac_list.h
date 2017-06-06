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
 * @brief Struct definining a single node of the tac_list
 * 
 * @details It contains a value, 
 * the actual three address code instruction, a link to the previous and a link
 * to the next instruction
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
 * @brief Struct defining a doubly-linked list of tac_nodes. 
 * @details It has a pointer to the first and to the last element.
 */
typedef struct tac_list{
	/** pointer to the first node of the list */
	struct tac_node * first;
	/** pointer to the last node of the list */
	struct tac_node * last;
} tac_list;

/**
 * append the second list to the first one and destroy the second argument
 * @param tlist the list to which the list to_append is appended
 * @param to_append the list to append
 * @return a pointer corresponding to tlist that represents the new concatenated
 * list
 */
tac_list* tac_append(tac_list * tlist , tac_list * to_append);
/**
 * attach a node at the end of the given tac_list
 * @param tlist the list to which the tac_node is appended
 * @param tnode a tac node that has to be appended to the tlist
 * @return a pointer corresponding to tlist that represents the new concatenated
 * list
 */
tac_list * tac_connect(tac_list * tlist, tac_node * tnode);
#endif /*__TAC_LIST_H__*/
