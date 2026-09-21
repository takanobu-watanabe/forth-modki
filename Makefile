CFLAGS = -Wall -Wextra -std=c11 -g

UNITTEST = unittest
TARGET   = forth_modoki

# 両方のビルドで共有するソース（main を持たないもの）
LIB_SRCS  = parser.c stack.c eval.c dict.c element.c primitives.c
TEST_SRCS = test_main.c $(LIB_SRCS)
MAIN_SRCS = main.c $(LIB_SRCS)
HDRS      = parser.h stack.h eval.h dict.h element.h primitives.h test_util.h

.PHONY: all test clean

# 引数なしの make で両方ビルドする
all: $(UNITTEST) $(TARGET)

# テストを走らせる（バイナリが最新でも必ず実行される）
test: $(UNITTEST)
	./$(UNITTEST)

# テストバイナリ（main は test_main.c のもの）
$(UNITTEST): $(TEST_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(UNITTEST) $(TEST_SRCS)

# 本体（main は main.c のもの）
$(TARGET): $(MAIN_SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_SRCS)

clean:
	rm -rf $(UNITTEST) $(TARGET) test *.dSYM
