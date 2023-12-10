#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// 字符类型
enum {
    T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
};

// 字符
struct token{
    int token;
    int intvalue;
};

// ast树节点类型，`intlit` 是 "integer literal" 的缩写，它表示整数文字，也就是整数常量。
enum {
    A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
};

// ast（Abstract Syntax Tree）结构
struct ASTnode{
    // 运算符
    int op;
    // 运算符左右两边的数/表达式
    struct ASTnode *left;
    struct ASTnode *right;
    // 整数值，A_INTLIT需要使用
    int intvalue;
};