/**
 * @brief definition of the fract type, internal representation of the operations,
 * and the internal representation of the type of the variables
 * 
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef _FACTYPE_H_
#define _FACTYPE_H_
#include <stdbool.h>

/**
 * struct representing a fract instance
 */
typedef struct fract_t
{
	/** numerator */
	int num;
	/** denominator */
	int den;
} fract_t;

/**
 * Enumeration that represents the possible tokens in the lexical phase.
 */
typedef enum op_t
{
	/* AOP_0 */
	/** sum */
	SUM,
	/** difference */
	DIFF,
	/* AOP_1 */
	/** mutiplication */
	MULT,
	/** division */
	DIV,
	/* BOP1 */
	/** logical negation */
	NOT,
	/* BOP2 */
	/** logical and */
	AND,
	/** logical or */
	OR,
	/** logical implication */
	IMPLY,
	/** logical biconditional */
	IFF,
	/** logical xor */
	XOR,
	/* RELOP_0 */
	/** less than */
	LT,
	/** less than or equal */
	LEQ,
	/** greater than or equal */
	GEQ,
	/** greater than */
	GT,
	/* RELOP_1 */
	/** equals */
	EQ,
	/** not equals */
	NEQ
} op_t;

/**
 * Enumeration used to encode the possible type of the variables
 */
typedef enum type_t
{
	/** boolean type */
	BOOL_T,
	/** fract type */
	FRACT_T
} type_t;

#endif /*_FACTYPE_H_*/
