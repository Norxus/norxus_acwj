#include "defs.h"
#include "data.h"
#include "decl.h"


// 返回字符c在字符串s中的位置，如果没有发现则返回-1
static int chrpos(char *s, int c){
    char *p;

    // 接受字符的整数值，比如'a'字符就会传入65，返回该字符在字符串中首次出现的位置的指针
    // 找不到则返回NULL
    p = strchr(s, c);

    // 如果p不为NULL，那么就返回c在字符串中的位置
    // 因为s是字符串的首地址，并且字符串是连续分配的，所以p - s就能得到字符c在字符串s中的索引。
    return (p ? p - s : -1);
}

// 从输入文件中返回下一个字符串
static int next(void){
    int c;

    // 如果Putback有值了，就直接用Putback的值，同时将Putback置0
    if (Putback){
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(Infile);
    // 如果c == '\n'说明这一行读完了，行数+1
    if ('\n' == c)
        Line++;
    return c;
}

static void putback(int c){
    Putback = c;
}

// 跳过一些干扰字符
static int skip(void) {
    int c;

    c = next();
    // 干扰项直接跳过
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return (c);
}

// 扫描出int类型的数据
static int scanint(int c){
    int k, val = 0;

    while(((k = chrpos("0123456789", c)) >= 0)){
        val = val * 10 + k;
        c = next();
    }

    // 如果不是整数字符串，将它放回去
    // 之所以需要放回去是因为，下次同感fgetc获取的时候，就会获取到下一个字符。
    // 但这个字符因为不是整数就没有被处理，所以需要保存一下，用于后续的处理。
    putback(c);
    return val;
}

// 扫描当前token并为其填充类型，同时将下一个token放入全局变量中，如果有效则返回1，否则退出
// 如果token有效则返回1，如果没有token剩余则返回0
int scan(struct token *t){
    int c;

    // 跳过空格
    c = skip();

    // 根据输入字符进行判断
    switch (c) {
        case EOF:
            return (0);
        case '+':
            t->token = T_PLUS;
            break;
        case '-':
            t->token = T_MINUS;
            break;
        case '*':
            t->token=T_STAR;
            break;
        case '/':
            t->token = T_SLASH;
            break;
        default:
            // 如果是整数，则获取整数值。
            if (isdigit(c)){
                t->intvalue = scanint(c);
                t->token=T_INTLIT;
                break;
            }
            printf("Unrecognised character %c on line %d\n",c,Line);
            exit(1);
    }

    // 发现了一个token
    return (1);

}