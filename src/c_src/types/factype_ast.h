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
/**
* @brief tokens of the Abstract Syntax Tree (AST).
* It represents the AST category of tokens
*/
typedef enum {
	/** represents a variable identifier */
	AST_ID,
	/** represents a fract literal */
	AST_FRACT,
	/** represents a bool literal */
	AST_BOOL,
	/** represents a fract operation with arity 1 */
	AST_AOP1,
	/** represents a fract operation with arity 2*/
	AST_AOP2,
	/** represents a boolean operation with arity 1*/
	AST_BOP1,
	/** represents a boolean operation with arity 2*/
	AST_BOP2,
	/** represents:
	 * - XOR corresponds to NEQ on boolean variables
	 * - IFF corresponds to EQ on boolean variables */
	AST_BOP_RELOP,
	/** represents a relational operator (<=, >=, <, >, ==, !=) */
	AST_RELOP,
	/** represents the function print */
	AST_PRINT,
	/** represents the while */
	AST_WHILE,
	/** represents an assignment */
	AST_ASSIGNMENT,
	/** represents a declaration */
	AST_DECLARATION,
	/** represents both the if with one and the if with two children */
	AST_IF,
	/** artificial label used to make "type checking" on the enum:
	 * @link  https://stackoverflow.com/questions/9161863/how-to-check-if-an-enum-variable-is-valid
	 */
	AST_END_MARKER
} ast_category;

#endif /*_FACTYPE_AST_H_*/
