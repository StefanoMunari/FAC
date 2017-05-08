#include "ast_record.h"
#include <stdlib.h>
#include <stdio.h>

record * ASTRecord(unsigned int token, op_t op, void * value){
	record * record = malloc(sizeof(record));
	record->token = token;
	record->op = op;
	record->value = value;
	return record;
}

void freeASTRecord(record * this){
	if(this->value != NULL)
		free(this->value);
	free(this);
}