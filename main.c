#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "stack.h"
#include "eval.h"
#include "primitives.h"
#include "exec_array.h"

int main(int argc, char **argv) {
    FILE *fp;
    CharSource src;
    Stack *st;
    Dict *dict = dict_new();

    // 演習8-1: items[] の分がサイズに含まれていないことの目視確認
    printf("sizeof(ExecArray) = %zu\n", sizeof(ExecArray));

    register_primitives(dict);

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file.ps>\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "cannot open: %s\n", argv[1]);
        return 1;
    }
    src.fp = fp;
    st = stack_new(100);
    eval(&src, st, dict);
    {
        Element result = stack_pop(st);
        if (result.type == ELEM_INT) {
            printf("result: %d\n", result.u.ival);
        } else {
            printf("result: /%s\n", result.u.name);
        }
    }
    fclose(fp); 
    stack_free(st);
    dict_free(dict);
    return 0;

}