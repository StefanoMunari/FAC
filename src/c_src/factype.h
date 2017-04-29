#ifndef _FACTYPE_H_
#define _FACTYPE_H_

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
	IFF, 
	AND, 
	OR, 
	IMPLY, 
	XOR
} bop2_t;

typedef enum
{
	LT, 
	LEQ, 
	EQ, 
	GEQ, 
	GT, 
	NEQ
} relop_t;

typedef enum 
{
	BOOL_T,
	FRACT_T	
} type_t;

#endif/*_FACTYPE_H_*/
