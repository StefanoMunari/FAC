/**
 * @file
 * @brief Operator Types of the Abstract Syntax Tree (AST) -
 *	represents any operator type of the AST
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __AST_OPTYPE__H__
#define __AST_OPTYPE__H__
#include "factype_ast.h"
/**
 * @brief operator types of the AST
 */
typedef union {
	aop_t aop;
	bop1_t bop1;
	bop2_t bop2;
	relop_t relop;
} ast_op_t;

#endif /* __AST_OPTYPE__H__ */