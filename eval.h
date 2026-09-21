#ifndef EVAL_H
#define EVAL_H

#include "parser.h"   // CharSource
#include "stack.h"    // Stack
#include "dict.h"

// src からトークンを読み、尽きるまで評価する。結果は st に残る。
void eval(CharSource *src, Stack *st, Dict *dict);

#endif
