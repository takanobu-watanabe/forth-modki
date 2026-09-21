#include "exec_array.h"

#include <stdlib.h>
#include <assert.h>

ExecArray *exec_array_new(int count) {
    // ここに実装を書く
    // ヒント: malloc するバイト数は「struct本体の分」+「Element が count 個分」
}

void exec_array_free(ExecArray *ea) {
    free(ea);
}
