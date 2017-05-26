#ifndef __TAC_AST_NODE_H__
#define __TAC_AST_NODE_H__
#include "ast.h"
#include "tac.h"
#include "tac_list.h"

tac_list* tac_ast_node(ast_node *);
tac_list* _tac_append(tac_list * , tac_list *);
#endif /*__TAC_AST_NODE_H__*/
