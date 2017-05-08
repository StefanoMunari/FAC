#include "ast_record.h"
#include <stdlib.h>
#include <stdio.h>

record * ASTRecord(unsigned int token, op_t op, void * value){
	record * r = malloc(sizeof(record));
	r->token = token;
	r->op = op;
	r->value = value;
	return r;
}

void freeASTRecord(record * this){
	if(this->value != NULL)
		free(this->value);
	free(this);
}
