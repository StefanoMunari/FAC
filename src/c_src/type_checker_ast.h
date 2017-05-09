/**
 * @file
 * @brief Performs type checking on an AST subtree
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TYPE__CHECKER__AST__H__
#define __TYPE__CHECKER__AST__H__
#include "ast.h"
/** Type checking on an AST subtree,
 * @param AST_node * - the root node of the AST subtree
 * @return bool true if the type checking is successful, false otherwise
 * @see type_checker_ast.h
 */
bool type_check_AST_node(AST_node *);

#endif /* __TYPE__CHECKER__AST__H__ */