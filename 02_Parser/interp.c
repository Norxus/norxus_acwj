#include "defs.h"
#include "data.h"
#include "decl.h"


// ast操作符
static char *ASTop[] = {"+", "-","*","/"};


// 解析整个ast并返回一个具体的值
int interpretAST(struct ASTnode *n){
    int leftval, rightval;

    // 递归解析左右子树
    if(n->left){
        leftval = interpretAST(n->left);
    }

    if(n->right){
        rightval = interpretAST(n->right);
    }

    // 调试信息，展示本层将要进行的操作
    // 如果是整型，打印这个整型
    if(n->op == A_INTLIT){
        printf("int %d\n",n->intvalue);
    }else{
        printf("%d %s %d\n",leftval,ASTop[n->op],rightval);
    }

    switch (n->op) {
        case A_ADD:
            return (leftval + rightval);
        case A_SUBTRACT:
            return (leftval - rightval);
        case A_MULTIPLY:
            return (leftval * rightval);
        case A_DIVIDE:
            return (leftval / rightval);
        case A_INTLIT:
            // 如果本层只是一个整型节点，那么直接返回即可
            return (n->intvalue);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", n->op);
            exit(1);
    }
}