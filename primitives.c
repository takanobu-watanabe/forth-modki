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

void register_primitives(Dict *dict) {
    // ここで dict_put(dict, "add", element_primitive(prim_add)); のように登録する
    dict_put(dict, "add", element_primitive(prim_add));
}
