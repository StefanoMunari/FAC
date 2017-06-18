/**
 * @file
 * @brief contains the interface of the symbol table
 */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "../types/factype.h"
#include "../../lib/uthash.h"

/** @brief Struct contained in the symbol table */
typedef struct symbol_table_entry {
	/** the key */
	char * id;
	/** type */
	type_t type;
	/** needed by uthash.h to make this structure hashable */
	UT_hash_handle hh;
} symbol_table_entry;

/**
 * Install an ID in the symbol table with the given type
 * @param id
 * @param type
 */
void install_ID(char* id, type_t type);
/**
 * @param id
 * @return the symbol table entry for the variable named id
 */
symbol_table_entry * lookup_ID(char* id);

/**
 * For its usage in the type_checker @see type_checker_ast_expr.c
 * @return the type of the entry "id" in the symbol table
 */
type_t get_type(char * id);
/**
 * Free the symbol table.
 */
void free_table();
/**
 * @return the symbol_table
 */
symbol_table_entry * get_table();

#endif /*__SYMBOL_TABLE_H__*/
