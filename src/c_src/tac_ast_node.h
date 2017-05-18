#ifndef __TAC_AST_NODE_H__
#define __TAC_AST_NODE_H__
#include "ast.h"
#include "tac.h"
#include "tac_list.h"
#include "stack.h"

tac_list* tac_ast_node(ast_node *, tac_list *, stack_t *);

#endif /*__TAC_AST_NODE_H__*/