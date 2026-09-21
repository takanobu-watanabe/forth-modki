#include "exec_array.h"

#include <stdlib.h>
#include <assert.h>

ExecArray *exec_array_new(int count) {
    ExecArray *array = malloc(sizeof(ExecArray)+sizeof(Element)*count);
    assert(array != NULL);
    array->count = count;
    return array;
}

void exec_array_free(ExecArray *ea) {
    free(ea);
}
