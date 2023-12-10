// 双引号表示先从源文件找，找不到再从标准库找
#include "defs.h"
// 在进入data.h之前将extern_的定义置为空
#define extern_
#include "data.h"
#undef extern_
// 取消extern_的置空
#include "decl.h"
// 尖括号表示直接从标准库找
#include <errno.h>

// 初始化全局变量
static void init(){
    Line = 1;
    Putback = '\n';
}

// 如果启动失败，打印相关信息
static void usage(char *prog){
    fprintf(stderr, "Usage: %s infile\n",prog);
    exit(1);
}

// 用于打印的token
char *tokstr[] = {"+","-","*","/","intlit"};

// 循环扫描输入文件中所有的token
// 打印每一个token的细节
static void scanfile(){
    struct token T;

    while(scan(&T)){
        printf("Token %s", tokstr[T.token]);
        if (T.token == T_INTLIT){
            printf(", value %d",T.intvalue);
        }
        printf("\n");
    }
}

// 主程序
// 检查参数，如果没有参数则留下记录。
// 打开文件并调用scanfile()扫描其中的token
void main(int argc, char *argv[]){
    struct ASTnode *n;

    if (argc != 2)
        usage(argv[0]);

    // 参数正确就开始初始化
    init();

    if((Infile = fopen(argv[1],"r")) == NULL){
        fprintf(stderr,"Unable to open %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    // 用scan启动一下读取，让全局变量token开始有值
    scan(&Token);

    // 启动之后就开始递归读取构建ast了
    n = binexpr();

    // 递归解析ast并得到最终的计算结果
    printf("%d\n", interpretAST(n));

    exit(0);
}
