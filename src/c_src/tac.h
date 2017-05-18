/**
 * @file
 * @brief Three Address Code (TAC) - translate an AST to the corresponding
 *	Three Address Code (TAC) representation using indirect triples. It is a
 * 	linearized representation of the AST.
 *	provides a minimal interface to manipulate its internal data structures
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

typedef struct tac_entry tac_entry;
typedef struct tac_node tac_node;

typedef struct tac_value{
	bool boolean;
	fract_t * fract;
	symbol_table_entry * address;
	tac_entry * instruction;
} tac_value;

typedef struct tac_entry{
	tac_op op;
	tac_value * arg0;
	tac_value * arg1;
} tac_entry;

struct tac_list * tac(seq_node * input);
void free_tac(tac_node * head);

#endif /*__TAC_H__*/
