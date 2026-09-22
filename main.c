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

    // 空のプログラム（あるいは値を残さないプログラム）を渡されたときに
    // 空スタックを pop しないようにする
    if (stack_is_empty(st)) {
        printf("result: (スタックは空)\n");
    } else {
        Element result = stack_pop(st);

        switch (result.type) {
        case ELEM_INT:
            printf("result: %d\n", result.u.ival);
            break;
        case ELEM_LITERAL_NAME:
            printf("result: /%s\n", result.u.name);
            break;
        case ELEM_EXEC_NAME:
            printf("result: %s\n", result.u.name);
            break;
        case ELEM_EXEC_ARRAY:
            printf("result: { ... }（%d要素の実行可能配列）\n",
                   result.u.exec_array->count);
            break;
        case ELEM_PRIMITIVE:
            printf("result: <組み込み関数>\n");
            break;
        }
    }
    fclose(fp); 
    stack_free(st);
    dict_free(dict);
    return 0;

}