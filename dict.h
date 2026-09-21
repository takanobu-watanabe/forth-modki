#ifndef DICT_H
#define DICT_H

#include <stdbool.h>

// 中身は dict.c に隠す（使う側は Dict* しか扱えない）
typedef struct Dict Dict;

Dict *dict_new(void);
void dict_free(Dict *dict);

// name に value を登録する。同じ name が既にあれば上書きする。
void dict_put(Dict *dict, const char *name, int value);

// name を引く。見つかれば *out_value に書き込んで true、無ければ false。
bool dict_get(Dict *dict, const char *name, int *out_value);

#endif
