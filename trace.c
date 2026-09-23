// 学習用: インタプリタの内部でいま何が起きているかを表示する。
//
//   make trace-run                      デフォルトの例をトレースする
//   ./trace "/sq { dup mul } def 7 sq"  任意のコードをトレースする
//
// eval / eval_element / eval_exec_array と同じ処理を、途中経過を
// 出しながらなぞっている（本体には手を入れず、別実装で追いかける）。

#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "stack.h"
#include "dict.h"
#include "eval.h"
#include "element.h"
#include "exec_array.h"
#include "primitives.h"

static CharSource src_from(const char *s) {
    CharSource src;

    src.fp = fmemopen((void *)s, strlen(s), "r");
    return src;
}

static void ind(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

// 1行に収まる短い表記。EXEC_ARRAY は中身を展開しない。
static void brief(const Element *e) {
    switch (e->type) {
    case ELEM_INT:          printf("INT %d", e->u.ival);                          break;
    case ELEM_LITERAL_NAME: printf("LITERAL_NAME \"%s\"", e->u.name);             break;
    case ELEM_EXEC_NAME:    printf("EXEC_NAME \"%s\"", e->u.name);                break;
    case ELEM_PRIMITIVE:    printf("PRIMITIVE <C関数>");                          break;
    case ELEM_EXEC_ARRAY:   printf("EXEC_ARRAY(count=%d)", e->u.exec_array->count); break;
    }
}

// 中身を展開して表示する。ネストした EXEC_ARRAY は段を下げて再帰表示。
static void detail(const Element *e, int depth) {
    brief(e);
    if (e->type != ELEM_EXEC_ARRAY) {
        return;
    }
    printf("\n");
    for (int i = 0; i < e->u.exec_array->count; i++) {
        ind(depth + 1);
        printf("items[%d] = ", i);
        detail(&e->u.exec_array->items[i], depth + 1);
        if (e->u.exec_array->items[i].type != ELEM_EXEC_ARRAY) {
            printf("\n");
        }
    }
}

// Stack は中身が隠されているので、全部 pop して表示し、積み直す。
static void dump_stack(Stack *st, int depth) {
    Element tmp[64];
    int n = stack_size(st);

    for (int i = n - 1; i >= 0; i--) {
        tmp[i] = stack_pop(st);
    }
    ind(depth);
    printf("スタック: ");
    if (n == 0) {
        printf("[空]");
    }
    for (int i = 0; i < n; i++) {
        printf("[ ");
        brief(&tmp[i]);
        printf(" ]");
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        stack_push(st, tmp[i]);
    }
}

static const char *token_type_name(TokenType t) {
    switch (t) {
    case TOKEN_INT:          return "INT";
    case TOKEN_EXEC_NAME:    return "EXEC_NAME";
    case TOKEN_LITERAL_NAME: return "LITERAL_NAME";
    case TOKEN_OPEN_BRACE:   return "OPEN_BRACE";
    case TOKEN_CLOSE_BRACE:  return "CLOSE_BRACE";
    default:                 return "EOF";
    }
}

static void trace_exec_array(ExecArray *ea, Stack *st, Dict *dict, int depth);

// eval_element と同じことを、途中経過を出しながらやる
static void trace_element(Element e, Stack *st, Dict *dict, int depth) {
    ind(depth);
    printf("評価: ");
    brief(&e);
    printf("\n");

    if (e.type == ELEM_EXEC_NAME) {
        Element found;

        if (dict_get(dict, e.u.name, &found)) {
            ind(depth);
            printf("  辞書を引く → ");
            brief(&found);
            printf("\n");
            switch (found.type) {
            case ELEM_PRIMITIVE:
                ind(depth);
                printf("  C関数なので呼ぶ\n");
                found.u.fn(st, dict);
                break;
            case ELEM_EXEC_ARRAY:
                ind(depth);
                printf("  実行可能配列なので中身を順に実行する\n");
                trace_exec_array(found.u.exec_array, st, dict, depth + 1);
                break;
            default:
                ind(depth);
                printf("  値なので積む\n");
                stack_push(st, found);
                break;
            }
        } else {
            ind(depth);
            printf("  辞書に無い → unknown word: %s\n", e.u.name);
        }
    } else {
        ind(depth);
        printf("  値なので積む\n");
        stack_push(st, e);
    }
    dump_stack(st, depth + 1);
}

// eval_exec_array と同じことを、途中経過を出しながらやる
static void trace_exec_array(ExecArray *ea, Stack *st, Dict *dict, int depth) {
    for (int i = 0; i < ea->count; i++) {
        ind(depth);
        printf("items[%d]:\n", i);
        trace_element(ea->items[i], st, dict, depth + 1);
    }
}

// eval と同じことを、途中経過を出しながらやる
static void trace_eval(const char *code, Stack *st, Dict *dict) {
    CharSource src = src_from(code);
    Token t;
    int n = 0;

    printf("入力: %s\n\n", code);
    while (parse_one(&src, &t)) {
        n++;
        switch (t.type) {
        case TOKEN_INT:
            printf("[%d] トークン: %d  (%s)\n", n, t.u.ival, token_type_name(t.type));
            printf("    値なので積む\n");
            stack_push(st, element_int(t.u.ival));
            break;

        case TOKEN_LITERAL_NAME:
            printf("[%d] トークン: /%s  (%s)\n", n, t.u.name, token_type_name(t.type));
            printf("    名前そのものを積む（実行しない）\n");
            stack_push(st, element_literal_name(t.u.name));
            break;

        case TOKEN_OPEN_BRACE: {
            ExecArray *ea;
            Element e;

            printf("[%d] トークン: {  (%s)\n", n, token_type_name(t.type));
            printf("    compile_exec_array で } まで読み取る（実行はしない）\n");
            ea = compile_exec_array(&src);
            e = element_exec_array(ea);
            printf("    できたもの: ");
            detail(&e, 1);
            printf("    それを1個の値として積む\n");
            stack_push(st, e);
            break;
        }

        case TOKEN_EXEC_NAME:
            // def も含めて特別扱いは無い。すべて辞書を引いて振り分ける。
            printf("[%d] トークン: %s  (%s)\n", n, t.u.name, token_type_name(t.type));
            trace_element(element_exec_name(t.u.name), st, dict, 1);
            printf("\n");
            continue;

        default:
            printf("[%d] トークン: (%s) — 無視\n", n, token_type_name(t.type));
            break;
        }
        dump_stack(st, 2);
        printf("\n");
    }
    fclose(src.fp);
}

static void run_trace(const char *code) {
    Stack *st = stack_new(100);
    Dict *dict = dict_new();

    register_primitives(dict);

    printf("=========================================\n");
    trace_eval(code, st, dict);
    printf("最終的なスタック:\n");
    dump_stack(st, 1);
    printf("\n");

    stack_free(st);
    dict_free(dict);
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        run_trace(argv[1]);
        return 0;
    }

    // 引数なしのときは、代表的な3例をトレースする
    run_trace("/double { 2 mul } def 5 double");
    run_trace("/inner { 2 mul } def /outer { 5 inner } def outer");
    run_trace("{ 1 { 2 3 add } }");
    return 0;
}
