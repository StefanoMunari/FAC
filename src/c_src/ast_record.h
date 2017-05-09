/**
 * @file
 * @brief Record of the Abstract Syntax Tree (AST) -
 *	represents the data stored in each node of the AST
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __AST_RECORD__H__
#define __AST_RECORD__H__
#include "factype.h"
#include "factype_ast.h"
/**
 * @brief record of the AST
 */
typedef struct record {
	AST_category token;
	op_t op;
	type_t type;
	void * value;
} record;

record * ASTRecord(AST_category, op_t, void *);
void freeASTRecord(record *);

#endif /* __AST_RECORD__H__ */
