/**
 * @file
 * @brief Three Address Code (TAC) - translate an AST to the corresponding
 *	Three Address Code (TAC) representation using indirect triples
 *	provides a minimal interface to manipulate its internal data structures
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
typedef struct tac_value{
	int constant;
	symbol_table_symbol_table_entry * address;
	tac_symbol_table_entry * instruction;
}

typedef struct tac_symbol_table_entry
{
	op_t op;
	tac_value * arg0;
	tac_value * arg1;
} tac_symbol_table_entry;

tac_symbol_table_entry * tac(AST_node * node);