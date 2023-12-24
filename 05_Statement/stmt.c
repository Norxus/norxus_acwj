#include "data.h"
#include "decl.h"


void statements(void){
    struct ASTnode *tree;
    int reg;

    // 循环以解析多个表达式
    while (1){
        // 第一个字符先匹配print
        match(T_PRINT, "print");

        // 解析下面的表达式并生成AST，遇到;就不再解析
        tree = binexpr(0);
        // 将AST 转化为 字节码，同时返回结果所在的寄存器
        reg = genAST(tree);
        // 打印计算结果
        genprintint(reg);
        // 释放所有寄存器
        genfreeregs();

        // 扫描下一个字符，一定是;，如果不是则说明语法错误，那么,如果遇到EOF那么就会停下来
        semi();
        if (Token.token == T_EOF){
            return;
        }
    }
}
