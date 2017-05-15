#include "symbol_table.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern symbol_table_entry * symbol_table;
extern void yyerror(char *, ...);

static void check_type (type_t type0, type_t type1) {
	if(type0 != type1)
		yyerror("The requested type differs from the symbol_table's one. Exiting");
};

void installID(char* _id, type_t _type) {
	symbol_table_entry * e;

	HASH_FIND_STR(symbol_table, _id, e);
	if(e != NULL) { /* ID ALREADY INSTALLED -> ERROR */
		yyerror("The id %s is already installed", _id);
	}
	e = (symbol_table_entry*)malloc(sizeof(symbol_table_entry));

	e->id = malloc(sizeof(char) * (strlen(_id) + 1)); //strlen does not take into account '\0'
	strcpy(e->id, _id);
	e->type = _type;
	e->value = NULL;

	HASH_ADD_KEYPTR(hh, symbol_table, e->id, strlen(e->id), e);
}


void* lookupID(char* _id, type_t type) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		yyerror("Failed lookup: The variable %s is not yet installed", _id);
	}
	check_type(e->type, type);
	if(e -> value == NULL) {
		yyerror("The variable %s is declared but not initialized. Exiting", _id);
	}
	return e->value;
}

type_t getType(char * _id) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		yyerror("Failed lookup: The variable %s is not yet installed", _id);
	}
	return e->type;
}


void setValue(char * _id, void * value) {
	symbol_table_entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		yyerror("The variable cannot be assigned because %s is not yet installed", _id);
	}
	/*
	switch(type){
		case FRACT_T :
			e->value = malloc(sizeof(fract_t));
			memcpy(e->value, value, sizeof(fract_t));
			break;
		case BOOL_T :
			e->value = malloc(sizeof(bool));
			memcpy(e->value, value, sizeof(bool));
			break;
	}*/
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
