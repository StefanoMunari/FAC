/**
 * @file
 * @brief Performs type checking on an AST structure
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __TYPE__CHECKER__H__
#define __TYPE__CHECKER__H__
#include "seq_tree.h"

/** Performs type checking on an AST
 * @param seq_node the root of the AST
 * @return bool true if the type checking is successful, false otherwise
 */
bool type_checking(seq_node *);

#endif /* __TYPE__CHECKER__H__ */
