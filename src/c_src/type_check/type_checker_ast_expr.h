/**
 * @file
 * @brief Performs type checking on an expression (ast subtree)
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TYPE__CHECKER__ast__EXPR__H__
#define __TYPE__CHECKER__ast__EXPR__H__
#include "../ast/ast.h"
#include "../types/factype.h"
/** Type checking on an expression, which is an ast subtree
 * pre-order traversing of the subtree
 * @param ast_node * - the root node of the ast subtree (expression)
 * @param type_t - the type associated with the root node of the subtree
 * @return bool true if the type checking is successful, false otherwise
 * @see type_checker_ast.h
 */
bool type_check_ast_expr(ast_node *, type_t);

#endif /* __TYPE__CHECKER__ast__EXPR__H__ */