#include "symbol_table.h"
#include <string.h>

void installID(char* _id, type_t _type) {
	entry * e;
	
	HASH_FIND_STR(table, _id, e);
	if(e != NULL) { /* ID ALREADY INSTALLED -> ERROR */
		fprintf(stderr, "The id %s is already installed\n", _id);
		exit(EXIT_FAILURE);
	}
	e = (entry*)malloc(sizeof(entry));
	
	e->id = malloc(sizeof(char) * (strlen(_id) + 1)); //strlen does not take into account '\0'
	strcpy(e->id, _id);
	e->type = _type;
	e->val = NULL;
	
	HASH_ADD_KEYPTR(hh, table, e->id, strlen(e->id), e); 
}


void* lookupID(char* _id, type_t type) {
	entry * e;
	HASH_FIND_STR(table, _id, e);
	if(e == NULL){
		fprintf(stderr, "Failed lookup: The variable %s is not yet installed\n", _id);
		exit(EXIT_FAILURE);
	}
	if(e->type != type){
		fprintf(stderr, "The requested type differs from the table's one. Exiting\n", _id);
		exit(EXIT_FAILURE);
	}
	return e->val;
}


void setValue(char * id, type_t type, void * value) {
	entry * e;
	HASH_FIND_STR(table, _id, e);
	if(e == NULL){
		fprintf(stderr, "The variable cannot be assigned because it not yet installed\n", _id);
		exit(EXIT_FAILURE);
	}
	if(e->type != type){
		fprintf(stderr, "The requested type differs from the table's one. Exiting\n", _id);
		exit(EXIT_FAILURE);
	}
	e->value = value;
}




void freeTable(){
	entry * el, *tmp;
	HASH_ITER(hh, table, el, tmp) {
		free(el->id);
		if(e->val != NULL)
			free(e->val);
		HASH_DEL(table, el);
		free(el);
	} 
}
