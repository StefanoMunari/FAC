/**
 * @brief Interface that provides the function to trasform an AST tree into a list
 * of three address code instructions.
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.t
 */

#ifndef __TAC_AST_NODE_H__
#define __TAC_AST_NODE_H__
#include "../ast/ast.h"
#include "tac.h"
#include "tac_list.h"

tac_list* tac_ast_node(ast_node * node);

#endif /*__TAC_AST_NODE_H__*/
