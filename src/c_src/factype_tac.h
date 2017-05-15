/**
 * @file
 * @brief Types for Three Address Code (TAC)
 *
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef _FACTYPE_TAC_H_
#define _FACTYPE_TAC_H_
/**
* @brief TAC operators
* @see tac.h
*/
typedef enum {
	TAC_SUM,
	TAC_DIFF,
	TAC_MULT,
	TAC_DIV,
	TAC_NOT,
	TAC_AND,
	TAC_OR,
	TAC_IMPLY,
	TAC_IFF,
	TAC_XOR,
	TAC_LT,
	TAC_LEQ,
	TAC_GEQ,
	TAC_GT,
	TAC_EQ,
	TAC_NEQ,
	TAC_FRACT,
	TAC_BOOL,
	TAC_AOP1,
	TAC_AOP2,
	TAC_BOP1,
	TAC_BOP2,
	TAC_RELOP,
	TAC_PRINT,
	TAC_WHILE,
	TAC_ASSIGNMENT,
	TAC_IF,
	TAC_ELSE
} tac_op;

#endif /*_FACTYPE_TAC_H_*/
