int scan(struct token *t);
// ast树节点创建方法
struct ASTnode *mkastnode(int op, struct ASTnode *left,
        struct  ASTnode *right, int intval);
// ast树叶子节点创建方法
struct ASTnode *mkastleaf(int op, int intvalue);
// ast树一元节点创建方法
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intval);
// ast树二进制节点
struct ASTnode *binexpr(void);
// 解释（解析）ast树
int interpretAST(struct ASTnode *n);