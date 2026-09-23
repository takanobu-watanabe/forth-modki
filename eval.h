#ifndef EVAL_H
#define EVAL_H

#include "parser.h"   // CharSource
#include "stack.h"    // Stack
#include "dict.h"
#include "cont.h"     // ContStack

// src からトークンを読み、尽きるまで評価する。結果は st に残る。
void eval(CharSource *src, Stack *st, Dict *dict);

// ELEM_EXEC_NAME なら辞書を引いて実行/積む、それ以外はそのまま積む。
void eval_element(Element e, Stack *st, Dict *dict);

// 実行可能配列の中身を、先頭から順に eval_element で評価する。
void eval_exec_array(ExecArray *ea, Stack *st, Dict *dict);

// --- ここから継続スタック版（11章）-----------------------------------
// 上の eval_element / eval_exec_array は Cの再帰で実行可能配列を辿るが、
// こちらは実行可能配列に出会っても呼ばずに継続スタックへ積むだけにする。
// 実際に辿るのは run の while ループ。

// 1つの Element を評価する。実行可能配列なら contstack_push するだけ。
void eval_element_vm(Element e, Stack *st, ContStack *cs, Dict *dict);

// 継続スタックが空になるまで、てっぺんのフレームを1ステップずつ進める。
void run(ContStack *cs, Stack *st, Dict *dict);

#endif
