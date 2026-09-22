#ifndef EVAL_H
#define EVAL_H

#include "parser.h"   // CharSource
#include "stack.h"    // Stack
#include "dict.h"

// src からトークンを読み、尽きるまで評価する。結果は st に残る。
void eval(CharSource *src, Stack *st, Dict *dict);

// ELEM_EXEC_NAME なら辞書を引いて実行/積む、それ以外はそのまま積む。
void eval_element(Element e, Stack *st, Dict *dict);

// 実行可能配列の中身を、先頭から順に eval_element で評価する。
void eval_exec_array(ExecArray *ea, Stack *st, Dict *dict);


#endif
