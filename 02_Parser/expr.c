#include "defs.h"
#include "data.h"
#include "decl.h"

// 用户获取整型ast节点
static struct ASTnode *primary(void){
    struct ASTnode *n;

    // 对于一个整型常量，使其成为一个叶子节点并扫描下一个token。
    // 否则报语法错误。
    switch (Token.token) {
        case T_INTLIT:
            n = mkastleaf(A_INTLIT,Token.intvalue);
            // 继续扫描&解析全局变量token中的内容，相当于会把指针移动到下一个token
            scan(&Token);
            return (n);
        default:
            fprintf(stderr,"syntax error on line%d\n", Line);
            exit(1);
    }
}

// 把一个token转化为一个语法树节点，简单的类型转换。
int arithop(int tok){
    switch (tok) {
        case T_PLUS:
            return (A_ADD);
        case T_MINUS:
            return (A_SUBTRACT);
        case T_STAR:
            return (A_MULTIPLY);
        case T_SLASH:
            return (A_DIVIDE);
        default:
            fprintf(stderr, "unknown token in arithop() on line %d\n", Line);
            exit(1);
    }
}

// 返回一个语法树，其根节点为二进制操作符。
struct  ASTnode *binexpr(void){
    struct ASTnode *n, *left, *right;
    int nodetype;

    // 获取一个整型节点
    left = primary();

    // 如果整型节点后面没有token了，直接返回
    if(Token.token == T_EOF){
        return left;
    }

    // 获取新节点的语法树节点类型，只是在做简单的类型转换
    nodetype = arithop(Token.token);

    // 扫描并解析完成当前token，并移动指针到下一个token
    scan(&Token);

    // 递归获取操作符右边的树节点
    right = binexpr();

    // 完成递归得到整个树。
    n = mkastnode(nodetype, left,right,0);
    return (n);
}
