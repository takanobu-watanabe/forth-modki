#ifndef EXEC_ARRAY_H
#define EXEC_ARRAY_H

#include "element.h"

// { ... } をコンパイルした結果。flexible array member (items[]) を使うので、
// struct 本体と要素の配列が1回の malloc で確保される。
struct ExecArray {
    int count;
    Element items[];   // 構造体の最後にしか書けない。サイズは malloc 時に決める。
};

// count 個の要素を持てる ExecArray を確保する（items の中身は未初期化）。
ExecArray *exec_array_new(int count);
void exec_array_free(ExecArray *ea);

#endif
