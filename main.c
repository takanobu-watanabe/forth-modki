#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "stack.h"
#include "eval.h"

int main(int argc, char **argv) {
    FILE *fp;
    CharSource src;
    Stack *st;

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
    eval(&src, st);
    printf("result: %d\n", stack_pop(st));  
    fclose(fp); 
    stack_free(st); 
    return 0;

}