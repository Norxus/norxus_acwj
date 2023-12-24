int scan(struct token *t);
// tree.c ast树节点创建方法
struct ASTnode *mkastnode(int op, struct ASTnode *left,
        struct  ASTnode *right, int intval);
// ast树叶子节点创建方法
struct ASTnode *mkastleaf(int op, int intvalue);
// ast树一元节点创建方法
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intval);

// gen.c
int genAST(struct ASTnode *n);
void genpreamble();
void genpostamble();
void genfreeregs();
void genprintint(int reg);

// 解释（解析）ast树
int interpretAST(struct ASTnode *n);

// cg.c 寄存器操作
void generatecode(struct ASTnode *n);
void freeall_registers(void);
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);

// expr.c ast树二进制节点
struct ASTnode *binexpr(int rbp);

// stmt.c
void statements(void);

// misc.c
void match(int t, char *what);
void semi(void);
