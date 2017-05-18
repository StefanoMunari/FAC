#include "ast_record.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

record * astRecord(ast_category token, int line, op_t op, void * value){
	assert(token < AST_END_MARKER);
	record * r = calloc(1, sizeof(record));
	r->line = line;
	r->token = token;
	r->op = op;
	r->value = value;
	return r;
}

void freeastRecord(record * this){
	if(this == NULL)
		return;
	free(this->value);
	free(this);
}
