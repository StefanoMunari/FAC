#include "stack.h"

extern void yyerror(char *, ...);

void init(stack_t * s)
{
    s->size = 0;
}

void * top(stack_t * s)
{
    if (s->size == 0)
        yyerror("Stack::Top - the stack is empty");
    return s->data[s->size-1];
}

void push(stack_t * s, void * data)
{
    if (s->size >= STACK_MAX)
        yyerror("Stack::Push - the stack is full");
    s->data[s->size++] = data;
}

void pop(stack_t * s)
{
    if (s->size == 0)
        yyerror("Stack::Pop - the stack is empty");
    s->size--;
}

bool isEmpty(stack_t * s){
    return (s->size == 0);
}
