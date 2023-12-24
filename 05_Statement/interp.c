#include "data.h"
#include "decl.h"


// ast操作符
static char *ASTop[] = {"+", "-","*","/"};


// 解析整个ast并返回一个具体的值
// 先计算左子树再计算右子树，最后将左右结果再进行计算，类似于后序遍历的思想，左右节点的优先级相同
// 同一层级从左到右代表了计算顺序从左到右，上下层之间才代表优先级，下层节点要先于上层节点先进行计算
// 将不同优先级的先后计算用不同层级的计算表示，层级越下计算优先级越高，将同一个优先级的先后顺序计算用左右节点
// 来表示，这样二元计算的逻辑可以完美用二叉树的后序遍历来模拟
// 对于只有相同优先级的表达式，构建树的上下层之间也表示的顺序执行。
/*
 *      1 + 2 - 3
 *        -
 *      /    \
 *     +      3
 *   1   2
 */
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
   //if(n->op == A_INTLIT){
   //    printf("int %d\n",n->intvalue);
   //}else{
   //    printf("%d %s %d\n",leftval,ASTop[n->op],rightval);
   //}

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