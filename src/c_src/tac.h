/**
 * @file
 * @brief Three Address Code (TAC) - translate an AST to the corresponding
 *	Three Address Code (TAC) representation using indirect triples. It is a
 * 	linearized representation of the AST which enables code optimization.
 *	Indeed the list of triples is stored as a doubly linked list, so the
 *	the instruction of the program are easily rearrangeable.
 *
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TAC_H__
#define __TAC_H__
#include "seq_tree.h"
#include "symbol_table.h"
#include "factype_tac.h"
#include "factype.h"
#include "tac_list.h"
#include <stdbool.h>


/**
 * A value of a TAC entry, it is one of the following:
 * a constant (boolean, fract);
 * an address (symbol table entry for a variable);
 * an instruction (pointer to another TAC entry);
 */
typedef struct tac_value{
	bool boolean;
	fract_t * fract;
	symbol_table_entry * address;
	struct tac_entry * instruction;
} tac_value;

/**
 * A TAC entry represented as a triple (operator, argument0, argument1)
 */
typedef struct tac_entry{
	tac_op op;
	tac_value * arg0;
	tac_value * arg1;
} tac_entry;

/** Generates a new TAC representation of the given AST
 * @param input a pointer to the head the AST
 * @return a doubly-linked list of indirect triples
 */
struct tac_list * generate_tac(seq_node * input);

/** Frees the list of indirected triples (TAC) given as input
 * @param a pointer to the list of indirected triples (TAC)
 */
void free_tac(struct tac_list * tlist);

#endif /*__TAC_H__*/
