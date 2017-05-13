/**
 * @file
 * @brief Record of the Abstract Syntax Tree (ast) -
 *	represents the data stored in each node of the ast
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __AST_RECORD__H__
#define __AST_RECORD__H__
#include "factype.h"
#include "factype_ast.h"
/**
 * @brief record of the ast
 */
typedef struct record {
	ast_category token;
	int line;
	op_t op;
	type_t type;
	void * value;
} record;

record * astRecord(ast_category, int line, op_t, void *);
void freeastRecord(record *);

#endif /* __AST_RECORD__H__ */
