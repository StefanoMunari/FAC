/**
 * @file
 * @brief Performs type checking on an ast subtree
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TYPE__CHECKER__ast__H__
#define __TYPE__CHECKER__ast__H__
#include "../ast/ast.h"
/** Type checking on an ast subtree,
 * @param ast_node * - the root node of the ast subtree
 * @return bool true if the type checking is successful, false otherwise
 * @see type_checker_ast.h
 */
bool type_check_ast_node(ast_node *);

#endif /* __TYPE__CHECKER__ast__H__ */