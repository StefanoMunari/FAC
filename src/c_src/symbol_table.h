/**
 * @file 
 * @brief contains the interface of the symbol table
 */
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__
#include "factype.h"
#include "../lib/uthash.h"

/** Struct contained in the symbol table */
typedef struct symbol_table_entry {
	/** the key */
	char * id;
	/** type */
	type_t type; 
	/** the concrete value */
	void * value;
	/** needed by uthash.h to make this structure hashable */
	UT_hash_handle hh;
} symbol_table_entry;

/**
 * Install an ID in the symbol table with the given type
 * @param id
 * @param type
 */
void installID(char* id, type_t type);
/**
 * @param id
 * @return the symbol table entry for the variable named id
 */
symbol_table_entry * lookupID(char* id);
/**
 * Set the value of the entry "id" to the given value
 * @param id
 * @param value
 */
void setValue(char * id, void * value);
/**
 * For its usage in the type_checker @see type_checker_ast_expr.c
 * @return the type of the entry "id" in the symbol table
 */
type_t getType(char * id);
/**
 * Free the symbol table.
 */
void freeTable();

#endif /*__SYMBOL_TABLE_H__*/
