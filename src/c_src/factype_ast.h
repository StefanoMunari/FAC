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
	ast_ID,
	ast_FRACT,
	ast_BOOL,
	ast_AOP,
	ast_BOP1,
	ast_BOP2,
	ast_RELOP,
	ast_RELOP1,
	ast_PRINT,
	ast_WHILE,
	ast_ASSIGNMENT,
	ast_DECLARATION,
	ast_SKIP,
	ast_IF,
	ast_ELSE,
	ast_END_MARKER
} ast_category;

#endif /*_FACTYPE_AST_H_*/
