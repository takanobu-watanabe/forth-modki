CFLAGS = -Wall -Wextra -std=c11 -g

UNITTEST = unittest
UNITTEST_ASAN = unittest_asan
TARGET   = forth_modoki
TRACE    = trace

# 両方のビルドで共有するソース（main を持たないもの）
LIB_SRCS  = parser.c stack.c eval.c dict.c element.c primitives.c exec_array.c
TEST_SRCS = test_main.c $(LIB_SRCS)
MAIN_SRCS = main.c $(LIB_SRCS)
TRACE_SRCS = trace.c $(LIB_SRCS)
HDRS      = parser.h stack.h eval.h dict.h element.h primitives.h exec_array.h test_util.h

.PHONY: all test test-asan trace-run clean

# 引数なしの make で両方ビルドする
all: $(UNITTEST) $(TARGET) $(TRACE)

# テストを走らせる（バイナリが最新でも必ず実行される）
test: $(UNITTEST)
	./$(UNITTEST)

# テストバイナリ（main は test_main.c のもの）
$(UNITTEST): $(TEST_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(UNITTEST) $(TEST_SRCS)

# AddressSanitizer 版。メモリ破壊（領域外アクセス・二重free・リーク）を検出する。
# 通常のテストが通っていても、はみ出した書き込みは見逃されることがあるので、
# malloc/realloc まわりを触ったときはこちらも走らせる。実行速度は2倍程度遅い。
test-asan: $(TEST_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -fsanitize=address -g -o $(UNITTEST_ASAN) $(TEST_SRCS)
	./$(UNITTEST_ASAN)

# 本体（main は main.c のもの）
$(TARGET): $(MAIN_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_SRCS)

clean:
	rm -rf $(UNITTEST) $(UNITTEST_ASAN) $(TARGET) $(TRACE) test *.dSYM

# 内部データ構造を表示するトレースプログラム（学習用）
$(TRACE): $(TRACE_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TRACE) $(TRACE_SRCS)

trace-run: $(TRACE)
	./$(TRACE)
