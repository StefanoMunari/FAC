#include "symbol_table.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern entry * symbol_table;

void installID(char* _id, type_t _type) {
	entry * e;
	
	HASH_FIND_STR(symbol_table, _id, e);
	if(e != NULL) { /* ID ALREADY INSTALLED -> ERROR */
		fprintf(stderr, "The id %s is already installed\n", _id);
		exit(EXIT_FAILURE);
	}
	e = (entry*)malloc(sizeof(entry));
	
	e->id = malloc(sizeof(char) * (strlen(_id) + 1)); //strlen does not take into account '\0'
	strcpy(e->id, _id);
	e->type = _type;
	e->value = NULL;
	
	HASH_ADD_KEYPTR(hh, symbol_table, e->id, strlen(e->id), e); 
}


void* lookupID(char* _id, type_t type) {
	entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		fprintf(stderr, "Failed lookup: The variable %s is not yet installed\n", _id);
		exit(EXIT_FAILURE);
	}
	if(e->type != type){
		fprintf(stderr, "The requested type for %s differs from the symbol_table's one. Exiting\n", _id);
		exit(EXIT_FAILURE);
	}
	return e->value;
}

type_t getType(char * _id) {
	entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		fprintf(stderr, "Failed lookup: The variable %s is not yet installed\n", _id);
		exit(EXIT_FAILURE);
	}
	return e->type;
}


void setValue(char * _id, type_t type, void * value) {
	entry * e;
	HASH_FIND_STR(symbol_table, _id, e);
	if(e == NULL){
		fprintf(stderr, "The variable cannot be assigned because %s is not yet installed\n", _id);
		exit(EXIT_FAILURE);
	}
	if(e->type != type){
		fprintf(stderr, "The requested type for variable %s differs from the symbol_table's one. Exiting\n", _id);
		exit(EXIT_FAILURE);
	}
	
	switch(type){
		case FRACT_T : 
			e->value = malloc(sizeof(fract_t));
			memcpy(e->value, value, sizeof(fract_t));
			break;
		case BOOL_T : 
			e->value = malloc(sizeof(bool));
			memcpy(e->value, value, sizeof(bool));
			break;
	}
}




void freeTable(){
	entry * e, *tmp;
	HASH_ITER(hh, symbol_table, e, tmp) {
		free(e->id);
		if(e->value != NULL)
			free(e->value);
		HASH_DEL(symbol_table, e);
		free(e);
	} 
}
