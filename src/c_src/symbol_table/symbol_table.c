#include "symbol_table.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern symbol_table_entry * symbol_table;
extern void yyerror(char *, ...);


void installID(char* id, type_t type) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e != NULL) { /* ID ALREADY INSTALLED -> ERROR */
		yyerror("symol_table::installID:The id %s is already installed", id);
	}
	e = (symbol_table_entry*)malloc(sizeof(symbol_table_entry));
	//strlen does not take into account '\0'
	e->id = malloc(sizeof(char) * (strlen(id) + 1)); 
	strcpy(e->id, id);
	e->type = type;
	e->value = NULL;
	
	HASH_ADD_KEYPTR(hh, symbol_table, e->id, strlen(e->id), e);
}


symbol_table_entry * lookupID(char* id) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e == NULL){
		yyerror("symbol_table::lookupID: \
				Failed lookup: The variable %s is not yet installed",
				id
		);
	}
	return e;
}

type_t getType(char * id) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e == NULL){
		yyerror("symbol_table::getType:\
				Failed lookup: The variable %s is not yet installed", 
				id
		);
	}
	return e->type;
}


void setValue(char * id, void * value) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e == NULL){
		yyerror(
			"symbol_table::setValue::\
			The variable cannot be assigned because %s is not yet installed",
			id
		);
	}
}

void freeTable(){
	symbol_table_entry * e, *tmp;
	HASH_ITER(hh, symbol_table, e, tmp) {
		free(e->id);
		if(e->value != NULL)
			free(e->value);
		HASH_DEL(symbol_table, e);
		free(e);
	}
}

symbol_table_entry * getTable(){
	return symbol_table;
}
