/**
 * @file
 * @brief Types for Abstract Syntax Tree - reduced version of extended types
 *	defined in FACTYPE
 *
 *  We need only simple/uniform types  in the AST, i.e. not divided
 *	in different precedence classes. Indeed, the structure of the tree enforces
 *	(by design) precedence between its elements (nodes and leaves)
 * @see factype.h
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef _FACTYPE_AST_H_
#define _FACTYPE_AST_H_
#include "factype.h"
/* Arithmetic OPerator Type */
typedef enum
{
	AST_SUM,
	AST_DIFF,
	AST_MULT,
	AST_DIV
} aop_t;
/* Boolean OPerator Type */
typedef enum
{
	AST_AND,
	AST_OR,
	AST_IMPLY,
	AST_IFF,
	AST_XOR
} bop2_t;
/* RELational OPerator Type */
typedef enum
{
	AST_LT,
	AST_LEQ,
	AST_GEQ,
	AST_GT,
	AST_EQ,
	AST_NEQ
} relop_t;

/**
 * Arithmetic OPerator Type constructor for AST
 * @brief constructs arithmetic operators for the AST
 * @param a class of arithmetic operators used in the parser (a class type)
 * @param an arithmetic operator used in the parser (a value)
 * @return aop_t the arithmetic operator type used in the AST (a value)
 */
aop_t aop(int, int);
/**
 * constructor of Boolean OPerator Type with arity=2 for AST
 * @brief constructs boolean operators for the AST
 * @param a class of boolean operators used in the parser (a class type)
 * @param a boolean operator used in the parser (a value)
 * @return bop2_t the boolean operator type used in the AST (a value)
 */
bop2_t bop2(int, int);
/**
 * constructor of Relational OPerator Type with for AST
 * @brief constructs relational operators for the AST
 * @param a class of relational operators used in the parser (a class type)
 * @param a relational operator used in the parser (a value)
 * @return relop_t the relational operator type used in the AST (a value)
 */
relop_t relop(int, int);

#endif /*_FACTYPE_AST_H_*/
