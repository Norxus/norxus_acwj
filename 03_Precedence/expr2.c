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

// 表达式加法计算逻辑
struct ASTnode *additive_expr(void);

// 乘除运算，返回一个ASTnode
// 这是获取一个乘除表达式，如果遇到其他符号比如'+'、'-'，那么就会停止解析
struct ASTnode *multiplicative_expr(void){
    struct ASTnode *left, *right;
    int tokentype;

    // 获取运算符左边的整数
    left = primary();

    // 取完左边的整数之后，在看现在token指针里面是不是已经指向结尾了
    // 如果已经指向了结尾那么就直接返回这个整数。
    tokentype = Token.token;
    if (tokentype == T_EOF){
        return left;
    }

    // 如果字符指针指向的是 '*' 或者 '/'
    while ((tokentype == T_STAR)|| (tokentype == T_SLASH)){
        // 继续扫描下一个token
        scan(&Token);
        // 获取下一个整形
        right = primary();
        // 将左右节点+本token组合成为一个ASTnode，因为这是一个运算符节点所以值为0
        // 将本次遍历得到整体作为一个左节点，下一次循环会讲下一个整形作为右节点。
        left = mkastnode(arithop(tokentype),left,right,0);

        // 获取完一个新的字符之后，查看还有没有其他字符
        // 如果没有其他字符那么就直接跳出
        tokentype = Token.token;
        if (tokentype == T_EOF){
            break;
        }
    }

    // 返回构建完成的节点树
    return (left);
}

/*
 * 2*3 + 4*5
      +
    /   \
   *     *
 2   3  4  5
 */
// 实际上对于additive_expr来说，乘除运算块实际上是一个整体，和普通的数字没有太多区别
// 都是作为AST的节点而存在的。在它眼中根本就没有乘除运算（乘除运算+常量是一个整体，是一个计算结果），只有加减运算
// 实际上代码中，是将常量也看作multiplicative_expr函数的一种返回结果。
// 整体上，这个构建思路就是不停的构建加减AST节点，如果后续还有运算，将其作为新的加减AST的左节点
struct ASTnode *additive_expr(void){
    struct ASTnode *left, *right;
    int tokentype;

    // 获取左节点
    // 这里先调用乘除语法树的构建，因为乘除优于加减
    // 这是先获取第一个乘除表达式，初始化左节点
    left = multiplicative_expr();

    // 获取完一个新的字符之后，查看还有没有其他字符
    // 如果没有其他字符那么就直接跳出
    tokentype = Token.token;
    if (tokentype == T_EOF){
        return (left);
    }

    // 递归构建AST
    while (1){
        // 获取下一个整形数据
        scan(&Token);

        // 获取下一个乘除表达式
        right = multiplicative_expr();

        // 将上一个乘除表达式和本次得到的乘除表达式组合成为一个新的优先级树
        // 因为解析计算过程是从左节点到右节点的，因此存放节点的位置表示了乘除计算的先后顺序
        left = mkastnode(arithop(tokentype),left,right,0);

        tokentype = Token.token;
        if (tokentype == T_EOF){
            return left;
        }

    }

    return (left);

}

// 返回一个语法树，其根节点为二进制操作符。
struct  ASTnode *binexpr(int n){
    return (additive_expr());
}
