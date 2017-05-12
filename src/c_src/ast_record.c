#include "ast_record.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

record * astRecord(ast_category token, op_t op, void * value){
	assert(token < ast_END_MARKER);
	record * r = calloc(1, sizeof(record));
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
