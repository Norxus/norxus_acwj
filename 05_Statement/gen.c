#include "data.h"
#include "decl.h"


// 根据给定的AST,生成相应的字节码,返回结果所在的寄存器
int genAST(struct ASTnode *n){
    int leftreg, rightreg;

    if (n->left){
        leftreg = genAST(n->left);
    }
    if (n->right){
        rightreg = genAST(n->right);
    }

    switch (n->op) {
        case A_ADD:
            return (cgadd(leftreg,rightreg));
        case A_SUBTRACT:
            return (cgsub(leftreg,rightreg));
        case A_MULTIPLY:
            return (cgmul(leftreg,rightreg));
        case A_DIVIDE:
            return (cgdiv(leftreg,rightreg));
        case A_INTLIT:
            // int类型就直接加载到寄存器就行
            return (cgload(n->intvalue));
        default:
            fprintf(stderr, "Unknown AST operator %d\n", n->op);
            exit(1);
    }
}

void generatecode(struct ASTnode *n){
    int reg;

    // 写前置指定，主要定义一下print函数
    cgpreamble();
    // 计算AST树
    reg = genAST(n);
    // 打印reg中的内容
    cgprintint(reg);
    // 后置处理逻辑，就是直接返回
    cgpostamble();

}

void genpreamble() {
    cgpreamble();
}
void genpostamble() {
    cgpostamble();
}
void genfreeregs() {
    freeall_registers();
}
void genprintint(int reg) {
    cgprintint(reg);
}
