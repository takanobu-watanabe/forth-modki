#include "element.h"

#include <string.h>
#include <assert.h>

Element element_int(int val) {
    Element e;

    e.type = ELEM_INT;
    e.u.ival = val;
    return e;
}

Element element_literal_name(const char *name) {
    Element e;

    assert(strlen(name) < ELEMENT_NAME_MAX_SIZE);
    e.type = ELEM_LITERAL_NAME;
    strcpy(e.u.name, name);
    return e;
}

Element element_primitive(PrimitiveFn fn) {
    Element e;

    e.type = ELEM_PRIMITIVE;
    e.u.fn = fn;
    return e;
}

Element element_exec_array(ExecArray *ea) {
    Element e;

    e.type = ELEM_EXEC_ARRAY;
    e.u.exec_array = ea;
    return e;
}

Element element_exec_name(const char *name) {
    Element e;

    assert(strlen(name) < ELEMENT_NAME_MAX_SIZE);
    e.type = ELEM_EXEC_NAME;
    strcpy(e.u.name, name);
    return e;
}
