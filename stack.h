#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "element.h"

// 中身は stack.c に隠す（使う側は Stack* しか扱えない）
typedef struct Stack Stack;

Stack* stack_new(int capacity);
void stack_free(Stack *stack);
void stack_push(Stack *stack, Element val);
Element stack_pop(Stack *stack);
Element stack_top(Stack *stack);
bool stack_is_empty(Stack *stack);
int stack_size(Stack *stack);
void stack_dup(Stack *stack);
void stack_exch(Stack *stack);

#endif
