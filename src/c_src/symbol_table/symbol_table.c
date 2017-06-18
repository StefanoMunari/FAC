#include "symbol_table.h"

extern symbol_table_entry * symbol_table;
extern void yyerror(char *, ...);


void install_ID(char* id, type_t type) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e != NULL) { /* ID ALREADY INSTALLED -> ERROR */
		yyerror("symol_table::install_ID:The id %s is already installed", id);
	}
	e = (symbol_table_entry*)malloc(sizeof(symbol_table_entry));
	//strlen does not take into account '\0'
	e->id = malloc(sizeof(char) * (strlen(id) + 1));
	strcpy(e->id, id);
	e->type = type;

	HASH_ADD_KEYPTR(hh, symbol_table, e->id, strlen(e->id), e);
}


symbol_table_entry * lookup_ID(char* id) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e == NULL){
		yyerror("symbol_table::lookup_ID: \
				Failed lookup: The variable %s is not yet installed",
				id
		);
	}
	return e;
}

type_t get_type(char * id) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, id, e);
	if(e == NULL){
		yyerror("symbol_table::get_type:\
				Failed lookup: The variable %s is not yet installed",
				id
		);
	}
	return e->type;
}

void free_table(){
	symbol_table_entry * e, *tmp;
	HASH_ITER(hh, symbol_table, e, tmp) {
		free(e->id);
		HASH_DEL(symbol_table, e);
		free(e);
	}
}

symbol_table_entry * get_table(){
	return symbol_table;
}
