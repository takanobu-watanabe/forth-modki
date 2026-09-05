#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    FILE *fp;
} CharSource;

int  cl_getc(CharSource *src);
void cl_ungetc(CharSource *src, int c);

// 戻り値: 数字を1つ以上読めたら true、読めなかった（EOF等）なら false
// 読み取った整数値は *out に書き込む
bool parse_int(CharSource *src, int *out);