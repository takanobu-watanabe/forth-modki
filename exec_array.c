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

ExecArray* compile_exec_array(CharSource *src) {
    Token out_token;
    int max_size = 256;
    int i = 0;
    
    ExecArray *array = exec_array_new(max_size);
    while (parse_one(src, &out_token)) {
        switch (out_token.type) {
            case TOKEN_INT:
                array->items[i] = element_int(out_token.u.ival);
                i++;
                break;
            case TOKEN_LITERAL_NAME:
                array->items[i] = element_literal_name(out_token.u.name);
                i++;
                break;
            case TOKEN_EXEC_NAME:
                array->items[i] = element_exec_name(out_token.u.name);
                i++;
                break;
            case TOKEN_CLOSE_BRACE:
                array->count = i;
                return array;
            default:
                break;
        }
    }
    return NULL;
}

