/**
 * @file type_checker.h
 */
#ifndef __TYPE__CHECKER__H__
#define __TYPE__CHECKER__H__
#include "AST.h"
#include "seq_tree.h"

/**
 * @param root the root of the abstract syntax tree
 * @return true if the type checking is successful false otherwise
 */
bool type_checking(seq_node * root);


#endif /* __TYPE__CHECKER__H__ */
