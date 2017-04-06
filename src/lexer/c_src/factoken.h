#ifndef _FACTOKEN_H_
#define _FACTOKEN_H_

typedef enum 
{
	ASSIGNMENT
} assignment_tk;

typedef enum 
{
	ID
} id_tk;

typedef enum 
{
	WHILE,
	IF,
	ELSE,
	SKIP,
	PRINT
} keyword_tk;

typedef enum 
{
	SEPARATOR,
	L_DEL_EXPR,
	R_DEL_EXPR,
	L_DEL_SCOPE,
	R_DEL_SCOPE
} punctuation_tk;

typedef enum 
{
	AOP0,
	AOP1
} aop_tk;

typedef enum 
{
	BOP1,
	BOP2
} bop_tk;

typedef enum 
{
	RELOP
} relop_tk;

typedef enum 
{
	TYPE
} type_tk;

#endif/*_FACTOKEN_H_*/