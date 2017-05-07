/**
 * @file
 * @brief Record of the Abstract Syntax Tree (AST) -
 *	represents the data stored in each node of the AST
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __AST_RECORD__H__
#define __AST_RECORD__H__
#include "ast_optype.h"
#include "factype.h"
/**
 * @brief record of the AST
 */
typedef struct record{
	int token;
	ast_op_t op;
	type_t type;
	void * value;
} record;

#endif /* __AST_RECORD__H__ */