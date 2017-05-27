/**
 * @file
 * @brief Performs type checking on an ast structure (seq_tree)
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 * @see seq_tree.h
 */
#ifndef __TYPE__CHECKER__H__
#define __TYPE__CHECKER__H__
#include "../ast/seq_tree.h"
/** Performs type checking by pre-order traversing the ast,
 * which is stored in RAM as a seq_tree
 * @param seq_node the root of the ast
 * @return bool true if the type checking is successful, false otherwise
 * @see Dragon Book - 2nd ed. (§2.8.2 - Construction of Syntax Trees)
 */
bool type_check(seq_node *);

#endif /* __TYPE__CHECKER__H__ */
