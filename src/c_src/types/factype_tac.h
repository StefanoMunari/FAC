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
	/** sum */
	TAC_SUM,
	/** difference */
	TAC_DIFF,
	/** multiplication */
	TAC_MULT,
	/** division */
	TAC_DIV,
	/** logical negation */
	TAC_NOT,
	/** logical conjunction */
	TAC_AND,
	/** logical disjunction */
	TAC_OR,
	/** logical implication */
	TAC_IMPLY,
	/** logical biconditional */
	TAC_IFF,
	/** logical xor */
	TAC_XOR,
	/** less than */
	TAC_LT,
	/** less or equal to */
	TAC_LEQ,
	/** greater or equal to */
	TAC_GEQ,
	/** greater than */
	TAC_GT,
	/** equals */
	TAC_EQ,
	/** not equals */
	TAC_NEQ,
	/** sign plus */
	TAC_PLUS,
	/** sign minus */
	TAC_MINUS,
	/** print-statement */
	TAC_PRINT,
	/** assignment */
	TAC_ASSIGNMENT,
	/** label */
	TAC_LABEL,
	/** goto instruction */
	TAC_GOTO,
	/** artificial label used to make "type checking" on the enum, as suggested
	 * by this 
	 * <a href="https://stackoverflow.com/questions/9161863/
	 * how-to-check-if-an-enum-variable-is-valid"> stackoverflow link </a>
	 */
	TAC_END_MARKER
} tac_op;

#endif /*_FACTYPE_TAC_H_*/
