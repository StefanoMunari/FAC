#include "ast_record.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

record * ASTRecord(AST_category token, op_t op, void * value){
	assert(token < AST_END_MARKER);
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
