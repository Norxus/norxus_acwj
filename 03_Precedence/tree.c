#include "defs.h"
#include "data.h"
#include "decl.h"

// 创建一个普通的ast节点
struct ASTnode *mkastnode(int op, struct ASTnode *left,
        struct ASTnode *right, int intvalue){
    struct ASTnode *n;

    // 创建一个ast节点，先分配内存
    n = (struct ASTnode *) malloc(sizeof(struct ASTnode));
    if(n == NULL){
        fprintf(stderr, "Unable to malloc mkastnode()\n");
        exit(1);
    }

    // 然后真正的进入创建流程，即赋值
    n->op = op;
    n->left = left;
    n->right = right;
    n->intvalue = intvalue;

    return (n);
}

// 创建一个ast叶子节点
struct ASTnode *mkastleaf(int op, int intvalue){
    return (mkastnode(op,NULL,NULL,intvalue));
}

// 创建一个一元节点，只有一个孩子，默认放在左节点
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue) {
    return (mkastnode(op, left, NULL, intvalue));
}