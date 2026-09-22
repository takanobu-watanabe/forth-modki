#include "primitives.h"

#include "stack.h"
#include "element.h"
#include <assert.h>

void prim_add(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival + b.u.ival));
}

void prim_sub(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival - b.u.ival));
}

void prim_mul(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival * b.u.ival));
}

void prim_div(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival / b.u.ival));
}

void prim_dup(Stack *st, Dict *dict) {
    (void)dict;
    stack_dup(st);
}

void prim_pop(Stack *st, Dict *dict) {
    (void)dict;
    stack_pop(st);
}

void prim_eq(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival == b.u.ival));
}

void prim_ne(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival != b.u.ival));
}

void prim_lt(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival < b.u.ival));
}

void prim_gt(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival > b.u.ival));
}

void prim_le(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival <= b.u.ival));
}

void prim_ge(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival >= b.u.ival));
}

void prim_exch(Stack *st, Dict *dict) {
    (void)dict;
    stack_exch(st);
}

void register_primitives(Dict *dict) {
    dict_put(dict, "add", element_primitive(prim_add));
    dict_put(dict, "sub", element_primitive(prim_sub));
    dict_put(dict, "mul", element_primitive(prim_mul));
    dict_put(dict, "div", element_primitive(prim_div));
    dict_put(dict, "dup", element_primitive(prim_dup));
    dict_put(dict, "pop", element_primitive(prim_pop));
    dict_put(dict, "eq", element_primitive(prim_eq));
    dict_put(dict, "ne", element_primitive(prim_ne));
    dict_put(dict, "lt", element_primitive(prim_lt));
    dict_put(dict, "gt", element_primitive(prim_gt));
    dict_put(dict, "le", element_primitive(prim_le));
    dict_put(dict, "ge", element_primitive(prim_ge));
    dict_put(dict, "exch", element_primitive(prim_exch));
}
