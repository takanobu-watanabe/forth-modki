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

// てっぺんから n 個下の値を、取り除かずに返す。
// n = 0 なら stack_top と同じ。
Element stack_index(Stack *stack, int n);

bool stack_is_empty(Stack *stack);
int stack_size(Stack *stack);
void stack_dup(Stack *stack);
void stack_exch(Stack *stack);

#endif
