#include "defs.h"
#include "data.h"
#include "decl.h"

// 用户获取整型ast节点，此时它默认全局变量Token存储着一个整数，如果不是整数则直接报错误
// 解析成功之后，再将指针往后移动一格
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

// 操作符优先级, 分别对应T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
static int OpPrec[] = {0,10,10,20,20,0};

static int op_precedence(int tokentype){
    int prec = OpPrec[tokentype];
    if (prec == 0){
        fprintf(stderr,"syntax error on line %d, token %d\n",Line, tokentype);
        exit(1);
    }
    return (prec);
}

// 返回一个语法树，其根节点为二进制操作符。
// 参数ptp是指前一个token的优先级
// 递归时，每层至少解析一个整数+该整数后面的符号（包括EOF）
// 二元运算符左右两边优先级相同，整体计算过程采用后序遍历
struct  ASTnode *binexpr(int ptp){
    struct ASTnode *left, *right;
    int tokentype;

    // 获取一个整型节点
    left = primary();

    // 如果整型节点后面没有token了，直接返回
    tokentype = Token.token;
    if(tokentype == T_EOF){
        return left;
    }

    // 判断当前token的优先级是否比上一个token的优先级更高
    // 因为上面调用了primary()函数，因此这里是操作符
    // 优先级更高进入递归
    // AST结构上下层之间代表着计算的先后顺序，同层级的左右关系代表着从左到右顺序计算
    while(op_precedence(tokentype) > ptp){
        // 移动token指针
        scan(&Token);

        // 递归运行组装
        right = binexpr(OpPrec[tokentype]);

        // 组装AST树
        left = mkastnode(arithop(tokentype), left,right,0);

        tokentype = Token.token;
        if (tokentype == T_EOF){
            return (left);
        }
    }
    // 如果本字符相比于上一个字符优先级更低则直接返回
    return (left);
}
