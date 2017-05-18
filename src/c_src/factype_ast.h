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
	AST_ID,
	AST_FRACT,
	AST_BOOL,
	AST_AOP1,
	AST_AOP2,
	AST_BOP1,
	AST_BOP2,
	AST_RELOP,
	AST_RELOP1,
	AST_PRINT,
	AST_WHILE,
	AST_ASSIGNMENT,
	AST_DECLARATION,
	AST_IF,
	AST_ELSE,
	AST_END_MARKER
} ast_category;

#endif /*_FACTYPE_AST_H_*/
