#ifndef _FACTYPE_AST_H_
#define _FACTYPE_AST_H_
#include "factype.h"

typedef enum
{
	SUM,
	DIFF,
	MULT,
	DIV
} aop_t;

typedef enum
{
	NOT
} bop1_t;

typedef enum
{
	NOT,
	AND,
	OR,
	IMPLY,
	IFF,
	XOR
} bop2_t;

typedef enum
{
	LT,
	LEQ,
	GEQ,
	GT,
	EQ,
	NEQ
} relop_t;

#endif /*_FACTYPE_AST_H_*/