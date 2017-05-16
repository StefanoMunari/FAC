#ifndef __TAC_AST_NODE_H__
#define __TAC_AST_NODE_H__
#include "ast.h"
#include "tac.h"
#include "tac_list.h"
#include "stack.h"

void tac_ast_node(ast_node * node, tac_node ** current,
	tac_node * successor, stack_t * stack);

#endif /*__TAC_AST_NODE_H__*/