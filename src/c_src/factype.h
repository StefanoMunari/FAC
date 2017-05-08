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
	/* AOP0 */
	SUM,
	DIFF,
	/* AOP1 */
	MULT,
	DIV,
	/* BOP1 */
	NOT,
	/* BOP2 */
	AND,
	OR,
	IMPLY,
	IFF,
	XOR,
	/* RELOP_0 */
	LT,
	LEQ,
	GEQ,
	GT,
	/* RELOP_1 */
	EQ,
	NEQ
} op_t;

typedef enum
{
	BOOL_T,
	FRACT_T
} type_t;

#endif /*_FACTYPE_H_*/
