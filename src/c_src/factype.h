#ifndef _FACTYPE_H_
#define _FACTYPE_H_
#include <stdbool.h>

typedef struct fract_t
{
	int num;
	int den;
} fract_t;

typedef struct value_t
{
	fract_t fract_val;
	bool bool_val;

} value_t;

typedef enum
{
	SUM,
	DIFF
} aop0_t;

typedef enum
{
	MULT,
	DIV
} aop1_t;

typedef enum
{
	NOT
} bop1_t;

typedef enum
{
	AND
} bop2_0_t;

typedef enum
{
	OR
} bop2_1_t;

typedef enum
{
	IMPLY
} bop2_2_t;

typedef enum
{
	IFF,
	XOR
} bop2_3_t;

typedef enum
{
	LT,
	LEQ,
	GEQ,
	GT
} relop0_t;

typedef enum
{
	EQ,
	NEQ
} relop1_t;

typedef enum
{
	BOOL_T,
	FRACT_T
} type_t;

#endif /*_FACTYPE_H_*/
