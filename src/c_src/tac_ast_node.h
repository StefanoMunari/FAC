#ifndef __TAC_AST_NODE_H__
#define __TAC_AST_NODE_H__
#include "ast.h"
#include "tac.h"
#include "stack.h"

void tac_ast_node(ast_node * node, tac_node ** predecessor,
	tac_node * current, stack_t * stack);

#endif /*__TAC_AST_NODE_H__*/