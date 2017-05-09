#include "type_checker.h"
#include "type_checker_ast.h"
#include <stddef.h>

bool type_check(seq_node * seqTree){
	if(seqTree == NULL)
		return true;
	bool result = type_check(seqTree->left);
	result &= type_check_AST_node(seqTree->right);
	return result;
}