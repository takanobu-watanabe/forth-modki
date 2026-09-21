#include "primitives.h"

#include "stack.h"
#include "element.h"
#include <assert.h>

// ここに prim_add などの PrimitiveFn を実装する
void prim_add(Stack *st) {
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival + b.u.ival));
}

void prim_sub(Stack *st) {
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival - b.u.ival));
}

void prim_mul(Stack *st) {
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival * b.u.ival));
}

void prim_div(Stack *st) {
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival / b.u.ival));
}

void prim_dup(Stack *st) {
    stack_dup(st);
}

void prim_pop(Stack *st) {
    stack_pop(st);
}

void register_primitives(Dict *dict) {
    // ここで dict_put(dict, "add", element_primitive(prim_add)); のように登録する
    dict_put(dict, "add", element_primitive(prim_add));
    dict_put(dict, "sub", element_primitive(prim_sub));
    dict_put(dict, "mul", element_primitive(prim_mul));
    dict_put(dict, "div", element_primitive(prim_div));
    dict_put(dict, "dup", element_primitive(prim_dup));
    dict_put(dict, "pop", element_primitive(prim_pop));
}
